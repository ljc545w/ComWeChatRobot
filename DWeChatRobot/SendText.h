#pragma once
#include<windows.h>

void __stdcall SendText(wchar_t* wsWxId, wchar_t* wsTextMsg);
extern "C" __declspec(dllexport) void SendTextRemote(LPVOID lpParameter);