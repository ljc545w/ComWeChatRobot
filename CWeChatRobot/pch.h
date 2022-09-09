// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。
#pragma once
#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#include "framework.h"

#endif //PCH_H
#include <iostream>
#include "stdlib.h"
#include <tchar.h>
#include <stdio.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <atlconv.h>
#include <tchar.h>
#include <sys/stat.h>
#include <direct.h>
#include <vector>
#include <map>

#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")

#include "robotdata.h"
#include "templatefunc.h"

using namespace std;

#define TEXTLENGTHW(buffer) buffer ? (wcslen(buffer) * 2 + 2) : 0
#define TEXTLENGTHA(buffer) buffer ? (strlen(buffer) + 1) : 0

#ifdef _UNICODE
#define tstring std::wstring
#define TEXTLENGTH TEXTLENGTHW
#else
#define tstring std::string
#define TEXTLENGTH TEXTLENGTHW
#endif

BOOL isFileExists_stat(string &name);
DWORD GetWeChatRobotBase(DWORD pid);
DWORD GetWeChatWinBase(DWORD pid);
DWORD GetWeChatPid();
DWORD StartRobotService(DWORD pid);
DWORD StopRobotService(DWORD pid);
string unicode_to_utf8(wchar_t *wstr);
wstring gb2312_to_unicode(const char *buffer);

BOOL CreateConsole();
wstring GetComWorkPath();

tstring GetWeChatInstallDir();
DWORD GetWeChatVerInt();
tstring GetWeChatVerStr();
DWORD StartWeChat();
BOOL CloseProcessHandle(DWORD pid, wchar_t *handlename);
DWORD GetRemoteProcAddr(DWORD pid, LPWSTR modulename, LPSTR procname);
