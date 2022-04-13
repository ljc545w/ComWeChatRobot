#pragma once
#include<windows.h>

void __stdcall SendAtText(wchar_t* wsChatRoomId, DWORD wsWxId[], wchar_t* wsTextMsg, int length);
extern "C" __declspec(dllexport) void SendAtTextRemote(LPVOID lpParameter);