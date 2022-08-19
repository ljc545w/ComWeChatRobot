#include "pch.h"

#define EditRemarkCallOffset 0x6806F6F0 - 0x67C10000

#ifndef USE_SOCKET
struct EditRemarkStruct
{
    wchar_t *wxid;
    wchar_t *remark;
};
BOOL EditRemarkRemote(LPVOID lpParameter)
{
    EditRemarkStruct *ers = (EditRemarkStruct *)lpParameter;
    return EditRemark(ers->wxid, ers->remark);
}
#endif // !USE_SOCKET

BOOL __stdcall EditRemark(wchar_t *wxid, wchar_t *remark)
{
    DWORD EditRemarkCall = GetWeChatWinBase() + EditRemarkCallOffset;
    WxString pwxid(wxid);
    WxString premark(remark);
    int isSuccess = 0;
    __asm {
		pushad;
		pushfd;
		lea eax, premark;
		push eax;
		lea eax, pwxid;
		push eax;
		call EditRemarkCall;
		mov isSuccess, eax;
		popfd;
		popad;
    }
    return isSuccess == 1;
}
