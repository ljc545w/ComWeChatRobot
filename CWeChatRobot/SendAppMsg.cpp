#include "pch.h"

struct SendAppMsgStruct
{
    DWORD wxid;
    DWORD appid;
};

BOOL SendAppMsg(DWORD pid,wchar_t* wxid, wchar_t* appid) {
    WeChatProcess hp(pid);
    if (!hp.m_init) return 1;
    DWORD SendAppMsgRemoteAddr = hp.GetProcAddr(SendAppMsgRemote);
    if (SendAppMsgRemoteAddr == 0) {
        return 1;
    }
    SendAppMsgStruct params = { 0 };
    WeChatData<wchar_t*> r_wxid(hp.GetHandle(), wxid, TEXTLENGTH(wxid));
    WeChatData<wchar_t*> r_appid(hp.GetHandle(), appid, TEXTLENGTH(appid));

    params.wxid = (DWORD)r_wxid.GetAddr();
    params.appid = (DWORD)r_appid.GetAddr();
    WeChatData<SendAppMsgStruct*> r_params(hp.GetHandle(), &params, sizeof(params));
    if (!params.wxid || !params.appid || !r_params.GetAddr()) {
        return 1;
    }
    DWORD dwRet = CallRemoteFunction(hp.GetHandle(), SendAppMsgRemoteAddr, r_params.GetAddr());
    return dwRet == 0;
}