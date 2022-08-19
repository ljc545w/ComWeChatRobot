#include "pch.h"
#include <typeinfo>
#include <string>
#include <vector>

// 获取好友信息CALL1偏移
#define GetUserInfoCall1Offset 0x100BD5C0 - 0x10000000
// 获取好友信息CALL2偏移
#define GetUserInfoCall2Offset 0x10771980 - 0x10000000
// 获取好友信息CALL3偏移
#define GetUserInfoCall3Offset 0x104662A0 - 0x10000000
// 清理好友信息缓存参数
#define DeleteUserInfoCacheCall1Offset 0x106C52B0 - 0x10000000
// 清理好友信息缓存CALL2
#define DeleteUserInfoCacheCall2Offset 0x100BE6D0 - 0x10000000

/*
* 外部调用时的返回类型
* message：wUserInfo.c_str()
* length：wUserInfo字符串长度
*/
#ifndef USE_SOCKET
struct GetUserInfoStruct
{
    DWORD message;
    DWORD length;
} ret;
#endif

/*
* 根据缓冲区内容拼接好友信息
* address：缓冲区地址
* return：void
*/
static wstring WxUserInfo(DWORD address)
{
    wstring wUserInfo = L"";
    vector<DWORD> InfoType{
        address + 0x10,
        address + 0x24,
        address + 0x38,
        address + 0x58,
        address + 0x6C,
        address + 0xFC,
        address + 0x110,
        address + 0x19C,
        address + 0x1B0,
        address + 0x1C4,
        address + 0x1D8,
        address + 0x27C};
    vector<wchar_t *> InfoTypeName{
        (WCHAR *)L"\"wxId\"",
        (WCHAR *)L"\"wxNumber\"",
        (WCHAR *)L"\"wxV3\"",
        (WCHAR *)L"\"wxRemark\"",
        (WCHAR *)L"\"wxNickName\"",
        (WCHAR *)L"\"wxBigAvatar\"",
        (WCHAR *)L"\"wxSmallAvatar\"",
        (WCHAR *)L"\"wxSignature\"",
        (WCHAR *)L"\"wxNation\"",
        (WCHAR *)L"\"wxProvince\"",
        (WCHAR *)L"\"wxCity\"",
        (WCHAR *)L"\"wxBackground\"",
    };
    wUserInfo += L"{";
    for (unsigned int i = 0; i < InfoType.size(); i++)
    {
        wchar_t *wstemp = ((*((DWORD *)InfoType[i])) != 0) ? (WCHAR *)(*((LPVOID *)InfoType[i])) : (WCHAR *)L"null";
        wstring wsrtemp = wreplace(wstemp, L'\"', L"\\\"");
        wUserInfo = wUserInfo + InfoTypeName[i] + L":\"" + wsrtemp + L"\"";
        if (i != InfoType.size() - 1)
        {
            wUserInfo += L",";
        }
    }
    wUserInfo += L"}";
#ifdef _DEBUG
    wcout.imbue(locale("chs"));
    wcout << wUserInfo.c_str() << endl;
#endif
    return wUserInfo;
}

/*
* 供外部调用的获取好友信息接口
* lparamter：保存好友wxid的地址
* return：DWORD，`ret`的首地址
*/
#ifndef USE_SOCKET
DWORD GetWxUserInfoRemote(LPVOID lparamter)
{
    wchar_t *userwxid = (wchar_t *)lparamter;

    wstring wUserInfo = GetUserInfoByWxId(userwxid);

    ZeroMemory(&ret, sizeof(GetUserInfoStruct));
    wchar_t *message = new wchar_t[wUserInfo.length() + 1];
    memcpy(message, wUserInfo.c_str(), (wUserInfo.length() + 1) * 2);
    ret.message = (DWORD)message;
    ret.length = wUserInfo.length();
    return (DWORD)&ret;
}
#endif
/*
* 供外部调用的清空好友信息缓存的接口
* return：void
*/
#ifndef USE_SOCKET
VOID DeleteUserInfoCacheRemote()
{
    if (ret.length)
    {
        delete[](wchar_t *) ret.message;
        ZeroMemory(&ret, sizeof(GetUserInfoStruct));
    }
}
#endif

/*
* 根据wxid获取好友信息的具体实现
* wxid：好友wxid
* return：wstring，成功返回好友信息，失败返回空字符串
*/
wstring __stdcall GetUserInfoByWxId(wchar_t *wxid)
{
    wstring wUserInfo = L"";
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD WxGetUserInfoCall1 = WeChatWinBase + GetUserInfoCall1Offset;
    DWORD WxGetUserInfoCall2 = WeChatWinBase + GetUserInfoCall2Offset;
    DWORD WxGetUserInfoCall3 = WeChatWinBase + GetUserInfoCall3Offset;
    DWORD DeleteUserInfoCacheCall1 = WeChatWinBase + DeleteUserInfoCacheCall1Offset;
    DWORD DeleteUserInfoCacheCall2 = WeChatWinBase + DeleteUserInfoCacheCall2Offset;
    char buffer[0x3FC] = {0};
    WxString pWxid(wxid);
    DWORD address = 0;
    DWORD isSuccess = 0;
    __asm
    {
		pushad;
		call WxGetUserInfoCall1;
		lea ebx, buffer;
		push ebx;
		sub esp, 0x14;
		mov esi, eax;
		lea eax, pWxid;
		mov ecx, esp;
		push eax;
		call WxGetUserInfoCall2;
		mov ecx, esi;
		call WxGetUserInfoCall3;
		mov isSuccess, eax;
		mov address, ebx;
		popad;
    }
    if (isSuccess)
        wUserInfo = WxUserInfo(address);

    char deletebuffer[0x410] = {0};
    __asm {
		pushad;
		lea ecx, deletebuffer;
		call DeleteUserInfoCacheCall1;
		push eax;
		lea ebx,buffer;
		mov ecx, ebx;
		call DeleteUserInfoCacheCall2;
		popad;
    }
    return wUserInfo;
}

/*
* 根据wxid获取联系人昵称，主要用于发送艾特消息接口
* wxid：联系人wxid
* return：wchar_t*，获取到的wxid
*/
wstring __stdcall GetUserNickNameByWxId(wchar_t *wxid)
{
    wstring wstr;
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD WxGetUserInfoCall1 = WeChatWinBase + GetUserInfoCall1Offset;
    DWORD WxGetUserInfoCall2 = WeChatWinBase + GetUserInfoCall2Offset;
    DWORD WxGetUserInfoCall3 = WeChatWinBase + GetUserInfoCall3Offset;
    DWORD DeleteUserInfoCacheCall1 = WeChatWinBase + DeleteUserInfoCacheCall1Offset;
    DWORD DeleteUserInfoCacheCall2 = WeChatWinBase + DeleteUserInfoCacheCall2Offset;
    char buffer[0x3FC] = {0};
    WxString pWxid(wxid);
    DWORD address = 0;
    DWORD isSuccess = 0;
    __asm
    {
		pushad;
		call WxGetUserInfoCall1;
		lea ebx, buffer;
		push ebx;
		sub esp, 0x14;
		mov esi, eax;
		lea eax, pWxid;
		mov ecx, esp;
		push eax;
		call WxGetUserInfoCall2;
		mov ecx, esi;
		call WxGetUserInfoCall3;
		mov isSuccess, eax;
		mov address, ebx;
		popad;
    }
    if (isSuccess)
    {
        DWORD length = *(DWORD *)(address + 0x6C + 0x4);
        wchar_t *buffer = new wchar_t[length + 1];
        ZeroMemory(buffer, (length + 1) * 2);
        memcpy(buffer, (wchar_t *)(*(DWORD *)(address + 0x6C)), length * 2);
        wstr = wstring(buffer);
        delete[] buffer;
        buffer = NULL;
    }
    char deletebuffer[0x410] = {0};
    __asm {
		pushad;
		lea ecx, deletebuffer;
		call DeleteUserInfoCacheCall1;
		push eax;
		lea ebx, buffer;
		mov ecx, ebx;
		call DeleteUserInfoCacheCall2;
		popad;
    }
    return wstr;
}
