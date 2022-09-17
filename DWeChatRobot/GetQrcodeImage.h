#pragma once
#include <windows.h>
BOOL __stdcall SwitchToQrcodeLogin();
void HookQrcodeImage();
void UnHookQrcodeImage();

#ifndef USE_SOCKET
extern "C" __declspec(dllexport) DWORD GetQrcodeImageRemote();
#else
BYTE *__stdcall GetQrcodeImage(int &size);
#endif
