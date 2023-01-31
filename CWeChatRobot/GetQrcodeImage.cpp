#include "pch.h"

ATL::CComVariant GetQrcodeImage(DWORD pid)
{
    WeChatProcess hp(pid);
    if (!hp.m_init)
        return NULL;
    DWORD GetQrcodeImageAddr = hp.GetProcAddr(GetQrcodeImageRemote);
    DWORD ret = CallRemoteFunction(hp.GetHandle(), GetQrcodeImageAddr, NULL);
    if (ret == 0)
        return NULL;
    DWORD ret_info[2] = {0};
    ReadProcessMemory(hp.GetHandle(), (LPCVOID)ret, &ret_info, sizeof(ret_info), 0);
    DWORD buf_addr = ret_info[0];
    int size = ret_info[1];
    if (size == 0 || buf_addr == 0)
        return NULL;
    unique_ptr<BYTE[]> image(new BYTE[size + 1]());
    ReadProcessMemory(hp.GetHandle(), (LPCVOID)buf_addr, image.get(), size, 0);
    SAFEARRAYBOUND rgsaBound = {(ULONG)size, 0};
    ATL::CComSafeArray<BYTE> cpsa(rgsaBound);
    BYTE *buf = NULL;
    ::SafeArrayAccessData(cpsa.m_psa, (void **)&buf);
    memcpy(buf, image.get(), size);
    ::SafeArrayUnaccessData(cpsa.m_psa);
    ATL::CComVariant cva(cpsa.m_psa);
    cpsa.Destroy();
    return cva;
}

BOOL isWxLogin(DWORD pid)
{
    WeChatProcess hp(pid);
    if (!hp.m_init)
        return 1;
    DWORD isWxLoginRemoteAddr = hp.GetProcAddr(isWxLoginRemote);
    if (isWxLoginRemoteAddr == 0)
        return 1;
    DWORD ret = CallRemoteFunction(hp.GetHandle(), isWxLoginRemoteAddr, NULL);
    return ret == 1;
}
