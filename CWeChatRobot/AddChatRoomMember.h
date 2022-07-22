#pragma once
#include<windows.h>

BOOL AddChatRoomMember(DWORD pid,wchar_t* chatroomid, wchar_t* wxid);
BOOL AddChatRoomMember(DWORD pid,wchar_t* chatroomid, SAFEARRAY* psaValue);