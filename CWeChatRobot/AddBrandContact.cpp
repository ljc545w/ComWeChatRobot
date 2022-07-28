#include "pch.h"

BOOL AddBrandContact(DWORD pid,wchar_t* PublicId) {
    WeChatProcess hp(pid);
    if (!hp.m_init) return 1;
    DWORD AddBrandContactAddr = hp.GetProcAddr(AddBrandContactRemote);
    if (AddBrandContactAddr == 0)
        return 1;
    WeChatData<wchar_t*> r_publicid(hp.GetHandle(), PublicId, TEXTLENGTH(PublicId));
    if (r_publicid.GetAddr() == 0)
        return 1;
    DWORD ret = CallRemoteFunction(hp.GetHandle(), AddBrandContactAddr, r_publicid.GetAddr());
    return ret == 0;
}