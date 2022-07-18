#pragma once
#include<windows.h>
BOOL SetChatRoomAnnouncement(wchar_t* chatroomid, wchar_t* announcement);
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) BOOL SetChatRoomAnnouncementRemote(LPVOID lpParameter);
#endif // !USE_SOCKET
