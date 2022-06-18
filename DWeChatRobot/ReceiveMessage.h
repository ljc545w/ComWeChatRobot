#pragma once
#include<windows.h>
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) DWORD GetHeadMessage();
extern "C" __declspec(dllexport) VOID PopHeadMessage();
extern "C" __declspec(dllexport) VOID HookReceiveMessage();
extern "C" __declspec(dllexport) VOID UnHookReceiveMessage();
extern "C" __declspec(dllexport) void UnHookVoiceMsg();
extern "C" __declspec(dllexport) BOOL HookVoiceMsgRemote(LPVOID lpParameter);
extern "C" __declspec(dllexport) void UnHookImageMsg();
extern "C" __declspec(dllexport) BOOL HookImageMsgRemote(LPVOID lpParameter);
#else
VOID HookReceiveMessage();
VOID UnHookReceiveMessage();
void UnHookImageMsg();
void UnHookVoiceMsg();
#endif

void __stdcall HookVoiceMsg();
void __stdcall HookImageMsg();