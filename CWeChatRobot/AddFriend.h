#pragma once
#include<windows.h>

BOOL AddFriendByWxid(DWORD pid,wchar_t* wxid, wchar_t* message);
BOOL AddFriendByV3(DWORD pid,wchar_t* v3, wchar_t* message,int AddType);
BOOL AddBrandContact(DWORD pid,wchar_t* PublicId);