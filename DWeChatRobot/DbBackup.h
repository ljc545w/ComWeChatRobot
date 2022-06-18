#pragma once
#include<windows.h>

int BackupSQLiteDB(DWORD DbHandle, const char* BackupFile);
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) int BackupSQLiteDBRemote(LPVOID lpParameter);
#endif