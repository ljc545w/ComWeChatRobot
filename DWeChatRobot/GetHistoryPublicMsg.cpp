#include "pch.h"
#include <sys/timeb.h>

#define GetHistoryPublicMsgCall1Offset 0x5B3F5CA0 - 0x5B050000
#define GetHistoryPublicMsgCall2Offset 0x65555C80 - 0x650A0000
#define GetHistoryPublicMsgHandleOffset 0x66C78701 - 0x650A0000

#define GetHistoryPublicHookAddrOffset 0x794EFC9E - 0x78ED0000
#define GetHistoryPublicJmpBackAddrOffset 0x794EFD14 - 0x78ED0000

static BOOL H5ExtBufHooked = FALSE;
static char H5ExtBufOldAsmCode[5] = {0};
static DWORD HookAddr = 0;
static DWORD JmpBackAddr = 0;
static string response = "";

static unsigned long long systemtime()
{
    timeb t;
    ftime(&t);
    return t.time * 1000 + t.millitm;
}

void GetResponseData(DWORD addr1, DWORD addr2)
{
    char *buffer = (char *)(*(DWORD *)addr1);
    int length = *(int *)(addr1 + 0x10);
    if (length != 0)
    {
        string json_data(buffer, length);
        response = json_data;
    }
    else
    {
        int err_msg_len = *(int *)(addr2 + 0x10);
        char *err_buffer = (err_msg_len <= 0xF) ? (char *)addr2 : (char *)(*(DWORD *)addr2);
        string err_msg(err_buffer, err_msg_len);
        response = err_msg;
    }
#ifndef USE_SOCKET
    response = utf8_to_gb2312(response.c_str());
#endif
}

_declspec(naked) void dealH5ExtBuf()
{
    __asm {
        pushad;
        pushfd;
        mov ecx,dword ptr ds:[ebp - 0x5C];
        sub ecx,0x40;
        add ecx,0x1D4;
        mov ecx,dword ptr ds:[ecx];
        add ecx,0x8;
        mov eax,dword ptr ds:[ecx];
        add eax,0x8;
        mov ecx,dword ptr ds:[eax];
        sub eax,0x4;
        mov eax,dword ptr ds:[eax];
        push eax;
        push ecx;
        call GetResponseData;
        add esp,0x8;
        popfd;
        popad;
        mov edi,dword ptr ds:[ebp - 0x5C];
        jmp JmpBackAddr;
    }
}

void HookH5ExtBuf()
{
    if (H5ExtBufHooked)
        return;
    DWORD WeChatWinBase = GetWeChatWinBase();
    HookAddr = WeChatWinBase + GetHistoryPublicHookAddrOffset;
    JmpBackAddr = WeChatWinBase + GetHistoryPublicJmpBackAddrOffset;
    HookAnyAddress(HookAddr, (LPVOID)dealH5ExtBuf, H5ExtBufOldAsmCode);
    H5ExtBufHooked = TRUE;
}

void UnHookH5ExtBuf()
{
    if (!H5ExtBufHooked)
        return;
    UnHookAnyAddress(HookAddr, H5ExtBufOldAsmCode);
    H5ExtBufHooked = FALSE;
}

BOOL __stdcall GetHistoryPublicMsg(wchar_t *PublicId, wchar_t *Offset)
{
    response = "";
    HookH5ExtBuf();
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD GetHistoryPublicMsgCall1 = WeChatWinBase + GetHistoryPublicMsgCall1Offset;
    DWORD GetHistoryPublicMsgCall2 = WeChatWinBase + GetHistoryPublicMsgCall2Offset;
    DWORD GetHistoryPublicMsgHandle = WeChatWinBase + GetHistoryPublicMsgHandleOffset;
    wstring format_str1 = L"weixin://resourceid/Subscription/profile.html?userName=%ws&subscribed=1&from=217&localOpenTime=%llu&wechatVersion=%d";
    int version = GetWeChatVerInt();
    unsigned long long timestamp = systemtime();
    wchar_t buffer[0x400] = {0};
    swprintf_s(buffer, format_str1.c_str(), PublicId, timestamp, version);
    wstring page_uri(buffer);
    wstring cgi_uri = L"/cgi-bin/mmbiz-bin/bizattr/bizprofilev2h5";
    ZeroMemory(buffer, 800);
    if (Offset != NULL)
    {
        wstring format_str2 = L"{\"BizUserName\":\"%ws\",\"Offset\":\"%ws\",\"ActionType\":1,\"PageSize\":10,\"BizSessionID\":1662700879,\"Scene\":207,\"PreLoad\":0}";
        swprintf_s(buffer, format_str2.c_str(), PublicId, Offset);
    }
    else
    {
        wstring format_str2 = L"{\"BizUserName\":\"%ws\",\"ActionType\":0,\"PageSize\":10,\"BizSessionID\":1662700879,\"Scene\":207,\"PreLoad\":0}";
        swprintf_s(buffer, format_str2.c_str(), PublicId);
    }
    wstring json_data(buffer);
    int h5ExtType = 0x16B6;
    wstring subscriptions = L"subscriptions";
    wstring index = L"3_1011_7";
    WxString p_page_uri(page_uri);
    WxString p_cgi_uri(cgi_uri);
    WxString p_json_data(json_data);
    WxString p_subscriptions(subscriptions);
    WxString p_index(index);
    int isSuccess = 0;
    __asm {
        pushad;
        pushfd;
        call GetHistoryPublicMsgCall1;
        push dword ptr ds:[GetHistoryPublicMsgHandle];
        lea ecx,p_subscriptions;
        push ecx;
        push dword ptr ds:[h5ExtType];
        lea ecx,p_json_data;
        push ecx;
        lea ecx,p_cgi_uri;
        push ecx;
        lea ecx,p_page_uri;
        push ecx;
        lea ecx,p_index;
        push ecx;
        mov ecx,eax;
        call GetHistoryPublicMsgCall2;
        mov isSuccess,eax;
        popfd;
        popad;
    }
    if (!H5ExtBufHooked || isSuccess == 0) return FALSE;
    return TRUE;
}
#ifndef USE_SOCKET
struct GetPublicMsgStruct
{
    wchar_t *PublicId;
    wchar_t *Offset;
};

struct PublicMsgResponseStruct
{
    DWORD buffer = 0;
    DWORD length = 0;
} static resp;
DWORD GetHistoryPublicMsgRemote(LPVOID param)
{
    GetPublicMsgStruct *gpms = (GetPublicMsgStruct *)param;
    BOOL ret = GetHistoryPublicMsg(gpms->PublicId, gpms->Offset);
    while (ret && response.length() == 0)
        Sleep(200);
    resp.buffer = (DWORD)response.c_str();
    resp.length = response.length();
    return (DWORD)&resp;
}
#else
string __stdcall GetHistoryPublicMsg(wstring PublicId, wstring Offset)
{
    wchar_t *pOffset = Offset.length() == 0 ? NULL : (wchar_t *)Offset.c_str();
    BOOL ret = GetHistoryPublicMsg((wchar_t *)PublicId.c_str(), pOffset);
    while (ret && response.length() == 0)
        Sleep(200);
    return response;
}
#endif
