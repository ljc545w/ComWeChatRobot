#include "pch.h"

#define VerifyFriendApplyCall1Offset 0x10771980 - 0x10000000
#define VerifyFriendApplyCall2Offset 0x102AE8D0 - 0x10000000
#define VerifyFriendApplyParamOffset 0x11EE40E0 - 0x10000000

#ifndef USE_SOCKET
struct VerifyFriendApplyStruct
{
    wchar_t *v3_data;
    wchar_t *v4_data;
};
#endif

struct VerifyFriendApplyParamStruct
{
    DWORD handle;
    DWORD *StatusCode;
    DWORD StatusCodeEndAddr1;
    DWORD StatusCodeEndAddr2;
    char buffer[0x3C] = {0};
};

#ifndef USE_SOCKET
BOOL VerifyFriendApplyRemote(LPVOID lparameter)
{
    VerifyFriendApplyStruct *vfas = (VerifyFriendApplyStruct *)lparameter;
    BOOL isSuccess = VerifyFriendApply(vfas->v3_data, vfas->v4_data);
    return isSuccess;
}
#endif

BOOL __stdcall VerifyFriendApply(wchar_t *v3_data, wchar_t *v4_data)
{
    WxString v3(v3_data);
    WxString v4(v4_data);
    DWORD VerifyFriendApplyCall1 = GetWeChatWinBase() + VerifyFriendApplyCall1Offset;
    DWORD VerifyFriendApplyCall2 = GetWeChatWinBase() + VerifyFriendApplyCall2Offset;
    DWORD VerifyFriendApplyParam = GetWeChatWinBase() + VerifyFriendApplyParamOffset;
    VerifyFriendApplyParamStruct *param = new VerifyFriendApplyParamStruct;
    DWORD StatusCode[9] = {0xB2, (DWORD)param, 0xB5, (DWORD)param, 0xB0, (DWORD)param, 0xB1, (DWORD)param, 0x0};
    param->handle = VerifyFriendApplyParam;
    param->StatusCode = StatusCode;
    param->StatusCodeEndAddr1 = (DWORD)&StatusCode[8];
    param->StatusCodeEndAddr2 = (DWORD)&StatusCode[8];

    char nullbuffer[0x94] = {0};
    BOOL isSuccess = false;
    __asm {
		pushad;
		pushfd;
		push 0x0;
		push 0x6;
		sub esp, 0x14;
		mov ecx, esp;
		lea eax, v4;
		push eax;
		call VerifyFriendApplyCall1;
		sub esp, 0x8;
		push 0x0;
		lea eax, nullbuffer;
		push eax;
		lea eax, v3;
		push eax;
		mov ecx, param;
		call VerifyFriendApplyCall2;
		mov isSuccess, eax;
		popfd;
		popad;
    }
    return isSuccess;
}
