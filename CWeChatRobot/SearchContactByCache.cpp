#include "pch.h"

struct GetUserInfoStruct {
	DWORD message;
	DWORD length;
};

VOID DeleteUserInfoCache() {
	if (!hProcess)
		return;
	DWORD dwId = 0;
	DWORD DeleteUserInfoCacheProcAddr = GetWeChatRobotBase() + DeleteUserInfoCacheOffset;
	HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)DeleteUserInfoCacheProcAddr, NULL, 0, &dwId);
	if (hThread) {
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	}
}

std::wstring GetWxUserInfo(wchar_t* wxid) {
	if (!hProcess)
		return L"{}";
	wstring WString = L"";
	DWORD GetUserInfoProcAddr = GetWeChatRobotBase() + GetWxUserInfoOffset;
	LPVOID wxidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
	DWORD dwWriteSize = 0;
	DWORD dwId = 0;
	DWORD dwHandle = 0;
	GetUserInfoStruct userinfo = { 0 };
	if (!wxidaddr)
		return L"{}";
	WriteProcessMemory(hProcess, wxidaddr, wxid, wcslen(wxid) * 2 + 2, &dwWriteSize);
	HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)GetUserInfoProcAddr, wxidaddr, 0, &dwId);
	if (hThread) {
		WaitForSingleObject(hThread, INFINITE);
		GetExitCodeThread(hThread, &dwHandle);
		CloseHandle(hThread);
	}

	if(dwHandle)
		ReadProcessMemory(hProcess, (LPCVOID)dwHandle, &userinfo, sizeof(GetUserInfoStruct), &dwWriteSize);
	if (userinfo.length) {
		wchar_t* wmessage = new wchar_t[userinfo.length + 1];
		ZeroMemory(wmessage, (userinfo.length + 1) * 2);
		ReadProcessMemory(hProcess, (LPCVOID)userinfo.message, wmessage, userinfo.length * 2, &dwWriteSize);
		WString += wmessage;
		delete[] wmessage;
		wmessage = NULL;
	}

	VirtualFreeEx(hProcess, wxidaddr, 0, MEM_RELEASE);
	DeleteUserInfoCache();
	return WString;
}

