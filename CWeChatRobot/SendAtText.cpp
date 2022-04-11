#include "pch.h"

struct SendAtTextStruct
{
    DWORD chatroomid;
    DWORD wxid;
    DWORD wxmsg;
};

int SendAtText(wchar_t* chatroomid, wchar_t* wxid, wchar_t* wxmsg) {
    if (!hProcess)
        return 1;
    DWORD WeChatRobotBase = GetWeChatRobotBase();
    DWORD dwId = 0;
    DWORD dwWriteSize = 0;
    SendAtTextStruct params;
    ZeroMemory(&params, sizeof(params));
    LPVOID chatroomidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    LPVOID wxidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    LPVOID wxmsgaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    SendAtTextStruct* paramAndFunc = (SendAtTextStruct*)::VirtualAllocEx(hProcess, 0, sizeof(SendAtTextStruct), MEM_COMMIT, PAGE_READWRITE);
    if (!chatroomidaddr || !wxidaddr || !wxmsgaddr || !paramAndFunc || !WeChatRobotBase) {
        return 1;
    }
    DWORD dwTId = 0;

    if (chatroomidaddr)
        WriteProcessMemory(hProcess, chatroomidaddr, chatroomid, wcslen(chatroomid) * 2 + 2, &dwWriteSize);

    if (wxidaddr)
        WriteProcessMemory(hProcess, wxidaddr, wxid, wcslen(wxid) * 2 + 2, &dwWriteSize);

    if (wxmsgaddr)
        WriteProcessMemory(hProcess, wxmsgaddr, wxmsg, wcslen(wxmsg) * 2 + 2, &dwWriteSize);

    params.chatroomid = (DWORD)chatroomidaddr;
    params.wxid = (DWORD)wxidaddr;
    params.wxmsg = (DWORD)wxmsgaddr;

    if (paramAndFunc) {
        if (!::WriteProcessMemory(hProcess, paramAndFunc, &params, sizeof(SendAtTextStruct), &dwTId))
        {
            return 1;
        }
    }
    else {
        return 1;
    }

    DWORD SendAtTextRemoteAddr = WeChatRobotBase + SendAtTextOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)SendAtTextRemoteAddr, (LPVOID)paramAndFunc, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
    }
    else {
        return 1;
    }
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, chatroomidaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, wxidaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, wxmsgaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, paramAndFunc, 0, MEM_RELEASE);
    return 0;
}