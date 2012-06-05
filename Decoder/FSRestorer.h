#pragma once
#include <Windows.h>
#include "DecodedDataAccessor.h"
#include <boost\filesystem.hpp>
using namespace boost ::filesystem;

#include "tests.h"

class FSRestorer
{
private:
	DecodedDataAccessor* reader;
	HANDLE restoringEnded;
	int restoredFiles;
	int errorCode;

	path resolveConflicts(path&, char);
	void finishRestoring();
public:
	FSRestorer(void);
	FSRestorer* create(HANDLE pipe, HANDLE restoringEnded);
	void start();
	int restoreFSObject(path home);
	int getErrorCode();
	int getEncodedFilesNumber();
	~FSRestorer(void);
	friend Friendly;
};

