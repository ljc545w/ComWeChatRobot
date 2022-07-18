#pragma once
#include<windows.h>

BOOL SetChatRoomName(wchar_t* chatroomid, wchar_t* chatroomname);

#ifndef USE_SOCKET
extern "C" __declspec(dllexport) BOOL SetChatRoomNameRemote(LPVOID lpParameter);
#endif // !USE_SOCKET