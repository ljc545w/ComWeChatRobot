#pragma once
#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <TlHelp32.h>
#include "driverdata.h"
#include "templatefunc.h"
using namespace std;
#pragma warning(disable : 4311; disable : 4302; disable : 4312)
#define DLLEXPORT extern "C" __declspec(dllexport)

#define TEXTLENGTHW(buffer) buffer ? (wcslen(buffer) * 2 + 2) : 0
#define TEXTLENGTHA(buffer) buffer ? (strlen(buffer) + 1) : 0

#ifdef _UNICODE
#define tstring std::wstring
#define TEXTLENGTH TEXTLENGTHW
#else
#define tstring std::string
#define TEXTLENGTH TEXTLENGTHW
#endif

#ifdef _WIN64
PVOID GetSystem32ProcAddr(PCWSTR ObjectName, PCSTR procName);
#endif

BOOL InjectDll(DWORD pid, const wchar_t *szPath);
BOOL RemoveDll(DWORD pid);
BOOL CreateConsole();
BOOL isFileExists_stat(wstring name);
BOOL CloseProcessHandle(DWORD pid, wchar_t *handlename);
tstring GetWeChatVersion();
tstring GetWeChatInstallDir();

DLLEXPORT BOOL start_listen(DWORD pid, int port);
DLLEXPORT BOOL stop_listen(DWORD pid);
DLLEXPORT DWORD new_wechat();
