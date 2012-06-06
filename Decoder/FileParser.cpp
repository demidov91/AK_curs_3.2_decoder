#include "FileParser.h"
#include <stdio.h>
#include "InformationEncoder.h"
#include "DataSender.h"


FileParser::FileParser(void)
{
	data_block = 0;
}


void FileParser ::setIO(FILE* file, HANDLE pipe)
{
	input = file;
	this ->pipe = pipe;
}


void FileParser ::parse(const char* key, const char* password, int thread)
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
	BYTE pointer[2];
	BYTE map[BLOCK_COUNT];
	while(ftell(input) < totalLength)
	{
		fread(&newMap, 1, 1, input);
		if(newMap)
		{
			checkEndFile(INFORMATION_PART_LENGTH + data_block*BLOCK_COUNT);
			fseek(input, thread*2, SEEK_CUR);
			fread(pointer, 1, 2, input);
			information.encodePointer((char*)pointer);
			checkPointer(pointer);
			fseek(input, (MAX_THREAD_COUNT-thread-1)*2 + mapStart, SEEK_CUR);
			fread(map, 1, mapLength, input);
			information.encodeMap((char*)map, mapLength);
			checkMap(map);
			fseek(input, BLOCK_COUNT-mapStart - mapLength, SEEK_CUR);
		}
		else
		{
			checkEndFile(data_block*BLOCK_COUNT);
		}
		fseek(input, map[0]*data_block, SEEK_CUR);
		for(int i = 0; i < mapLength - 1; i++)
		{
			fread(dataBuffer, 1, data_block, input);
			data.send(dataBuffer);
			fseek(input, (map[i+1] - map[i] - 1)*data_block, SEEK_CUR);
		}
		fread(dataBuffer, 1, data_block, input);
		data.send(dataBuffer);
		fseek(input, (BLOCK_COUNT - map[mapLength - 1] - 1)*data_block, SEEK_CUR);
	}
	delete[] dataBuffer;
}



void FileParser ::checkPointer(BYTE* pointer)
{
	mapStart = pointer[0];
	mapLength = pointer[1];
	if(mapStart + mapLength > BLOCK_COUNT)
	{
		throw INVALID_PASSWORD;
	}
	if (mapStart == 0 && mapLength == 0)
	{
		mapLength = 256;
	}	
}


void FileParser ::checkMap(BYTE* map)
{
	bool used[BLOCK_COUNT];
	memset(used, 0, BLOCK_COUNT);
	for(int i = 0; i < mapLength; i++)
	{
		if(used[map[i]])
		{
			throw INVALID_PASSWORD;
		}
		used[map[i]] = true;
	}
}

void FileParser ::checkEndFile(long int minLength)
{
	if (totalLength - ftell(input) < minLength)
	{
		throw INVALID_PASSWORD;
	}
}



FileParser::~FileParser(void)
{
}


