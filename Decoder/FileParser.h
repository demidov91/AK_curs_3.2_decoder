#pragma once
#include <stdio.h>
#include <Windows.h>
#include "Constants.h"

#include "tests.h"

class FileParser
{
private:
	FILE* input;
	HANDLE pipe;
	BYTE mapStart;
	int mapLength;
	unsigned long int totalLength;
	BYTE data_block;


	void checkPointer(BYTE*);
	void checkMap(BYTE*);
	void checkEndFile(long int minLength);
public:
	FileParser(void);
	void setIO(FILE* file, HANDLE pipe);
	void parse(const char* key, const char* password, int thread);
	~FileParser(void);
	friend Friendly;
};

