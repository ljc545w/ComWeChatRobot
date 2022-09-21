#pragma once
#include <windows.h>

BOOL __stdcall GetA8Key(wchar_t *url);
void UnHookA8KeyBuf();
void HookA8KeyBuf();

#ifndef USE_SOCKET
extern "C" __declspec(dllexport) DWORD GetA8KeyRemote(wchar_t *url);
#else
string __stdcall GetA8Key(wstring url);
#endif
