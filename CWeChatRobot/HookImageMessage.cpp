#include "pch.h"

BOOL HookImageMsg(wchar_t* savepath) {
    if (!hProcess)
        return 1;
    DWORD WeChatRobotBase = GetWeChatRobotBase();
    DWORD dwId = 0;
    DWORD dwRet = 0x0;
    LPVOID savepathaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    DWORD dwWriteSize = 0;
    if (!savepathaddr)
        return 1;
    WriteProcessMemory(hProcess, savepathaddr, savepath, wcslen(savepath) * 2 + 2, &dwWriteSize);
    DWORD HookImageMsgRemoteAddr = WeChatRobotBase + HookImageMsgRemoteOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)HookImageMsgRemoteAddr, savepathaddr, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &dwRet);
        CloseHandle(hThread);
    }
    VirtualFreeEx(hProcess, savepathaddr, 0, MEM_RELEASE);
    return dwRet == 0;
}

void UnHookImageMsg() {
    if (!hProcess)
        return;
    DWORD dwId = 0x0;
    DWORD UnHookImageMsgRemoteAddr = GetWeChatRobotBase() + UnHookImageMsgRemoteOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)UnHookImageMsgRemoteAddr, NULL, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }
}