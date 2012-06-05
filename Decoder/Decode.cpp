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
	BYTE pointer[2];
	BYTE map[BLOCK_COUNT+1];
	map[0] = -1;

	fseek(input, 0, SEEK_END);
	unsigned long int totalLength = ftell(input);
	fseek(input, 0, SEEK_SET);
	BYTE data_block = 0;
	fread(&data_block, sizeof(BYTE), 1, input);
	char* dataBuffer = new char[data_block];

	DataSender data;
	data.create(key, pipe, data_block);
	
	while(ftell(input) < totalLength)
	{
		fseek(input, thread*2, SEEK_CUR);
		fread(pointer, 1, 2, input);
		information.encodePointer((char*)pointer);

		fseek(input, (MAX_THREAD_COUNT-thread-1)*2 + pointer[0], SEEK_CUR);
		fread(map+1, 1, pointer[1], input);
		information.encodeMap((char*)map+1, pointer[1]);

		fseek(input, BLOCK_COUNT-pointer[0] - pointer[1], SEEK_CUR);

		for(int i = 0; i < pointer[1]; i++)
		{
			fseek(input, (map[i+1] - map[i])*data_block, SEEK_CUR);
			fread(dataBuffer, 1, data_block, input);
			data.send(dataBuffer);
		}
		fseek(input, BLOCK_COUNT - map[pointer[1]] - 1, SEEK_CUR);
	}
}

