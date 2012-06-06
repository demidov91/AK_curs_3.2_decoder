#include "tests.h"
#include <iostream>
#include <string>
#include <vector>
#include "Decode.h"
#include "main.h"
#include "tests.h"
using namespace std;



int main(int argc, char** argv)
{
	//Friendly ::beginTests();
	//return 0;
	if (argc < 2)
	{
		cerr << NOT_ENOUGH_PARAMETERS << endl;
		return 1;
	}
		
	
	
	if (!strcmp(argv[1], "/d"))
	{
		if(argc < 5)
		{
			cerr << WRONG_ARG_D << endl;
			return 1;
		}
		char* str_threadNumber = strrchr(argv[4], '_');
		if (!str_threadNumber || !(*(str_threadNumber+1)))
		{
			cerr << WRONG_ARG_D << endl;
			return 1;
		}
		int int_threadNumber = atoi(str_threadNumber+1);
		if(!int_threadNumber && str_threadNumber[0] != '0')
		{
			cerr << WRONG_ARG_D << endl;
			return 1;
		}
		*str_threadNumber = 0;
		Decode* decoder = new Decode();
		try{
			int decoded = decoder ->start(argv[2], argv[3], argv[4], int_threadNumber);
		}
		catch(int error)
		{
			delete decoder;
			if(error == INVALID_FILE_FORMAT)
			{
				cerr << INVALID_FILE_FORMAT_MSG << endl;
			}
			return 1;
		}
		cout << ENCODED_SUCCESSFULLY << endl;
		delete decoder;
	}
	else
	{
		cerr << WRONG_USAGE << argv[1] << endl;
	}
	
	return 0;
}