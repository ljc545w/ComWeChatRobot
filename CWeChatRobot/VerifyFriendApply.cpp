#include "pch.h"

struct VerifyFriendApplyStruct {
	DWORD v3;
	DWORD v4;
};

BOOL VerifyFriendApply(DWORD pid,wchar_t* v3,wchar_t* v4) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!hProcess)
		return 1;
	DWORD WeChatRobotBase = GetWeChatRobotBase(pid);
	if (!WeChatRobotBase) {
		CloseHandle(hProcess);
		return 1;
	}
	DWORD VerifyFriendApplyProcAddr = WeChatRobotBase + VerifyFriendApplyOffset;
	LPVOID v3addr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
	LPVOID v4addr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
	DWORD dwWriteSize = 0;
	DWORD dwId = 0;
	DWORD dwHandle = 0;
	VerifyFriendApplyStruct apply_data = { 0 };
	if (!v3addr || !v4addr) {
		CloseHandle(hProcess);
		return 1;
	}
	WriteProcessMemory(hProcess, v3addr, v3, wcslen(v3) * 2 + 2, &dwWriteSize);
	WriteProcessMemory(hProcess, v4addr, v4, wcslen(v4) * 2 + 2, &dwWriteSize);
	VerifyFriendApplyStruct* paramAndFunc = (VerifyFriendApplyStruct*)::VirtualAllocEx(hProcess, 0, sizeof(VerifyFriendApplyStruct), MEM_COMMIT, PAGE_READWRITE);
	apply_data.v3 = (DWORD)v3addr;
	apply_data.v4 = (DWORD)v4addr;

	if (paramAndFunc)
		WriteProcessMemory(hProcess, paramAndFunc, &apply_data, sizeof(apply_data), &dwId);
	else {
		CloseHandle(hProcess);
		return 1;
	}
	HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)VerifyFriendApplyProcAddr, paramAndFunc, 0, &dwId);
	if (hThread) {
		WaitForSingleObject(hThread, INFINITE);
		GetExitCodeThread(hThread, &dwHandle);
		CloseHandle(hThread);
	}

	VirtualFreeEx(hProcess, v3addr, 0, MEM_RELEASE);
	VirtualFreeEx(hProcess, v4addr, 0, MEM_RELEASE);
	VirtualFreeEx(hProcess, paramAndFunc, 0, MEM_RELEASE);
	CloseHandle(hProcess);
	return dwHandle == 0;
}