#pragma once
#include<windows.h>

void __stdcall SendAtText(wchar_t* wxChatRoomId, wchar_t* wsWxId, wchar_t* wsTextMsg);
extern "C" __declspec(dllexport) void SendAtTextRemote(LPVOID lpParameter);