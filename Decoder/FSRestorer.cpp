#include "FSRestorer.h"
#include <Windows.h>

FSRestorer::FSRestorer(void)
{
}


FSRestorer* FSRestorer::create(HANDLE pipe, HANDLE restoringEnded)
{
	this ->pipe = pipe;
	this ->restoringEnded = restoringEnded;
	return this;
}

void FSRestorer::finishRestoring()
{
	CloseHandle(pipe);
	SetEvent(restoringEnded);
}

int FSRestorer ::getErrorCode()
{
	return 0;
}

int FSRestorer ::getEncodedFilesNumber()
{
	return 0;
}


FSRestorer::~FSRestorer(void)
{
}
