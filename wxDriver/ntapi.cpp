#include "pch.h"
#ifdef _WIN64
#include "driver.h"
#include "ntapi.h"
#pragma comment(lib,"ntdll.lib")

HMODULE hNtdll = GetModuleHandle(L"ntdll.dll");
pZwOpenSection ZwOpenSection = (pZwOpenSection)GetProcAddress(hNtdll, "ZwOpenSection");
pZwQuerySection ZwQuerySection = (pZwQuerySection)GetProcAddress(hNtdll, "ZwQuerySection");
pZwMapViewOfSection ZwMapViewOfSection = (pZwMapViewOfSection)GetProcAddress(hNtdll, "ZwMapViewOfSection");
pZwUnmapViewOfSection ZwUnmapViewOfSection = (pZwUnmapViewOfSection)GetProcAddress(hNtdll, "ZwUnmapViewOfSection");
pNtClose NtClose = (pNtClose)GetProcAddress(hNtdll, "NtClose");
pRtlImageNtHeader RtlImageNtHeader = (pRtlImageNtHeader)GetProcAddress(hNtdll, "RtlImageNtHeader");
pRtlImageDirectoryEntryToData RtlImageDirectoryEntryToData = (pRtlImageDirectoryEntryToData)GetProcAddress(hNtdll, "RtlImageDirectoryEntryToData");

FORCEINLINE VOID RtlInitUnicodeString(
    _Out_ PUNICODE_STRING DestinationString,
    _In_opt_ PCWSTR SourceString
)
{
    if (SourceString)
        DestinationString->MaximumLength = (DestinationString->Length = (USHORT)(wcslen(SourceString) * sizeof(WCHAR))) + sizeof(UNICODE_NULL);
    else
        DestinationString->MaximumLength = DestinationString->Length = 0;

    DestinationString->Buffer = (PWCH)SourceString;
}

static PVOID getRVA(PVOID Base, ULONG_PTR BaseAddress, PCSTR Name)
{
    if (PIMAGE_NT_HEADERS32 pinth = (PIMAGE_NT_HEADERS32)RtlImageNtHeader(Base))
    {
        BaseAddress -= pinth->OptionalHeader.AddressOfEntryPoint;
        DWORD Size, exportRVA;
        if (PIMAGE_EXPORT_DIRECTORY pied = (PIMAGE_EXPORT_DIRECTORY)
            RtlImageDirectoryEntryToData(Base, TRUE, IMAGE_DIRECTORY_ENTRY_EXPORT, &Size))
        {
            exportRVA = RtlPointerToOffset(Base, pied);
            DWORD NumberOfFunctions = pied->NumberOfFunctions;
            DWORD NumberOfNames = pied->NumberOfNames;
            if (0 < NumberOfNames && NumberOfNames <= NumberOfFunctions)
            {
                PDWORD AddressOfFunctions = (PDWORD)RtlOffsetToPointer(Base, pied->AddressOfFunctions);
                PDWORD AddressOfNames = (PDWORD)RtlOffsetToPointer(Base, pied->AddressOfNames);
                PWORD AddressOfNameOrdinals = (PWORD)RtlOffsetToPointer(Base, pied->AddressOfNameOrdinals);
                DWORD a = 0, b = NumberOfNames, o;
                do
                {
                    o = (a + b) >> 1;
                    int i = strcmp(RtlOffsetToPointer(Base, AddressOfNames[o]), Name);
                    if (!i)
                    {
                        DWORD Rva = AddressOfFunctions[AddressOfNameOrdinals[o]];
                        return (ULONG_PTR)Rva - (ULONG_PTR)exportRVA < Size ? 0 : RtlOffsetToPointer(BaseAddress, Rva);
                    }
                    0 > i ? a = o + 1 : b = o;
                } while (a < b);
            }
        }
    }
    return 0;
}

PVOID GetSystem32ProcAddr(PCWSTR ObjectName, PCSTR procName)
{
    PVOID pv = 0;
    OBJECT_ATTRIBUTES oa;
    UNICODE_STRING oadll;
    RtlInitUnicodeString(&oadll, ObjectName);
    InitializeObjectAttributes(&oa, &oadll, OBJ_INHERIT, NULL, NULL);
    HANDLE hSection;
    int hr = ZwOpenSection(&hSection, SECTION_QUERY | SECTION_MAP_READ, &oa);
    if (0 <= hr)
    {
        SECTION_IMAGE_INFORMATION sii;
        if (0 <= ZwQuerySection(hSection, SectionImageInformation, &sii, sizeof(sii), 0))
        {
            PVOID BaseAddress = 0;
            SIZE_T ViewSize = 0;
            if (0 <= ZwMapViewOfSection(hSection, NtCurrentProcess(), &BaseAddress, 0, 0, 0, &ViewSize, ViewUnmap, 0, PAGE_READONLY))
            {
                pv = getRVA(BaseAddress, (ULONG_PTR)sii.TransferAddress, procName);
                ZwUnmapViewOfSection(NtCurrentProcess(), BaseAddress);
            }
        }
        NtClose(hSection);
    }
    return pv;
}
#endif