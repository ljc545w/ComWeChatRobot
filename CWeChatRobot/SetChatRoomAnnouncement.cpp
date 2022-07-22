#include "pch.h"

struct ChatRoomAnnouncementStruct
{
    DWORD chatroomid;
    DWORD announcement;
};

BOOL SetChatRoomAnnouncement(DWORD pid,wchar_t* chatroomid, wchar_t* announcement) {
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
    ChatRoomAnnouncementStruct params;
    ZeroMemory(&params, sizeof(params));
    LPVOID chatroomidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    LPVOID announcementaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    ChatRoomAnnouncementStruct* paramAndFunc = (ChatRoomAnnouncementStruct*)::VirtualAllocEx(hProcess, 0, sizeof(ChatRoomAnnouncementStruct), MEM_COMMIT, PAGE_READWRITE);
    if (!chatroomidaddr || !announcementaddr || !paramAndFunc) {
        CloseHandle(hProcess);
        return 1;
    }
    DWORD dwTId = 0;

    if (chatroomidaddr)
        WriteProcessMemory(hProcess, chatroomidaddr, chatroomid, wcslen(chatroomid) * 2 + 2, &dwWriteSize);

    if (announcement && announcementaddr)
        WriteProcessMemory(hProcess, announcementaddr, announcement, wcslen(announcement) * 2 + 2, &dwWriteSize);

    params.chatroomid = (DWORD)chatroomidaddr;
    params.announcement = announcement ? (DWORD)announcementaddr : 0;

    if (paramAndFunc) {
        WriteProcessMemory(hProcess, paramAndFunc, &params, sizeof(params), &dwTId);
    }
    else {
        CloseHandle(hProcess);
        return 1;
    }

    DWORD SetChatRoomAnnouncementAddr = WeChatRobotBase + SetChatRoomAnnouncementRemoteOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)SetChatRoomAnnouncementAddr, (LPVOID)paramAndFunc, 0, &dwId);
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
    VirtualFreeEx(hProcess, announcementaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, paramAndFunc, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return dwRet == 0;
}