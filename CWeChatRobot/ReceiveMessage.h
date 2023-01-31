#pragma once
#include <windows.h>
#include <iostream>
using namespace std;
BOOL StartReceiveMessage(DWORD pid, int port);
BOOL StopReceiveMessage(DWORD pid);

BOOL HookImageMsg(DWORD pid, wchar_t *savepath);
BOOL HookVoiceMsg(DWORD pid, wchar_t *savepath);
void UnHookImageMsg(DWORD pid);
void UnHookVoiceMsg(DWORD pid);
wstring GetMsgCND(DWORD pid, ULONG64 msgid);
