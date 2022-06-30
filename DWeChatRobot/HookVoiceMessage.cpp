#include "pch.h"

#define HookVoiceMsgAddrOffset 0x105728DA - 0x10000000
#define HookVoiceMsgNextCallOffset 0x11612540 - 0x10000000

BOOL VoiceMsgHooked = false;
static DWORD WeChatWinBase = GetWeChatWinBase();
static DWORD HookVoiceMsgAddr = WeChatWinBase + HookVoiceMsgAddrOffset;
static DWORD HookVoiceMsgNextCall = WeChatWinBase + HookVoiceMsgNextCallOffset;
static DWORD HookVoiceMsgJmpBackAddr = HookVoiceMsgAddr + 0x5;
static char VoiceMsgOldAsm[5] = { 0 };
static wstring savepath = L"";

void SaveVoiceMsg(unsigned char* buffer, int length, DWORD msgHandle) {
	/*time_t curtime = time(0);
	wchar_t timestamp[32] = { 0 };
	_itow_s((int)curtime, timestamp, 10);*/
	wchar_t* temp;
	int wxid_length = *(DWORD*)(msgHandle + 0x174);
	temp = new wchar_t[wxid_length + 1];
	memcpy(temp, (void*)(*(DWORD*)(msgHandle + 0x170)), (wxid_length + 1) * 2);
	wstring sender(temp);
	delete[] temp;
	temp = NULL;
	
	int clientmsg_length = *(DWORD*)(msgHandle + 0x188);
	temp = new wchar_t[clientmsg_length + 1];
	memcpy(temp, (void*)(*(DWORD*)(msgHandle + 0x184)), (clientmsg_length + 1) * 2);
	wstring clientmsgid(temp);
	delete[] temp;
	temp = NULL;
	wstring filename = savepath + sender + L"-" + clientmsgid + L".amr";
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

#ifndef USE_SOCKET
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
#endif