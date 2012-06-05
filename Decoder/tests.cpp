#include "tests.h"
#include "InformationEncoder.h"
#include "logging.h"
#include <conio.h>
#include "Decode.h"

void Friendly ::test_InformationEncoder_encodeMap()
{
	string password("This is password");
	InformationEncoder* tester = new InformationEncoder();
	tester ->create(&password);
	char* initFrase = "Some very long frase";
	char toEncode[30];
	int length = strlen(initFrase) + 1;
	memcpy(toEncode, initFrase, length);
	tester ->encodeMap(toEncode, 15);
	tester ->encodeMap(toEncode+15, length-15);
	delete tester;
	tester = new InformationEncoder();
	tester ->create(&password);
	tester ->encodeMap(toEncode, 5);
	tester ->encodeMap(toEncode+5, length-5);
	delete tester;
	if (strcmp(initFrase, toEncode))
	{
		log_test("InformationEncoder");
		return;
	}	
}



void Friendly ::beginTests()
{
	test_InformationEncoder_encodeMap();
	getch();

}