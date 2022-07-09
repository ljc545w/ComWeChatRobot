#include "pch.h"

BOOL ChangeWeChatVer(wchar_t* verStr) {
    if (!hProcess)
        return 1;
    DWORD WeChatRobotBase = GetWeChatRobotBase();
    DWORD dwId = 0;
    DWORD dwRet = 0x0;
    LPVOID verStraddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    DWORD dwWriteSize = 0;
    if (!verStraddr)
        return 1;
    WriteProcessMemory(hProcess, verStraddr, verStr, wcslen(verStr) * 2 + 2, &dwWriteSize);
    DWORD ChangeWeChatVerRemoteAddr = WeChatRobotBase + ChangeWeChatVerRemoteOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)ChangeWeChatVerRemoteAddr, verStraddr, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &dwRet);
        CloseHandle(hThread);
    }
    VirtualFreeEx(hProcess, verStraddr, 0, MEM_RELEASE);
    return dwRet == 0;
}