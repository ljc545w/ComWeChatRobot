#include "pch.h"

#define DeleteUserCall1Offset 0x100B75D0 - 0x10000000
#define DeleteUserCall2Offset 0x100BD5C0 - 0x10000000
#define DeleteUserCall3Offset 0x104685A0 - 0x10000000

BOOL __stdcall DeleteUser(wchar_t *wxid)
{
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD DeleteUserCall1 = WeChatWinBase + DeleteUserCall1Offset;
    DWORD DeleteUserCall2 = WeChatWinBase + DeleteUserCall2Offset;
    DWORD DeleteUserCall3 = WeChatWinBase + DeleteUserCall3Offset;

    WxString pwxid(wxid);
    char buffer[0x70] = {0};
    BOOL isSuccess = 0x0;
    __asm {
		pushad;
		pushfd;
		lea ecx, buffer;
		call DeleteUserCall1;
		call DeleteUserCall2;
		push ecx;
		lea esi, pwxid;
		push esi;
		mov ecx, eax;
		call DeleteUserCall3;
		mov isSuccess, eax;
		popfd;
		popad;
    }
    return isSuccess;
}

#ifndef USE_SOCKET
BOOL DeleteUserRemote(LPVOID lpParameter)
{
    BOOL isSuccess = DeleteUser((wchar_t *)lpParameter);
    return isSuccess;
}
#endif
