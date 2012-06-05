#include "DecodedDataAccessor.h"
#include <Windows.h>
#include <string>
using namespace std;


DecodedDataAccessor::DecodedDataAccessor(void)
{
}

DecodedDataAccessor* DecodedDataAccessor::create(HANDLE pipe)
{
	this ->pipe = pipe;
	return this;
}

void DecodedDataAccessor ::getBuffer(char* buffer, unsigned long int length)
{
	DWORD wasRead = 0;
	DWORD processed = 0;
	while(processed < length)
	{
		ReadFile(pipe, buffer + processed ,length-processed, &wasRead, 0);
		processed += wasRead;
	}
}

char DecodedDataAccessor ::getType()
{
	char buffer = 0;
	getBuffer(&buffer, 1);
	return buffer;
}

unsigned long int DecodedDataAccessor ::getSize()
{
	unsigned long int buffer = 0;
	getBuffer((char*)&buffer, 4);
	return buffer;	
}

string DecodedDataAccessor::getName()
{
	char length = 0;
	getBuffer(&length, 1);
	char* bufferForName = new char[length+1];
	getBuffer(bufferForName, length);
	bufferForName[length] = 0;
	string toReturn(bufferForName);
	delete[] bufferForName;
	return toReturn;
}


DecodedDataAccessor::~DecodedDataAccessor(void)
{
	
}
