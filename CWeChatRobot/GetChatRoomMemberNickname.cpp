#include "pch.h"

struct ChatRoomMemberNicknameStruct
{
    DWORD chatroomid;
    DWORD wxid;
    DWORD nickname;
};

wstring GetChatRoomMemberNickname(wchar_t* chatroomid, wchar_t* wxid) {
    if (!hProcess)
        return L"";
    DWORD WeChatRobotBase = GetWeChatRobotBase();
    DWORD dwId = 0;
    DWORD dwWriteSize = 0;
    DWORD dwRet = 0;
    ChatRoomMemberNicknameStruct params;
    ZeroMemory(&params, sizeof(params));
    LPVOID chatroomidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    LPVOID wxidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    LPVOID nicknameaddr = VirtualAllocEx(hProcess, NULL, 33 * 2, MEM_COMMIT, PAGE_READWRITE);
    ChatRoomMemberNicknameStruct* paramAndFunc = (ChatRoomMemberNicknameStruct*)::VirtualAllocEx(hProcess, 0, sizeof(ChatRoomMemberNicknameStruct), MEM_COMMIT, PAGE_READWRITE);
    if (!chatroomidaddr || !wxidaddr || !nicknameaddr || !paramAndFunc) {
        return L"";
    }
    DWORD dwTId = 0;

    if (chatroomidaddr)
        WriteProcessMemory(hProcess, chatroomidaddr, chatroomid, wcslen(chatroomid) * 2 + 2, &dwWriteSize);

    if (wxidaddr)
        WriteProcessMemory(hProcess, wxidaddr, wxid, wcslen(wxid) * 2 + 2, &dwWriteSize);

    params.chatroomid = (DWORD)chatroomidaddr;
    params.wxid = (DWORD)wxidaddr;
    params.nickname = (DWORD)nicknameaddr;

    if (paramAndFunc) {
        WriteProcessMemory(hProcess, paramAndFunc, &params, sizeof(params), &dwTId);
    }
    else {
        return L"";
    }

    DWORD GetChatRoomMemberNicknameAddr = WeChatRobotBase + GetChatRoomMemberNicknameRemoteOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)GetChatRoomMemberNicknameAddr, (LPVOID)paramAndFunc, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &dwRet);
        CloseHandle(hThread);
    }
    else {
        return L"";
    }
    wchar_t* buffer = new wchar_t[33];
    ZeroMemory(buffer, 33 * 2);
    ReadProcessMemory(hProcess, nicknameaddr, buffer, 32 * 2, 0);
    wstring nickname(buffer);
    delete[] buffer;
    VirtualFreeEx(hProcess, chatroomidaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, nicknameaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, paramAndFunc, 0, MEM_RELEASE);
    return nickname;
}