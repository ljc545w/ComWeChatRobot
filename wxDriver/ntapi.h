#pragma once
#include<windows.h>

/*
* ����ָ�롢���ݽṹ�Լ���ο����²ֿ�
* https://github.com/winsiderss/systeminformer
*/

#define RtlPointerToOffset(Base, Pointer) ((ULONG)(((PCHAR)(Pointer)) - ((PCHAR)(Base))))
#define RtlOffsetToPointer(Base, Offset) ((PCHAR)(((PCHAR)(Base)) + ((ULONG_PTR)(Offset))))
#define NtCurrentProcess() ((HANDLE)(LONG_PTR)-1)

#ifndef OBJ_PROTECT_CLOSE
#define OBJ_PROTECT_CLOSE 0x00000001
#endif
#ifndef OBJ_INHERIT
#define OBJ_INHERIT 0x00000002
#endif
#ifndef OBJ_AUDIT_OBJECT_CLOSE
#define OBJ_AUDIT_OBJECT_CLOSE 0x00000004
#endif

#define InitializeObjectAttributes(p, n, a, r, s) { \
    (p)->Length = sizeof(OBJECT_ATTRIBUTES); \
    (p)->RootDirectory = r; \
    (p)->Attributes = a; \
    (p)->ObjectName = n; \
    (p)->SecurityDescriptor = s; \
    (p)->SecurityQualityOfService = NULL; \
    }

typedef enum _SECTION_INHERIT
{
    ViewShare = 1,
    ViewUnmap = 2
} SECTION_INHERIT;

typedef enum _SECTION_INFORMATION_CLASS
{
    SectionBasicInformation,
    SectionImageInformation,
    SectionRelocationInformation,
    SectionOriginalBaseInformation,
    SectionInternalImageInformation,
    MaxSectionInfoClass
} SECTION_INFORMATION_CLASS;

typedef struct _UNICODE_STRING
{
    USHORT Length;
    USHORT MaximumLength;
    _Field_size_bytes_part_(MaximumLength, Length) PWCH Buffer;
} UNICODE_STRING, * PUNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES
{
    ULONG Length;
    HANDLE RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG Attributes;
    PVOID SecurityDescriptor;
    PVOID SecurityQualityOfService;
} OBJECT_ATTRIBUTES, * POBJECT_ATTRIBUTES;

typedef struct _SECTION_IMAGE_INFORMATION
{
    PVOID TransferAddress;
    ULONG ZeroBits;
    SIZE_T MaximumStackSize;
    SIZE_T CommittedStackSize;
    ULONG SubSystemType;
    union
    {
        struct
        {
            USHORT SubSystemMinorVersion;
            USHORT SubSystemMajorVersion;
        };
        ULONG SubSystemVersion;
    };
    union
    {
        struct
        {
            USHORT MajorOperatingSystemVersion;
            USHORT MinorOperatingSystemVersion;
        };
        ULONG OperatingSystemVersion;
    };
    USHORT ImageCharacteristics;
    USHORT DllCharacteristics;
    USHORT Machine;
    BOOLEAN ImageContainsCode;
    union
    {
        UCHAR ImageFlags;
        struct
        {
            UCHAR ComPlusNativeReady : 1;
            UCHAR ComPlusILOnly : 1;
            UCHAR ImageDynamicallyRelocated : 1;
            UCHAR ImageMappedFlat : 1;
            UCHAR BaseBelow4gb : 1;
            UCHAR ComPlusPrefer32bit : 1;
            UCHAR Reserved : 2;
        };
    };
    ULONG LoaderFlags;
    ULONG ImageFileSize;
    ULONG CheckSum;
} SECTION_IMAGE_INFORMATION, * PSECTION_IMAGE_INFORMATION;


typedef
NTSYSCALLAPI
PIMAGE_NT_HEADERS
(NTAPI*
pRtlImageNtHeader)(
    _In_ PVOID BaseOfImage
);

typedef
NTSYSCALLAPI
PVOID
(NTAPI*
pRtlImageDirectoryEntryToData)(
    _In_ PVOID BaseOfImage,
    _In_ BOOLEAN MappedAsImage,
    _In_ USHORT DirectoryEntry,
    _Out_ PULONG Size
);

typedef
NTSYSCALLAPI
NTSTATUS
(NTAPI*
pZwOpenSection)(
    _Out_ PHANDLE SectionHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes
);

typedef
NTSYSCALLAPI
NTSTATUS
(NTAPI*
pZwQuerySection)(
    _In_ HANDLE SectionHandle,
    _In_ SECTION_INFORMATION_CLASS SectionInformationClass,
    _Out_writes_bytes_(SectionInformationLength) PVOID SectionInformation,
    _In_ SIZE_T SectionInformationLength,
    _Out_opt_ PSIZE_T ReturnLength
);

typedef
NTSYSCALLAPI
NTSTATUS
(NTAPI*
pZwMapViewOfSection)(
    _In_ HANDLE SectionHandle,
    _In_ HANDLE ProcessHandle,
    _Inout_ _At_(*BaseAddress, _Readable_bytes_(*ViewSize) _Writable_bytes_(*ViewSize) _Post_readable_byte_size_(*ViewSize)) PVOID* BaseAddress,
    _In_ ULONG_PTR ZeroBits,
    _In_ SIZE_T CommitSize,
    _Inout_opt_ PLARGE_INTEGER SectionOffset,
    _Inout_ PSIZE_T ViewSize,
    _In_ SECTION_INHERIT InheritDisposition,
    _In_ ULONG AllocationType,
    _In_ ULONG Win32Protect
);

typedef
NTSYSCALLAPI
NTSTATUS
(NTAPI*
pZwUnmapViewOfSection)(
    _In_ HANDLE ProcessHandle,
    _In_opt_ PVOID BaseAddress
);

typedef
NTSYSCALLAPI
NTSTATUS
(NTAPI*
pNtClose)(
    _In_ _Post_ptr_invalid_ HANDLE Handle
);