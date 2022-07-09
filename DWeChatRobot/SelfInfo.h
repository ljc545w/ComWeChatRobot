#pragma once
#include<windows.h>
#include<iostream>
using namespace std;
wstring GetSelfInfo();
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) DWORD GetSelfInfoRemote();
extern "C" __declspec(dllexport) VOID DeleteSelfInfoCacheRemote();
extern "C" __declspec(dllexport) BOOL isWxLogin();
#else
BOOL isWxLogin();
#endif