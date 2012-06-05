#pragma once
#include <string>
#include "ByteEncoder.h"
using namespace std;

class InformationEncoder
{
private:
	ByteEncoder* processor;

public:
	InformationEncoder(void);
	InformationEncoder* create(string*);
	void encodePointer(char* buffer);
	void encodeMap(char* buffer, int length);
	~InformationEncoder(void);
};

