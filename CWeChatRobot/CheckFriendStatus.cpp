#include "pch.h"

BOOL CheckFriendStatusInit() {
    if (!hProcess)
        return 1;
    DWORD WeChatRobotBase = GetWeChatRobotBase();
    DWORD dwId = 0;
    DWORD CheckFriendStatusInitRemoteAddr = WeChatRobotBase + CheckFriendStatusInitRemoteOffset;
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)CheckFriendStatusInitRemoteAddr, NULL, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }
    return 0;
}

BOOL CheckFriendStatusFinish() {
    if (!hProcess)
        return 1;
    DWORD WeChatRobotBase = GetWeChatRobotBase();
    DWORD dwId = 0;
    DWORD CheckFriendStatusFinishRemoteAddr = WeChatRobotBase + CheckFriendStatusFinishRemoteOffset;
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)CheckFriendStatusFinishRemoteAddr, NULL, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }
    return 0;
}

DWORD CheckFriendStatus(wchar_t* wxid) {
    if (!hProcess)
        return 1;
    DWORD WeChatRobotBase = GetWeChatRobotBase();
    DWORD dwId = 0;
    DWORD dwWriteSize = 0;
    DWORD dwStatus = 0;

    LPVOID wxidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    if (!wxidaddr)
        return 1;
    WriteProcessMemory(hProcess, wxidaddr, wxid, wcslen(wxid) * 2 + 2, &dwWriteSize);
    DWORD CheckFriendStatusRemoteAddr = WeChatRobotBase + CheckFriendStatusRemoteOffset;
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)CheckFriendStatusRemoteAddr, (LPVOID)wxidaddr, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &dwStatus);
        CloseHandle(hThread);
    }

    VirtualFreeEx(hProcess, wxidaddr, 0, MEM_RELEASE);
    return dwStatus;
}