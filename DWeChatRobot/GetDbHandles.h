#pragma once
#include<windows.h>

void* GetDbHandles();
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) DWORD GetDbHandlesRemote();
#endif
DWORD GetDbHandleByDbName(wchar_t* dbname);