#include "pch.h"

BOOL ReceiveMessageHooked = FALSE;

BOOL StartReceiveMessage(int port) {
    if (!hProcess || ReceiveMessageHooked)
        return 1;
    DWORD WeChatRobotBase = GetWeChatRobotBase();
    DWORD dwId = 0;

    DWORD HookReceiveMessageAddr = WeChatRobotBase + HookReceiveMessageRemoteOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)HookReceiveMessageAddr, (LPVOID)port, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
    }
    else {
        return 1;
    }
    CloseHandle(hThread);
    ReceiveMessageHooked = TRUE;
    return 0;
}

BOOL StopReceiveMessage() {
    if (!hProcess || !ReceiveMessageHooked) {
        ReceiveMessageHooked = FALSE;
        return 1;
    }
    DWORD WeChatRobotBase = GetWeChatRobotBase();
    DWORD dwId = 0;

    DWORD UnHookReceiveMessageAddr = WeChatRobotBase + UnHookReceiveMessageRemoteOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)UnHookReceiveMessageAddr, NULL, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
    }
    else {
        return 1;
    }
    CloseHandle(hThread);
    ReceiveMessageHooked = FALSE;
    return 0;
}