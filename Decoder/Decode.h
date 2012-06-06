#pragma once
#include <stdio.h>
#include <Windows.h>
#include "Constants.h"
#include "FileParser.h"
#define FSRESTORER_WAIT_TIME 15000

#include "tests.h"

class Decode
{
private:
	FileParser parser;
public:
	Decode(void);
	int start(char* input, char* key, char* verbalKey, int thread);	
	void runDecoding(const char* inputFile, const char* key, const char* password, int thread, HANDLE output);
	friend Friendly;
};

