#include "pch.h"

struct DelChatRoomMemberStruct
{
    DWORD chatroomid;
    DWORD wxids;
    DWORD length;
};

BOOL DelChatRoomMember(DWORD pid,wchar_t* chatroomid, wchar_t* wxid) {
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
    DelChatRoomMemberStruct params;
    ZeroMemory(&params, sizeof(params));
    LPVOID chatroomidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    LPVOID wxidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    DelChatRoomMemberStruct* paramAndFunc = (DelChatRoomMemberStruct*)::VirtualAllocEx(hProcess, 0, sizeof(DelChatRoomMemberStruct), MEM_COMMIT, PAGE_READWRITE);
    if (!chatroomidaddr || !wxidaddr || !paramAndFunc) {
        CloseHandle(hProcess);
        return 1;
    }
    DWORD dwTId = 0;

    if (chatroomidaddr)
        WriteProcessMemory(hProcess, chatroomidaddr, chatroomid, wcslen(chatroomid) * 2 + 2, &dwWriteSize);

    if (wxidaddr)
        WriteProcessMemory(hProcess, wxidaddr, wxid, wcslen(wxid) * 2 + 2, &dwWriteSize);

    params.chatroomid = (DWORD)chatroomidaddr;
    params.wxids = (DWORD)wxidaddr;
    params.length = 1;

    if (paramAndFunc) {
        WriteProcessMemory(hProcess, paramAndFunc, &params, sizeof(DelChatRoomMemberStruct), &dwTId);
    }
    else {
        CloseHandle(hProcess);
        return 1;
    }

    DWORD DelChatRoomMemberAddr = WeChatRobotBase + DelChatRoomMemberRemoteOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)DelChatRoomMemberAddr, (LPVOID)paramAndFunc, 0, &dwId);
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
    VirtualFreeEx(hProcess, wxidaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, paramAndFunc, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return dwRet == 0;
}

BOOL DelChatRoomMember(DWORD pid,wchar_t* chatroomid, SAFEARRAY* psaValue) {
    VARIANT rgvar;
    rgvar.vt = VT_BSTR;
    HRESULT hr = S_OK;
    long lLbound = psaValue->rgsabound->lLbound;
    long cElements = psaValue->rgsabound->cElements;
    if (cElements == 1) {
        VariantInit(&rgvar);
        long pIndex = 0;
        hr = SafeArrayGetElement(psaValue, &pIndex, &rgvar);
        return DelChatRoomMember(pid,chatroomid, rgvar.bstrVal);
    }
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess)
        return 1;
    DWORD WeChatRobotBase = GetWeChatRobotBase(pid);
    if (!WeChatRobotBase) {
        CloseHandle(hProcess);
        return 1;
    }
    vector<void*> wxidptrs;
    DWORD dwWriteSize = 0;
    DWORD dwTId = 0; DWORD dwId = 0; DWORD dwRet = 0;
    DelChatRoomMemberStruct params = { 0 };
    LPVOID chatroomidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    LPVOID wxidptrsaddr = VirtualAllocEx(hProcess, NULL, sizeof(void*) * cElements, MEM_COMMIT, PAGE_READWRITE);
    DelChatRoomMemberStruct* paramAndFunc = (DelChatRoomMemberStruct*)::VirtualAllocEx(hProcess, 0, sizeof(DelChatRoomMemberStruct), MEM_COMMIT, PAGE_READWRITE);
    if (!chatroomidaddr || !wxidptrsaddr || !paramAndFunc) {
        CloseHandle(hProcess);
        return 1;
    }
    for (long i = lLbound; i < lLbound + cElements; i++) {
        VariantInit(&rgvar);
        hr = SafeArrayGetElement(psaValue, &i, &rgvar);
        LPVOID wxidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
        if (wxidaddr) {
            WriteProcessMemory(hProcess, wxidaddr, rgvar.bstrVal, wcslen(rgvar.bstrVal) * 2 + 2, &dwWriteSize);
            wxidptrs.push_back(wxidaddr);
        }
    }
    if (chatroomidaddr)
        WriteProcessMemory(hProcess, chatroomidaddr, chatroomid, wcslen(chatroomid) * 2 + 2, &dwWriteSize);

    if (wxidptrsaddr)
        WriteProcessMemory(hProcess, wxidptrsaddr, &wxidptrs[0], wxidptrs.size() * sizeof(void*), &dwWriteSize);

    params.chatroomid = (DWORD)chatroomidaddr;
    params.wxids = (DWORD)wxidptrsaddr;
    params.length = wxidptrs.size();
    if (paramAndFunc) {
        WriteProcessMemory(hProcess, paramAndFunc, &params, sizeof(DelChatRoomMemberStruct), &dwTId);
    }
    else {
        CloseHandle(hProcess);
        return 1;
    }
    DWORD DelChatRoomMemberAddr = WeChatRobotBase + DelChatRoomMemberRemoteOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)DelChatRoomMemberAddr, (LPVOID)paramAndFunc, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &dwRet);
        CloseHandle(hThread);
    }
    else {
        CloseHandle(hProcess);
        return 1;
    }
    for (unsigned int i = 0; i < wxidptrs.size(); i++) {
        VirtualFreeEx(hProcess, wxidptrs[i], 0, MEM_RELEASE);
    }
    VirtualFreeEx(hProcess, chatroomidaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, wxidptrsaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, paramAndFunc, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return dwRet == 0;
}