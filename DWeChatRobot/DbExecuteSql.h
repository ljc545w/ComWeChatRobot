#pragma once
#include<windows.h>

int GetDbInfo(void* data, int argc, char** argv, char** azColName);
int selectdbinfo(void* data, int argc, char** argv, char** azColName);
int query(void* data, int argc, char** argv, char** azColName);

extern "C" __declspec(dllexport) DWORD ExecuteSQLRemote(LPVOID lpParameter);
extern "C" __declspec(dllexport) int SelectDataRemote(LPVOID lpParameter);
BOOL ExecuteSQL(DWORD ptrDb, const char* sql, DWORD callback, void* data);