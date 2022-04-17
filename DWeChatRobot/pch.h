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
#include<windows.h>
#include<iostream>
#include "SendImage.h"
#include "SendText.h"
#include "SendFile.h"
#include "SendArticle.h"
#include "FriendList.h"
#include "UserInfo.h"
#include "SelfInfo.h"
#include "SendCard.h"
#include "CheckFriendStatus.h"
#include "LogMsgInfo.h"
#include "ReceiveMessage.h"
#include "SendAtText.h"
#include "GetChatRoomMembers.h"
#include "GetDbHandles.h"
#include "DbExecuteSql.h"
#include "DbBackup.h"
#include <vector>
#endif //PCH_H

using namespace std;
#define DLLEXPORT extern "C" __declspec(dllexport)

struct WxBaseStruct
{
    wchar_t* buffer;
    DWORD length;
    DWORD maxLength;
    DWORD fill1;
    DWORD fill2;

    WxBaseStruct(wchar_t* pStr) {
        buffer = pStr;
        length = wcslen(pStr);
        maxLength = wcslen(pStr) * 2;
        fill1 = 0x0;
        fill2 = 0x0;
    }
};

struct WxString
{
    wchar_t* buffer = NULL;
    DWORD length = 0;
    DWORD maxLength = 0;
    DWORD fill1 = 0;
    DWORD fill2 = 0;
};

BOOL CreateConsole(void);
DWORD GetWeChatWinBase();
void Wchar_tToString(std::string& szDst, wchar_t* wchar);
string UTF8ToGBK(const std::string& strUTF8);
void HookAnyAddress(DWORD dwHookAddr, LPVOID dwJmpAddress, char* originalRecieveCode);
void UnHookAnyAddress(DWORD dwHookAddr, char* originalRecieveCode);
DLLEXPORT void UnHookAll();
wstring wreplace(wstring source, wchar_t replaced, wstring replaceto);
