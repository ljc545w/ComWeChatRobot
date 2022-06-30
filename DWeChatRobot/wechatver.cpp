#include "pch.h"

#define WeChatVersionOffset 0x2385AF0

DWORD GetWeChatVerInt()
{
    WCHAR VersionFilePath[MAX_PATH];
    BYTE WeChatVersion[4] = { 0 };
    if (GetModuleFileName((HMODULE)GetWeChatWinBase(), VersionFilePath, MAX_PATH) == 0)
    {
        return 0;
    }

    VS_FIXEDFILEINFO* pVsInfo;
    unsigned int iFileInfoSize = sizeof(VS_FIXEDFILEINFO);
    int iVerInfoSize = GetFileVersionInfoSize(VersionFilePath, NULL);
    if (iVerInfoSize != 0) {
        char* pBuf = new char[iVerInfoSize];
        if (GetFileVersionInfo(VersionFilePath, 0, iVerInfoSize, pBuf)) {
            if (VerQueryValue(pBuf, TEXT("\\"), (void**)&pVsInfo, &iFileInfoSize)) {
                WeChatVersion[3] = (BYTE)(0x60 + (pVsInfo->dwFileVersionMS >> 16) & 0x0000FFFF);
                WeChatVersion[2] = (BYTE)(pVsInfo->dwFileVersionMS & 0x0000FFFF);
                WeChatVersion[1] = (BYTE)((pVsInfo->dwFileVersionLS >> 16) & 0x0000FFFF);
                WeChatVersion[0] = (BYTE)(pVsInfo->dwFileVersionLS & 0x0000FFFF);
            }
        }
        delete[] pBuf;
    }
    return *(DWORD*)WeChatVersion;
}

string GetWeChatVerStr() {
    DWORD WeChatVersion = GetWeChatVerInt();
    if (WeChatVersion == 0)
        return "null";
    string wxver = "";
    BYTE* pWxVer = (BYTE*)&WeChatVersion;
    strstream wxVer;
    wxVer << (int)pWxVer[3] - 0x60 << "." << (int)pWxVer[2] << "." << (int)pWxVer[1] << "." << (int)pWxVer[0];
    wxVer >> wxver;
    return wxver;
}

vector<wstring> split(const wstring& str, const wstring& delim) {
    vector<wstring> res;
    if (L"" == str) 
        return res;
    wchar_t* strs = new wchar_t[str.length() + 1]; 
    lstrcpy(strs, str.c_str());
    wchar_t* buffer = NULL;
    wchar_t* d = new wchar_t[delim.length() + 1];
    lstrcpy(d, delim.c_str());
    wchar_t* p = wcstok_s(strs, d, &buffer);
    while (p) {
        wstring s = p;
        res.push_back(s);
        p = wcstok_s(NULL, d, &buffer);
    }
    delete[] strs;
    delete[] d;
    return res;
}

#ifndef USE_SOCKET
BOOL ChangeWeChatVerRemote(LPVOID verStr) {
    return ChangeWeChatVer((wchar_t*)verStr);
}
#endif

BOOL ChangeWeChatVer(wchar_t* verStr) {
    wstring wverStr(verStr);
    vector<wstring> v_split = split(wverStr, L".");
    BYTE bVer[4] = { 0 };
    bVer[3] = (BYTE)0x60;
    for (unsigned int i = 0; i < v_split.size(); i++) {
        bVer[3 - i] += (BYTE)_wtoi(v_split[i].c_str());
    }
    DWORD version = *(DWORD*)bVer;
    if (version < 0x60000000)
        return false;
    DWORD WeChatVersionAddr = GetWeChatWinBase() + WeChatVersionOffset;
    *(DWORD*)WeChatVersionAddr = version;
    return true;
}