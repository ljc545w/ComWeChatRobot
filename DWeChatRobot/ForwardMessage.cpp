#include "pch.h"

#define ForwardMessageCall1Offset 0x771980
#define ForwardMessageCall2Offset 0x521760

#ifndef USE_SOCKET
struct ForwardMessageStruct
{
    wchar_t *wxid;
    int localId;
};

BOOL ForwardMessageRemote(LPVOID lpParameter)
{
    ForwardMessageStruct *fms = (ForwardMessageStruct *)lpParameter;
    return ForwardMessage(fms->wxid, fms->localId);
}
#endif

BOOL __stdcall ForwardMessage(wchar_t *wxid, int localId)
{
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD ForwardMessageCall1 = WeChatWinBase + ForwardMessageCall1Offset;
    DWORD ForwardMessageCall2 = WeChatWinBase + ForwardMessageCall2Offset;
    WxString p_wxid(wxid);
    int isSuccess = 0;
    __asm {
		pushad;
		pushfd;
		mov eax, 0x5000100;
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
