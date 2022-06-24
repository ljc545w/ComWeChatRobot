#pragma once
#include "SendImage.h"
#include "SendText.h"
#include "SendFile.h"
#include "SendArticle.h"
#include "FriendList.h"
#include "SearchContact.h"
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
#include "VerifyFriendApply.h"
#include "AddFriend.h"
#include "sqlite3.h"
#include "wechatver.h"
#include "DeleteUser.h"
#include "SendAppMsg.h"

using namespace std;
#pragma comment(lib,"version.lib")
#pragma warning(disable:4731)
// 对于导出函数，需要使用此宏修饰
#define DLLEXPORT extern "C" __declspec(dllexport)

BOOL CreateConsole(void);
DWORD GetWeChatWinBase();
void Wchar_tToString(std::string& szDst, wchar_t* wchar);
string UTF8ToGBK(const std::string& strUTF8);
void HookAnyAddress(DWORD dwHookAddr, LPVOID dwJmpAddress, char* originalRecieveCode);
void UnHookAnyAddress(DWORD dwHookAddr, char* originalRecieveCode);
DLLEXPORT void UnHookAll();
wstring wreplace(wstring source, wchar_t replaced, wstring replaceto);
void PrintProcAddr();
wchar_t* GetTimeW(long long timestamp);
BOOL ProcessIsWeChat();
BOOL FindOrCreateDirectory(const wchar_t* pszPath);