#include "pch.h"

struct EditRemarkStruct {
    DWORD wxid;
    DWORD remark;
};

BOOL EditRemark(DWORD pid,wchar_t* wxid, wchar_t* remark) {
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
    DWORD dwRet = 1;

    LPVOID wxidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    LPVOID remarkaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    EditRemarkStruct* paramAndFunc = (EditRemarkStruct*)VirtualAllocEx(hProcess, 0, sizeof(EditRemarkStruct), MEM_COMMIT, PAGE_READWRITE);
    if (!wxidaddr || !remarkaddr || !paramAndFunc) {
        CloseHandle(hProcess);
        return 1;
    }
    WriteProcessMemory(hProcess, wxidaddr, wxid, wcslen(wxid) * 2 + 2, &dwWriteSize);
    if (remark)
        WriteProcessMemory(hProcess, remarkaddr, remark, wcslen(remark) * 2 + 2, &dwWriteSize);

    EditRemarkStruct params = { 0 };
    params.wxid = (DWORD)wxidaddr;
    params.remark = remark ? (DWORD)remarkaddr : 0;
    WriteProcessMemory(hProcess, paramAndFunc, &params, sizeof(params), &dwWriteSize);
    DWORD EditRemarkAddr = WeChatRobotBase + EditRemarkRemoteOffset;
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)EditRemarkAddr, (LPVOID)paramAndFunc, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &dwRet);
        CloseHandle(hThread);
    }

    VirtualFreeEx(hProcess, wxidaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, remarkaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, paramAndFunc, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return dwRet == 0;
}