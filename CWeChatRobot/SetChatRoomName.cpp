#include "pch.h"

struct ChatRoomNameStruct
{
    DWORD chatroomid;
    DWORD name;
};

BOOL SetChatRoomName(DWORD pid,wchar_t* chatroomid, wchar_t* name) {
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
    DWORD dwRet = 0;
    ChatRoomNameStruct params;
    ZeroMemory(&params, sizeof(params));
    LPVOID chatroomidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    LPVOID nameaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    ChatRoomNameStruct* paramAndFunc = (ChatRoomNameStruct*)::VirtualAllocEx(hProcess, 0, sizeof(ChatRoomNameStruct), MEM_COMMIT, PAGE_READWRITE);
    if (!chatroomidaddr || !nameaddr || !paramAndFunc) {
        CloseHandle(hProcess);
        return 1;
    }
    DWORD dwTId = 0;

    if (chatroomidaddr)
        WriteProcessMemory(hProcess, chatroomidaddr, chatroomid, wcslen(chatroomid) * 2 + 2, &dwWriteSize);

    if (nameaddr)
        WriteProcessMemory(hProcess, nameaddr, name, wcslen(name) * 2 + 2, &dwWriteSize);

    params.chatroomid = (DWORD)chatroomidaddr;
    params.name = (DWORD)nameaddr;

    if (paramAndFunc) {
        WriteProcessMemory(hProcess, paramAndFunc, &params, sizeof(params), &dwTId);
    }
    else {
        CloseHandle(hProcess);
        return 1;
    }

    DWORD SetChatRoomNameAddr = WeChatRobotBase + SetChatRoomNameRemoteOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)SetChatRoomNameAddr, (LPVOID)paramAndFunc, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &dwRet);
        CloseHandle(hThread);
    }
    else {
        CloseHandle(hProcess);
        return 1;
    }
    VirtualFreeEx(hProcess, chatroomidaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, nameaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, paramAndFunc, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return dwRet == 0;
}