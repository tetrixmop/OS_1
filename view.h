#pragma once
#include "filemanager.h"

using namespace fManager;

class View
{
public:
	void Run();
private:
	void printDriveInfo();
	void createDir();
	void removeDir();
	void createFile();
	void copyFile();
	void moveFile();
	void printFileAtts();
	void setFileAtts();
	void printFileInfo();
	void printFileTime();
	void setFileTime(int option);
};