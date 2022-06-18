#include "pch.h"
#include "driver.h"

BOOL InjectDll(DWORD dwId, const wchar_t* szPath)
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwId);
    if (!hProcess)
        return false;

    LPVOID pRemoteAddress = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    SIZE_T dwWriteSize = 0;
    if (pRemoteAddress)
    {
        WriteProcessMemory(hProcess, pRemoteAddress, szPath, wcslen(szPath) * 2 + 2, &dwWriteSize);
    }
    else {
        CloseHandle(hProcess);
        return false;
    }
#ifdef _WIN64
    PVOID pFunc = GetSystem32ProcAddr(L"\\KnownDlls32\\kernel32.dll","LoadLibraryW");
    if (pFunc == 0) {
        VirtualFreeEx(hProcess, pRemoteAddress, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }
#else
    PVOID pFunc = LoadLibraryW;
#endif
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunc, pRemoteAddress, NULL, NULL);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }
    else {
        VirtualFreeEx(hProcess, pRemoteAddress, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }
    VirtualFreeEx(hProcess, pRemoteAddress, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return true;
}

BOOL RemoveDll(DWORD dwId,PCWSTR dllname) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwId);
    if (!hProcess)
        return false;
    LPVOID pRemoteAddress = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    SIZE_T dwWriteSize = 0;
    HANDLE hThread = NULL;
    DWORD dwHandle, dwID;
    PVOID pFunc = NULL;
    if (pRemoteAddress)
        WriteProcessMemory(hProcess, pRemoteAddress, dllname, wcslen(dllname) * 2 + 2, &dwWriteSize);
    else {
        return false;
    }
#ifdef _WIN64
    pFunc = GetSystem32ProcAddr(L"\\KnownDlls32\\kernel32.dll", "GetModuleHandleW");
#else
    pFunc = GetModuleHandleW;
#endif
    hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunc, pRemoteAddress, 0, &dwID);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &dwHandle);
        CloseHandle(hThread);
    }
    else {
        VirtualFreeEx(hProcess, pRemoteAddress, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }
    if (!dwHandle) {
        VirtualFreeEx(hProcess, pRemoteAddress, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return true;
    }
#ifdef _WIN64
    pFunc = GetSystem32ProcAddr(L"\\KnownDlls32\\kernel32.dll", "FreeConsole");
#else
    pFunc = FreeConsole;
#endif
    hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunc, NULL, 0, &dwID);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }
    else {
        VirtualFreeEx(hProcess, pRemoteAddress, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }
#ifdef _WIN64
    pFunc = GetSystem32ProcAddr(L"\\KnownDlls32\\kernel32.dll", "FreeLibrary");
#else
    pFunc = FreeLibrary;
#endif
    hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunc, (LPVOID)(unsigned long long)dwHandle, 0, &dwID);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }
    else {
        VirtualFreeEx(hProcess, pRemoteAddress, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }
    VirtualFreeEx(hProcess, pRemoteAddress, 0, MEM_RELEASE);
    return true;
}