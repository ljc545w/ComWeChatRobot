#pragma once
#include<windows.h>

BOOL __stdcall GetChatRoomMembers(wchar_t* chatroomid);
extern "C" __declspec(dllexport) DWORD GetChatRoomMembersRemote(LPVOID lparameter);