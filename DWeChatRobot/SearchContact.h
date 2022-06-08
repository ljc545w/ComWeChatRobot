#pragma once
#include<windows.h>
BOOL __stdcall GetUserInfoByWxId(wchar_t* wxid);
extern "C" __declspec(dllexport) DWORD GetWxUserInfoRemote(LPVOID lparamter);
extern "C" __declspec(dllexport) VOID DeleteUserInfoCacheRemote();
wchar_t* __stdcall GetUserNickNameByWxId(wchar_t* wxid);

void __stdcall SearchContactByNet(wchar_t* keyword);
extern "C" __declspec(dllexport) DWORD SearchContactByNetRemote(LPVOID keyword);
void StopSearchContactHook();