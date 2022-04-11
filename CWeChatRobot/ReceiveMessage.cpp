#include "pch.h"

BOOL ReceiveMessageHooked = FALSE;

struct GetRemoteMessageStruct {
    DWORD type;
    DWORD sender;
    DWORD l_sender;
    DWORD wxid;
    DWORD l_wxid;
    DWORD message;
    DWORD l_message;
    DWORD filepath;
    DWORD l_filepath;
};

struct MessageStruct {
    DWORD type;
    wchar_t* sender;
    wchar_t* wxid;
    wchar_t* message;
    wchar_t* filepath;
};

BOOL StartReceiveMessage() {
    if (!hProcess || ReceiveMessageHooked)
        return 1;
    DWORD WeChatRobotBase = GetWeChatRobotBase();
    DWORD dwId = 0;

    DWORD HookReceiveMessageAddr = WeChatRobotBase + HookReceiveMessageRemoteOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)HookReceiveMessageAddr, NULL, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
    }
    else {
        return 1;
    }
    CloseHandle(hThread);
    ReceiveMessageHooked = TRUE;
    return 0;
}

BOOL StopReceiveMessage() {
    if (!hProcess || !ReceiveMessageHooked)
        return 1;
    DWORD WeChatRobotBase = GetWeChatRobotBase();
    DWORD dwId = 0;

    DWORD UnHookReceiveMessageAddr = WeChatRobotBase + UnHookReceiveMessageRemoteOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)UnHookReceiveMessageAddr, NULL, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
    }
    else {
        return 1;
    }
    CloseHandle(hThread);
    ReceiveMessageHooked = FALSE;
    return 0;
}

BOOL GetHeadMessage(GetRemoteMessageStruct* message) {
    if (!hProcess || !ReceiveMessageHooked)
        return 1;
    DWORD WeChatRobotBase = GetWeChatRobotBase();
    DWORD dwId = 0;
    DWORD dwHandle = 0;
    DWORD GetHeadMessageAddr = WeChatRobotBase + GetHeadMessageRemoteOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)GetHeadMessageAddr, NULL, 0, &dwId);
    
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &dwHandle);
    }
    else {
        return 1;
    }
    if (!dwHandle)
        return 0;
    ReadProcessMemory(hProcess,(LPCVOID)dwHandle, message,sizeof(GetRemoteMessageStruct),0);
    CloseHandle(hThread);
    return 0;
}

BOOL PopHeadMessage() {
    if (!hProcess || !ReceiveMessageHooked)
        return 1;
    DWORD WeChatRobotBase = GetWeChatRobotBase();
    DWORD dwId = 0;
    DWORD PopHeadMessageAddr = WeChatRobotBase + PopHeadMessageRemoteOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)PopHeadMessageAddr, NULL, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
    }
    else {
        return 1;
    }
    CloseHandle(hThread);
    return 0;
}

SAFEARRAY* ReceiveMessage() {
    if (!hProcess || !ReceiveMessageHooked)
        return NULL;
    GetRemoteMessageStruct remotemessage = { 0 };
    MessageStruct message = { 0 };
    HRESULT hr = S_OK;
    GetHeadMessage(&remotemessage);
    printf("0x%X,0x%08X,0x%08X,0x%08X\n", remotemessage.type, remotemessage.sender, remotemessage.wxid, remotemessage.message);
    DWORD dwInfoAddr = 0;
    if (remotemessage.type) {
        message.type = remotemessage.type;
        message.sender = new wchar_t[remotemessage.l_sender + 1];
        ReadProcessMemory(hProcess, (LPCVOID)remotemessage.sender, message.sender, (remotemessage.l_sender + 1) * sizeof(wchar_t), 0);
        message.wxid = new wchar_t[remotemessage.l_wxid + 1];
        ReadProcessMemory(hProcess, (LPCVOID)remotemessage.wxid, message.wxid, (remotemessage.l_wxid + 1) * sizeof(wchar_t), 0);
        message.message = new wchar_t[remotemessage.l_message + 1];
        ReadProcessMemory(hProcess, (LPCVOID)remotemessage.message, message.message, (remotemessage.l_message + 1) * sizeof(wchar_t), 0);
        message.filepath = new wchar_t[remotemessage.l_filepath + 1];
        ZeroMemory(message.filepath, (remotemessage.l_filepath + 1) * 2);
        if(remotemessage.l_filepath)
            ReadProcessMemory(hProcess, (LPCVOID)remotemessage.filepath, message.filepath, (remotemessage.l_filepath + 1) * sizeof(wchar_t), 0);
    }
    else {
        return NULL;
    }
    PopHeadMessage();
    SAFEARRAY* psaValue;
    vector<wstring> MessageInfoKey = {
        L"type",
        L"sender",
        L"wxid",
        L"message",
        L"filepath",
    };
    SAFEARRAYBOUND rgsaBound[2] = { {5,0},{2,0} };
    psaValue = SafeArrayCreate(VT_VARIANT, 2, rgsaBound);
    long keyIndex[2] = { 0,0 };
    keyIndex[0] = 0; keyIndex[1] = 0;
    hr = SafeArrayPutElement(psaValue, keyIndex, &(_variant_t)MessageInfoKey[0].c_str());
    keyIndex[0] = 0; keyIndex[1] = 1;
    hr = SafeArrayPutElement(psaValue, keyIndex, &(_variant_t)message.type);

    keyIndex[0] = 1; keyIndex[1] = 0;
    hr = SafeArrayPutElement(psaValue, keyIndex, &(_variant_t)MessageInfoKey[1].c_str());
    keyIndex[0] = 1; keyIndex[1] = 1;
    hr = SafeArrayPutElement(psaValue, keyIndex, &(_variant_t)message.sender);
    delete[] message.sender;
    message.sender = NULL;

    keyIndex[0] = 2; keyIndex[1] = 0;
    hr = SafeArrayPutElement(psaValue, keyIndex, &(_variant_t)MessageInfoKey[2].c_str());
    keyIndex[0] = 2; keyIndex[1] = 1;
    hr = SafeArrayPutElement(psaValue, keyIndex, &(_variant_t)message.wxid);
    delete[] message.wxid;
    message.wxid = NULL;

    keyIndex[0] = 3; keyIndex[1] = 0;
    hr = SafeArrayPutElement(psaValue, keyIndex, &(_variant_t)MessageInfoKey[3].c_str());
    keyIndex[0] = 3; keyIndex[1] = 1;
    hr = SafeArrayPutElement(psaValue, keyIndex, &(_variant_t)message.message);
    delete[] message.message;
    message.message = NULL;

    keyIndex[0] = 4; keyIndex[1] = 0;
    hr = SafeArrayPutElement(psaValue, keyIndex, &(_variant_t)MessageInfoKey[4].c_str());
    keyIndex[0] = 4; keyIndex[1] = 1;
    hr = SafeArrayPutElement(psaValue, keyIndex, &(_variant_t)message.filepath);
    delete[] message.filepath;
    message.filepath = NULL;

    return psaValue;
}