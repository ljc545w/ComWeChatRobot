#include "pch.h"

struct SendTextStruct
{
    DWORD wxid;
    DWORD wxmsg;
};

int SendText(DWORD pid,wchar_t* wxid, wchar_t* wxmsg) {
    WeChatProcess hp(pid);
    if (!hp.m_init) return 1;
    DWORD SendTextRemoteAddr = hp.GetProcAddr(SendTextRemote);
    if (SendTextRemoteAddr == 0) {
        return 1;
    }
    SendTextStruct params = { 0 };
    WeChatData<wchar_t*> r_wxid(hp.GetHandle(),wxid,TEXTLENGTH(wxid));
    WeChatData<wchar_t*> r_wxmsg(hp.GetHandle(), wxmsg, TEXTLENGTH(wxmsg));
    params.wxid = (DWORD)r_wxid.GetAddr();
    params.wxmsg = (DWORD)r_wxmsg.GetAddr();
    WeChatData<SendTextStruct*> r_params(hp.GetHandle(), &params, sizeof(params));
    if (!params.wxid || !params.wxmsg || !r_params.GetAddr()) {
        return 1;
    }
    DWORD dwRet = CallRemoteFunction(hp.GetHandle(), SendTextRemoteAddr, r_params.GetAddr());
    return 0;
}