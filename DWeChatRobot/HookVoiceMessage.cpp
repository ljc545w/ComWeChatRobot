#include "pch.h"

#define HookVoiceMsgAddrOffset 0x105728DA - 0x10000000
#define HookVoiceMsgNextCallOffset 0x11612540 - 0x10000000

BOOL VoiceMsgHooked = false;
static DWORD WeChatWinBase = GetWeChatWinBase();
static DWORD HookVoiceMsgAddr = WeChatWinBase + HookVoiceMsgAddrOffset;
static DWORD HookVoiceMsgNextCall = WeChatWinBase + HookVoiceMsgNextCallOffset;
static DWORD HookVoiceMsgJmpBackAddr = HookVoiceMsgAddr + 0x5;
static char VoiceMsgOldAsm[5] = {0};
static wstring global_save_path = L"";

void SaveVoiceMsg(unsigned char *buffer, int length, DWORD msgHandle)
{
    wstring save_path = global_save_path + GetSelfWxid();
    if (!FindOrCreateDirectory(save_path.c_str()))
    {
        return;
    }
    wchar_t *temp;

    int wxid_length = *(DWORD *)(msgHandle + 0x174);
    temp = new wchar_t[wxid_length + 1];
    memcpy(temp, (void *)(*(DWORD *)(msgHandle + 0x170)), (wxid_length + 1) * 2);
    wstring sender(temp);
    delete[] temp;
    temp = NULL;

    int clientmsg_length = *(DWORD *)(msgHandle + 0x188);
    temp = new wchar_t[clientmsg_length + 1];
    memcpy(temp, (void *)(*(DWORD *)(msgHandle + 0x184)), (clientmsg_length + 1) * 2);
    wstring clientmsgid(temp);
    delete[] temp;
    temp = NULL;
    wstring file_path = save_path + L"\\" + clientmsgid + L".amr";
    HANDLE hFile = CreateFile(file_path.c_str(), GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return;
    }
    DWORD dwWrite = 0;
    WriteFile(hFile, (LPCVOID)buffer, length, &dwWrite, 0);
    CloseHandle(hFile);
}

__declspec(naked) void dealVoiceMsg()
{
    __asm {
		pushad;
		pushfd;
		push edi;
		push esi;
		push eax;
		call SaveVoiceMsg;
		add esp, 0xC;
		popfd;
		popad;
		call HookVoiceMsgNextCall;
		jmp HookVoiceMsgJmpBackAddr;
    }
}

void __stdcall HookVoiceMsg()
{
    WeChatWinBase = GetWeChatWinBase();
    if (VoiceMsgHooked || !WeChatWinBase)
        return;
    SetDownloadTime();
    HookVoiceMsgAddr = WeChatWinBase + HookVoiceMsgAddrOffset;
    HookVoiceMsgNextCall = WeChatWinBase + HookVoiceMsgNextCallOffset;
    HookVoiceMsgJmpBackAddr = HookVoiceMsgAddr + 0x5;
    HookAnyAddress(HookVoiceMsgAddr, dealVoiceMsg, VoiceMsgOldAsm);
    VoiceMsgHooked = true;
}

void UnHookVoiceMsg()
{
    if (!VoiceMsgHooked)
        return;
    UnHookAnyAddress(HookVoiceMsgAddr, VoiceMsgOldAsm);
    VoiceMsgHooked = false;
}

#ifndef USE_SOCKET
BOOL HookVoiceMsgRemote(LPVOID lpParameter)
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
    HookVoiceMsg();
    return true;
}
#else
BOOL __stdcall HookVoiceMsg(wstring save_path)
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
    HookVoiceMsg();
    return true;
}
#endif
