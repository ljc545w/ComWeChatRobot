#include "pch.h"

bool InjectDll(DWORD dwId, WCHAR* szPath)//参数1：目标进程PID  参数2：DLL路径
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwId);
    if (!hProcess)
        return 1;
    if (GetWeChatRobotBase(dwId) != 0) {
        CloseHandle(hProcess);
        return 0;
    }

    LPVOID pRemoteAddress = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    DWORD dwWriteSize = 0;
    if (pRemoteAddress)
    {
        WriteProcessMemory(hProcess, pRemoteAddress, szPath, wcslen(szPath) * 2 + 2, &dwWriteSize);
    }
    else {
        CloseHandle(hProcess);
        return 1;
    }

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibrary, pRemoteAddress, NULL, NULL);
    if (hThread) {
        WaitForSingleObject(hThread, -1);
    }
    else {
        CloseHandle(hProcess);
        return 1;
    }
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pRemoteAddress, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return 0;
}

bool Inject(DWORD dwPid,wchar_t* workPath) {
    wchar_t* dllpath = new wchar_t[MAX_PATH];
    swprintf_s(dllpath, MAX_PATH, L"%ws%ws%ws", workPath, L"\\", dllname);
    string name = _com_util::ConvertBSTRToString((BSTR)workPath); 
    if (!isFileExists_stat(name)) {
        MessageBox(NULL, dllpath, L"文件不存在", MB_ICONWARNING);
        delete[] dllpath;
        dllpath = NULL;
        return 1;
    }
    bool status = InjectDll(dwPid, dllpath);
    delete[] dllpath;
    dllpath = NULL;
    return status;
}

BOOL RemoveDll(DWORD dwId) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwId);
    if (!hProcess)
        return 1;
    DWORD WeChatRobotBase = GetWeChatRobotBase(dwId);
    if (!WeChatRobotBase) {
        CloseHandle(hProcess);
        return 0;
    }
    DWORD dwWriteSize = 0;
    HANDLE hThread = NULL;
    DWORD dwID = 0;
    hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)FreeConsole, NULL, 0, &dwID);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }
    else {
        CloseHandle(hProcess);
        return 1;
    }
    
    hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)FreeLibrary, (LPVOID)WeChatRobotBase, 0, &dwID);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }
    else {
        CloseHandle(hProcess);
        return 1;
    }
    CloseHandle(hProcess);
    return 0;
}
