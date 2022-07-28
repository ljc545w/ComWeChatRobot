#include "pch.h"

DWORD CheckFriendStatus(DWORD pid,wchar_t* wxid) {
    WeChatProcess hp(pid);
    if (!hp.m_init) return 1;
    DWORD CheckFriendStatusRemoteAddr = hp.GetProcAddr(CheckFriendStatusRemote);
    if (CheckFriendStatusRemoteAddr == 0)
        return 1;
    WeChatData<wchar_t*> r_wxid(hp.GetHandle(), wxid, TEXTLENGTH(wxid));
    if (r_wxid.GetAddr() == 0)
        return 1;
    DWORD ret = CallRemoteFunction(hp.GetHandle(), CheckFriendStatusRemoteAddr, r_wxid.GetAddr());
    return ret;
}