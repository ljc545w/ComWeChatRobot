#pragma once
#include <windows.h>
#include <vector>
using namespace std;

vector<void *> GetDbHandles();
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) DWORD GetDbHandlesRemote();
#endif
DWORD GetDbHandleByDbName(wchar_t *dbname);
unsigned int GetLocalIdByMsgId(ULONG64 msgid, int &dbIndex);
