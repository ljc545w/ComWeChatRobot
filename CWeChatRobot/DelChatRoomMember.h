#pragma once
#include<windows.h>

BOOL DelChatRoomMember(wchar_t* chatroomid, wchar_t* wxid);
BOOL DelChatRoomMember(wchar_t* chatroomid, SAFEARRAY* psaValue);