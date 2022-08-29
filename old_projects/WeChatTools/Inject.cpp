#include "base.h"
HANDLE hProcess = NULL;

bool isFileExists_stat(string &name)
{
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

string wstring2string(wstring wstr)
{
    std::string result;
    //获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的
    int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
    char *buffer = new char[len + 1];
    //宽字节编码转换成多字节编码
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
    buffer[len] = '\0';
    //删除缓冲区并返回值
    result.append(buffer);
    delete[] buffer;
    return result;
}

bool InjectDll(DWORD dwId, WCHAR *szPath) //参数1：目标进程PID  参数2：DLL路径
{
    if (!hProcess)
        return 1;
    if (GetWeChatRobotBase() != 0)
    {
        printf("请勿重复注入\n");
        return 1;
    }

    /*
    【1.2 在目标进程的内存里开辟空间】
    */
    LPVOID pRemoteAddress = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);

    //二、 把dll的路径写入到目标进程的内存空间中
    DWORD dwWriteSize = 0;
    /*
    【写一段数据到刚才给指定进程所开辟的内存空间里】
    */
    if (pRemoteAddress)
    {
        WriteProcessMemory(hProcess, pRemoteAddress, szPath, wcslen(szPath) * 2 + 2, &dwWriteSize);
    }
    else
    {
        printf("写入失败!\n");
        return 1;
    }

    //三、 创建一个远程线程，让目标进程调用LoadLibrary
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibrary, pRemoteAddress, NULL, NULL);
    if (hThread)
    {
        WaitForSingleObject(hThread, -1);
    }
    else
    {
        printf("调用失败!\n");
        return 1;
    }
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pRemoteAddress, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return 0;
}

void Inject(DWORD dwPid, wchar_t *wStr)
{
    wchar_t dllpath[MAX_PATH];
    wchar_t workPath[MAX_PATH];
    wchar_t *pworkPath = _wgetcwd(workPath, MAX_PATH);
    swprintf_s(dllpath, MAX_PATH, L"%ws%ws%ws", pworkPath, L"\\", wStr);
    string name = wstring2string((wstring)dllpath);
    if (!isFileExists_stat(name))
    {
        wstring info = L"请检查目标文件路径！";
        MessageBox(NULL, info.c_str(), _T("警告"), MB_ICONWARNING);
        return;
    }
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    InjectDll(dwPid, dllpath);
}

BOOL RemoveDll(DWORD dwId)
{
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwId);
    LPVOID pRemoteAddress = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    DWORD dwWriteSize = 0;
    HANDLE hThread = NULL;
    DWORD dwHandle, dwID;
    LPVOID pFunc = NULL;
    if (pRemoteAddress)
        WriteProcessMemory(hProcess, pRemoteAddress, dllname, wcslen(dllname) * 2 + 2, &dwWriteSize);
    else
    {
        printf("写入失败!\n");
        return 1;
    }
    pFunc = GetModuleHandleW;
    hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunc, pRemoteAddress, 0, &dwID);
    if (hThread)
    {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &dwHandle);
    }
    else
    {
        printf("GetModuleHandleW调用失败!\n");
        return 1;
    }
    CloseHandle(hThread);

    // 释放console窗口，不然关闭console的同时微信也会退出
    pFunc = FreeConsole;
    hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunc, NULL, 0, &dwID);
    if (hThread)
    {
        WaitForSingleObject(hThread, INFINITE);
    }
    else
    {
        printf("FreeConsole调用失败!\n");
        return 1;
    }
    CloseHandle(hThread);

    // 使目标进程调用FreeLibrary，卸载DLL
    pFunc = FreeLibrary;
    hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunc, (LPVOID)dwHandle, 0, &dwID);
    if (hThread)
    {
        WaitForSingleObject(hThread, INFINITE);
    }
    else
    {
        printf("FreeLibrary调用失败!\n");
        return 1;
    }
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pRemoteAddress, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return 0;
}
