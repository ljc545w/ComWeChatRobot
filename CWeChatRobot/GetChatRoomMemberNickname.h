#pragma once
#include<windows.h>
#include<iostream>
using namespace std;

wstring GetChatRoomMemberNickname(DWORD pid,wchar_t* chatroomid, wchar_t* wxid);