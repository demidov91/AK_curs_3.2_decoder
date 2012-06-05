#pragma once
#include <stdio.h>
#include <Windows.h>
#include "Constants.h"
#define FSRESTORER_WAIT_TIME 5000

#include "tests.h"

class Decode
{
private:
	FILE* input;
	BYTE pointer[2];
	BYTE map[BLOCK_COUNT+1];
	unsigned long int totalLength;
	BYTE data_block;


	void checkPointer();
	void checkMap();
	void checkEndFile(long int minLength);
public:
	Decode(void);
	int start(char* input, char* key, char* verbalKey, int thread);	
	void runDecoding(char* key, char* password, int thread, HANDLE pipe);
	friend Friendly;
};

