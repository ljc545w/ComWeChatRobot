#pragma once
#include <windows.h>

BOOL __stdcall SendText(wchar_t *wsWxId, wchar_t *wsTextMsg);
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) void SendTextRemote(LPVOID lpParameter);
#endif
