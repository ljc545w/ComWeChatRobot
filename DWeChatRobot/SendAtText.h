#pragma once
#include <windows.h>

BOOL __stdcall SendAtText(wchar_t *wsChatRoomId, DWORD wsWxId[], wchar_t *wsTextMsg, int length, BOOL AutoNickName);
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) void SendAtTextRemote(LPVOID lpParameter);
#endif
