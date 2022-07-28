#include "pch.h"

BOOL HookImageMsg(DWORD pid,wchar_t* savepath) {
    WeChatProcess hp(pid);
    if (!hp.m_init) return 1;
    DWORD HookImageMsgRemoteAddr = hp.GetProcAddr(HookImageMsgRemote);
    if (HookImageMsgRemoteAddr == 0)
        return 1;
    WeChatData<wchar_t*> r_savepath(hp.GetHandle(), savepath, TEXTLENGTH(savepath));
    if (r_savepath.GetAddr() == 0)
        return 1;
    DWORD ret = CallRemoteFunction(hp.GetHandle(), HookImageMsgRemoteAddr, r_savepath.GetAddr());
    return ret == 0;
}

void UnHookImageMsg(DWORD pid) {
    WeChatProcess hp(pid);
    if (!hp.m_init) return;
    DWORD UnHookImageMsgRemoteAddr = hp.GetProcAddr(UnHookImageMsgRemote);
    if (UnHookImageMsgRemoteAddr == 0)
        return;
    CallRemoteFunction(hp.GetHandle(), UnHookImageMsgRemoteAddr, NULL);
}