#include "pch.h"

#define HookImageMsgAddrOffset 0x10732D26 - 0x10000000
#define HookImageMsgNextCallOffset 0x10732160 - 0x10000000

BOOL ImageMsgHooked = false;
static DWORD HookImageMsgAddr = 0;
static DWORD HookImageMsgNextCall = 0;
static DWORD HookImageMsgJmpBackAddr = 0;
static char ImageMsgOldAsm[5] = {0};
static wstring global_save_path = L"";

void SaveImageMsg(unsigned char *buffer, int length, DWORD msgHandle)
{
    wstring save_path = global_save_path + GetSelfWxid();
    if (!FindOrCreateDirectory(save_path.c_str()))
    {
        return;
    }
    int l_datpath = *(int *)(msgHandle + 0x4) + 1;
    wchar_t *datpath = new wchar_t[l_datpath];
    memcpy(datpath, (void *)(*(DWORD *)msgHandle), l_datpath * 2);
    wstring wdatpath(datpath);
    delete[] datpath;
    datpath = NULL;
    if (wdatpath.find(L"_t.dat") != wstring::npos)
    {
        return;
    }
    int pos_begin = wdatpath.find_last_of(L"\\") + 1;
    int pos_end = wdatpath.find_last_of(L".");
    wstring file_name = wdatpath.substr(pos_begin, pos_end - pos_begin);

    unsigned char magic_head[4] = {0};
    wchar_t postfix[5] = {0};
    memcpy(magic_head, buffer, 3);
    if (magic_head[0] == 137 && magic_head[1] == 80 && magic_head[2] == 78)
    {
        lstrcpy(postfix, L".png");
    }
    else if (magic_head[0] == 71 && magic_head[1] == 73 && magic_head[2] == 70)
    {
        lstrcpy(postfix, L".gif");
    }
    else if (magic_head[0] == 255 && magic_head[1] == 216 && magic_head[2] == 255)
    {
        lstrcpy(postfix, L".jpg");
    }
    else
    {
        lstrcpy(postfix, L"");
    }
    wstring file_path = save_path + L"\\" + file_name + postfix;
    HANDLE hFile = CreateFile(file_path.c_str(), GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return;
    }
    DWORD dwWrite = 0;
    WriteFile(hFile, (LPCVOID)buffer, length, &dwWrite, 0);
    CloseHandle(hFile);
}

__declspec(naked) void dealImageMsg()
{
    __asm {
		pushad;
		pushfd;
		push esi;
		push dword ptr [ebp - 0x20];
		push edi;
		call SaveImageMsg;
		add esp, 0xC;
		popfd;
		popad;
		call HookImageMsgNextCall;
		jmp HookImageMsgJmpBackAddr;
    }
}

void __stdcall HookImageMsg()
{
    DWORD WeChatWinBase = GetWeChatWinBase();
    if (ImageMsgHooked || !WeChatWinBase)
        return;
    SetDownloadTime();
    HookImageMsgAddr = WeChatWinBase + HookImageMsgAddrOffset;
    HookImageMsgNextCall = WeChatWinBase + HookImageMsgNextCallOffset;
    HookImageMsgJmpBackAddr = HookImageMsgAddr + 0x5;
    HookAnyAddress(HookImageMsgAddr, dealImageMsg, ImageMsgOldAsm);
    ImageMsgHooked = true;
}

void UnHookImageMsg()
{
    if (!ImageMsgHooked)
        return;
    UnHookAnyAddress(HookImageMsgAddr, ImageMsgOldAsm);
    ImageMsgHooked = false;
}

#ifndef USE_SOCKET
BOOL HookImageMsgRemote(LPVOID lpParameter)
{
    global_save_path = (wstring)(wchar_t *)lpParameter;
    if (global_save_path.back() != '\\')
    {
        global_save_path += L"\\";
    }
    if (!FindOrCreateDirectory(global_save_path.c_str()))
    {
        return false;
    }
    HookImageMsg();
    return true;
}
#else
BOOL __stdcall HookImageMsg(wstring save_path)
{
    global_save_path = save_path;
    if (global_save_path.back() != '\\')
    {
        global_save_path += L"\\";
    }
    wstring createpath = global_save_path.substr(0, global_save_path.length() - 1);
    if (!FindOrCreateDirectory(createpath.c_str()))
    {
        return false;
    }
    HookImageMsg();
    return true;
}
#endif
