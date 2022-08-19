#pragma once
#include <windows.h>
BOOL __stdcall EditRemark(wchar_t *wxid, wchar_t *remark);
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) BOOL EditRemarkRemote(LPVOID lpParameter);
#endif // !USE_SOCKET
