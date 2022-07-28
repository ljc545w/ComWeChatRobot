#include "pch.h"

BOOL ChangeWeChatVer(DWORD pid,wchar_t* verStr) {
    WeChatProcess hp(pid);
    if (!hp.m_init) return 1;
    DWORD ChangeWeChatVerRemoteAddr = hp.GetProcAddr(ChangeWeChatVerRemote);
    if (ChangeWeChatVerRemoteAddr == 0)
        return 1;
    WeChatData<wchar_t*> r_version(hp.GetHandle(), verStr, TEXTLENGTH(verStr));
    if (r_version.GetAddr() == 0)
        return 1;
    DWORD ret = CallRemoteFunction(hp.GetHandle(), ChangeWeChatVerRemoteAddr, r_version.GetAddr());
    return ret == 0;
}