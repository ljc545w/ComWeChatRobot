#pragma once
#include<windows.h>
BOOL __stdcall GetWxUserInfoByWxid2(wchar_t* wxid, DWORD& address);
BOOL __stdcall GetUserInfoByWxId(wchar_t* wxid, DWORD& address);
extern "C" __declspec(dllexport) DWORD GetWxUserInfoRemote(LPVOID lparamter);
extern "C" __declspec(dllexport) VOID DeleteUserInfoCacheRemote();