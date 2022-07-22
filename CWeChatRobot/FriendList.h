#pragma once
#include<windows.h>
#include<iostream>
using namespace std;
SAFEARRAY* GetFriendList(DWORD pid);
std::wstring GetFriendListString(DWORD pid);