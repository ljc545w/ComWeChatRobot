#include "pch.h"

BOOL Logout(DWORD pid)
{
    WeChatProcess hp(pid);
    if (!hp.m_init)
        return 1;
    DWORD WxLogoutRemoteAddr = hp.GetProcAddr(LogoutRemote);
    if (WxLogoutRemoteAddr == 0)
        return 1;
    DWORD ret = CallRemoteFunction(hp.GetHandle(), WxLogoutRemoteAddr, NULL);
    return ret != 1;
}
