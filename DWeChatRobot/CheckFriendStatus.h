#pragma once
#include<windows.h>

#ifndef USE_SOCKET
extern "C" __declspec(dllexport) DWORD CheckFriendStatusRemote(LPVOID lparameter);
#endif
VOID __stdcall CheckFriendStatus(wchar_t* wxid);
VOID UnHookFriendStatusCode();
VOID HookFriendStatusCode();