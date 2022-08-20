#pragma once
#include <windows.h>

BOOL __stdcall SendImage(wchar_t *receiver, wchar_t *ImagePath);
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) void SendImageRemote(LPVOID lpParamStruct);
#endif
