#pragma once
#include<windows.h>

BOOL __stdcall VerifyFriendApply(wchar_t* v3_data, wchar_t* v4_data);
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) BOOL VerifyFriendApplyRemote(LPVOID lparameter);
#endif