#include "pch.h"

struct GetA8KeyRespStruct
{
    DWORD message;
    DWORD length;
};

wstring GetA8Key(DWORD pid, wchar_t *url)
{
    WeChatProcess hp(pid);
    if (!hp.m_init)
        return L"";
    DWORD GetA8KeyReomteAddr = hp.GetProcAddr(GetA8KeyRemote);
    if (GetA8KeyReomteAddr == 0)
    {
        return L"";
    }
    WeChatData<wchar_t *> r_url(hp.GetHandle(), url, TEXTLENGTH(url));
    if (!r_url.GetAddr())
    {
        return L"";
    }
    DWORD dwRet = CallRemoteFunction(hp.GetHandle(), GetA8KeyReomteAddr, r_url.GetAddr());
    DWORD dwReadSize;
    GetA8KeyRespStruct ret_info = {0};
    ReadProcessMemory(hp.GetHandle(), (LPCVOID)dwRet, &ret_info, sizeof(ret_info), &dwReadSize);
    if (ret_info.message == 0)
        return L"";
    unique_ptr<char[]> buffer(new char[ret_info.length + 1]());
    ReadProcessMemory(hp.GetHandle(), (LPCVOID)ret_info.message, buffer.get(), ret_info.length, &dwReadSize);
    string result(buffer.get(), ret_info.length);
    wstring wresult = gb2312_to_unicode(result.c_str());
    return wresult;
}
