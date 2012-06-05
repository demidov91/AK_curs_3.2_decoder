#pragma once
#include <string>
#include "tests.h"
using namespace std;

class ByteEncoder
{
private:
	char* key;
	int currentPosition;
	int length;
public:
	ByteEncoder(string*);
	void encodeByte(char*);
	~ByteEncoder();
friend Friendly;
};	