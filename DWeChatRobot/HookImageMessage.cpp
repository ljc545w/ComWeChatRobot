#include "pch.h"

#define HookImageMsgAddrOffset 0x10732D26 - 0x10000000
#define HookImageMsgNextCallOffset 0x10732160 - 0x10000000
#define AutoDownloadTimeSettingOffset 0x239EC50

BOOL ImageMsgHooked = false;
static DWORD WeChatWinBase = GetWeChatWinBase();
static DWORD HookImageMsgAddr = WeChatWinBase + HookImageMsgAddrOffset;
static DWORD HookImageMsgNextCall = WeChatWinBase + HookImageMsgNextCallOffset;
static DWORD HookImageMsgJmpBackAddr = HookImageMsgAddr + 0x5;
static char ImageMsgOldAsm[5] = { 0 };
static wstring savepath = L"";

void SaveImageMsg(unsigned char* buffer, int length, DWORD msgHandle) {
	int l_datpath = *(int*)(msgHandle + 0x4) + 1;
	wchar_t* datpath = new wchar_t[l_datpath];
	memcpy(datpath, (void*)(*(DWORD*)msgHandle), l_datpath * 2);
	wstring wdatpath(datpath);
	delete[] datpath;
	datpath = NULL;
	if (wdatpath.find(L"_t.dat") != wstring::npos) {
		return;
	}
	int pos_begin = wdatpath.find_last_of(L"\\") + 1;
	int pos_end = wdatpath.find_last_of(L".");
	wstring filename = wdatpath.substr(pos_begin, pos_end - pos_begin);

	unsigned char magic_head[4] = { 0 };
	wchar_t postfix[5] = { 0 };
	memcpy(magic_head, buffer, 3);
	if (magic_head[0] == 137 && magic_head[1] == 80 && magic_head[2] == 78)
	{
		lstrcpy(postfix,L".png");
	}
	else if (magic_head[0] == 71 && magic_head[1] == 73 && magic_head[2] == 70)
	{
		lstrcpy(postfix, L".gif");
	}
	else if (magic_head[0] == 255 && magic_head[1] == 216 && magic_head[2] == 255)
	{
		lstrcpy(postfix, L".jpg");
	}
	else {
		lstrcpy(postfix, L"");
	}
	wstring filepath = savepath + filename + postfix;
	HANDLE hFile = CreateFile(filepath.c_str(), GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	DWORD dwWrite = 0;
	WriteFile(hFile, (LPCVOID)buffer, length, &dwWrite, 0);
	CloseHandle(hFile);
}

__declspec(naked) void dealImageMsg() {
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

void __stdcall HookImageMsg() {
	WeChatWinBase = GetWeChatWinBase();
	if (ImageMsgHooked || !WeChatWinBase)
		return;
	HookImageMsgAddr = WeChatWinBase + HookImageMsgAddrOffset;
	HookImageMsgNextCall = WeChatWinBase + HookImageMsgNextCallOffset;
	HookImageMsgJmpBackAddr = HookImageMsgAddr + 0x5;
	HookAnyAddress(HookImageMsgAddr, dealImageMsg, ImageMsgOldAsm);
	char settime[] = "00:00-00:00";
	DWORD AutoDownloadTimeSettingAddr = GetWeChatWinBase() + AutoDownloadTimeSettingOffset;
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)AutoDownloadTimeSettingAddr, settime, strlen(settime) + 1, 0);
	ImageMsgHooked = true;
}

void UnHookImageMsg() {
	if (!ImageMsgHooked)
		return;
	UnHookAnyAddress(HookImageMsgAddr, ImageMsgOldAsm);
	ImageMsgHooked = false;
}

#ifndef USE_SOCKET
BOOL HookImageMsgRemote(LPVOID lpParameter) {
	savepath = (wstring)(wchar_t*)lpParameter;
	if (savepath.back() != '\\') {
		savepath += L"\\";
	}
	wstring createpath = savepath.substr(0, savepath.length() - 1);
	if (!FindOrCreateDirectory(createpath.c_str())) {
		return false;
	}
	HookImageMsg();
	return true;
}
#endif