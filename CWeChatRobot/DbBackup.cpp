#include "pch.h"

// 调用DLL接口时的参数
struct BackupParams {
	DWORD ptrDb;
	DWORD savepath;
};

BOOL BackupSQLiteDB(DWORD DbHandle, BSTR savepath) {
	if (!hProcess)
		return 1;
	DWORD dwHandle = 0x0;
	DWORD dwId = 0x0;
	DWORD dwWriteSize = 0x0;
	LPVOID savepathAddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
	BackupParams* paramAndFunc = (BackupParams*)::VirtualAllocEx(hProcess, 0, sizeof(BackupParams), MEM_COMMIT, PAGE_READWRITE);
	if (!savepathAddr || !paramAndFunc)
		return 1;
	char* a_savepath = _com_util::ConvertBSTRToString(savepath);
	if (savepathAddr) 
		WriteProcessMemory(hProcess, savepathAddr, a_savepath, strlen(a_savepath) + 1, &dwWriteSize);
	BackupParams param = { 0 };
	param.ptrDb = DbHandle;
	param.savepath = (DWORD)savepathAddr;

	if (paramAndFunc)
		WriteProcessMemory(hProcess, paramAndFunc, &param, sizeof(BackupParams), &dwWriteSize);

	DWORD BackupSQLiteDBRemoteAddr = GetWeChatRobotBase() + BackupSQLiteDBRemoteOffset;
	HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)BackupSQLiteDBRemoteAddr, (LPVOID)paramAndFunc, 0, &dwId);
	if (hThread) {
		WaitForSingleObject(hThread, INFINITE);
		GetExitCodeThread(hThread, &dwHandle);
		CloseHandle(hThread);
	}
	else {
		return 1;
	}
	VirtualFreeEx(hProcess, savepathAddr, 0, MEM_RELEASE);
	VirtualFreeEx(hProcess, paramAndFunc, 0, MEM_RELEASE);
	return dwHandle;
}