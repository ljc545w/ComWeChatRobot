#pragma once
#include<windows.h>

void __stdcall SendFile(wchar_t* receiver, wchar_t* FilePath);
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) void SendFileRemote(LPVOID lpParamStruct);
#endif