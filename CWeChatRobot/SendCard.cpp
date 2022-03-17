#include "pch.h"

struct SendCardStruct {
	DWORD receiver;
	DWORD sharedwxid;
	DWORD nickname;
};

BOOL SendCard(wchar_t* receiver, wchar_t* sharedwxid, wchar_t* nickname) {
	if (!hProcess)
		return 1;
	DWORD WeChatRobotBase = GetWeChatRobotBase();
	DWORD dwId = 0;
	DWORD dwWriteSize = 0;
	SendCardStruct params;
	ZeroMemory(&params, sizeof(params));
	DWORD SendCardProcAddr = WeChatRobotBase + SendCardOffset;
	LPVOID receiveraddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
	LPVOID sharedwxidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
	LPVOID nicknameaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
	SendCardStruct* paramAndFunc = (SendCardStruct*)::VirtualAllocEx(hProcess, 0, sizeof(SendCardStruct), MEM_COMMIT, PAGE_READWRITE);
	if (!receiveraddr || !sharedwxidaddr || !nicknameaddr ||
		!paramAndFunc || !WeChatRobotBase)
	{
		return 1;
	}
	if (receiveraddr)
		WriteProcessMemory(hProcess, receiveraddr, receiver, wcslen(receiver) * 2 + 2, &dwWriteSize);
	if (sharedwxidaddr)
		WriteProcessMemory(hProcess, sharedwxidaddr, sharedwxid, wcslen(sharedwxid) * 2 + 2, &dwWriteSize);
	if (nicknameaddr)
		WriteProcessMemory(hProcess, nicknameaddr, nickname, wcslen(nickname) * 2 + 2, &dwWriteSize);
	params.receiver = (DWORD)receiveraddr;
	params.sharedwxid = (DWORD)sharedwxidaddr;
	params.nickname = (DWORD)nicknameaddr;

	if (paramAndFunc)
		WriteProcessMemory(hProcess, paramAndFunc, &params, sizeof(params), &dwId);
	HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)SendCardProcAddr, (LPVOID)paramAndFunc, 0, &dwId);
	if (hThread) {
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	}
	VirtualFreeEx(hProcess, receiveraddr, 0, MEM_RELEASE);
	VirtualFreeEx(hProcess, sharedwxidaddr, 0, MEM_RELEASE);
	VirtualFreeEx(hProcess, nicknameaddr, 0, MEM_RELEASE);
	VirtualFreeEx(hProcess, paramAndFunc, 0, MEM_RELEASE);
	return 0;
}