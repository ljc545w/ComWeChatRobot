#include "pch.h"

VARIANT GetQrcodeImage(DWORD pid)
{
    VARIANT vsa;
    vsa.vt = VT_ARRAY | VT_UI1;
    V_ARRAY(&vsa) = NULL;
    WeChatProcess hp(pid);
    if (!hp.m_init)
        return vsa;
    DWORD GetQrcodeImageAddr = hp.GetProcAddr(GetQrcodeImageRemote);
    DWORD ret = CallRemoteFunction(hp.GetHandle(), GetQrcodeImageAddr, NULL);
    if (ret == 0)
        return vsa;
    DWORD ret_info[2] = {0};
    ReadProcessMemory(hp.GetHandle(), (LPCVOID)ret, &ret_info, sizeof(ret_info), 0);
    DWORD buf_addr = ret_info[0];
    int size = ret_info[1];
    if (size == 0 || buf_addr == 0)
        return vsa;
    unique_ptr<BYTE[]> image(new BYTE[size + 1]());
    ReadProcessMemory(hp.GetHandle(), (LPCVOID)buf_addr, image.get(), size, 0);
    SAFEARRAYBOUND rgsaBound = {(ULONG)size, 0};
    SAFEARRAY *psaValue = SafeArrayCreate(VT_UI1, 1, &rgsaBound);
    BYTE *buf = NULL;
    ::SafeArrayAccessData(psaValue, (void **)&buf);
    memcpy(buf, image.get(), size);
    ::SafeArrayUnaccessData(psaValue);
    V_ARRAY(&vsa) = psaValue;
    return vsa;
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
