#include "pch.h"

#define OpenBrowserCall1Offset 0x050E1980 - 0x04970000
#define OpenBrowserCall2Offset 0x05699450 - 0x04970000
#define OpenBrowserHandle1Offset 0x05000300 - 0x04970000

#ifndef USE_SOCKET
BOOL OpenBrowserRemote(LPVOID url)
{
    return OpenBrowser((wchar_t *)url);
}
#endif

BOOL OpenBrowser(wchar_t *url)
{
    WxString p_url(url);
    WxString p_null(NULL);
    DWORD WeChatWin = GetWeChatWinBase();
    DWORD OpenBrowserCall1 = WeChatWin + OpenBrowserCall1Offset;
    DWORD OpenBrowserCall2 = WeChatWin + OpenBrowserCall2Offset;
    DWORD OpenBrowserHandle1 = WeChatWin + OpenBrowserHandle1Offset;
    int isSuccess = 0;
    __asm {
        pushad;
        pushfd;
        sub esp,0x14;
        mov edi,0x2CA9;
        mov ecx,esp;
        mov esi,dword ptr[OpenBrowserHandle1];
        mov eax,0x1;
        lea eax,p_null;
        push eax;
        call OpenBrowserCall1;
        push 0x1;
        push 0x1;
        push 0x0;
        push esi;
        push edi;
        push 0x15;
        lea eax,p_url;
        sub esp,0x14;
        mov ecx,esp;
        push eax;
        call OpenBrowserCall1;
        mov edx,0x1;
        mov cl,0x0;
        call OpenBrowserCall2;
        mov isSuccess,eax;
        add esp,0x40;
        popfd;
        popad;
    }
    return isSuccess == 0;
}
