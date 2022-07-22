#pragma once
#include<windows.h>
BOOL StartReceiveMessage(DWORD pid,int port);
BOOL StopReceiveMessage(DWORD pid);

BOOL HookImageMsg(DWORD pid,wchar_t* savepath);
BOOL HookVoiceMsg(DWORD pid,wchar_t* savepath);
void UnHookImageMsg(DWORD pid);
void UnHookVoiceMsg(DWORD pid);