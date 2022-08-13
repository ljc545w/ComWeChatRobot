#include "pch.h"
#include "driver.h"

BOOL isFileExists_stat(wstring name)
{
    struct _stat buffer;
    return (_wstat(name.c_str(), &buffer) == 0);
}

DWORD GetWeChatWinBase(DWORD pid)
{
    wchar_t *WeChatWin = (wchar_t *)L"WeChatWin.dll";
    WeChatProcess hp(pid);
    if (!hp.m_init)
        return 0;
    WeChatData<wchar_t *> r_dllname(hp.GetHandle(), WeChatWin, TEXTLENGTH(WeChatWin));
    if (r_dllname.GetAddr() == 0)
        return 0;
#ifdef _WIN64
    DWORD pGetModuleHandleW = (DWORD)GetSystem32ProcAddr(L"\\KnownDlls32\\kernel32.dll", "GetModuleHandleW");
#else
    DWORD pGetModuleHandleW = (DWORD)GetModuleHandleW;
#endif
    DWORD ret = CallRemoteFunction(hp.GetHandle(), pGetModuleHandleW, r_dllname.GetAddr());
    return ret;
}

BOOL CreateConsole()
{
    if (AllocConsole())
    {
        AttachConsole(GetCurrentProcessId());
        FILE *retStream;
        freopen_s(&retStream, "CONOUT$", "w", stdout);
        if (!retStream)
            throw std::runtime_error("Stdout redirection failed.");
        freopen_s(&retStream, "CONOUT$", "w", stderr);
        if (!retStream)
            throw std::runtime_error("Stderr redirection failed.");
        return TRUE;
    }
    return FALSE;
}

wstring GetWorkPath()
{
    wchar_t szFilePath[MAX_PATH + 1] = {0};
#ifdef _WIN64
    HMODULE hd = GetModuleHandle(L"wxDriver64.dll");
#else
    HMODULE hd = GetModuleHandle(L"wxDriver.dll");
#endif
    GetModuleFileName(hd, szFilePath, MAX_PATH);
    wstring wpath = szFilePath;
    int pos = (int)wpath.find_last_of(L"\\");
    wpath = wpath.substr(0, pos);
    return wpath;
}

BOOL start_listen(DWORD pid, int port)
{
    wstring dllpath = GetWorkPath() + L"\\" + DLLNAME;
    if (!isFileExists_stat(dllpath))
    {
        return FALSE;
    }
    BOOL status = InjectDll(pid, dllpath.c_str());
    if (status == false)
        return FALSE;
    WeChatProcess hp(pid);
    if (hp.m_init == false)
    {
        return FALSE;
    }
    DWORD http_start = hp.GetProcAddr((LPSTR) "http_start");
    if (http_start == 0)
        return FALSE;
    CallRemoteFunction(hp.GetHandle(), http_start, port);
    return TRUE;
}

BOOL stop_listen(DWORD pid)
{
    WeChatProcess hp(pid);
    if (hp.m_init == false)
    {
        return FALSE;
    }
    DWORD http_close = hp.GetProcAddr((LPSTR) "http_close");
    if (http_close == 0)
        return FALSE;
    CallRemoteFunction(hp.GetHandle(), http_close, NULL);
    BOOL status = RemoveDll(pid);
    return status;
}

static BOOL GetWeChatInstallInfo(TCHAR *lpValueName, VOID *Value, DWORD lpcbData)
{
    HKEY hKey = NULL;
    ZeroMemory(Value, lpcbData);
    LSTATUS lRet = RegOpenKeyEx(HKEY_CURRENT_USER, _T("SOFTWARE\\Tencent\\WeChat"), 0, KEY_QUERY_VALUE, &hKey);
    if (lRet != 0)
    {
        return false;
    }
    lRet = RegQueryValueEx(hKey, lpValueName, NULL, NULL, (LPBYTE)Value, &lpcbData);
    RegCloseKey(hKey);
    if (lRet != 0)
    {
        return false;
    }
    return true;
}

tstring GetWeChatInstallDir()
{
    TCHAR *szProductType = new TCHAR[MAX_PATH];
    GetWeChatInstallInfo((TCHAR *)TEXT("InstallPath"), (void *)szProductType, MAX_PATH);
    tstring wxdir(szProductType);
    delete[] szProductType;
    szProductType = NULL;
    return wxdir.length() == 0 ? TEXT("") : wxdir;
}

tstring GetWeChatVersion()
{
    BYTE pversion[4] = {0};
    GetWeChatInstallInfo((TCHAR *)TEXT("CrashVersion"), (void *)pversion, sizeof(DWORD));
    TCHAR *temp = new TCHAR[20];
    _stprintf_s(temp, 20, _T("%d.%d.%d.%d\0"), (int)(pversion[3] - 0x60), (int)pversion[2], (int)pversion[1], (int)pversion[0]);
    tstring verStr(temp);
    delete[] temp;
    temp = NULL;
    return verStr;
}

static bool CloseAllWxProcessMutexHandle()
{
    HANDLE hsnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hsnapshot == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);
    int flag = Process32First(hsnapshot, &pe);
    while (flag != 0)
    {
        if (lstrcmp(pe.szExeFile, L"WeChat.exe") == 0)
        {
            CloseProcessHandle(pe.th32ProcessID, (wchar_t *)L"_WeChat_App_Instance_Identity_Mutex_Name");
        }
        flag = Process32Next(hsnapshot, &pe);
    }
    CloseHandle(hsnapshot);
    return true;
}

DWORD new_wechat()
{
    CloseAllWxProcessMutexHandle();
    tstring szAppName = GetWeChatInstallDir();
    if (szAppName.length() == 0)
        return 0;
    szAppName += TEXT("\\WeChat.exe");
    STARTUPINFO StartInfo;
    ZeroMemory(&StartInfo, sizeof(StartInfo));
    PROCESS_INFORMATION procStruct;
    ZeroMemory(&procStruct, sizeof(procStruct));
    StartInfo.cb = sizeof(STARTUPINFO);
    if (CreateProcess((LPCTSTR)szAppName.c_str(), NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &StartInfo, &procStruct))
    {
        CloseHandle(procStruct.hProcess);
        CloseHandle(procStruct.hThread);
    }
    if (procStruct.dwProcessId == 0)
        return 0;
    DWORD WeChatWinBase = 0;
    while ((WeChatWinBase = GetWeChatWinBase(procStruct.dwProcessId)) == 0)
    {
        Sleep(500);
    }
    return procStruct.dwProcessId;
}
