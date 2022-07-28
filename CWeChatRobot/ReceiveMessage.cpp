#include "pch.h"

BOOL StartReceiveMessage(DWORD pid,int port) {
    WeChatProcess hp(pid);
    if (!hp.m_init) return 1;
    DWORD StartReceiveMessageRemoteAddr = hp.GetProcAddr(HookReceiveMessageRemote);
    if (StartReceiveMessageRemoteAddr == 0)
        return 1;
    CallRemoteFunction(hp.GetHandle(), StartReceiveMessageRemoteAddr, port);
    return 0;
}

BOOL StopReceiveMessage(DWORD pid) {
    WeChatProcess hp(pid);
    if (!hp.m_init) return 1;
    DWORD UnHookReceiveMsgRemoteAddr = hp.GetProcAddr(UnHookReceiveMessageRemote);
    if (UnHookReceiveMsgRemoteAddr == 0)
        return 1;
    CallRemoteFunction(hp.GetHandle(), UnHookReceiveMsgRemoteAddr, NULL);
    return 0;
}