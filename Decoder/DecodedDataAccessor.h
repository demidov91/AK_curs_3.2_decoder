#pragma once
#include <Windows.h>
#include <string>
using namespace std;

#include "tests.h"

class DecodedDataAccessor
{
private:
	HANDLE pipe;	
public:
	DecodedDataAccessor(void);
	DecodedDataAccessor* create(HANDLE pipe);
	char getType();
	unsigned long int getSize();
	string getName();
	void getBuffer(char* buffer, unsigned long int length);
	~DecodedDataAccessor(void);
	friend Friendly;
};

