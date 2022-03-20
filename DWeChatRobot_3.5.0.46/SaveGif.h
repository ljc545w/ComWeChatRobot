#pragma once
#include <windows.h>

extern "C" __declspec(dllexport) void HookExtractExpression();
void __stdcall OutputExpression(DWORD dwExpressionAddr);
void UnHookExtractExpression();