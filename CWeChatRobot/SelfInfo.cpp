#include "pch.h"

struct GetSelfInfoStruct {
	DWORD message;
	DWORD length;
};

VOID DeleteSelfInfoCache(DWORD pid,HANDLE hProcess) {
	DWORD dwId = 0;
	DWORD WeChatRobotBase = GetWeChatRobotBase(pid);
	if (!WeChatRobotBase) {
		return;
	}
	DWORD DeleteSelfInfoCacheProcAddr = WeChatRobotBase + DeleteSelfInfoCacheOffset;
	HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)DeleteSelfInfoCacheProcAddr, NULL, 0, &dwId);
	if (hThread) {
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	}
}

std::wstring GetSelfInfo(DWORD pid) {
	if (PidToSelfInfoString.count(pid)!=0)
	{
		return PidToSelfInfoString[pid];
	}
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!hProcess)
		return L"{}";
	DWORD WeChatRobotBase = GetWeChatRobotBase(pid);
	if (!WeChatRobotBase) {
		CloseHandle(hProcess);
		return L"{}";
	}
	DWORD GetSelfInfoProcAddr = WeChatRobotBase + GetSelfInfoOffset;
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
		PidToSelfInfoString[pid] = wmessage;
		delete[] wmessage;
		wmessage = NULL;
	}

	DeleteSelfInfoCache(pid,hProcess);
	CloseHandle(hProcess);
	return PidToSelfInfoString[pid];
}

BOOL isWxLogin(DWORD pid) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!hProcess)
		return false;
	DWORD WeChatRobotBase = GetWeChatRobotBase(pid);
	if (!WeChatRobotBase) {
		CloseHandle(hProcess);
		return false;
	}
	DWORD isWxLoginAddr = WeChatRobotBase + isWxLoginOffset;
	DWORD dwId, dwRet = 0;
	HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)isWxLoginAddr, NULL, 0, &dwId);
	if (hThread) {
		WaitForSingleObject(hThread, INFINITE);
		GetExitCodeThread(hThread, &dwRet);
		CloseHandle(hThread);
	}
	CloseHandle(hProcess);
	return dwRet == 1;
}