#pragma once
#include <windows.h>

int GetDbInfo(void *data, int argc, char **argv, char **azColName);
int selectdbinfo(void *data, int argc, char **argv, char **azColName);
int query(void *data, int argc, char **argv, char **azColName);
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) DWORD ExecuteSQLRemote(LPVOID lpParameter);
extern "C" __declspec(dllexport) int SelectDataRemote(LPVOID lpParameter);
#endif
vector<vector<string>> SelectData(DWORD db_hanle, const char *sql);
void *ExecuteSQL(DWORD ptrDb, const char *sql, DWORD callback, void *data);
void *SelectData(DWORD db, const char *sql, void *data);
