#include "pch.h"

#define GetTransferCallOffset 0x10CD2770 - 0x10000000
#define GetTransferHandleOffset 0x5D02F4F4 - 0x5B1B0000

struct TransferStruct
{
    DWORD handle = 0;
    DWORD unknown_value1 = 0x1;
    WxString amount = {0};
    WxString transcationid = {0};
    DWORD time1 = 0;
    DWORD time2 = 0;
    WxString transferid = {0};
    DWORD unknown_value2 = 0x1;
    char null_buf1[0xE0] = {0};
    DWORD unknown_value3 = 0x820000;
    char null_buf2[0x64] = {0};
};

#ifndef USE_SOCKET
struct GetTransferStruct
{
    wchar_t *wxid = NULL;
    wchar_t *transcationid = NULL;
    wchar_t *transferid = NULL;
};
BOOL GetTransferRemote(LPVOID lpParameter)
{
    GetTransferStruct *gts = (GetTransferStruct *)lpParameter;
    return GetTransfer(gts->wxid, gts->transcationid, gts->transferid);
}
#endif

BOOL __stdcall GetTransfer(wchar_t *wxid, wchar_t *transcationid, wchar_t *transferid)
{
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD GetTransferCallAddr = WeChatWinBase + GetTransferCallOffset;
    DWORD GetTransferHandleAddr = WeChatWinBase + GetTransferHandleOffset;
    TransferStruct tf;
    tf.handle = GetTransferHandleAddr;
    tf.transcationid.set_value(transcationid);
    tf.transferid.set_value(transferid);
    WxString p_wxid(wxid);
    int isSuccess = 0x0;
    __asm {
        pushad;
        pushfd;
        push 0x1;
        lea edx,p_wxid;
        lea ecx,tf;
        sub esp,0x8;
        call GetTransferCallAddr;
        add esp,0xC;
        movzx eax,al;
        mov isSuccess,eax;
        popfd;
        popad;
    }
    return isSuccess;
}
