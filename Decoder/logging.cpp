#include "logging.h"
#include <iostream>
#include <string>
using namespace std;

void log_error(void* args)
{
	cout << (char*)args << endl;
};

void log_test(const char* args)
{
	cout << "TEST: " << args << endl;
};

void log_test(const string* args)
{
	cout << "TEST: " << *args << endl;
};