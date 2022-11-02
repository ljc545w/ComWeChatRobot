#include "pch.h"

wstring GetMsgCND(DWORD pid, ULONG64 msgid)
{
    WeChatProcess hp(pid);
    if (!hp.m_init)
        return L"";
    DWORD GetMsgCDNRemoteAddr = hp.GetProcAddr(GetMsgCDNRemote);
    if (GetMsgCDNRemoteAddr == 0)
    {
        return L"";
    }
    WeChatData<ULONG64 *> r_msgid(hp.GetHandle(), &msgid, sizeof(ULONG64));
    if (!r_msgid.GetAddr())
    {
        return L"";
    }
    DWORD dwRet = CallRemoteFunction(hp.GetHandle(), GetMsgCDNRemoteAddr, r_msgid.GetAddr());
    if (dwRet == 0)
        return L"";
    DWORD get_cdn_resp[2] = {0};
    ReadProcessMemory(hp.GetHandle(), (LPVOID)dwRet, &get_cdn_resp, sizeof(get_cdn_resp), 0);
    if (!get_cdn_resp[0] || !get_cdn_resp[1])
        return L"";
    unique_ptr<wchar_t[]> path_buf = make_unique<wchar_t[]>(get_cdn_resp[1] + 1);
    ReadProcessMemory(hp.GetHandle(), (LPVOID)get_cdn_resp[0], path_buf.get(), get_cdn_resp[1] * 2, 0);
    return wstring(path_buf.get(), get_cdn_resp[1]);
}
