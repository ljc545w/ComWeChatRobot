#include "pch.h"

BOOL AddBrandContact(wchar_t* PublicId) {
    if (!hProcess)
        return 1;
    DWORD WeChatRobotBase = GetWeChatRobotBase();
    DWORD dwId = 0;
    DWORD dwWriteSize = 0;
    DWORD dwRet = 1;

    LPVOID PublicIdaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    if (!PublicIdaddr)
        return 1;
    WriteProcessMemory(hProcess, PublicIdaddr, PublicId, wcslen(PublicId) * 2 + 2, &dwWriteSize);
    DWORD AddBrandContactAddr = WeChatRobotBase + AddBrandContactRemoteOffset;
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)AddBrandContactAddr, (LPVOID)PublicIdaddr, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &dwRet);
        CloseHandle(hThread);
    }

    VirtualFreeEx(hProcess, PublicIdaddr, 0, MEM_RELEASE);
    return dwRet == 0;
}