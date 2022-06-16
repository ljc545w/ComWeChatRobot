#pragma once
#include<windows.h>

extern "C" __declspec(dllexport) DWORD CheckFriendStatusRemote(LPVOID lparameter);
VOID __stdcall CheckFriendStatus(wchar_t* wxid);
VOID UnHookFriendStatusCode();
VOID HookFriendStatusCode();