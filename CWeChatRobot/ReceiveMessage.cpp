#include "pch.h"

BOOL StartReceiveMessage(DWORD pid,int port) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess)
        return 1;
    DWORD WeChatRobotBase = GetWeChatRobotBase(pid);
    if (!WeChatRobotBase) {
        CloseHandle(hProcess);
        return 1;
    }
    DWORD dwId = 0;

    DWORD HookReceiveMessageAddr = WeChatRobotBase + HookReceiveMessageRemoteOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)HookReceiveMessageAddr, (LPVOID)port, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
    }
    else {
        CloseHandle(hProcess);
        return 1;
    }
    CloseHandle(hThread);
    CloseHandle(hProcess);
    return 0;
}

BOOL StopReceiveMessage(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess) {
        return 1;
    }
    DWORD WeChatRobotBase = GetWeChatRobotBase(pid);
    if (!WeChatRobotBase) {
        CloseHandle(hProcess);
        return 1;
    }
    DWORD dwId = 0;

    DWORD UnHookReceiveMessageAddr = WeChatRobotBase + UnHookReceiveMessageRemoteOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)UnHookReceiveMessageAddr, NULL, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
    }
    else {
        CloseHandle(hProcess);
        return 1;
    }
    CloseHandle(hThread);
    CloseHandle(hProcess);
    return 0;
}