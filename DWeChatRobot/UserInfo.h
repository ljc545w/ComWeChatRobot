#pragma once
#include<windows.h>
BOOL GetWxUserInfoByWxid2(wchar_t* wxid, DWORD& address);
BOOL GetUserInfoByWxId(wchar_t* wxid, DWORD& address);
extern "C" __declspec(dllexport) DWORD GetWxUserInfoRemote(LPVOID lparamter);
extern "C" __declspec(dllexport) VOID DeleteUserInfoCacheRemote();