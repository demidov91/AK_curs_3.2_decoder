#pragma once
#include <stdio.h>
#include <Windows.h>
#define FSRESTORER_WAIT_TIME 5000

class Decode
{
private:
	FILE* input;
public:
	Decode(void);
	int start(char* input, char* key, char* verbalKey, int thread);	
	void runDecoding(char*, char*, int, HANDLE);
};

