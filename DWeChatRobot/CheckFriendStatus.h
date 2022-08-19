#pragma once
#include <windows.h>
using namespace std;

#ifndef USE_SOCKET
extern "C" __declspec(dllexport) DWORD CheckFriendStatusRemote(LPVOID lparameter);
#else
DWORD __stdcall CheckFriendStatus(wstring wxid);
#endif
VOID __stdcall CheckFriendStatus(wchar_t *wxid);
VOID UnHookFriendStatusCode();
VOID HookFriendStatusCode();
