#include "pch.h"

struct ForwardMessageStruct
{
    DWORD wxid;
    ULONG64 msgid;
};

BOOL ForwardMessage(DWORD pid, wchar_t *wxid, ULONG64 msgid)
{
    WeChatProcess hp(pid);
    if (!hp.m_init)
        return 1;
    DWORD ForwardMessageRemoteAddr = hp.GetProcAddr(ForwardMessageRemote);
    if (ForwardMessageRemoteAddr == 0)
    {
        return 1;
    }
    ForwardMessageStruct params = {0};
    WeChatData<wchar_t *> r_wxid(hp.GetHandle(), wxid, TEXTLENGTH(wxid));
    params.wxid = (DWORD)r_wxid.GetAddr();
    params.msgid = msgid;
    WeChatData<ForwardMessageStruct *> r_params(hp.GetHandle(), &params, sizeof(params));
    if (!params.wxid || !r_params.GetAddr())
    {
        return 1;
    }
    DWORD dwRet = CallRemoteFunction(hp.GetHandle(), ForwardMessageRemoteAddr, r_params.GetAddr());
    return dwRet == 0;
}
