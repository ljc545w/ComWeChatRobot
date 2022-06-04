#include "pch.h"

struct AddFriendByWxidStruct {
    DWORD wxid;
    DWORD message;
};

BOOL AddFriendByWxid(wchar_t* wxid,wchar_t* message) {
    if (!hProcess)
        return 1;
    DWORD WeChatRobotBase = GetWeChatRobotBase();
    DWORD dwId = 0;
    DWORD dwWriteSize = 0;
    DWORD dwRet = 1;

    LPVOID wxidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    LPVOID messageaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    AddFriendByWxidStruct* paramAndFunc = (AddFriendByWxidStruct*)VirtualAllocEx(hProcess, 0, sizeof(AddFriendByWxidStruct), MEM_COMMIT, PAGE_READWRITE);
    if (!wxidaddr || !messageaddr || !paramAndFunc)
        return 1;
    WriteProcessMemory(hProcess, wxidaddr, wxid, wcslen(wxid) * 2 + 2, &dwWriteSize);
    if(message)
        WriteProcessMemory(hProcess, messageaddr, message, wcslen(message) * 2 + 2, &dwWriteSize);
    
    AddFriendByWxidStruct params = { 0 };
    params.wxid = (DWORD)wxidaddr;
    params.message = message ? (DWORD)messageaddr : 0;
    WriteProcessMemory(hProcess, paramAndFunc, &params, sizeof(params), &dwWriteSize);
    DWORD AddFriendByWxidAddr = WeChatRobotBase + AddFriendByWxidRemoteOffset;
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)AddFriendByWxidAddr, (LPVOID)paramAndFunc, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &dwRet);
        CloseHandle(hThread);
    }

    VirtualFreeEx(hProcess, wxidaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, messageaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, paramAndFunc, 0, MEM_RELEASE);
    return dwRet == 0;
}