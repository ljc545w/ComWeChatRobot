#pragma once
#include<windows.h>
extern "C" __declspec(dllexport) DWORD GetHeadMessage();
extern "C" __declspec(dllexport) VOID PopHeadMessage();

extern "C" __declspec(dllexport) VOID HookReceiveMessage();
extern "C" __declspec(dllexport) VOID UnHookReceiveMessage();

void __stdcall HookVoiceMsg();
extern "C" __declspec(dllexport) void UnHookVoiceMsg();
extern "C" __declspec(dllexport) BOOL HookVoiceMsgRemote(LPVOID lpParameter);

void __stdcall HookImageMsg();
extern "C" __declspec(dllexport) void UnHookImageMsg();
extern "C" __declspec(dllexport) BOOL HookImageMsgRemote(LPVOID lpParameter);