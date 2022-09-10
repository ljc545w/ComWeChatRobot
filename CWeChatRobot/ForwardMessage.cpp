#include "pch.h"

struct ForwardMessageStruct
{
    DWORD wxid;
    DWORD localId;
};

BOOL ForwardMessage(DWORD pid, wchar_t *wxid, int localId)
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
    params.localId = localId;
    WeChatData<ForwardMessageStruct *> r_params(hp.GetHandle(), &params, sizeof(params));
    if (!params.wxid || !r_params.GetAddr())
    {
        return 1;
    }
    DWORD dwRet = CallRemoteFunction(hp.GetHandle(), ForwardMessageRemoteAddr, r_params.GetAddr());
    return dwRet == 0;
}
