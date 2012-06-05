#pragma once
#include <stdio.h>
#include <Windows.h>

#include "tests.h"


class DataEncoder{
private:
	__int16 step;
public:
	DataEncoder(FILE* key);
	void encodeBlock(char* buffer);
	friend Friendly;
};