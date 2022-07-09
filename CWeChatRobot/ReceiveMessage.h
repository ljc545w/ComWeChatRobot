#pragma once
#include<windows.h>
BOOL StartReceiveMessage(int port);
BOOL StopReceiveMessage();

BOOL HookImageMsg(wchar_t* savepath);
BOOL HookVoiceMsg(wchar_t* savepath);
void UnHookImageMsg();
void UnHookVoiceMsg();