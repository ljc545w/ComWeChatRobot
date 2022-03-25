#include "pch.h"

struct SendTextStruct
{
    DWORD wxid;
    DWORD wxmsg;
};

int SendText(wchar_t* wxid, wchar_t* wxmsg) {
    if (!hProcess)
        return 1;
    DWORD WeChatRobotBase = GetWeChatRobotBase();
    DWORD dwId = 0;
    DWORD dwWriteSize = 0;
    SendTextStruct params;
    ZeroMemory(&params, sizeof(params));
    LPVOID wxidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    LPVOID wxmsgaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    SendTextStruct* paramAndFunc = (SendTextStruct*)::VirtualAllocEx(hProcess, 0, sizeof(SendTextStruct), MEM_COMMIT, PAGE_READWRITE);
    if (!wxidaddr || !wxmsgaddr || !paramAndFunc || !WeChatRobotBase) {
        return 1;
    }
    DWORD dwTId = 0;

    if (wxidaddr)
        WriteProcessMemory(hProcess, wxidaddr, wxid, wcslen(wxid) * 2 + 2, &dwWriteSize);

    if (wxmsgaddr)
        WriteProcessMemory(hProcess, wxmsgaddr, wxmsg, wcslen(wxmsg) * 2 + 2, &dwWriteSize);

    params.wxid = (DWORD)wxidaddr;
    params.wxmsg = (DWORD)wxmsgaddr;

    if (paramAndFunc) {
        if (!::WriteProcessMemory(hProcess, paramAndFunc, &params, sizeof(params), &dwTId))
        {
            return 1;
        }
    }
    else {
        return 1;
    }

    DWORD SendTextRemoteAddr = WeChatRobotBase + SendTextOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)SendTextRemoteAddr, (LPVOID)paramAndFunc, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
    }
    else {
        return 1;
    }
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, wxidaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, wxmsgaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, paramAndFunc, 0, MEM_RELEASE);
    return 0;
}