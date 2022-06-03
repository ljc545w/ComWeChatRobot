#include "pch.h"

// Ìí¼ÓºÃÓÑCALL1Æ«ÒÆ
#define AddFriendByWxidCall1Offset 0x594944E0 - 0x593B0000
// Ìí¼ÓºÃÓÑCALL2Æ«ÒÆ
#define AddFriendByWxidCall2Offset 0x59B20890 - 0x593B0000
// Ìí¼ÓºÃÓÑCALL3Æ«ÒÆ
#define AddFriendByWxidCall3Offset 0x59B20980 - 0x593B0000
// Ìí¼ÓºÃÓÑCALL4Æ«ÒÆ
#define AddFriendByWxidCall4Offset 0x59813940 - 0x593B0000
// Ìí¼ÓºÃÓÑ²ÎÊýÆ«ÒÆ
#define AddFriendByWxidParamOffset 0x5B7138F4 - 0x593B0000

struct AddFriendByWxidParamStruct {
	DWORD fill0 = 0x0;
	DWORD fill1 = 0x0;
	DWORD fill2 = -0x1;
	DWORD fill3 = 0x0;
	DWORD fill4 = 0x0;
	DWORD fill5 = 0xF;
	char nullbuffer[0xC] = { 0 };
};

struct AddFriendByWxidStruct {
	wchar_t* wxid;
	wchar_t* message;
};

BOOL AddFriendByWxidRemote(LPVOID lpParameter) {
	AddFriendByWxidStruct* afbws = (AddFriendByWxidStruct*)lpParameter;
	BOOL isSuccess = AddFriendByWxid(afbws->wxid, afbws->message);
	return isSuccess;
}

BOOL __stdcall AddFriendByWxid(wchar_t* wxid,wchar_t* message) {
	DWORD WeChatWinBase = GetWeChatWinBase();
	DWORD AddFriendByWxidCall1 = WeChatWinBase + AddFriendByWxidCall1Offset;
	DWORD AddFriendByWxidCall2 = WeChatWinBase + AddFriendByWxidCall2Offset;
	DWORD AddFriendByWxidCall3 = WeChatWinBase + AddFriendByWxidCall3Offset;
	DWORD AddFriendByWxidCall4 = WeChatWinBase + AddFriendByWxidCall4Offset;
	DWORD AddFriendByWxidParamAddr = WeChatWinBase + AddFriendByWxidParamOffset;

	WxBaseStruct pwxid(wxid);
	AddFriendByWxidParamStruct AddFriendParam;

	char* swxid = new char[wcslen(wxid) + 1];
	ZeroMemory(swxid, wcslen(wxid) + 1);
	WideCharToMultiByte(CP_ACP, 0, wxid, -1, swxid, wcslen(wxid), NULL, NULL);
	pwxid.fill1 = (DWORD)swxid;
	pwxid.fill2 = wcslen(wxid);
	wchar_t* pmessage = message ? message : (wchar_t*)L"";
	BOOL isSuccess = 0x0;

	__asm {
		pushad;
		pushfd;
		mov edi, 0x6;
		mov esi, 0x0;
		sub esp, 0x18;
		mov eax, esp;
		mov dword ptr[eax], 0x0;
		mov dword ptr[eax + 0x14], 0xF;
		mov dword ptr[eax + 0x10], 0x0;
		sub esp, 0x18;
		lea eax, AddFriendParam;
		mov ecx, esp;
		push eax;
		call AddFriendByWxidCall1;
		push esi;
		push edi;
		mov edi, pmessage;
		sub esp, 0x14;
		mov ecx, esp;
		push - 0x1;
		mov eax, edi;
		push eax;
		call AddFriendByWxidCall2;
		push 0x2;
		lea eax, pwxid;
		sub esp, 0x14;
		mov ecx, esp;
		push eax;
		call AddFriendByWxidCall3;
		mov eax, [AddFriendByWxidParamAddr];
		mov eax, [eax];
		mov ecx, eax;
		call AddFriendByWxidCall4;
		mov isSuccess, eax;
		popfd;
		popad;
	}
	return isSuccess;
}