#pragma once
#include <windows.h>
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) VOID HookReceiveMessage(int port);
extern "C" __declspec(dllexport) VOID UnHookReceiveMessage();
extern "C" __declspec(dllexport) void UnHookVoiceMsg();
extern "C" __declspec(dllexport) BOOL HookVoiceMsgRemote(LPVOID lpParameter);
extern "C" __declspec(dllexport) void UnHookImageMsg();
extern "C" __declspec(dllexport) BOOL HookImageMsgRemote(LPVOID lpParameter);
#else
VOID HookReceiveMessage(int port);
VOID UnHookReceiveMessage();
void UnHookImageMsg();
void UnHookVoiceMsg();
BOOL __stdcall HookVoiceMsg(wstring save_path);
BOOL __stdcall HookImageMsg(wstring save_path);
#endif

void __stdcall HookVoiceMsg();
void __stdcall HookImageMsg();
