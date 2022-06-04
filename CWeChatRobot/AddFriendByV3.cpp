#include "pch.h"

struct AddFriendByV3Struct {
    DWORD v3;
    DWORD message;
    DWORD AddType;
};

BOOL AddFriendByV3(wchar_t* v3, wchar_t* message,int AddType) {
    if (!hProcess)
        return 1;
    DWORD WeChatRobotBase = GetWeChatRobotBase();
    DWORD dwId = 0;
    DWORD dwWriteSize = 0;
    DWORD dwRet = 1;

    LPVOID v3addr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    LPVOID messageaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    AddFriendByV3Struct* paramAndFunc = (AddFriendByV3Struct*)VirtualAllocEx(hProcess, 0, sizeof(AddFriendByV3Struct), MEM_COMMIT, PAGE_READWRITE);
    if (!v3addr || !messageaddr || !paramAndFunc)
        return 1;
    WriteProcessMemory(hProcess, v3addr, v3, wcslen(v3) * 2 + 2, &dwWriteSize);
    if(message)
        WriteProcessMemory(hProcess, messageaddr, message, wcslen(message) * 2 + 2, &dwWriteSize);

    AddFriendByV3Struct params = { 0 };
    params.v3 = (DWORD)v3addr;
    params.message = message ? (DWORD)messageaddr : 0;
    params.AddType = AddType;
    WriteProcessMemory(hProcess, paramAndFunc, &params, sizeof(params), &dwWriteSize);
    DWORD AddFriendByV3Addr = WeChatRobotBase + AddFriendByV3RemoteOffset;
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)AddFriendByV3Addr, (LPVOID)paramAndFunc, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &dwRet);
        CloseHandle(hThread);
    }

    VirtualFreeEx(hProcess, v3addr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, messageaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, paramAndFunc, 0, MEM_RELEASE);
    return dwRet == 0;
}