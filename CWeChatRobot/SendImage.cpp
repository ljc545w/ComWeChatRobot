#include "pch.h"

struct ImageParamStruct {
    DWORD wxid;
    DWORD imagepath;
};

int SendImage(DWORD pid,wchar_t* wxid, wchar_t* imagepath) {
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
    ImageParamStruct params;
    ZeroMemory(&params, sizeof(params));
    LPVOID wxidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    LPVOID imagepathaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    ImageParamStruct* paramAndFunc = (ImageParamStruct*)::VirtualAllocEx(hProcess, 0, sizeof(ImageParamStruct), MEM_COMMIT, PAGE_READWRITE);
    if (!wxidaddr || !imagepathaddr || !paramAndFunc || !WeChatRobotBase) {
        CloseHandle(hProcess);
        return 1;
    }
    DWORD dwTId = 0;

    if (wxidaddr)
        WriteProcessMemory(hProcess, wxidaddr, wxid, wcslen(wxid) * 2 + 2, &dwWriteSize);

    if (imagepathaddr)
        WriteProcessMemory(hProcess, imagepathaddr, imagepath, wcslen(imagepath) * 2 + 2, &dwWriteSize);

    params.wxid = (DWORD)wxidaddr;
    params.imagepath = (DWORD)imagepathaddr;

    if (paramAndFunc) {
        WriteProcessMemory(hProcess, paramAndFunc, &params, sizeof(params), &dwTId);
    }
    else {
        CloseHandle(hProcess);
        return 1;
    }

    DWORD SendImageRemoteAddr = WeChatRobotBase + SendImageOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)SendImageRemoteAddr, (LPVOID)paramAndFunc, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
    }
    else {
        CloseHandle(hProcess);
        return 1;
    }
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, wxidaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, imagepathaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, paramAndFunc, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return 0;
}
