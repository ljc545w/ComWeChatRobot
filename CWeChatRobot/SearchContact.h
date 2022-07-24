#pragma once
#include <windows.h>
#include <iostream>
using namespace std;
std::wstring GetWxUserInfo(DWORD pid,wchar_t* wxid);
SAFEARRAY* SearchContactByNet(DWORD pid,wchar_t* keyword);