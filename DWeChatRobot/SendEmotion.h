#pragma once
#include <windows.h>

BOOL __stdcall SendEmotion(wchar_t *wxid, wchar_t *img_path);

#ifndef USE_SOCKET
extern "C" __declspec(dllexport) BOOL SendEmotionRemote(LPVOID lparameter);
#endif
