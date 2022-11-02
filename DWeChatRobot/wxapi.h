#pragma once
#include "wxsignal.h"
#include "SendImage.h"
#include "SendText.h"
#include "SendFile.h"
#include "SendArticle.h"
#include "SendXmlMsg.h"
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
#include "EditRemark.h"
#include "DelChatRoomMember.h"
#include "AddChatRoomMember.h"
#include "SetChatRoomAnnouncement.h"
#include "SetChatRoomSelfNickname.h"
#include "SetChatRoomName.h"
#include "GetChatRoomMemberNickname.h"
#include "OpenBrowser.h"
#include "GetHistoryPublicMsg.h"
#include "ForwardMessage.h"
#include "GetQrcodeImage.h"
#include "GetA8Key.h"
#include "Logout.h"
#include "GetTransfer.h"
#include "SendEmotion.h"

using namespace std;
#pragma comment(lib, "version.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4731 26812)
// 对于导出函数，需要使用此宏修饰
#define DLLEXPORT extern "C" __declspec(dllexport)

BOOL CreateConsole(void);
DWORD GetWeChatWinBase();
string unicode_to_gb2312(wchar_t *wchar);
string unicode_to_utf8(wchar_t *wstr);

string utf8_to_gb2312(const char *strUTF8);
wstring utf8_to_unicode(const char *buffer);

string gb2312_to_utf8(const char *strGB2312);
wstring gb2312_to_unicode(const char *buffer);

void HookAnyAddress(DWORD dwHookAddr, LPVOID dwJmpAddress, char *originalRecieveCode);
void UnHookAnyAddress(DWORD dwHookAddr, char *originalRecieveCode);
DLLEXPORT void UnHookAll();
wstring wreplace(wstring source, wchar_t replaced, wstring replaceto);
void PrintProcAddr();
wstring GetTimeW(long long timestamp);
BOOL ProcessIsWeChat();
BOOL FindOrCreateDirectory(const wchar_t *pszPath);
void gLogInit();

template <typename T1, typename T2>
vector<T1> split(T1 str, T2 letter)
{
    vector<T1> arr;
    size_t pos;
    while ((pos = str.find_first_of(letter)) != T1::npos)
    {
        T1 str1 = str.substr(0, pos);
        arr.push_back(str1);
        str = str.substr(pos + 1, str.length() - pos - 1);
    }
    arr.push_back(str);
    return arr;
}

template <typename T1, typename T2>
T1 replace(T1 source, T2 replaced, T1 replaceto)
{
    vector<T1> v_arr = split(source, replaced);
    if (v_arr.size() < 2)
        return source;
    T1 temp;
    for (unsigned int i = 0; i < v_arr.size() - 1; i++)
    {
        temp += v_arr[i];
        temp += replaceto;
    }
    temp += v_arr[v_arr.size() - 1];
    return temp;
}
