#include "pch.h"

#define LogoutParam1Offset 0x7AE9C438 - 0x78F80000
#define LogoutParam2Offset 0x7AE9C428 - 0x78F80000
#define LogoutParam3Offset 0x7AE9C47C - 0x78F80000

BOOL Logout()
{
    if (!isWxLogin())
        return FALSE;
    DWORD arr[955] = {0};
    DWORD WeChatWin = GetWeChatWinBase();
    arr[0] = WeChatWin + LogoutParam1Offset;
    arr[1] = WeChatWin + LogoutParam2Offset;
    arr[2] = WeChatWin + LogoutParam3Offset;
    arr[3] = 0x3C0A16;
    arr[4] = 0x1;
    __asm {
        pushad;
        pushfd;
        lea esi,arr;
        mov eax,dword ptr [esi];
        push 0x0;
        push 0x0;
        push 0x0;
        push 0x2C6;
        mov ecx,esi;
        call dword ptr ds: [eax];
        popfd;
        popad;
    }
    return (isWxLogin() == 0);
}
