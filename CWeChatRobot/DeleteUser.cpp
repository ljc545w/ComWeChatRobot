#include "pch.h"

BOOL DeleteUser(DWORD pid,wchar_t* wxid) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!hProcess)
		return 1;
	DWORD WeChatRobotBase = GetWeChatRobotBase(pid);
	if (!WeChatRobotBase) {
		CloseHandle(hProcess);
		return 1;
	}
	DWORD DeleteUserRemoteAddr = WeChatRobotBase + DeleteUserRemoteOffset;
	LPVOID wxidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
	DWORD dwWriteSize = 0;
	DWORD dwId = 0;
	DWORD dwRet = 0;
	if (!wxidaddr) {
		CloseHandle(hProcess);
		return 1;
	}
	WriteProcessMemory(hProcess, wxidaddr, wxid, wcslen(wxid) * 2 + 2, &dwWriteSize);
	HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)DeleteUserRemoteAddr, wxidaddr, 0, &dwId);
	if (hThread) {
		WaitForSingleObject(hThread, INFINITE);
		GetExitCodeThread(hThread, &dwRet);
		CloseHandle(hThread);
	}
	VirtualFreeEx(hProcess, wxidaddr, 0, MEM_RELEASE);
	CloseHandle(hProcess);
	return dwRet == 0;
}