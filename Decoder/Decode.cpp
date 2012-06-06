#include "Decode.h"
#include <Windows.h>
#include <process.h>
#include "Decoding.h"
#include "FSRestorer.h"
#include "main.h"
#include "Constants.h"

using namespace std;

Decode::Decode(void)
{
	
}

int Decode ::start(char* inputFile, char* key, char* password, int thread)
{
	HANDLE rPipe = NULL;
	HANDLE wPipe = NULL;
	HANDLE decodingEndedEvent = CreateEvent(NULL, false, false, NULL);
	CreatePipe(&rPipe, &wPipe, NULL, 1000000);
	FSRestorer* decode_arg = new FSRestorer();
	decode_arg ->create(rPipe, decodingEndedEvent);
	_beginthread(decode_async, NULL, decode_arg);
	runDecoding(inputFile, key, password, thread, wPipe);
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



void Decode:: runDecoding(const char* inputFile, const char* key, const char* password, int thread, HANDLE output)
{
	FILE* input;
	fopen_s(&input, inputFile, "rb");
	parser = FileParser();
	parser.setIO(input, output);
	parser.parse(key, password, thread);
	fclose(input);
}