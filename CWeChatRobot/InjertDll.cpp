#include "pch.h"


bool InjectDll(DWORD dwId, WCHAR* szPath)//参数1：目标进程PID  参数2：DLL路径
{
    if (!hProcess)
        return 1;
    if (GetWeChatRobotBase() != 0) {
        return 1;
    }

    LPVOID pRemoteAddress = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    DWORD dwWriteSize = 0;
    if (pRemoteAddress)
    {
        WriteProcessMemory(hProcess, pRemoteAddress, szPath, wcslen(szPath) * 2 + 2, &dwWriteSize);
    }
    else {
        return 1;
    }

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibrary, pRemoteAddress, NULL, NULL);
    if (hThread) {
        WaitForSingleObject(hThread, -1);
    }
    else {
        return 1;
    }
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pRemoteAddress, 0, MEM_RELEASE);
    return 0;
}

bool Injert(DWORD dwPid,wchar_t* workPath) {
    wchar_t* dllpath = new wchar_t[MAX_PATH];
    swprintf_s(dllpath, MAX_PATH, L"%ws%ws%ws", workPath, L"\\", dllname);
    string name = _com_util::ConvertBSTRToString((BSTR)workPath); 
    if (!isFileExists_stat(name)) {
        MessageBox(NULL, dllpath, L"文件不存在", MB_ICONWARNING);
        return 1;
    }
    bool status = InjectDll(dwPid, dllpath);
    delete[] dllpath;
    dllpath = NULL;
    return status;
}

BOOL RemoveDll(DWORD dwId) {
    if (!hProcess)
        return 1;
    LPVOID pRemoteAddress = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    DWORD dwWriteSize = 0;
    HANDLE hThread = NULL;
    DWORD dwHandle, dwID;
    LPVOID pFunc = NULL;
    if (pRemoteAddress)
        WriteProcessMemory(hProcess, pRemoteAddress, dllname, wcslen(dllname) * 2 + 2, &dwWriteSize);
    else {
        return 1;
    }
    pFunc = GetModuleHandleW;
    hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunc, pRemoteAddress, 0, &dwID);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &dwHandle);
    }
    else {
        return 1;
    }
    CloseHandle(hThread);

    pFunc = FreeConsole;
    hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunc, NULL, 0, &dwID);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
    }
    else {
        return 1;
    }
    CloseHandle(hThread);

    pFunc = FreeLibrary;
    hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunc, (LPVOID)dwHandle, 0, &dwID);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
    }
    else {
        return 1;
    }
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pRemoteAddress, 0, MEM_RELEASE);
    return 0;
}
