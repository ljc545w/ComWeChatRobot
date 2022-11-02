#include "pch.h"

#define ForwardMessageCall1Offset 0x771980
#define ForwardMessageCall2Offset 0x521760

#ifndef USE_SOCKET
struct ForwardMessageStruct
{
    wchar_t *wxid;
    unsigned long long msgid;
};

BOOL ForwardMessageRemote(LPVOID lpParameter)
{
    ForwardMessageStruct *fms = (ForwardMessageStruct *)lpParameter;
    return ForwardMessage(fms->wxid, fms->msgid);
}
#endif

BOOL __stdcall ForwardMessage(wchar_t *wxid, unsigned long long msgid)
{
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD ForwardMessageCall1 = WeChatWinBase + ForwardMessageCall1Offset;
    DWORD ForwardMessageCall2 = WeChatWinBase + ForwardMessageCall2Offset;
    int dbIndex = 0;
    int localId = GetLocalIdByMsgId(msgid, dbIndex);
    if (localId == 0)
        return FALSE;
    WxString p_wxid(wxid);
    int isSuccess = 0;
    __asm {
		pushad;
		pushfd;
		mov eax, dword ptr ds:[dbIndex];
		push eax;
		mov ecx, dword ptr ds:[localId];
		push ecx;
		lea edi, p_wxid;
		sub esp, 0x14;
		mov ecx, esp;
		push edi;
		call ForwardMessageCall1;
		call ForwardMessageCall2;
		movzx eax,al;
		mov isSuccess,eax;
		add esp, 0x1C;
		popfd;
		popad;
    }
    return isSuccess == 0x1;
}
