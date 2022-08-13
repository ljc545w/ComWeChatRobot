#pragma once
#include <windows.h>
#include <iostream>

template <typename T1, typename T2, typename T3>
T2 WriteWeChatMemory(T1 hProcess, T2 ptrvalue, T3 size)
{
    if (!hProcess)
        return NULL;
    SIZE_T dwWriteSize;
    T2 addr = (T2)VirtualAllocEx(hProcess, NULL, size, MEM_COMMIT, PAGE_READWRITE);
    if (addr)
        WriteProcessMemory(hProcess, (LPVOID)addr, ptrvalue, size, &dwWriteSize);
    return addr;
}

template <typename T1, typename T2, typename T3>
DWORD CallRemoteFunction(T1 hProcess, T2 FunctionAddr, T3 params)
{
    DWORD dwRet = 0;
    DWORD dwThreadId = 0;
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)FunctionAddr, (LPVOID)params, 0, &dwThreadId);
    if (hThread)
    {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &dwRet);
        CloseHandle(hThread);
    }
    else
    {
        return 0;
    }
    return dwRet;
}

template <typename T>
class WeChatData
{

public:
    WeChatData(HANDLE hProcess, T data, size_t size)
    {
        this->hProcess = hProcess;
        this->size = size;
        if (size == 0)
            this->addr = data;
        else
            this->addr = WriteWeChatMemory(hProcess, data, size);
    }

    ~WeChatData()
    {
        if (this->size)
            VirtualFreeEx(this->hProcess, this->addr, 0, MEM_RELEASE);
    }

    T GetAddr()
    {
        return this->addr;
    }

private:
    T addr;
    size_t size;
    HANDLE hProcess;
};

class WeChatProcess
{
public:
    WeChatProcess(DWORD pid)
    {
        this->handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        if (!this->handle)
            m_init = FALSE;
        else
        {
            AsmProcAddr = this->GetAsmFunAddr();
            m_init = AsmProcAddr != 0 ? TRUE : FALSE;
        }
    }
    ~WeChatProcess()
    {
        if (AsmProcAddr)
            VirtualFreeEx(handle, AsmProcAddr, 0, MEM_RELEASE);
        if (handle)
            CloseHandle(handle);
        AsmProcAddr = NULL;
        handle = NULL;
    }
    HANDLE GetHandle()
    {
        return this->handle;
    }
    DWORD GetProcAddr(LPSTR functionname);
    DWORD WeChatRobotBase();
    BOOL m_init = FALSE;

private:
    HANDLE handle;
    LPVOID AsmProcAddr = NULL;
    virtual LPVOID GetAsmFunAddr();
};
