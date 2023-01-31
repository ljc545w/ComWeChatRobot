#include "pch.h"

#define ChatMsgHandleOffset 0x5AF6803C - 0x590F0000
#define GetChatMsgCallOffset 0x59522730 - 0x590F0000
#define ReleaseChatMsgCallOffset 0x78757780 - 0x786A0000

BOOL __stdcall GetChatMsgBySvrId(ULONG64 msgid, PCHAT_MSG p_chat_msg)
{
    int dbIndex = 0;
    int localId = GetLocalIdByMsgId(msgid, dbIndex);
    if (localId == 0)
        return FALSE;
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD ChatMsgHandle = WeChatWinBase + ChatMsgHandleOffset;
    DWORD GetChatMsgCall = WeChatWinBase + GetChatMsgCallOffset;
    p_chat_msg->handle1 = ChatMsgHandle;
    int isSuccess = 0x0;
    __asm {
        pushad;
        pushfd;
        push dword ptr [dbIndex];
        mov ecx,dword ptr [p_chat_msg];
        push dword ptr [localId];
        call GetChatMsgCall;
        add esp,0x8;
        movzx eax,al;
        mov isSuccess,eax;
        popfd;
        popad;
    }
    return (isSuccess == 0x1);
}

BOOL __stdcall ReleaseChatMsg(PCHAT_MSG p_chat_msg)
{
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD ReleaseChatMsgCall = WeChatWinBase + ReleaseChatMsgCallOffset;
    int isSuccess = 0;
    __asm {
        pushad;
        mov ecx, dword ptr [p_chat_msg];
        call ReleaseChatMsgCall;
        popad;
    }
    return TRUE;
}
