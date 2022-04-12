#include "pch.h"

struct ChatRoomInfoStruct {
	DWORD members;
	DWORD length;
};

SAFEARRAY* GetChatRoomMembers(wchar_t* chatroomid) {
	if (!hProcess)
		return NULL;
    DWORD WeChatRobotBase = GetWeChatRobotBase();
    DWORD dwId = 0;
    DWORD dwWriteSize = 0;
    DWORD dwHandle = 0;
    HRESULT hr = S_OK;
    ChatRoomInfoStruct chatroominfo = { 0 };
    LPVOID chatroomidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    if (!chatroomidaddr || !WeChatRobotBase) {
        return NULL;
    }
    else {
        WriteProcessMemory(hProcess, chatroomidaddr, chatroomid, wcslen(chatroomid) * 2 + 2, &dwWriteSize);
    }

    DWORD GetChatRoomMembersRemoteAddr = WeChatRobotBase + GetChatRoomMembersRemoteOffset;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)GetChatRoomMembersRemoteAddr, (LPVOID)chatroomidaddr, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &dwHandle);
    }
    else {
        return NULL;
    }
    if (!dwHandle)
        return NULL;
    ReadProcessMemory(hProcess,(LPCVOID)dwHandle,&chatroominfo,sizeof(ChatRoomInfoStruct),0);
    wchar_t* members = new wchar_t[chatroominfo.length + 1];
    ZeroMemory(members, (chatroominfo.length + 1) * 2);
    ReadProcessMemory(hProcess, (LPCVOID)chatroominfo.members, members, chatroominfo.length * 2, 0);
    cout << members << endl;
    SAFEARRAYBOUND rgsaBound[2] = { {2,0},{2,0} };
    SAFEARRAY* psaValue = SafeArrayCreate(VT_VARIANT, 2, rgsaBound);
    long keyIndex[2] = { 0,0 };
    keyIndex[0] = 0; keyIndex[1] = 0;
    hr = SafeArrayPutElement(psaValue, keyIndex, &(_variant_t)L"chatroomid");
    keyIndex[0] = 0; keyIndex[1] = 1;
    hr = SafeArrayPutElement(psaValue, keyIndex, &(_variant_t)chatroomid);
    keyIndex[0] = 1; keyIndex[1] = 0;
    hr = SafeArrayPutElement(psaValue, keyIndex, &(_variant_t)L"members");
    keyIndex[0] = 1; keyIndex[1] = 1;
    hr = SafeArrayPutElement(psaValue, keyIndex, &(_variant_t)members);
    delete[] members;
    members = NULL;
    return psaValue;
}