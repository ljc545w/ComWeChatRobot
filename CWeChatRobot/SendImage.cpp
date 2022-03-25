#include "pch.h"

struct ImageParamStruct {
    DWORD wxid;
    DWORD imagepath;
};

int SendImage(wchar_t* wxid, wchar_t* imagepath) {
    if (!hProcess)
        return 1;
    DWORD WeChatRobotBase = GetWeChatRobotBase();
    DWORD dwId = 0;
    DWORD dwWriteSize = 0;
    ImageParamStruct params;
    ZeroMemory(&params, sizeof(params));
    LPVOID wxidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    LPVOID imagepathaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    ImageParamStruct* paramAndFunc = (ImageParamStruct*)::VirtualAllocEx(hProcess, 0, sizeof(ImageParamStruct), MEM_COMMIT, PAGE_READWRITE);
    if (!wxidaddr || !imagepathaddr || !paramAndFunc || !WeChatRobotBase) {
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
        if (!::WriteProcessMemory(hProcess, paramAndFunc, &params, sizeof(params), &dwTId))
        {
            return 1;
        }
    }
    else {
        return 1;
    }

    DWORD SendImageRemoteAddr = WeChatRobotBase + SendImageOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)SendImageRemoteAddr, (LPVOID)paramAndFunc, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
    }
    else {
        return 1;
    }
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, wxidaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, imagepathaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, paramAndFunc, 0, MEM_RELEASE);
    return 0;
}
