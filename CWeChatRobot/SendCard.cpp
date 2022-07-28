#include "pch.h"

struct SendCardStruct {
	DWORD receiver;
	DWORD sharedwxid;
	DWORD nickname;
};

BOOL SendCard(DWORD pid,wchar_t* receiver, wchar_t* sharedwxid, wchar_t* nickname) {
    WeChatProcess hp(pid);
    if (!hp.m_init) return 1;
    DWORD SendCardRemoteAddr = hp.GetProcAddr(SendCardRemote);
    if (SendCardRemoteAddr == 0) {
        return 1;
    }
    SendCardStruct params = { 0 };
    WeChatData<wchar_t*> r_receiver(hp.GetHandle(), receiver, TEXTLENGTH(receiver));
    WeChatData<wchar_t*> r_sharedwxid(hp.GetHandle(), sharedwxid, TEXTLENGTH(sharedwxid));
    WeChatData<wchar_t*> r_nickname(hp.GetHandle(), nickname, TEXTLENGTH(nickname));
    params.receiver = (DWORD)r_receiver.GetAddr();
    params.sharedwxid = (DWORD)r_sharedwxid.GetAddr();
    params.nickname = (DWORD)r_nickname.GetAddr();
    WeChatData<SendCardStruct*> r_params(hp.GetHandle(), &params, sizeof(params));
    if (!params.receiver || !params.sharedwxid || !params.nickname || !r_params.GetAddr()) {
        return 1;
    }
    DWORD dwRet = CallRemoteFunction(hp.GetHandle(), SendCardRemoteAddr, r_params.GetAddr());
    return 0;
}