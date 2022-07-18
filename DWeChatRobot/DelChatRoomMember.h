#pragma once
#include<windows.h>
BOOL DelChatRoomMember(wchar_t* chatroomid, wchar_t** wxids, int length);
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) BOOL DelChatRoomMemberRemote(LPVOID lpParameter);
#endif // !USE_SOCKET
