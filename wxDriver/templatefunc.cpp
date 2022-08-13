#include "pch.h"
#include "driver.h"

static unsigned char GetProcAsmCode[] = {
    0x55,                         // push ebp;
    0x8B, 0xEC,                   // mov ebp, esp;
    0x83, 0xEC, 0x40,             // sub esp, 0x40;
    0x57,                         // push edi;
    0x51,                         // push ecx;
    0x8B, 0x7D, 0x08,             // mov edi, dword ptr[ebp + 0x8];
    0x8B, 0x07,                   // mov eax,dword ptr[edi];
    0x50,                         // push eax;
    0xE8, 0x00, 0x00, 0x00, 0x00, // call GetModuleHandleW;
    0x83, 0xC4, 0x04,             // add esp,0x4;
    0x83, 0xC7, 0x04,             // add edi,0x4;
    0x8B, 0x0F,                   // mov ecx, dword ptr[edi];
    0x51,                         // push ecx;
    0x50,                         // push eax;
    0xE8, 0x00, 0x00, 0x00, 0x00, // call GetProcAddress;
    0x83, 0xC4, 0x08,             // add esp, 0x8;
    0x59,                         // pop ecx;
    0x5F,                         // pop edi;
    0x8B, 0xE5,                   // mov esp, ebp;
    0x5D,                         // pop ebp;
    0xC3                          // retn;
};

LPVOID WeChatProcess::GetAsmFunAddr()
{
#ifdef _WIN64
    DWORD pGetModuleHandleW = (DWORD)GetSystem32ProcAddr(L"\\KnownDlls32\\kernel32.dll", "GetModuleHandleW");
    DWORD pGetProcAddress = (DWORD)GetSystem32ProcAddr(L"\\KnownDlls32\\kernel32.dll", "GetProcAddress");
#else
    DWORD pGetModuleHandleW = (DWORD)GetModuleHandleW;
    DWORD pGetProcAddress = (DWORD)GetProcAddress;
#endif
    PVOID call1 = (PVOID)&GetProcAsmCode[15];
    PVOID call2 = (PVOID)&GetProcAsmCode[30];
    LPVOID pAsmFuncAddr = VirtualAllocEx(handle, NULL, 1, MEM_COMMIT, PAGE_EXECUTE);
    if (!pAsmFuncAddr)
        return 0;
    *(DWORD *)call1 = pGetModuleHandleW - (DWORD)pAsmFuncAddr - 14 - 5;
    *(DWORD *)call2 = pGetProcAddress - (DWORD)pAsmFuncAddr - 29 - 5;
    SIZE_T dwWriteSize;
    WriteProcessMemory(handle, pAsmFuncAddr, GetProcAsmCode, sizeof(GetProcAsmCode), &dwWriteSize);
    return pAsmFuncAddr;
}

DWORD WeChatProcess::GetProcAddr(LPSTR functionname)
{
    if (!AsmProcAddr || !handle)
        return 0;
    WeChatData<wchar_t *> r_modulename(handle, (wchar_t *)DLLNAME, TEXTLENGTH(DLLNAME));
    WeChatData<LPSTR> r_functionname(handle, functionname, TEXTLENGTHA(functionname));
    DWORD params[2] = {0};
    params[0] = (DWORD)r_modulename.GetAddr();
    params[1] = (DWORD)r_functionname.GetAddr();
    WeChatData<DWORD *> r_params(handle, &params[0], sizeof(params));
    DWORD dwProcAddr = CallRemoteFunction(handle, AsmProcAddr, r_params.GetAddr());
    return dwProcAddr;
}

DWORD WeChatProcess::WeChatRobotBase()
{
    if (!handle)
        return 0;
    WeChatData<wchar_t *> r_dllname(handle, (wchar_t *)DLLNAME, TEXTLENGTH(DLLNAME));
    if (r_dllname.GetAddr() == 0)
        return 0;
#ifdef _WIN64
    DWORD pGetModuleHandleW = (DWORD)GetSystem32ProcAddr(L"\\KnownDlls32\\kernel32.dll", "GetModuleHandleW");
#else
    DWORD pGetModuleHandleW = (DWORD)GetModuleHandleW;
#endif
    DWORD ret = CallRemoteFunction(handle, pGetModuleHandleW, r_dllname.GetAddr());
    return ret;
}
