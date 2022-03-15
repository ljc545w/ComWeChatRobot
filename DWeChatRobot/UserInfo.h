#pragma once
#include<windows.h>
BOOL GetWxUserInfoByWxid(wchar_t* wxid, DWORD& address);
BOOL GetUserDetailInfoByWxId(wchar_t* wxid, DWORD& address);
extern "C" __declspec(dllexport) DWORD GetWxUserInfoRemote(LPVOID lparamter);