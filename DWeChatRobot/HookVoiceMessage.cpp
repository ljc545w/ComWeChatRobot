#include "pch.h"

#define HookVoiceMsgAddrOffset 0x610528DA - 0x60AE0000
#define HookVoiceMsgNextCallOffset 0x620F1040 - 0x60AE0000

BOOL VoiceMsgHooked = false;
static DWORD WeChatWinBase = GetWeChatWinBase();
static DWORD HookVoiceMsgAddr = WeChatWinBase + HookVoiceMsgAddrOffset;
static DWORD HookVoiceMsgNextCall = WeChatWinBase + HookVoiceMsgNextCallOffset;
static DWORD HookVoiceMsgJmpBackAddr = HookVoiceMsgAddr + 0x5;
static char VoiceMsgOldAsm[5] = { 0 };
static wstring savepath = L"";

void SaveVoiceMsg(unsigned char* buffer, int length, DWORD msgHandle) {
	time_t curtime = time(0);
	wchar_t timestamp[32] = { 0 };
	_itow_s((int)curtime, timestamp, 10);
	wstring filename = savepath + timestamp + L".amr";
	HANDLE hFile = CreateFile(filename.c_str(), GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	DWORD dwWrite = 0;
	WriteFile(hFile, (LPCVOID)buffer, length, &dwWrite, 0);
	CloseHandle(hFile);
}

__declspec(naked) void dealVoiceMsg() {
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

void __stdcall HookVoiceMsg() {
	if (VoiceMsgHooked)
		return;
	HookAnyAddress(HookVoiceMsgAddr, dealVoiceMsg, VoiceMsgOldAsm);
	VoiceMsgHooked = true;
}

void UnHookVoiceMsg() {
	if (!VoiceMsgHooked)
		return;
	UnHookAnyAddress(HookVoiceMsgAddr, VoiceMsgOldAsm);
	VoiceMsgHooked = false;
}

BOOL HookVoiceMsgRemote(LPVOID lpParameter) {
	savepath = (wstring)(wchar_t*)lpParameter;
	if (savepath.back() != '\\') {
		savepath += L"\\";
	}
	wstring createpath = savepath.substr(0, savepath.length() - 1);
	if (!FindOrCreateDirectory(createpath.c_str())) {
		return false;
	}
	HookVoiceMsg();
	return true;
}