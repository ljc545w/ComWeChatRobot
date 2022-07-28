#include "pch.h"

BOOL DeleteUser(DWORD pid,wchar_t* wxid) {
    WeChatProcess hp(pid);
    if (!hp.m_init) return 1;
    DWORD DeleteUserRemoteAddr = hp.GetProcAddr(DeleteUserRemote);
    if (DeleteUserRemoteAddr == 0)
        return 1;
    WeChatData<wchar_t*> r_wxid(hp.GetHandle(), wxid, TEXTLENGTH(wxid));
    if (r_wxid.GetAddr() == 0)
        return 1;
    DWORD ret = CallRemoteFunction(hp.GetHandle(), DeleteUserRemoteAddr, r_wxid.GetAddr());
    return ret == 0;
}