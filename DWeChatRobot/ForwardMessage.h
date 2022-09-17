#pragma once
#include <windows.h>
BOOL __stdcall ForwardMessage(wchar_t *wxid, unsigned long long localId);

#ifndef USE_SOCKET
extern "C" __declspec(dllexport) BOOL ForwardMessageRemote(LPVOID lpParameter);
#endif
