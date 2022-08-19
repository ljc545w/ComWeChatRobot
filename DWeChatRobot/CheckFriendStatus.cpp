#include "pch.h"

// 检查好友状态CALL1偏移
#define CheckFriendStatusCall1Offset 0x78784540 - 0x786A0000
// 检查好友状态CALL2偏移
#define CheckFriendStatusCall2Offset 0x78E11890 - 0x786A0000
// 检查好友状态CALL3偏移
#define CheckFriendStatusCall3Offset 0x78E11980 - 0x786A0000
// 检查好友状态CALL4偏移
#define CheckFriendStatusCall4Offset 0x78B03970 - 0x786A0000
// 检查好友状态参数偏移
#define CheckFriendStatusParamOffset 0x7AA068F4 - 0x786A0000

// 好友状态码HOOK地址偏移
#define CheckFriendStatusHookOffset 0x10587373 - 0x10000000
// HOOK的CALL偏移
#define CheckFriendStatusNextCallOffset 0x10587410 - 0x10000000
// HOOK跳转的地址偏移
#define CheckFriendStatusHookJmpBackOffset 0x10587378 - 0x10000000

// HOOK的CALL地址
DWORD CheckFriendStatusNextCallAddress = GetWeChatWinBase() + CheckFriendStatusNextCallOffset;
// HOOK跳转的地址
DWORD CheckFriendStatusHookJmpBackAddress = GetWeChatWinBase() + CheckFriendStatusHookJmpBackOffset;

// 保存HOOK前的字节码，用于恢复
char OldAsmCode[5] = {0};
// 是否HOOK标志
BOOL CheckFriendStatusHooked = false;
// 保存好友状态码并作为调用返回
DWORD LocalFriendStatus = 0x0;

/*
* 用于内存中平衡堆栈
*/
struct FriendStatusParamStruct
{
    DWORD fill0 = 0x0;
    DWORD fill1 = 0x0;
    DWORD fill2 = -0x1;
    DWORD fill3 = 0x0;
    DWORD fill4 = 0x0;
    DWORD fill5 = 0xF;
    char nullbuffer[0xC] = {0};
};

/*
* 处理函数，参数不在状态码范围则不处理
* result：好友状态码
* return：void
*/
void dealVerifyUserResult(DWORD result)
{
    if (result < 0xB0 || result > 0xB5)
        return;
    LocalFriendStatus = result;
}

/*
* HOOK的具体实现，记录状态码并跳转到处理函数
*/
__declspec(naked) void doHookVerifyUserResult()
{
    __asm {
		pushfd;
		pushad;
		mov eax, [esi];
		push eax;
		call dealVerifyUserResult;
		add esp, 0x4;
		popad;
		popfd;
		call CheckFriendStatusNextCallAddress;
		jmp CheckFriendStatusHookJmpBackAddress;
    }
}

/*
* 开始HOOK好友状态
* return：void
*/
VOID HookFriendStatusCode()
{
    if (CheckFriendStatusHooked)
        return;
    DWORD WeChatWinBase = GetWeChatWinBase();
    CheckFriendStatusNextCallAddress = WeChatWinBase + CheckFriendStatusNextCallOffset;
    CheckFriendStatusHookJmpBackAddress = WeChatWinBase + CheckFriendStatusHookJmpBackOffset;
    DWORD dwHookAddress = WeChatWinBase + CheckFriendStatusHookOffset;
    HookAnyAddress(dwHookAddress, doHookVerifyUserResult, OldAsmCode);
    CheckFriendStatusHooked = true;
}

/*
* 取消HOOK好友状态
* return：void
*/
VOID UnHookFriendStatusCode()
{
    if (!CheckFriendStatusHooked)
        return;
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD dwHookAddress = WeChatWinBase + CheckFriendStatusHookOffset;
    UnHookAnyAddress(dwHookAddress, OldAsmCode);
    CheckFriendStatusHooked = false;
}

#ifndef USE_SOCKET
/*
* 供外部调用的检查好友状态接口，检查并返回状态码
* lparameter：要检查的联系人wxid保存地址
* return：DWORD，好友状态码
*/
DWORD CheckFriendStatusRemote(LPVOID lparameter)
{
    CheckFriendStatus((wchar_t *)lparameter);
    return LocalFriendStatus;
}
#else
DWORD __stdcall CheckFriendStatus(wstring wxid)
{
    CheckFriendStatus((LPWSTR)wxid.c_str());
    return LocalFriendStatus;
}
#endif

/*
* 检查好友状态的具体实现
* wxid：要检查的联系人wxid
* return：void
*/
VOID __stdcall CheckFriendStatus(wchar_t *wxid)
{
    if (!CheckFriendStatusHooked)
        HookFriendStatusCode();
    LocalFriendStatus = 0x0;
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD CheckFriendStatusCall1 = WeChatWinBase + CheckFriendStatusCall1Offset;
    DWORD CheckFriendStatusCall2 = WeChatWinBase + CheckFriendStatusCall2Offset;
    DWORD CheckFriendStatusCall3 = WeChatWinBase + CheckFriendStatusCall3Offset;
    DWORD CheckFriendStatusCall4 = WeChatWinBase + CheckFriendStatusCall4Offset;
    DWORD CheckFriendStatusParam = WeChatWinBase + CheckFriendStatusParamOffset;

    WxString pwxid(wxid);
    FriendStatusParamStruct FriendStatusParam;

    char *swxid = new char[wcslen(wxid) + 1];
    ZeroMemory(swxid, wcslen(wxid) + 1);
    WideCharToMultiByte(CP_ACP, 0, wxid, -1, swxid, wcslen(wxid), NULL, NULL);
    pwxid.fill1 = (DWORD)swxid;
    pwxid.fill2 = wcslen(wxid);

    wchar_t *message = (WCHAR *)L"我是";

    __asm {
		pushad;
		pushfd;
		mov edi, 0x6;
		mov esi, 0x0;
		sub esp, 0x18;
		mov ecx, esp;
		mov dword ptr[ecx], 0x0;
		mov dword ptr[ecx + 0x14], 0xF;
		mov dword ptr[ecx + 0x10], 0x0;
		sub esp, 0x18;
		lea eax, FriendStatusParam;
		mov ecx, esp;
		push eax;
		call CheckFriendStatusCall1;
		push esi;
		push edi;
		mov edi, message;
		sub esp, 0x14;
		mov ecx, esp;
		push -0x1;
		mov eax, edi;
		push eax;
		call CheckFriendStatusCall2;
        // 这里改成0x2就是添加好友，0x1是请求好友状态
		push 0x1;
		lea eax, pwxid;
		sub esp, 0x14;
		mov ecx, esp;
		push eax;
		call CheckFriendStatusCall3;
		mov eax, [CheckFriendStatusParam];
		mov eax, [eax];
		mov ecx, eax;
		call CheckFriendStatusCall4;
		popfd;
		popad;
    }
    while (!LocalFriendStatus && CheckFriendStatusHooked)
    {
        Sleep(10);
    }
#ifdef _DEBUG
    printf("wxid:%ws,status:0x%02X\n", wxid, LocalFriendStatus);
#endif
    delete[] swxid;
    swxid = NULL;
    return;
}
