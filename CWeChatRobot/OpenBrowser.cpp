#include "pch.h"

BOOL OpenBrowser(DWORD pid, wchar_t *url)
{
    WeChatProcess hp(pid);
    if (!hp.m_init)
        return 1;
    DWORD OpenBrowserRemoteAddr = hp.GetProcAddr(OpenBrowserRemote);
    if (OpenBrowserRemoteAddr == 0)
    {
        return 1;
    }
    WeChatData<wchar_t *> r_url(hp.GetHandle(), url, TEXTLENGTH(url));
    if (!r_url.GetAddr())
    {
        return 1;
    }
    DWORD dwRet = CallRemoteFunction(hp.GetHandle(), OpenBrowserRemoteAddr, r_url.GetAddr());
    return dwRet == 0;
}
