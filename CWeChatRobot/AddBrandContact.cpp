#include "pch.h"

BOOL AddBrandContact(DWORD pid,wchar_t* PublicId) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess)
        return 1;
    DWORD WeChatRobotBase = GetWeChatRobotBase(pid);
    if (!WeChatRobotBase) {
        CloseHandle(hProcess);
        return 1;
    }
    DWORD dwId = 0;
    DWORD dwWriteSize = 0;
    DWORD dwRet = 1;

    LPVOID PublicIdaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    if (!PublicIdaddr) {
        CloseHandle(hProcess);
        return 1;
    }
    WriteProcessMemory(hProcess, PublicIdaddr, PublicId, wcslen(PublicId) * 2 + 2, &dwWriteSize);
    DWORD AddBrandContactAddr = WeChatRobotBase + AddBrandContactRemoteOffset;
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)AddBrandContactAddr, (LPVOID)PublicIdaddr, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &dwRet);
        CloseHandle(hThread);
    }

    VirtualFreeEx(hProcess, PublicIdaddr, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return dwRet == 0;
}