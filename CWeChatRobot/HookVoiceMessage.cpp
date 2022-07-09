#include "pch.h"

BOOL HookVoiceMsg(wchar_t* savepath) {
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
    DWORD HookVoiceMsgRemoteAddr = WeChatRobotBase + HookVoiceMsgRemoteOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)HookVoiceMsgRemoteAddr, savepathaddr, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &dwRet);
        CloseHandle(hThread);
    }
    VirtualFreeEx(hProcess, savepathaddr, 0, MEM_RELEASE);
    return dwRet == 0;
}

void UnHookVoiceMsg() {
    if (!hProcess)
        return;
    DWORD dwId = 0x0;
    DWORD UnHookVoiceMsgRemoteAddr = GetWeChatRobotBase() + UnHookVoiceMsgRemoteOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)UnHookVoiceMsgRemoteAddr, NULL, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }
}