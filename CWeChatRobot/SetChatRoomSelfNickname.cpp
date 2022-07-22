#include "pch.h"

struct ChatRoomSelfNicknameStruct
{
    DWORD chatroomid;
    DWORD nickname;
};

BOOL SetChatRoomSelfNickname(DWORD pid,wchar_t* chatroomid, wchar_t* nickname) {
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
    ChatRoomSelfNicknameStruct params;
    ZeroMemory(&params, sizeof(params));
    LPVOID chatroomidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    LPVOID nicknameaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    ChatRoomSelfNicknameStruct* paramAndFunc = (ChatRoomSelfNicknameStruct*)::VirtualAllocEx(hProcess, 0, sizeof(ChatRoomSelfNicknameStruct), MEM_COMMIT, PAGE_READWRITE);
    if (!chatroomidaddr || !nicknameaddr || !paramAndFunc) {
        CloseHandle(hProcess);
        return 1;
    }
    DWORD dwTId = 0;

    if (chatroomidaddr)
        WriteProcessMemory(hProcess, chatroomidaddr, chatroomid, wcslen(chatroomid) * 2 + 2, &dwWriteSize);

    if (nicknameaddr)
        WriteProcessMemory(hProcess, nicknameaddr, nickname, wcslen(nickname) * 2 + 2, &dwWriteSize);

    params.chatroomid = (DWORD)chatroomidaddr;
    params.nickname = (DWORD)nicknameaddr;

    if (paramAndFunc) {
        WriteProcessMemory(hProcess, paramAndFunc, &params, sizeof(params), &dwTId);
    }
    else {
        CloseHandle(hProcess);
        return 1;
    }

    DWORD SetChatRoomSelfNicknameAddr = WeChatRobotBase + SetChatRoomSelfNicknameRemoteOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)SetChatRoomSelfNicknameAddr, (LPVOID)paramAndFunc, 0, &dwId);
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
    VirtualFreeEx(hProcess, nicknameaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, paramAndFunc, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return dwRet == 0;
}