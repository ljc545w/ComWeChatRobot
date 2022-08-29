#include "base.h"

DWORD GetWeChatRobotBase()
{
    if (!hProcess)
        return 0;
    DWORD dwWriteSize = 0;
    LPVOID pRemoteAddress = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    if (pRemoteAddress)
        WriteProcessMemory(hProcess, pRemoteAddress, dllname, wcslen(dllname) * 2 + 2, &dwWriteSize);
    else
        return 0;
    DWORD dwHandle, dwID;
    LPVOID pFunc = GetModuleHandleW;
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunc, pRemoteAddress, 0, &dwID);
    if (hThread)
    {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &dwHandle);
    }
    else
    {
        return 0;
    }
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pRemoteAddress, 0, MEM_RELEASE);
    return dwHandle;
}
