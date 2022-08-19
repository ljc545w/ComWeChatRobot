#pragma once
#include <windows.h>

BOOL __stdcall GetChatRoomMembers(wchar_t *chatroomid);
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) DWORD GetChatRoomMembersRemote(LPVOID lparameter);
#else
wstring __stdcall GetChatRoomMembers(wstring chatroomid);
#endif
