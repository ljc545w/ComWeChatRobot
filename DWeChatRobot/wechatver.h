#pragma once
#include<windows.h>
#include<iostream>
using namespace std;
DWORD GetWeChatVerInt();
string GetWeChatVerStr();
extern "C" __declspec(dllexport) BOOL ChangeWeChatVerRemote(wchar_t* verStr);