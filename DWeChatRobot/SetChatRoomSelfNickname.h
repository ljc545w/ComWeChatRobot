#pragma once
#include<windows.h>
BOOL SetChatRoomSelfNickname(wchar_t* chatroomid, wchar_t* nickname);
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) BOOL SetChatRoomSelfNicknameRemote(LPVOID lpParameter);
#endif // !USE_SOCKET
