#include "Decode.h"
#include <Windows.h>
#include <process.h>
#include "Decoding.h"
#include "FSRestorer.h"
#include "main.h"
#include "Constants.h"
#include "InformationEncoder.h"
#include "DataSender.h"
using namespace std;

Decode::Decode(void)
{
	data_block = 0;
}

int Decode ::start(char* inputFile, char* key, char* password, int thread)
{
	HANDLE rPipe = NULL;
	HANDLE wPipe = NULL;
	HANDLE decodingEndedEvent = CreateEvent(NULL, false, false, NULL);
	fopen_s(&input, inputFile, "rb");
	CreatePipe(&rPipe, &wPipe, NULL, 1000000);
	FSRestorer* decode_arg = new FSRestorer();
	decode_arg ->create(rPipe, decodingEndedEvent);
	_beginthread(decode_async, NULL, decode_arg);
	runDecoding(key, password, thread, wPipe);
	CloseHandle(wPipe);
	DWORD waitResult = WaitForSingleObject(decodingEndedEvent, FSRESTORER_WAIT_TIME);
	if (waitResult == WAIT_OBJECT_0)
	{
		if(!decode_arg ->getErrorCode())
		{
			return decode_arg ->getEncodedFilesNumber();
		}
		else 
		{
			throw decode_arg ->getErrorCode();
		}		
	}
	else
	{
		throw INVALID_FILE_FORMAT;
	}
	return 0;
};

void Decode ::runDecoding(char* key, char* password, int thread, HANDLE pipe)
{
	InformationEncoder information;
	information.create(&string(password));
	
	fseek(input, 0, SEEK_END);
	totalLength = ftell(input);
	fseek(input, 0, SEEK_SET);
	fread(&data_block, sizeof(BYTE), 1, input);
	char* dataBuffer = new char[data_block];

	DataSender data;
	data.create(key, pipe, data_block);
	char newMap = 1;		
	while(ftell(input) < totalLength)
	{
		fread(&newMap, 1, 1, input);
		if(newMap)
		{
			checkEndFile(INFORMATION_PART_LENGTH + data_block*BLOCK_COUNT);
			fseek(input, thread*2, SEEK_CUR);
			fread(pointer, 1, 2, input);
			information.encodePointer((char*)pointer);
			checkPointer();
			fseek(input, (MAX_THREAD_COUNT-thread-1)*2 + pointer[0], SEEK_CUR);
			fread(map, 1, pointer[1], input);
			information.encodeMap((char*)map, pointer[1]);
			checkMap();
			fseek(input, BLOCK_COUNT-pointer[0] - pointer[1], SEEK_CUR);
		}
		else
		{
			checkEndFile(data_block*BLOCK_COUNT);
		}
		for(int i = 0; i < pointer[1] - 1; i++)
		{
			fread(dataBuffer, 1, data_block, input);
			data.send(dataBuffer);
			fseek(input, (map[i+1] - map[i] - 1)*data_block, SEEK_CUR);
		}
		fread(dataBuffer, 1, data_block, input);
		data.send(dataBuffer);
		fseek(input, (BLOCK_COUNT - map[pointer[1] - 1] - 1)*data_block, SEEK_CUR);
	}
	delete[] dataBuffer;
}

void Decode ::checkPointer()
{
	if(pointer[0] + pointer[1] > BLOCK_COUNT)
	{
		throw INVALID_PASSWORD;
	}	
}


void Decode ::checkMap()
{
	bool used[BLOCK_COUNT];
	memset(used, 0, BLOCK_COUNT);
	for(int i = 0; i < pointer[1]; i++)
	{
		if(used[map[i+1]])
		{
			throw INVALID_PASSWORD;
		}
		used[map[i+1]] = true;
	}
}

void Decode ::checkEndFile(long int minLength)
{
	if (totalLength - ftell(input) < minLength)
	{
		throw INVALID_PASSWORD;
	}
}