#include "pch.h"

struct GetTransferStruct
{
    DWORD wxid = 0;
    DWORD transcationid = 0;
    DWORD transferid = 0;
};

int GetTransfer(DWORD pid, wchar_t *wxid, wchar_t *transcationid, wchar_t *transferid)
{
    WeChatProcess hp(pid);
    if (hp.m_init == false)
        return 1;
    DWORD GetTransferRemoteAddr = hp.GetProcAddr(GetTransferRemote);
    if (GetTransferRemoteAddr == 0)
        return 1;
    WeChatData<wchar_t *> r_wxid(hp.GetHandle(), wxid, TEXTLENGTH(wxid));
    WeChatData<wchar_t *> r_transcationid(hp.GetHandle(), transcationid, TEXTLENGTH(transcationid));
    WeChatData<wchar_t *> r_transferid(hp.GetHandle(), transferid, TEXTLENGTH(transferid));
    GetTransferStruct param = {(DWORD)r_wxid.GetAddr(), (DWORD)r_transcationid.GetAddr(), (DWORD)r_transferid.GetAddr()};
    WeChatData<GetTransferStruct *> r_param(hp.GetHandle(), &param, sizeof(GetTransferStruct));
    if (!r_param.GetAddr() || !r_wxid.GetAddr() || !r_transcationid.GetAddr() || !r_transferid.GetAddr())
        return 1;
    DWORD dwRet = CallRemoteFunction(hp.GetHandle(), GetTransferRemoteAddr, r_param.GetAddr());
    return (dwRet != 1);
}
