#pragma once
#include<windows.h>
BOOL AddChatRoomMember(wchar_t* chatroomid, wchar_t** wxids, int length);
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) BOOL AddChatRoomMemberRemote(LPVOID lpParameter);
#endif // !USE_SOCKET