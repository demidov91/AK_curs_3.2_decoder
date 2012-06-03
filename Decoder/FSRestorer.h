#pragma once
#include <Windows.h>


class FSRestorer
{
private:
	HANDLE pipe;
	HANDLE restoringEnded;

	void finishRestoring();
public:
	FSRestorer(void);
	FSRestorer* create(HANDLE pipe, HANDLE restoringEnded);
	int getErrorCode();
	int getEncodedFilesNumber();
	~FSRestorer(void);
};

