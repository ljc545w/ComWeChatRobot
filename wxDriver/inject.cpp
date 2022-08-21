#include "pch.h"
#include "driver.h"

BOOL InjectDll(DWORD pid, const wchar_t *szPath)
{
    WeChatProcess hp(pid);
    if (hp.m_init == false)
        return FALSE;
    WeChatData<wchar_t *> pRemoteAddress(hp.GetHandle(), (wchar_t *)szPath, TEXTLENGTH(szPath));
#ifdef _WIN64
    PVOID pFunc = GetSystem32ProcAddr(L"\\KnownDlls32\\kernel32.dll", "LoadLibraryW");
#else
    PVOID pFunc = LoadLibraryW;
#endif
    if (pFunc == NULL)
        return false;
    DWORD ret = CallRemoteFunction(hp.GetHandle(), pFunc, pRemoteAddress.GetAddr());
    return true;
}

BOOL RemoveDll(DWORD pid)
{
    WeChatProcess hp(pid);
    if (hp.m_init == false)
        return FALSE;
    DWORD dwHandle = hp.WeChatRobotBase();
    if (dwHandle == 0)
        return true;
    PVOID pFunc = NULL;
#ifdef _WIN64
    pFunc = GetSystem32ProcAddr(L"\\KnownDlls32\\kernel32.dll", "FreeConsole");
#else
    pFunc = FreeConsole;
#endif
    if (pFunc == NULL)
        return false;
    CallRemoteFunction(hp.GetHandle(), pFunc, NULL);
#ifdef _WIN64
    pFunc = GetSystem32ProcAddr(L"\\KnownDlls32\\kernel32.dll", "FreeLibrary");
#else
    pFunc = FreeLibrary;
#endif
    if (pFunc == NULL)
        return false;
    do
        CallRemoteFunction(hp.GetHandle(), pFunc, dwHandle);
    while (hp.WeChatRobotBase() != 0);
    return true;
}
