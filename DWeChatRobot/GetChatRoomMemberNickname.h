#pragma once
#include<windows.h>
#include<iostream>
using namespace std;

wstring GetChatRoomMemberNickname(wchar_t* chatroomid, wchar_t* wxid);
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) BOOL GetChatRoomMemberNicknameRemote(LPVOID lpParameter);
#endif // !USE_SOCKET
