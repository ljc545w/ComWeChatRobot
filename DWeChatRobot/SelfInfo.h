#pragma once
#include <windows.h>
#include <iostream>
using namespace std;
wstring GetSelfInfo();
wstring GetSelfWxid();
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) DWORD GetSelfInfoRemote();
extern "C" __declspec(dllexport) VOID DeleteSelfInfoCacheRemote();
#endif
