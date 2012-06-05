#include "FSRestorer.h"
#include <Windows.h>
#include "DecodedDataAccessor.h"
#include "Constants.h"
#include <boost\filesystem.hpp>
using namespace boost ::filesystem;

FSRestorer::FSRestorer(void)
{
	reader = 0;
	restoredFiles = 0;
	errorCode = 0;
}


FSRestorer* FSRestorer::create(HANDLE pipe, HANDLE restoringEnded)
{
	reader = new DecodedDataAccessor();
	reader ->create(pipe);
	this ->restoringEnded = restoringEnded;
	return this;
}

void FSRestorer::start()
{
	try
	{
		restoredFiles = restoreFSObject(path("."));
	}
	catch(int error)
	{
		this ->errorCode = error;
	}
	finishRestoring();
}

int FSRestorer::restoreFSObject(path home)
{
	BYTE type = reader ->getType();
	unsigned long int size = reader ->getSize();
	string name = reader ->getName();
	path newPath = home / name;
	newPath = resolveConflicts(newPath, type);
	if(type == FILE_BYTE)
	{
		char* buffer = new char[size];
		reader ->getBuffer(buffer, size);
		home /= name;
		FILE* file;
		fopen_s(&file, newPath.string().c_str(), "wb");
		fwrite(buffer, size, 1, file);
		fclose(file);
		delete buffer;
	}
	else
	{
		if(type == DIRECTORY_BYTE)
		{
			create_directory(newPath);
			for(int i = 0; i < size; i++)
			{
				restoreFSObject(newPath);
			}
		}
		else
		{
			throw INVALID_KEY;
		}
	}
}

path FSRestorer ::resolveConflicts(path& initPath, char type)
{
	if(!exists(initPath))
	{
		return initPath;
	}
	char rootName[256];
	memset(rootName, 0, 256);
	strcpy(rootName, initPath.string().c_str());
	int lastIndex = strlen(rootName);
	for(int i = 0; i < INT_MAX; i++)
	{
		itoa(i, rootName+lastIndex, 10);
		if(!exists(rootName))
		{
			return path(rootName);
		}
	}
	return initPath;

}


void FSRestorer::finishRestoring()
{
	SetEvent(restoringEnded);
}

int FSRestorer ::getErrorCode()
{
	return errorCode;
}

int FSRestorer ::getEncodedFilesNumber()
{
	return restoredFiles;
}


FSRestorer::~FSRestorer(void)
{
	delete reader;
}
