#pragma once
#include <windows.h>

BOOL __stdcall GetTransfer(wchar_t *wxid, wchar_t *transcationid, wchar_t *transferid);

#ifndef USE_SOCKET
extern "C" __declspec(dllexport) BOOL GetTransferRemote(LPVOID lpParameter);
#endif
