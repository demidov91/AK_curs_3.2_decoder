#include "DataSender.h"
#include <stdio.h>
#include "DataEncoder.h"
#include "logging.h"

DataSender::DataSender(void)
{
	bytesInBuffer = 0;
}

DataSender* DataSender::create(const char* keyFilename, HANDLE pipe, int blockSize)
{
	FILE* key;
	fopen_s(&key, keyFilename, "rb");
	processor = new DataEncoder(key);

	fclose(key);
	this ->pipe = pipe;
	this ->blockSize = blockSize;
	return this;
}

void DataSender ::flushBuffer()
{
	DWORD bytesWritten = 0;
	processor ->encodeBlock(buffer);
	WriteFile(pipe, buffer, 16, &bytesWritten, 0);
	if(bytesWritten < 16)
	{
		log_error("flush buffer. written less than 16");
	}
}

void DataSender ::send(char* buffer)
{
	char* endBuffer = buffer + blockSize;
	if(blockSize + bytesInBuffer >= 16)
	{
		memcpy(this ->buffer + bytesInBuffer, buffer, 16 - bytesInBuffer);
		flushBuffer();
		buffer += 16 - bytesInBuffer;
		bytesInBuffer = 0;		
	}
	while(endBuffer - buffer >= 16)
	{
		memcpy(this ->buffer, buffer, 16);
		flushBuffer();
		buffer += 16;
	}
	memcpy(this ->buffer + bytesInBuffer, buffer, endBuffer - buffer);
	bytesInBuffer += (endBuffer - buffer);
}


DataSender::~DataSender(void)
{
	DWORD bytesWritten = 0;
	processor ->encodeBlock(buffer);
	WriteFile(pipe, buffer, bytesInBuffer, &bytesWritten, 0);
	if(bytesWritten < bytesInBuffer)
	{
		log_error("flush buffer. written less than 16");
	}
	delete processor;
}
