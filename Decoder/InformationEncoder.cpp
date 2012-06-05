#include "InformationEncoder.h"
#include "ByteEncoder.h"


InformationEncoder::InformationEncoder(void)
{
	processor = NULL;
}

InformationEncoder* InformationEncoder::create(string* password)
{
	processor = new ByteEncoder(password);
	return this;
}

void InformationEncoder ::encodePointer(char* buffer)
{
	processor ->encodeByte(buffer);
	processor ->encodeByte(buffer+1);
}

void InformationEncoder ::encodeMap(char* buffer, int length)
{
	for (int i = 0; i < length; i++)
	{
		processor ->encodeByte(buffer+i);
	}
}


InformationEncoder::~InformationEncoder(void)
{
	if(processor)
	{
		delete processor;
	}
}
