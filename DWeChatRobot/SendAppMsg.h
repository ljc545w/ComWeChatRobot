#pragma once
#include<windows.h>
BOOL __stdcall SendAppMsg(wchar_t* wxid, wchar_t* appid);
extern "C" __declspec(dllexport) BOOL SendAppMsgRemote(LPVOID lpParameter);