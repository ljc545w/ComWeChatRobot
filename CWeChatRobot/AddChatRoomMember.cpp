#include "pch.h"

struct AddChatRoomMemberStruct
{
    DWORD chatroomid;
    DWORD wxids;
    DWORD length;
};

BOOL AddChatRoomMember(wchar_t* chatroomid, wchar_t* wxid) {
    if (!hProcess)
        return 1;
    DWORD WeChatRobotBase = GetWeChatRobotBase();
    DWORD dwId = 0;
    DWORD dwWriteSize = 0;
    DWORD dwRet = 0;
    AddChatRoomMemberStruct params;
    ZeroMemory(&params, sizeof(params));
    LPVOID chatroomidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    LPVOID wxidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    AddChatRoomMemberStruct* paramAndFunc = (AddChatRoomMemberStruct*)::VirtualAllocEx(hProcess, 0, sizeof(AddChatRoomMemberStruct), MEM_COMMIT, PAGE_READWRITE);
    if (!chatroomidaddr || !wxidaddr || !paramAndFunc || !WeChatRobotBase) {
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
        WriteProcessMemory(hProcess, paramAndFunc, &params, sizeof(AddChatRoomMemberStruct), &dwTId);
    }
    else {
        return 1;
    }

    DWORD AddChatRoomMemberAddr = WeChatRobotBase + AddChatRoomMemberRemoteOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)AddChatRoomMemberAddr, (LPVOID)paramAndFunc, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &dwRet);
        CloseHandle(hThread);
    }
    else {
        return 1;
    }
    VirtualFreeEx(hProcess, chatroomidaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, wxidaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, paramAndFunc, 0, MEM_RELEASE);
    return dwRet == 0;
}

BOOL AddChatRoomMember(wchar_t* chatroomid, SAFEARRAY* psaValue) {
    if (!hProcess)
        return 1;
    VARIANT rgvar;
    rgvar.vt = VT_BSTR;
    HRESULT hr = S_OK;
    long lLbound = psaValue->rgsabound->lLbound;
    long cElements = psaValue->rgsabound->cElements;
    if (cElements == 1) {
        VariantInit(&rgvar);
        long pIndex = 0;
        hr = SafeArrayGetElement(psaValue, &pIndex, &rgvar);
        return AddChatRoomMember(chatroomid, rgvar.bstrVal);
    }
    vector<void*> wxidptrs;
    DWORD dwWriteSize = 0;
    DWORD dwTId = 0; DWORD dwId = 0; DWORD dwRet = 0;
    DWORD WeChatRobotBase = GetWeChatRobotBase();
    AddChatRoomMemberStruct params = { 0 };
    LPVOID chatroomidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    LPVOID wxidptrsaddr = VirtualAllocEx(hProcess, NULL, sizeof(void*) * cElements, MEM_COMMIT, PAGE_READWRITE);
    AddChatRoomMemberStruct* paramAndFunc = (AddChatRoomMemberStruct*)::VirtualAllocEx(hProcess, 0, sizeof(AddChatRoomMemberStruct), MEM_COMMIT, PAGE_READWRITE);
    if (!chatroomidaddr || !wxidptrsaddr || !paramAndFunc || !WeChatRobotBase) {
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
        WriteProcessMemory(hProcess, paramAndFunc, &params, sizeof(AddChatRoomMemberStruct), &dwTId);
    }
    else {
        return 1;
    }
    DWORD AddChatRoomMemberAddr = WeChatRobotBase + AddChatRoomMemberRemoteOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)AddChatRoomMemberAddr, (LPVOID)paramAndFunc, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &dwRet);
        CloseHandle(hThread);
    }
    else {
        return 1;
    }
    for (unsigned int i = 0; i < wxidptrs.size(); i++) {
        VirtualFreeEx(hProcess, wxidptrs[i], 0, MEM_RELEASE);
    }
    VirtualFreeEx(hProcess, chatroomidaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, wxidptrsaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, paramAndFunc, 0, MEM_RELEASE);
    return dwRet == 0;
}