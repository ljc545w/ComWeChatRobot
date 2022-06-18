#pragma once
#include<windows.h>
wstring __stdcall GetUserInfoByWxId(wchar_t* wxid);
wchar_t* __stdcall GetUserNickNameByWxId(wchar_t* wxid);
void* __stdcall SearchContactByNet(wchar_t* keyword);
void UnHookSearchContact();

#ifndef USE_SOCKET
extern "C" __declspec(dllexport) DWORD GetWxUserInfoRemote(LPVOID lparamter);
extern "C" __declspec(dllexport) VOID DeleteUserInfoCacheRemote();
extern "C" __declspec(dllexport) DWORD SearchContactByNetRemote(LPVOID keyword);
#endif