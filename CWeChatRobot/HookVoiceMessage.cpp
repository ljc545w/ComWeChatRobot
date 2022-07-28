#include "pch.h"

BOOL HookVoiceMsg(DWORD pid,wchar_t* savepath) {
    WeChatProcess hp(pid);
    if (!hp.m_init) return 1;
    DWORD HookVoiceMsgRemoteAddr = hp.GetProcAddr(HookVoiceMsgRemote);
    if (HookVoiceMsgRemoteAddr == 0)
        return 1;
    WeChatData<wchar_t*> r_savepath(hp.GetHandle(), savepath, TEXTLENGTH(savepath));
    if (r_savepath.GetAddr() == 0)
        return 1;
    DWORD ret = CallRemoteFunction(hp.GetHandle(), HookVoiceMsgRemoteAddr, r_savepath.GetAddr());
    return ret == 0;
}

void UnHookVoiceMsg(DWORD pid) {
    WeChatProcess hp(pid);
    if (!hp.m_init) return;
    DWORD UnHookVoiceMsgRemoteAddr = hp.GetProcAddr(UnHookVoiceMsgRemote);
    if (UnHookVoiceMsgRemoteAddr == 0)
        return;
    CallRemoteFunction(hp.GetHandle(), UnHookVoiceMsgRemoteAddr, NULL);
}