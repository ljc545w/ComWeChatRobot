// pch.cpp: 与预编译标头对应的源文件

#include "pch.h"

/*
 * 保证RobotService可以重复调用及停止，类似于可重入锁，用于支持多个客户端同时调用
 * StopRobotService的次数必须不小于StartRobotService才可以停止服务，当服务已经停止后
 * 可以重复调用StopRobotService，但只需一次StartRobotService即可将计数置0并开启服务
 */
map<DWORD, short> ServiceCount;

// 当使用预编译的头时，需要使用此源文件，编译才能成功。

string unicode_to_utf8(wchar_t *wstr)
{
    int c_size = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, false);
    char *buffer = new char[c_size + 1];
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, buffer, c_size, NULL, false);
    buffer[c_size] = '\0';
    string str(buffer);
    delete[] buffer;
    buffer = NULL;
    return str;
}

/*
 * 将GB2312编码数据转换为GBK编码
 */
wstring gb2312_to_unicode(const char *buffer)
{
    int c_size = MultiByteToWideChar(CP_ACP, 0, buffer, -1, 0, 0);
    wchar_t *temp = new wchar_t[c_size + 1];
    MultiByteToWideChar(CP_ACP, 0, buffer, -1, temp, c_size);
    temp[c_size] = L'\0';
    wstring ret(temp);
    delete[] temp;
    temp = NULL;
    return ret;
}

BOOL isFileExists_stat(string &name)
{
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
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
        return 0;
    }
    return 1;
}

DWORD GetWeChatRobotBase(DWORD pid)
{
    WeChatProcess hp(pid);
    if (!hp.m_init)
        return 0;
    WeChatData<wchar_t *> r_dllname(hp.GetHandle(), DLLNAME, TEXTLENGTH(DLLNAME));
    if (r_dllname.GetAddr() == 0)
        return 0;
    DWORD ret = CallRemoteFunction(hp.GetHandle(), GetModuleHandleW, r_dllname.GetAddr());
    return ret;
}

DWORD GetWeChatWinBase(DWORD pid)
{
    wchar_t *WeChatWin = L"WeChatWin.dll";
    WeChatProcess hp(pid);
    if (!hp.m_init)
        return 0;
    WeChatData<wchar_t *> r_dllname(hp.GetHandle(), WeChatWin, TEXTLENGTH(WeChatWin));
    if (r_dllname.GetAddr() == 0)
        return 0;
    DWORD ret = CallRemoteFunction(hp.GetHandle(), GetModuleHandleW, r_dllname.GetAddr());
    return ret;
}

DWORD GetWeChatPid()
{
    HWND hCalc = FindWindow(NULL, L"微信");
    DWORD wxPid = 0;
    GetWindowThreadProcessId(hCalc, &wxPid);
    if (wxPid == 0)
    {
        hCalc = FindWindow(NULL, L"微信测试版");
        GetWindowThreadProcessId(hCalc, &wxPid);
    }
    return wxPid;
}

DWORD StartRobotService(DWORD pid)
{
    if (ServiceCount[pid] < 0)
    {
        ServiceCount[pid] = 0;
    }
    if (ServiceCount[pid] == 0)
    {
        wstring wworkPath = GetComWorkPath();
        wchar_t *workPath = (wchar_t *)wworkPath.c_str();
        bool status = Inject(pid, workPath);
        if (!status)
        {
            ++ServiceCount[pid];
        }
        return status;
    }
    ++ServiceCount[pid];
    return 0;
}

DWORD StopRobotService(DWORD pid)
{
    DWORD cpid = GetCurrentProcessId();
    if (pid == 0)
        return cpid;
    --ServiceCount[pid];
    if (ServiceCount[pid] == 0)
    {
        RemoveDll(pid);
    }
    return 0;
}

wstring GetComWorkPath()
{
    wchar_t szFilePath[MAX_PATH + 1] = {0};
    GetModuleFileName(NULL, szFilePath, MAX_PATH);
    wstring wpath = szFilePath;
    int pos = wpath.find_last_of(L"\\");
    wpath = wpath.substr(0, pos);
    return wpath;
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

DWORD GetWeChatVerInt()
{
    DWORD version = 0x0;
    GetWeChatInstallInfo((TCHAR *)TEXT("CrashVersion"), (void *)&version, sizeof(DWORD));
    return version;
}

tstring GetWeChatVerStr()
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
            CloseProcessHandle(pe.th32ProcessID, L"_WeChat_App_Instance_Identity_Mutex_Name");
        }
        flag = Process32Next(hsnapshot, &pe);
    }
    CloseHandle(hsnapshot);
    return true;
}

DWORD StartWeChat()
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

DWORD GetRemoteProcAddr(DWORD pid, LPWSTR modulename, LPSTR procname)
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    DWORD dwId = 0, dwProcAddr = 0;
    unsigned char getremoteprocasmcode[] = {
        0x55,                         // push ebp;
        0x8B, 0xEC,                   // mov ebp, esp;
        0x83, 0xEC, 0x40,             // sub esp, 0x40;
        0x57,                         // push edi;
        0x51,                         // push ecx;
        0x8B, 0x7D, 0x08,             // mov edi, dword ptr[ebp + 0x8];
        0x8B, 0x07,                   // mov eax,dword ptr[edi];
        0x50,                         // push eax;
        0xE8, 0x00, 0x00, 0x00, 0x00, // call GetModuleHandleW;
        0x83, 0xC4, 0x04,             // add esp,0x4;
        0x83, 0xC7, 0x04,             // add edi,0x4;
        0x8B, 0x0F,                   // mov ecx, dword ptr[edi];
        0x51,                         // push ecx;
        0x50,                         // push eax;
        0xE8, 0x00, 0x00, 0x00, 0x00, // call GetProcAddress;
        0x83, 0xC4, 0x08,             // add esp, 0x8;
        0x59,                         // pop ecx;
        0x5F,                         // pop edi;
        0x8B, 0xE5,                   // mov esp, ebp;
        0x5D,                         // pop ebp;
        0xC3                          // retn;
    };
    DWORD pGetModuleHandleW = (DWORD)GetModuleHandleW;
    DWORD pGetProcAddress = (DWORD)GetProcAddress;
    PVOID call1 = (PVOID)&getremoteprocasmcode[15];
    PVOID call2 = (PVOID)&getremoteprocasmcode[30];
    LPVOID pRemoteAddress = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_EXECUTE);
    if (!pRemoteAddress)
        return 0;
    *(DWORD *)call1 = pGetModuleHandleW - (DWORD)pRemoteAddress - 14 - 5;
    *(DWORD *)call2 = pGetProcAddress - (DWORD)pRemoteAddress - 29 - 5;
    SIZE_T dwWriteSize;
    WriteProcessMemory(hProcess, pRemoteAddress, getremoteprocasmcode, sizeof(getremoteprocasmcode), &dwWriteSize);
    struct GetProcAddrStruct
    {
        DWORD hModuleNameAddr;
        DWORD funcnameAddr;
    } params;
    WeChatData<wchar_t *> r_modulename(hProcess, modulename, TEXTLENGTH(modulename));
    WeChatData<char *> r_procname(hProcess, procname, TEXTLENGTHA(procname));
    params.funcnameAddr = (DWORD)r_procname.GetAddr();
    params.hModuleNameAddr = (DWORD)r_modulename.GetAddr();
    WeChatData<GetProcAddrStruct *> r_params(hProcess, &params, sizeof(params));
    if (r_modulename.GetAddr() == 0 || r_procname.GetAddr() == 0 || r_params.GetAddr() == 0)
        return 0;
    DWORD ret = CallRemoteFunction(hProcess, pRemoteAddress, r_params.GetAddr());
    VirtualFreeEx(hProcess, pRemoteAddress, 0, MEM_RELEASE);
    return ret;
}
