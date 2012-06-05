#include "tests.h"
#include "InformationEncoder.h"
#include "logging.h"
#include <conio.h>
#include "Decode.h"

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


void Friendly ::test_Decode_runDecoding()
{
	const char* inputFilename = "test\\Decode\\sampleEncoded.txt";
	char originalData[500];
	for(int i = 0; i < 500; i++)
	{
		originalData[i] = i;
	}
	makeSampleEncodedFileToCheckInfoBlock(inputFilename, originalData);
	Decode tester;
	tester.input = fopen(inputFilename, "rb");
	HANDLE rPipe = 0;
	HANDLE wPipe = 0;
	CreatePipe(&rPipe, &wPipe, 0, 800);
	tester.runDecoding("test\\blank.key", "", 1, wPipe);
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

void Friendly ::beginTests()
{
	test_InformationEncoder_encodeMap();
	test_Decode_runDecoding();
	getch();

}