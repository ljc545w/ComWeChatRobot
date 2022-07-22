#include "pch.h"
#include "ntapi.h"
#pragma comment(lib,"ntdll.lib")

HMODULE hNtdll = GetModuleHandle(L"ntdll.dll");

pNtQuerySystemInformation NtQuerySystemInformation = (pNtQuerySystemInformation)GetProcAddress(hNtdll, "NtQuerySystemInformation");
pNtDuplicateObject NtDuplicateObject = (pNtDuplicateObject)GetProcAddress(hNtdll, "NtDuplicateObject");
pNtQueryObject NtQueryObject = (pNtQueryObject)GetProcAddress(hNtdll, "NtQueryObject");

BOOL CloseProcessHandle(DWORD pid, wchar_t* handlename) {
    wstring name(handlename);
    NTSTATUS status;
    PSYSTEM_HANDLE_INFORMATION handleInfo;
    ULONG handleInfoSize = 0x10000;
    HANDLE processHandle, dupHandle;
    POBJECT_TYPE_INFORMATION objectTypeInfo;
    SYSTEM_HANDLE handle = { 0 };
    bool thao = false;
    wstring str = L"";
    handleInfo = (PSYSTEM_HANDLE_INFORMATION)malloc(handleInfoSize);
    while ((status = NtQuerySystemInformation(SystemHandleInformation, handleInfo, handleInfoSize, NULL)
        ) == STATUS_INFO_LENGTH_MISMATCH)
    {
        handleInfoSize *= 2;
        PSYSTEM_HANDLE_INFORMATION tempinfo = (PSYSTEM_HANDLE_INFORMATION)realloc(handleInfo, (size_t)handleInfoSize);
        if (tempinfo)
            handleInfo = tempinfo;
    }
    if (handleInfo == NULL) {
        return false;
    }
    for (ULONG i = 0; i < handleInfo->HandleCount; i++)
    {
        thao = false;
        handle = handleInfo->Handles[i];
        if (handle.ProcessId != pid)
            continue;

        processHandle = OpenProcess(PROCESS_DUP_HANDLE, FALSE, pid);
        if (processHandle != NULL)
        {
            status = NtDuplicateObject(processHandle, (HANDLE)handle.Handle, GetCurrentProcess(), &dupHandle, 0, 0, 0);
            if (status == 0)
            {
                objectTypeInfo = (POBJECT_TYPE_INFORMATION)malloc(0x2000);
                if (NtQueryObject(dupHandle, ObjectTypeInformation, objectTypeInfo, 0x1000, NULL) == 0)
                {
                    if (objectTypeInfo != NULL) {
                        str = wstring(objectTypeInfo->Name.Buffer);
                    }
                    if (str == L"Mutant")
                    {
                        NtQueryObject(dupHandle, ObjectNameInformation, objectTypeInfo, 0x1000, NULL);
                        if (objectTypeInfo != NULL) {
                            str = wstring(objectTypeInfo->Name.Buffer ? objectTypeInfo->Name.Buffer : L"");
                        }
                        if (str.find(name) != wstring::npos)
                        {
                            thao = true;
                        }
                    }
                    else if (str == L"Semaphore")
                    {
                        NtQueryObject(dupHandle, ObjectNameInformation, objectTypeInfo, 0x1000, NULL);
                        if (objectTypeInfo != NULL) {
                            str = wstring(objectTypeInfo->Name.Buffer ? objectTypeInfo->Name.Buffer : L"");
                        }
                        if (str.find(name) != wstring::npos)
                        {
                            thao = true;
                        }
                    }
                }
                CloseHandle(dupHandle);
                free(objectTypeInfo);
                objectTypeInfo = NULL;
                if (thao == true)
                {
                    HANDLE h_another_proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
                    DuplicateHandle(h_another_proc, (HANDLE)handle.Handle, GetCurrentProcess(), &dupHandle, 0, FALSE, DUPLICATE_SAME_ACCESS | DUPLICATE_CLOSE_SOURCE); // ¹Ø±Õ      
                    CloseHandle(dupHandle);
                    CloseHandle(h_another_proc);
                }
            }
            CloseHandle(processHandle);
        }
    }
    free(handleInfo);
    handleInfo = NULL;
    return thao;
}