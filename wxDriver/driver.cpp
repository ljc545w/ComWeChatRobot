#include "pch.h"
#include "driver.h"

HANDLE GlobalProcess = NULL;
PVOID pRemoteGetProc = NULL;

struct GetProcAddrStruct {
    DWORD hModuleNameAddr;
    DWORD funcnameAddr;
};

DWORD GetWeChatPid() {
    HWND hCalc = FindWindow(NULL, L"Œ¢–≈");
    DWORD wxPid = 0;
    GetWindowThreadProcessId(hCalc, &wxPid);
    if (wxPid == 0) {
        hCalc = FindWindow(NULL, L"Œ¢–≈≤‚ ‘∞Ê");
        GetWindowThreadProcessId(hCalc, &wxPid);
    }
    return wxPid;
}

BOOL isFileExists_stat(wstring name) {
    struct _stat buffer;
    return (_wstat(name.c_str(), &buffer) == 0);
}

BOOL CreateConsole() {
    if (AllocConsole()) {
        AttachConsole(GetCurrentProcessId());
        FILE* retStream;
        freopen_s(&retStream, "CONOUT$", "w", stdout);
        if (!retStream) throw std::runtime_error("Stdout redirection failed.");
        freopen_s(&retStream, "CONOUT$", "w", stderr);
        if (!retStream) throw std::runtime_error("Stderr redirection failed.");
        return 0;
    }
    return 1;
}

wstring GetWorkPath() {
    wchar_t szFilePath[MAX_PATH + 1] = { 0 };
#ifdef _WIN64
    HMODULE hd = GetModuleHandle(L"wxDriver64.dll");
#else
    HMODULE hd = GetModuleHandle(L"wxDriver.dll");
#endif
    GetModuleFileName(hd, szFilePath, MAX_PATH);
    wstring wpath = szFilePath;
    int pos = (int)wpath.find_last_of(L"\\");
    wpath = wpath.substr(0, pos);
    return wpath;
}

static PVOID CreateGetProcFuncInWeChatProcess() {
#ifdef _WIN64
    DWORD pGetModuleHandleW = (DWORD)GetSystem32ProcAddr(L"\\KnownDlls32\\kernel32.dll", "GetModuleHandleW");
    DWORD pGetProcAddress = (DWORD)GetSystem32ProcAddr(L"\\KnownDlls32\\kernel32.dll", "GetProcAddress");
#else
    DWORD pGetModuleHandleW = (DWORD)GetModuleHandleW;
    DWORD pGetProcAddress = (DWORD)GetProcAddress;
#endif
    unsigned char asmcode[] = { 
        0x55,                                   // push ebp;
        0x8B,0xEC,                              // mov ebp, esp;
        0x83,0xEC,0x40,                         // sub esp, 0x40;
        0x57,                                   // push edi;
        0x51,                                   // push ecx;
        0x8B,0x7D,0x08,                         // mov edi, dword ptr[ebp + 0x8];
        0x8B,0x07,                              // mov eax,dword ptr[edi];
        0x50,                                   // push eax;
        0xE8,0x00,0x00,0x00,0x00,               // call GetModuleHandleW;
        0x83,0xC4,0x04,                         // add esp,0x4;
        0x83,0xC7,0x04,                         // add edi,0x4;
        0x8B,0x0F,                              // mov ecx, dword ptr[edi];
        0x51,                                   // push ecx;
        0x50,                                   // push eax;
        0xE8,0x00,0x00,0x00,0x00,               // call GetProcAddress;
        0x83,0xC4,0x08,                         // add esp, 0x8;
        0x59,                                   // pop ecx;
        0x5F,                                   // pop edi;
        0x8B,0xE5,                              // mov esp, ebp;
        0x5D,                                   // pop ebp;
        0xC3                                    // retn;
    };
    PVOID call1 = (PVOID)&asmcode[15];
    PVOID call2 = (PVOID)&asmcode[30];
    DWORD wxpid = GetWeChatPid();
    SIZE_T dwWriteSize;
    LPVOID pRemoteAddress = VirtualAllocEx(GlobalProcess, NULL, 1, MEM_COMMIT, PAGE_EXECUTE);
    if (!pRemoteAddress)
        return 0;
    *(DWORD*)call1 = pGetModuleHandleW - (DWORD)pRemoteAddress - 14 - 5;
    *(DWORD*)call2 = pGetProcAddress - (DWORD)pRemoteAddress - 29 - 5;
    WriteProcessMemory(GlobalProcess, pRemoteAddress, asmcode, 43, &dwWriteSize);
    return pRemoteAddress;
}

DWORD GetRemoteProcAddr(const wchar_t* hModuleName,const char* funcname) {
    if (!GlobalProcess || !pRemoteGetProc)
        return 0;
    LPVOID hModuleNameAddr = VirtualAllocEx(GlobalProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    LPVOID funcnameAddr = VirtualAllocEx(GlobalProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    GetProcAddrStruct* paramAndFunc = (GetProcAddrStruct*)::VirtualAllocEx(GlobalProcess, 0, sizeof(GetProcAddrStruct), MEM_COMMIT, PAGE_READWRITE);
    if (!hModuleNameAddr || !funcnameAddr || !paramAndFunc) {
        return 0;
    }
    SIZE_T dwWriteSize;
    DWORD dwId;
    DWORD dwProcAddr = 0;
    if (hModuleNameAddr)
        WriteProcessMemory(GlobalProcess, hModuleNameAddr, hModuleName, wcslen(hModuleName) * 2 + 2, &dwWriteSize);
    if (funcnameAddr)
        WriteProcessMemory(GlobalProcess, funcnameAddr, funcname, strlen(funcname) + 1, &dwWriteSize);
    GetProcAddrStruct params = { 0 };
    params.hModuleNameAddr = (DWORD)hModuleNameAddr;
    params.funcnameAddr = (DWORD)funcnameAddr;
    if (paramAndFunc)
        WriteProcessMemory(GlobalProcess, paramAndFunc, &params, sizeof(params), &dwWriteSize);
    HANDLE hThread = ::CreateRemoteThread(GlobalProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pRemoteGetProc, (LPVOID)paramAndFunc, 0, &dwId);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &dwProcAddr);
        CloseHandle(hThread);
    }
    VirtualFreeEx(GlobalProcess, hModuleNameAddr, 0, MEM_RELEASE);
    VirtualFreeEx(GlobalProcess, funcnameAddr, 0, MEM_RELEASE);
    VirtualFreeEx(GlobalProcess, paramAndFunc, 0, MEM_RELEASE);
    return dwProcAddr;
}

BOOL StartRobotService() {
    DWORD wxPid = GetWeChatPid();
    if (!wxPid) {
        return false;
    }
    wstring dllpath = GetWorkPath() + L"\\" + DLLNAME;
    if (!isFileExists_stat(dllpath)) {
        return false;
    }
    BOOL status = InjectDll(wxPid, dllpath.c_str());
    if (status) {
        GlobalProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, wxPid);
        pRemoteGetProc = CreateGetProcFuncInWeChatProcess();
        DWORD pHttpStart = GetRemoteProcAddr(DLLNAME,"HttpStart");
        HANDLE hThread = CreateRemoteThread(GlobalProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pHttpStart, NULL, 0, 0);
        if (hThread) {
            CloseHandle(hThread);
        }
    }
    return status;
}

BOOL StopRobotService() {
    DWORD wxPid = GetWeChatPid();
    if (!wxPid) {
        return true;
    }
    if (pRemoteGetProc && GlobalProcess) {
        DWORD pHttpClose = GetRemoteProcAddr(DLLNAME, "HttpClose");
        if (pHttpClose) {
            HANDLE hThread = CreateRemoteThread(GlobalProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pHttpClose, NULL, 0, 0);
            if (hThread) {
                WaitForSingleObject(hThread, INFINITE);
                CloseHandle(hThread);
            }
        }
        VirtualFreeEx(GlobalProcess, pRemoteGetProc, 0, MEM_RELEASE);
    }
    BOOL status = RemoveDll(wxPid, DLLNAME);
    if (GlobalProcess)
    {
        CloseHandle(GlobalProcess);
        GlobalProcess = NULL;
    }
    return status;
}