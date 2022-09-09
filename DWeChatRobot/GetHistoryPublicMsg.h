#pragma once
#include <windows.h>
#include <iostream>
using namespace std;
void HookH5ExtBuf();
void UnHookH5ExtBuf();
BOOL __stdcall GetHistoryPublicMsg(wchar_t *PublicId, wchar_t *Offset);
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) DWORD GetHistoryPublicMsgRemote(LPVOID param);
#else
string __stdcall GetHistoryPublicMsg(wstring PublicId, wstring Offset);
#endif
