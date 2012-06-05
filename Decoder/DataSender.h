#pragma once
#include <Windows.h>
#include "DataEncoder.h"

class DataSender
{
private:
	DataEncoder* processor;
	HANDLE pipe;
	int blockSize;
	char buffer[16];
	char bytesInBuffer;

	void flushBuffer();
public:
	DataSender(void);
	DataSender* create(const char* keyFilename, HANDLE pipe, int blockSize);
	void send(char* buffer);
	~DataSender(void);
};

