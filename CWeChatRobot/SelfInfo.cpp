#include "pch.h"

struct GetSelfInfoStruct {
	DWORD message;
	DWORD length;
};

VOID DeleteSelfInfoCache() {
	if (!hProcess)
		return;
	DWORD dwId = 0;
	DWORD DeleteSelfInfoCacheProcAddr = GetWeChatRobotBase() + DeleteSelfInfoCacheOffset;
	HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)DeleteSelfInfoCacheProcAddr, NULL, 0, &dwId);
	if (hThread) {
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	}
}

std::wstring GetSelfInfo() {
	if (!hProcess)
		return L"{}";
	if (SelfInfoString.compare(L"")) {
		return SelfInfoString;
	}
	DWORD GetSelfInfoProcAddr = GetWeChatRobotBase() + GetSelfInfoOffset;
	DWORD dwWriteSize = 0;
	DWORD dwId = 0;
	DWORD dwHandle = 0;
	GetSelfInfoStruct selfinfo = { 0 };
	HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)GetSelfInfoProcAddr, NULL, 0, &dwId);
	if (hThread) {
		WaitForSingleObject(hThread, INFINITE);
		GetExitCodeThread(hThread, &dwHandle);
		CloseHandle(hThread);
	}
	if (dwHandle)
		ReadProcessMemory(hProcess, (LPCVOID)dwHandle, &selfinfo, sizeof(GetSelfInfoStruct), &dwWriteSize);
	if (selfinfo.length) {
		wchar_t* wmessage = new wchar_t[selfinfo.length + 1];
		ZeroMemory(wmessage, (selfinfo.length + 1) * 2);
		ReadProcessMemory(hProcess, (LPCVOID)selfinfo.message, wmessage, selfinfo.length * 2, &dwWriteSize);
		SelfInfoString += wmessage;
		delete[] wmessage;
		wmessage = NULL;
	}

	DeleteSelfInfoCache();
	return SelfInfoString;
}