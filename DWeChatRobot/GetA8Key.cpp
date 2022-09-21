#include "pch.h"
#include "json/json.hpp"
using namespace nlohmann;

#define GetBufferCallOffset 0x79C2F9FD - 0x78220000

#define GetA8KeyCall1Offset 0x78991980 - 0x78220000
#define GetA8KeyCall2Offset 0x78801650 - 0x78220000
#define GetA8KeyCall3Offset 0x784B98F0 - 0x78220000
#define GetA8KeyCall4Offset 0x783441F0 - 0x78220000
#define GetA8KeyCall5Offset 0x787B9490 - 0x78220000
#define DelNetSceneA8KeyCallOffset 0x105E1B40 - 0x10000000

#define HookA8KeyAddrOffset 0x78BD368E - 0x785F0000
#define HookA8KeyJmpBackAddrOffset 0x78BD3704 - 0x785F0000

static BOOL A8KeyBufHooked = FALSE;
static char A8KeyBufOldAsmCode[5] = {0};
static DWORD HookAddr = 0;
static DWORD JmpBackAddr = 0;
static string response = "";
static DWORD SignalThreadId = 0;

void GetA8KeyBuf(DWORD addr)
{
    json jData;
    DWORD p_url = *(DWORD *)(addr + 0x8);
    int length = *(int *)(p_url + 0x10);
    string full_url;
    if (length == 0)
    {
        full_url = "";
    }
    else
    {
        char *buffer = (length <= 0xF) ? (char *)p_url : (char *)(*(DWORD *)p_url);
        full_url = string(buffer, length);
    }
    jData["full_url"] = full_url;
    vector<string> arr_1 = split(full_url, '?');
    if (arr_1.size() > 1)
    {
        vector<string> params = split(arr_1[1], '&');
        for (auto param : params)
        {
            vector<string> temp = split(param, '=');
            if (temp.size() > 1)
            {
                string key = temp[0], value = temp[1];
                if (key == "__biz")
                {
                    value += "==";
                }
                else if (key == "uin")
                {
                    value = value.substr(0, value.length() - 6) + "==";
                    jData["wxuin"] = base64_decode(value.c_str());
                }
                jData[key] = value;
            }
        }
    }
#ifndef USE_SOCKET
    response = utf8_to_gb2312(jData.dump().c_str());
#else
    response = jData.dump();
#endif
    SIGNAL(SignalThreadId, WM_WAIT_HOOK_DATA);
}

_declspec(naked) void dealA8KeyBuf()
{
    __asm {
        pushad;
        pushfd;
        mov eax,dword ptr ds:[ebp - 0x4C];
        push eax;
        call GetA8KeyBuf;
        add esp,0x4;
        popfd;
        popad;
        mov edi,dword ptr ds:[ebp - 0x5C];
        jmp JmpBackAddr;
    }
}

void HookA8KeyBuf()
{
    if (A8KeyBufHooked)
        return;
    DWORD WeChatWinBase = GetWeChatWinBase();
    HookAddr = WeChatWinBase + HookA8KeyAddrOffset;
    JmpBackAddr = WeChatWinBase + HookA8KeyJmpBackAddrOffset;
    HookAnyAddress(HookAddr, (LPVOID)dealA8KeyBuf, A8KeyBufOldAsmCode);
    A8KeyBufHooked = TRUE;
}

void UnHookA8KeyBuf()
{
    if (!A8KeyBufHooked)
        return;
    UnHookAnyAddress(HookAddr, A8KeyBufOldAsmCode);
    A8KeyBufHooked = FALSE;
}

BOOL __stdcall GetA8Key(wchar_t *url)
{
    HookA8KeyBuf();
    response = "";
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD GetBufferCall = WeChatWinBase + GetBufferCallOffset;

    DWORD GetA8KeyCall1 = WeChatWinBase + GetA8KeyCall1Offset;
    DWORD GetA8KeyCall2 = WeChatWinBase + GetA8KeyCall2Offset;
    DWORD GetA8KeyCall3 = WeChatWinBase + GetA8KeyCall3Offset;
    DWORD GetA8KeyCall4 = WeChatWinBase + GetA8KeyCall4Offset;
    DWORD GetA8KeyCall5 = WeChatWinBase + GetA8KeyCall5Offset;

    DWORD DelNetSceneA8KeyCall = WeChatWinBase + DelNetSceneA8KeyCallOffset;
    DWORD NetSceneA8Key = 0;
    DWORD param[2] = {0};
    WxString p_url(url);
    WxString p_null(NULL);
    WxSignal sg(WM_WAIT_HOOK_DATA, SignalThreadId);
    int isSuccess = 0;
    __asm {
        pushad;
        pushfd;
        mov ebx,0x7;
        push 0x238;
        call GetBufferCall;
        mov edi,eax;
        add esp,0x4;
        mov NetSceneA8Key,edi;
        push 0;
        sub esp,0x14;
        mov esi,0x1;
        lea eax,p_null;
        mov ecx,esp;
        push eax;
        call GetA8KeyCall1;
        push 0x2;
        push esi;
        push 0;
        lea eax,p_url;
        push ebx;
        sub esp,0x14;
        mov ecx,esp;
        push eax;
        call GetA8KeyCall1;
        mov ecx,edi;
        call GetA8KeyCall2;
        push eax;
        lea ecx,param;
        call GetA8KeyCall3;
        call GetA8KeyCall4;
        lea esi,param;
        mov esi,dword ptr [esi + 0x4];
        sub esp,0x8;
        mov ecx,esp;
        mov edx,eax;
        mov dword ptr [ecx],0x0;
        mov dword ptr [ecx + 0x4],0x0;
        inc dword ptr [esi + 0x4];
        mov eax,dword ptr [param];
        mov dword ptr [ecx],eax;
        mov dword ptr [ecx + 0x4],esi;
        mov ecx,edx;
        call GetA8KeyCall5;
        movzx eax,al;
        mov isSuccess,eax;
        popfd;
        popad;
    }
    sg.wait(5000);
    // 必须等待一定时长才可以析构，不然会出现指针悬挂
    Sleep(100);
    __asm {
        pushad;
        pushfd;
        push 0x1;
        mov ecx, dword ptr [NetSceneA8Key];
        call DelNetSceneA8KeyCall;
        popfd;
        popad;
    }
    return isSuccess == 1;
}

#ifndef USE_SOCKET
struct GetA8KeyStruct
{
    DWORD response;
    DWORD length;
} static ret = {0};

DWORD GetA8KeyRemote(wchar_t *url)
{
    if (url == NULL || wcslen(url) == 0)
        return 0;
    BOOL isSuccess = GetA8Key(url);
    if (!isSuccess || response.length() == 0)
        return 0;
    ret.response = (DWORD)response.c_str();
    ret.length = response.length();
    return (DWORD)&ret;
}
#else
string __stdcall GetA8Key(wstring url)
{
    if (url.length() == 0)
        return "";
    BOOL isSuccess = GetA8Key((wchar_t *)url.c_str());
    if (!isSuccess)
        return "";
    return response;
}
#endif
