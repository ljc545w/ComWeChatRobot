#include "pch.h"

struct GetSelfInfoStruct
{
    DWORD message;
    DWORD length;
};

std::wstring GetSelfInfo(DWORD pid)
{
    wstring SelfInfoString = L"";
    DWORD dwReadSize = 0;
    WeChatProcess hp(pid);
    if (!hp.m_init)
        return L"{}";
    DWORD GetSelfInfoRemoteAddr = hp.GetProcAddr(GetSelfInfoRemote);
    DWORD DeleteSelfInfoCacheRemoteAddr = hp.GetProcAddr(DeleteSelfInfoCacheRemote);
    if (GetSelfInfoRemoteAddr == 0)
        return L"{}";
    DWORD ret = CallRemoteFunction(hp.GetHandle(), GetSelfInfoRemoteAddr, NULL);
    if (ret == 0)
        return L"{}";
    GetSelfInfoStruct selfinfo = {0};
    ReadProcessMemory(hp.GetHandle(), (LPCVOID)ret, &selfinfo, sizeof(GetSelfInfoStruct), &dwReadSize);
    if (selfinfo.length)
    {
        wchar_t *wmessage = new wchar_t[selfinfo.length + 1];
        ZeroMemory(wmessage, (selfinfo.length + 1) * 2);
        ReadProcessMemory(hp.GetHandle(), (LPCVOID)selfinfo.message, wmessage, selfinfo.length * 2, &dwReadSize);
        SelfInfoString = (wstring)wmessage;
        delete[] wmessage;
        wmessage = NULL;
    }
    CallRemoteFunction(hp.GetHandle(), DeleteSelfInfoCacheRemoteAddr, NULL);
    return SelfInfoString;
}
