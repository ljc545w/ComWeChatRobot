#pragma once
#include<windows.h>

BOOL __stdcall AddFriendByV3(wchar_t* v3, wchar_t* message,int AddType);
BOOL __stdcall AddFriendByWxid(wchar_t* wxid, wchar_t* message);
BOOL __stdcall AddBrandContact(wchar_t* PublicId);
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) BOOL AddFriendByV3Remote(LPVOID lpParameter);
extern "C" __declspec(dllexport) BOOL AddFriendByWxidRemote(LPVOID lpParameter);
extern "C" __declspec(dllexport) BOOL AddBrandContactRemote(LPVOID lpParameter);
#endif