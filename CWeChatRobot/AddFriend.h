#pragma once
#include<windows.h>

BOOL AddFriendByWxid(wchar_t* wxid, wchar_t* message);
BOOL AddFriendByV3(wchar_t* v3, wchar_t* message,int AddType);