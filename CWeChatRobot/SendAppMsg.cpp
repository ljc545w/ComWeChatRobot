#include "pch.h"

struct SendAppMsgStruct
{
    DWORD wxid;
    DWORD appid;
};

BOOL SendAppMsg(wchar_t* wxid, wchar_t* appid) {
    if (!hProcess)
        return 1;
    DWORD WeChatRobotBase = GetWeChatRobotBase();
    DWORD dwId = 0;
    DWORD dwWriteSize = 0;
    DWORD dwRet = 0x0;
    SendAppMsgStruct params;
    ZeroMemory(&params, sizeof(params));
    LPVOID wxidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    LPVOID appidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    SendAppMsgStruct* paramAndFunc = (SendAppMsgStruct*)::VirtualAllocEx(hProcess, 0, sizeof(SendAppMsgStruct), MEM_COMMIT, PAGE_READWRITE);
    if (!wxidaddr || !appidaddr || !paramAndFunc || !WeChatRobotBase) {
        return 1;
    }

    if (wxidaddr)
        WriteProcessMemory(hProcess, wxidaddr, wxid, wcslen(wxid) * 2 + 2, &dwWriteSize);

    if (appidaddr)
        WriteProcessMemory(hProcess, appidaddr, appid, wcslen(appid) * 2 + 2, &dwWriteSize);

    params.wxid = (DWORD)wxidaddr;
    params.appid = (DWORD)appidaddr;

    if (paramAndFunc)
        WriteProcessMemory(hProcess, paramAndFunc, &params, sizeof(params), &dwWriteSize);
      
    DWORD SendAppMsgRemoteAddr = WeChatRobotBase + SendAppMsgRemoteOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)SendAppMsgRemoteAddr, (LPVOID)paramAndFunc, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &dwRet);
        CloseHandle(hThread);
    }
    
    VirtualFreeEx(hProcess, wxidaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, appidaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, paramAndFunc, 0, MEM_RELEASE);
    return dwRet == 0;
}