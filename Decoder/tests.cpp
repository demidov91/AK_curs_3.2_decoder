#include "tests.h"
#include "InformationEncoder.h"
#include "logging.h"
#include <conio.h>
#include "Decode.h"
#include "DecodedDataAccessor.h"
#include "Constants.h"
#include <boost\filesystem.hpp>
#include "FSRestorer.h"
#include "FileParser.h"
using namespace boost ::filesystem;

void Friendly ::test_InformationEncoder_encodeMap()
{
	string password("This is password");
	InformationEncoder* tester = new InformationEncoder();
	tester ->create(&password);
	char* initFrase = "Some very long frase";
	char toEncode[30];
	int length = strlen(initFrase) + 1;
	memcpy(toEncode, initFrase, length);
	tester ->encodeMap(toEncode, 15);
	tester ->encodeMap(toEncode+15, length-15);
	delete tester;
	tester = new InformationEncoder();
	tester ->create(&password);
	tester ->encodeMap(toEncode, 5);
	tester ->encodeMap(toEncode+5, length-5);
	delete tester;
	if (strcmp(initFrase, toEncode))
	{
		log_test("InformationEncoder");
		return;
	}	
}


void makeSampleEncodedFileToCheckInfoBlock(const char* fileName, char* data)
{
	BYTE blockSize = 125;
	char dummyByte = 'A';
	FILE* file = fopen(fileName, "wb");
	fwrite(&blockSize,1, 1, file);

	BYTE bufferByte = 1;
	fwrite(&dummyByte,1, 1, file);

	fwrite(&dummyByte,1, 1, file);
	fwrite(&dummyByte,1, 1, file);

	bufferByte = 254;
	fwrite(&bufferByte,1, 1, file);
	bufferByte = 2;
	fwrite(&bufferByte,1, 1, file);

	for(int i = 0; i < ((256-2)*2 + 254); i++)
	{
		fwrite(&dummyByte,1, 1, file);
	}
	bufferByte = 0;
	fwrite(&bufferByte,1, 1, file);
	bufferByte = 255;
	fwrite(&bufferByte,1, 1, file);

	fwrite(data, 1, blockSize, file);
	dummyByte = 'D';
	for(int i = 0; i <254*blockSize; i++)
	{
		fwrite(&dummyByte,1, 1, file);
	}
	fwrite(data+blockSize, 1, blockSize, file);

	bufferByte = 0;
	fwrite(&bufferByte,1, 1, file);
	fwrite(data+blockSize*2, 1, blockSize, file);
	for(int i = 0; i <254*blockSize; i++)
	{
		fwrite(&dummyByte,1, 1, file);
	}
	fwrite(data+blockSize*3, 1, blockSize, file);
	fclose(file);
}


void Friendly ::test_FileParser_parse()
{
	const char* inputFilename = "test\\Decode\\sampleEncoded.txt";
	char originalData[500];
	for(int i = 0; i < 500; i++)
	{
		originalData[i] = i;
	}
	makeSampleEncodedFileToCheckInfoBlock(inputFilename, originalData);
	FileParser tester;
	FILE* input = fopen(inputFilename, "rb");
	HANDLE rPipe = 0;
	HANDLE wPipe = 0;
	CreatePipe(&rPipe, &wPipe, 0, 800);
	tester.input = input;
	tester.pipe = wPipe;
	tester.parse("test\\blank.key", "", 1);
	fclose(tester.input);
	char gotData[500];
	DWORD read = 0;
	ReadFile(rPipe, gotData, 500, &read, 0);
	if(read == 500 && memcmp(gotData, originalData, 500))
	{
		log_test("Decode runDecoding");
		return;
	}

}

void Friendly ::test_DecodedDataAccessor_get()
{
	HANDLE rPipe = 0;
	HANDLE wPipe = 0;
	CreatePipe(&rPipe, &wPipe, 0, 100);
	DWORD processed = 0;
	const char* bufferedFrase = "Frase 1";
	WriteFile(wPipe, bufferedFrase, strlen(bufferedFrase)+1, &processed, 0);
	char type = 5;
	WriteFile(wPipe, &type, 1, &processed, 0);
	unsigned long int size = 12345678;
	WriteFile(wPipe, &size, sizeof(size), &processed, 0);
	const char* bufferedName = "Some text";
	BYTE lenOfName = strlen(bufferedName);
	WriteFile(wPipe, &lenOfName, 1, &processed, 0);
	WriteFile(wPipe, bufferedName, lenOfName, &processed, 0);

	CloseHandle(wPipe);
	DecodedDataAccessor reader;
	reader.create(rPipe);
	char pipedFrase[10];
	reader.getBuffer(pipedFrase, strlen(bufferedFrase)+1);
	if(strcmp(pipedFrase,bufferedFrase))
	{
		log_test("test_DecodedDataAccessor_get");
		return;
	}

	char pipedType = reader.getType();
	if(type != pipedType)
	{
		log_test("test_DecodedDataAccessor_get");
		return;
	}
	DWORD pipedSize = reader.getSize();
	if(size != pipedSize)
	{
		log_test("test_DecodedDataAccessor_get");
		return;
	}
	string pipedName = reader.getName();
	if(pipedName.compare(bufferedName))
	{
		log_test("test_DecodedDataAccessor_get");
		return;
	}
	CloseHandle(rPipe);
}

void Friendly ::test_FSRestorer_restoreObject()
{
	const char* testDir = "TestRestoredDir";
	const char* testFile = "TestRestoredFile.txt";
	remove(testDir);
	remove(testFile);
	HANDLE rPipe = 0;
	HANDLE wPipe = 0;
	CreatePipe(&rPipe, &wPipe, 0, 100000);
	//begin dir 1
	char typeBuffer = DIRECTORY_BYTE;
	DWORD written = 0;
	WriteFile(wPipe, &typeBuffer, 1, &written, 0); 
	DWORD sizeBuffer = 3;
	WriteFile(wPipe, &sizeBuffer, sizeof(DWORD), &written, 0); 
	BYTE nameLengthBuf = strlen(testDir);
	WriteFile(wPipe, &nameLengthBuf, sizeof(BYTE), &written, 0);
	WriteFile(wPipe, testDir, nameLengthBuf, &written, 0);

	//begin file 1 in dir 1

	typeBuffer = FILE_BYTE;
	const char* f1namae = "test1.txt";
	sizeBuffer = 10000;
	nameLengthBuf = strlen(f1namae);
	char fileBuffer[10000];
	memset(fileBuffer, '1', sizeBuffer);

	WriteFile(wPipe, &typeBuffer, 1, &written, 0); 
	WriteFile(wPipe, &sizeBuffer, sizeof(DWORD), &written, 0); 
	WriteFile(wPipe, &nameLengthBuf, sizeof(BYTE), &written, 0);
	WriteFile(wPipe, f1namae, nameLengthBuf, &written, 0);
	WriteFile(wPipe, fileBuffer, sizeBuffer, &written, 0);

	//end file1 in dir 1
	//begin dir in dir 1

	typeBuffer = DIRECTORY_BYTE;
	const char* dnamae = "test2";
	sizeBuffer = 0;
	nameLengthBuf = strlen(dnamae);
	
	WriteFile(wPipe, &typeBuffer, 1, &written, 0); 
	WriteFile(wPipe, &sizeBuffer, sizeof(DWORD), &written, 0); 
	WriteFile(wPipe, &nameLengthBuf, sizeof(BYTE), &written, 0);
	WriteFile(wPipe, dnamae, nameLengthBuf, &written, 0);

	//end dir in dir 1
	//begin file 3 in dir 1

	typeBuffer = FILE_BYTE;
	const char* f3namae = "test3.txt";
	sizeBuffer = 200;
	nameLengthBuf = strlen(f3namae);
	memset(fileBuffer, '3', sizeBuffer);

	WriteFile(wPipe, &typeBuffer, 1, &written, 0); 
	WriteFile(wPipe, &sizeBuffer, sizeof(DWORD), &written, 0); 
	WriteFile(wPipe, &nameLengthBuf, sizeof(BYTE), &written, 0);
	WriteFile(wPipe, f3namae, nameLengthBuf, &written, 0);
	WriteFile(wPipe, fileBuffer, sizeBuffer, &written, 0);

	//end file 3 in dir 1
	//begin file

	typeBuffer = FILE_BYTE;
	sizeBuffer = 1;
	nameLengthBuf = strlen(testFile);
	memset(fileBuffer, '2', sizeBuffer);

	WriteFile(wPipe, &typeBuffer, 1, &written, 0); 
	WriteFile(wPipe, &sizeBuffer, sizeof(DWORD), &written, 0); 
	WriteFile(wPipe, &nameLengthBuf, sizeof(BYTE), &written, 0);
	WriteFile(wPipe, testFile, nameLengthBuf, &written, 0);
	WriteFile(wPipe, fileBuffer, sizeBuffer, &written, 0);

	//end file

	CloseHandle(wPipe);
	HANDLE decodingEndedEvent = CreateEvent(NULL, false, false, NULL);
	FSRestorer tester;
	tester.create(rPipe, decodingEndedEvent);
	tester.start();
	CloseHandle(rPipe);

	if(!exists(testDir))
	{
		log_test("FSRestorer");
		return;		
	}


	if(!exists(path(testDir) / path(f1namae)) || file_size(path(testDir) / path(f1namae)) != 10000)
	{
		log_test("FSRestorer");
		return;		
	}

	if(!exists(path(testDir) / path(f3namae)) || file_size(path(testDir) / path(f3namae)) != 200)
	{
		log_test("FSRestorer");
		return;		
	}

	if(!exists(path(testDir) / path(dnamae)))
	{
		log_test("FSRestorer");
		return;		
	}


}

void Friendly ::beginTests()
{
	test_InformationEncoder_encodeMap();
	test_FileParser_parse();
	test_DecodedDataAccessor_get();
	test_FSRestorer_restoreObject();
	getch();
}