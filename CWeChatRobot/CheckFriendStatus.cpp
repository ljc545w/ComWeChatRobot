#include "pch.h"

DWORD CheckFriendStatus(DWORD pid,wchar_t* wxid) {
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
    DWORD dwStatus = 0;

    LPVOID wxidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    if (!wxidaddr) {
        CloseHandle(hProcess);
        return 1;
    }
    WriteProcessMemory(hProcess, wxidaddr, wxid, wcslen(wxid) * 2 + 2, &dwWriteSize);
    DWORD CheckFriendStatusRemoteAddr = WeChatRobotBase + CheckFriendStatusRemoteOffset;
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)CheckFriendStatusRemoteAddr, (LPVOID)wxidaddr, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &dwStatus);
        CloseHandle(hThread);
    }

    VirtualFreeEx(hProcess, wxidaddr, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return dwStatus;
}