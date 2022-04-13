#include "pch.h"

struct SendAtTextStruct
{
    DWORD chatroomid;
    DWORD wxid;
    DWORD wxmsg;
    DWORD length;
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
    params.length = 1;

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

BOOL SendAtText(wchar_t* chatroomid, SAFEARRAY* psaValue, wchar_t* wxmsg) {
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
        return SendAtText(chatroomid, rgvar.bstrVal, wxmsg);
    }
    vector<void*> wxidptrs;
    DWORD dwWriteSize = 0;
    DWORD dwTId = 0; DWORD dwId = 0;
    DWORD WeChatRobotBase = GetWeChatRobotBase();
    SendAtTextStruct params = { 0 };
    LPVOID chatroomidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    LPVOID wxidptrsaddr = VirtualAllocEx(hProcess, NULL, sizeof(void*) * cElements, MEM_COMMIT, PAGE_READWRITE);
    LPVOID wxmsgaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE); 
    SendAtTextStruct* paramAndFunc = (SendAtTextStruct*)::VirtualAllocEx(hProcess, 0, sizeof(SendAtTextStruct), MEM_COMMIT, PAGE_READWRITE);
    if (!chatroomidaddr || !wxidptrsaddr || !wxmsgaddr || !paramAndFunc || !WeChatRobotBase) {
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

    if (wxmsgaddr)
        WriteProcessMemory(hProcess, wxmsgaddr, wxmsg, wcslen(wxmsg) * 2 + 2, &dwWriteSize);
    
    params.chatroomid = (DWORD)chatroomidaddr;
    params.wxid = (DWORD)wxidptrsaddr;
    params.wxmsg = (DWORD)wxmsgaddr;
    params.length = wxidptrs.size();
    if (paramAndFunc) {
        WriteProcessMemory(hProcess, paramAndFunc, &params, sizeof(SendAtTextStruct), &dwTId);
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
    for (unsigned int i = 0; i < wxidptrs.size(); i++) {
        VirtualFreeEx(hProcess, wxidptrs[i], 0, MEM_RELEASE);
    }
    VirtualFreeEx(hProcess, chatroomidaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, wxmsgaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, wxidptrsaddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, paramAndFunc, 0, MEM_RELEASE);
    return 0;
}