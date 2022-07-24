#pragma once
#include<windows.h>
#include<iostream>
#include "driverdata.h"
using namespace std;
#pragma warning(disable:4311;disable:4302;disable:4312)
#define DLLEXPORT extern "C" __declspec(dllexport)

#ifdef _WIN64
	PVOID GetSystem32ProcAddr(PCWSTR ObjectName, PCSTR procName);
#endif

BOOL CloseProcessHandle(DWORD pid, wchar_t* handlename);
BOOL InjectDll(DWORD dwId, const wchar_t* szPath);
BOOL RemoveDll(DWORD dwId, PCWSTR dllname);

BOOL CreateConsole();
BOOL isFileExists_stat(wstring name);
DWORD GetWeChatPid();

extern HANDLE GlobalProcess;
extern PVOID pRemoteGetProc;

DLLEXPORT BOOL StartRobotService();
DLLEXPORT BOOL StopRobotService();