#pragma once
#include<windows.h>

BOOL DelChatRoomMember(DWORD pid,wchar_t* chatroomid, wchar_t* wxid);
BOOL DelChatRoomMember(DWORD pid,wchar_t* chatroomid, SAFEARRAY* psaValue);