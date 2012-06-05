#include "ByteEncoder.h"
#include <vector>
using namespace std;


ByteEncoder::ByteEncoder(string* key)
{
	length = key ->length();
	this ->key = (char*)malloc(length + 1);
	strcpy(this ->key, key ->c_str());
	currentPosition = 0;
}

void ByteEncoder ::encodeByte(char* pointer)
{
	*pointer = *pointer ^ key[currentPosition++];
	if(currentPosition >= length)
	{
		currentPosition = 0;
	}
}


ByteEncoder::~ByteEncoder()
{
	free(key);
}