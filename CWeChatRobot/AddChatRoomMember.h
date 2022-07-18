#pragma once
#include<windows.h>

BOOL AddChatRoomMember(wchar_t* chatroomid, wchar_t* wxid);
BOOL AddChatRoomMember(wchar_t* chatroomid, SAFEARRAY* psaValue);