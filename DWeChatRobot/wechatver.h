#pragma once
#include<windows.h>
#include<iostream>
using namespace std;
DWORD GetWeChatVerInt();
string GetWeChatVerStr();
BOOL ChangeWeChatVer(wchar_t* verStr);
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) BOOL ChangeWeChatVerRemote(wchar_t* verStr);
#endif // !USE_SOCKET