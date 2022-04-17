#pragma once
#include<windows.h>
struct BackupStruct {
	DWORD DbHandle;
	char* BackupFile;
};
int BackupSQLiteDB(DWORD DbHandle, const char* BackupFile);
extern "C" __declspec(dllexport) BOOL BackupSQLiteDBRemote(LPVOID lpParameter);