#pragma once
#include<windows.h>

BOOL __stdcall VerifyFriendApply(wchar_t* v3_data, wchar_t* v4_data);
extern "C" __declspec(dllexport) BOOL VerifyFriendApplyRemote(LPVOID lparameter);