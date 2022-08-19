#include "pch.h"

#define SearchContactByNetCall1Offset 0x102FF150 - 0x10000000
#define SearchContactByNetCall2Offset 0x1050F9C0 - 0x10000000

#define HookSearchContactErrcodeAddrOffset 0x106397C4 - 0x10000000
#define HookSearchContactErrcodeNextCallOffset 0x10770170 - 0x10000000

#define HookUserInfoAddrOffset 0x1031B520 - 0x10000000
#define HookUserInfoNextCallOffset 0x1031B770 - 0x10000000

static BOOL SearchContactHooked = false;
static char HookSearchContactErrcodeOldAsm[5] = {0};
static char HookUserInfoOldAsm[5] = {0};
static DWORD WeChatWinBase = GetWeChatWinBase();

static UserInfo userinfo;

DWORD HookSearchContactErrcodeNextCall = WeChatWinBase + HookSearchContactErrcodeNextCallOffset;
DWORD HookSearchContactErrcodeAddr = WeChatWinBase + HookSearchContactErrcodeAddrOffset;
DWORD HookSearchContactErrcodeJmpBackAddr = HookSearchContactErrcodeAddr + 0x5;

DWORD HookUserInfoNextCall = WeChatWinBase + HookUserInfoNextCallOffset;
DWORD HookUserInfoAddr = WeChatWinBase + HookUserInfoAddrOffset;
DWORD HookUserInfoJmpBackAddr = HookUserInfoAddr + 0x5;

void GetNetUserInfoFromMemory(DWORD address)
{
    DWORD length = *(DWORD *)(address + 0x8);
    userinfo.keyword = new wchar_t[length + 1];
    userinfo.l_keyword = length;
    if (length)
        memcpy(userinfo.keyword, (wchar_t *)(*(DWORD *)(address + 0x4)), (length + 1) * sizeof(wchar_t));
    else
        ZeroMemory(userinfo.keyword, (length + 1) * sizeof(wchar_t));

    length = *(DWORD *)(address + 0x1C);
    userinfo.v3 = new wchar_t[length + 1];
    userinfo.l_v3 = length;
    if (length)
        memcpy(userinfo.v3, (wchar_t *)(*(DWORD *)(address + 0x18)), (length + 1) * sizeof(wchar_t));
    else
        ZeroMemory(userinfo.v3, (length + 1) * sizeof(wchar_t));

    length = *(DWORD *)(address + 0x30);
    userinfo.BigAvatar = new wchar_t[length + 1];
    userinfo.l_BigAvatar = length;
    if (length)
        memcpy(userinfo.BigAvatar, (wchar_t *)(*(DWORD *)(address + 0x2C)), (length + 1) * sizeof(wchar_t));
    else
        ZeroMemory(userinfo.BigAvatar, (length + 1) * sizeof(wchar_t));

    length = *(DWORD *)(address + 0xC8);
    userinfo.NickName = new wchar_t[length + 1];
    userinfo.l_NickName = length;
    if (length)
        memcpy(userinfo.NickName, (wchar_t *)(*(DWORD *)(address + 0xC4)), (length + 1) * sizeof(wchar_t));
    else
        ZeroMemory(userinfo.NickName, (length + 1) * sizeof(wchar_t));

    length = *(DWORD *)(address + 0x108);
    userinfo.v2 = new wchar_t[length + 1];
    userinfo.l_v2 = length;
    if (length)
        memcpy(userinfo.v2, (wchar_t *)(*(DWORD *)(address + 0x104)), (length + 1) * sizeof(wchar_t));
    else
        ZeroMemory(userinfo.v2, (length + 1) * sizeof(wchar_t));

    length = *(DWORD *)(address + 0x16C);
    userinfo.SmallAvatar = new wchar_t[length + 1];
    userinfo.l_SmallAvatar = length;
    if (length)
        memcpy(userinfo.SmallAvatar, (wchar_t *)(*(DWORD *)(address + 0x168)), (length + 1) * sizeof(wchar_t));
    else
        ZeroMemory(userinfo.SmallAvatar, (length + 1) * sizeof(wchar_t));

    length = *(DWORD *)(address + 0x1F8);
    userinfo.Signature = new wchar_t[length + 1];
    userinfo.l_Signature = length;
    if (length)
        memcpy(userinfo.Signature, (wchar_t *)(*(DWORD *)(address + 0x1F4)), (length + 1) * sizeof(wchar_t));
    else
        ZeroMemory(userinfo.Signature, (length + 1) * sizeof(wchar_t));

    length = *(DWORD *)(address + 0x20C);
    userinfo.Nation = new wchar_t[length + 1];
    userinfo.l_Nation = length;
    if (length)
        memcpy(userinfo.Nation, (wchar_t *)(*(DWORD *)(address + 0x208)), (length + 1) * sizeof(wchar_t));
    else
        ZeroMemory(userinfo.Nation, (length + 1) * sizeof(wchar_t));

    length = *(DWORD *)(address + 0x220);
    userinfo.Province = new wchar_t[length + 1];
    userinfo.l_Province = length;
    if (length)
        memcpy(userinfo.Province, (wchar_t *)(*(DWORD *)(address + 0x21C)), (length + 1) * sizeof(wchar_t));
    else
        ZeroMemory(userinfo.Province, (length + 1) * sizeof(wchar_t));

    length = *(DWORD *)(address + 0x234);
    userinfo.City = new wchar_t[length + 1];
    userinfo.l_City = length;
    if (length)
        memcpy(userinfo.City, (wchar_t *)(*(DWORD *)(address + 0x230)), (length + 1) * sizeof(wchar_t));
    else
        ZeroMemory(userinfo.City, (length + 1) * sizeof(wchar_t));

    userinfo.sex = *(DWORD *)(address + 0x1BC);
    userinfo.over = true;
}

void ChangeSearchContactErrcode(int errcode)
{
    userinfo.errcode = errcode;
}

__declspec(naked) void dealSearchContactErrcode()
{
    __asm {
		pushad;
		pushfd;
		push edi;
		call ChangeSearchContactErrcode;
		add esp, 0x4;
		popfd;
		popad;
		call HookSearchContactErrcodeNextCall;
		jmp HookSearchContactErrcodeJmpBackAddr;
    }
}

__declspec(naked) void dealUserInfo()
{
    __asm {
		pushad;
		pushfd;
		push dword ptr [ebp + 0x14];
		call GetNetUserInfoFromMemory;
		add esp, 0x4;
		popfd;
		popad;
		call HookUserInfoNextCall;
		jmp HookUserInfoJmpBackAddr;
    }
}

static void HookSearchContact()
{
    WeChatWinBase = GetWeChatWinBase();
    if (SearchContactHooked || WeChatWinBase == 0)
        return;
    HookSearchContactErrcodeNextCall = WeChatWinBase + HookSearchContactErrcodeNextCallOffset;
    HookSearchContactErrcodeAddr = WeChatWinBase + HookSearchContactErrcodeAddrOffset;
    HookSearchContactErrcodeJmpBackAddr = HookSearchContactErrcodeAddr + 0x5;

    HookUserInfoNextCall = WeChatWinBase + HookUserInfoNextCallOffset;
    HookUserInfoAddr = WeChatWinBase + HookUserInfoAddrOffset;
    HookUserInfoJmpBackAddr = HookUserInfoAddr + 0x5;
    HookAnyAddress(HookSearchContactErrcodeAddr, (LPVOID)dealSearchContactErrcode, HookSearchContactErrcodeOldAsm);
    HookAnyAddress(HookUserInfoAddr, (LPVOID)dealUserInfo, HookUserInfoOldAsm);
    SearchContactHooked = true;
}

void UnHookSearchContact()
{
    if (!SearchContactHooked)
        return;
    UnHookAnyAddress(HookSearchContactErrcodeAddr, HookSearchContactErrcodeOldAsm);
    UnHookAnyAddress(HookUserInfoAddr, HookUserInfoOldAsm);
    SearchContactHooked = false;
}

static void DeleteUserInfoCache()
{
    if (userinfo.keyword)
    {
        delete userinfo.keyword;
    }
    if (userinfo.v2)
    {
        delete userinfo.v2;
    }
    if (userinfo.v3)
    {
        delete userinfo.v3;
    }
    if (userinfo.NickName)
    {
        delete userinfo.NickName;
    }
    if (userinfo.Nation)
    {
        delete userinfo.Nation;
    }
    if (userinfo.Province)
    {
        delete userinfo.Province;
    }
    if (userinfo.City)
    {
        delete userinfo.City;
    }
    if (userinfo.Signature)
    {
        delete userinfo.Signature;
    }
    if (userinfo.SmallAvatar)
    {
        delete userinfo.SmallAvatar;
    }
    if (userinfo.BigAvatar)
    {
        delete userinfo.BigAvatar;
    }
    ZeroMemory(&userinfo, sizeof(UserInfo));
    userinfo.errcode = 1;
}

void *__stdcall SearchContactByNet(wchar_t *keyword)
{
    HookSearchContact();
    DeleteUserInfoCache();
    DWORD SearchContactByNetCall1 = GetWeChatWinBase() + SearchContactByNetCall1Offset;
    DWORD SearchContactByNetCall2 = GetWeChatWinBase() + SearchContactByNetCall2Offset;
    WxString pkeyword(keyword);

    __asm {
		pushad;
		pushfd;
		call SearchContactByNetCall1;
		lea ebx, pkeyword;
		push ebx;
		mov ecx, eax;
		call SearchContactByNetCall2;
		popfd;
		popad;
    }
    while (userinfo.errcode == 1 && SearchContactHooked)
    {
        Sleep(50);
    }
    if (userinfo.errcode == 0)
    {
        while (userinfo.over == false && SearchContactHooked)
        {
            Sleep(50);
        }
#ifdef _DEBUG
        wcout << userinfo.v2 << endl;
        wcout << userinfo.v3 << endl;
#endif
    }
    return &userinfo;
}

#ifndef USE_SOCKET
DWORD SearchContactByNetRemote(LPVOID keyword)
{
    SearchContactByNet((wchar_t *)keyword);
    return (DWORD)&userinfo;
}
#else
map<wstring, wstring> __stdcall SearchContactByNet(wstring keyword)
{
    SearchContactByNet((wchar_t *)keyword.c_str());
    map<wstring, wstring> m_info;
    m_info[L"keyword"] = userinfo.keyword;
    m_info[L"v3"] = userinfo.v3;
    m_info[L"NickName"] = userinfo.NickName;
    m_info[L"Signature"] = userinfo.Signature;
    m_info[L"Nation"] = userinfo.Nation;
    m_info[L"Province"] = userinfo.Province;
    m_info[L"City"] = userinfo.City;
    m_info[L"BigAvatar"] = userinfo.BigAvatar;
    m_info[L"SmallAvatar"] = userinfo.SmallAvatar;
    m_info[L"Sex"] = userinfo.sex == 1 ? L"1" : L"2";
    DeleteUserInfoCache();
    return m_info;
}
#endif
