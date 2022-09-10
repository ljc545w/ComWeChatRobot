#pragma once
#include <windows.h>
BOOL __stdcall ForwardMessage(wchar_t *wxid, int localId);

#ifndef USE_SOCKET
extern "C" __declspec(dllexport) BOOL ForwardMessageRemote(LPVOID lpParameter);
#endif
