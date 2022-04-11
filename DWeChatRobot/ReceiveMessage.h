#pragma once
#include<windows.h>
extern "C" __declspec(dllexport) DWORD GetHeadMessage();
extern "C" __declspec(dllexport) VOID PopHeadMessage();

extern "C" __declspec(dllexport) VOID HookReceiveMessage();
extern "C" __declspec(dllexport) VOID UnHookReceiveMessage();