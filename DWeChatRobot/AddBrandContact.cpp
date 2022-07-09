#include "pch.h"

#define AddPublicNoticeCallOffset  0x1203DB14 - 0x10000000
#define AddPublicNoticeParamOffset 0x7A583DCC - 0x786A0000

struct AddBrandContactStruct {
	DWORD handle;
	WxString command;
	char buffer1[0x28] = { 0 };
	WxString params;
	char buffer2[0x7C] = { 0 };
	AddBrandContactStruct(wchar_t* param) {
		command.buffer = (wchar_t*)L"quicklyAddBrandContact";
		command.length = wcslen(command.buffer);
		command.maxLength = command.length * 2;
		params.buffer = param;
		params.length = wcslen(param);
		params.maxLength = wcslen(param) * 2;
	}
};

BOOL __stdcall AddBrandContact(wchar_t* PublicId) {
	DWORD WeChatWinBase = GetWeChatWinBase();
	DWORD AddPublicNoticeCall = WeChatWinBase + AddPublicNoticeCallOffset;
	DWORD AddPublicNoticeParam = WeChatWinBase + AddPublicNoticeParamOffset;
	wchar_t url[0x100] = { 0 };
	swprintf_s(url, L"weixin://resourceid/Subscription/profile.html?userName=%ws", PublicId);
	WxBaseStruct ptrurl(url);
	wchar_t param[0x100] = { 0 };
	swprintf_s(param, L"{\"username\":\"%ws\",\"webtype\":\"1\"}\n", PublicId);
	AddBrandContactStruct ptrparam(param);
	ptrparam.handle = AddPublicNoticeParam;
	DWORD NoticeCallHandle[10] = { 0 };
	NoticeCallHandle[0] = AddPublicNoticeCall;
	BOOL isSuccess = 0x0;
	__asm {
		pushad;
		pushfd;
		lea eax, ptrparam;
		push eax;
		lea eax, NoticeCallHandle;
		mov edx, dword ptr [eax];
		mov ecx, eax;
		lea esi, ptrurl;
		push esi;
		mov edi, 0x3;
		push edi;
		call dword ptr [edx + 0x4];
		mov isSuccess, eax;
		popfd;
		popad;
	}
	return isSuccess;
}

#ifndef USE_SOCKET
BOOL AddBrandContactRemote(LPVOID lpParameter) {
	int isSuccess = AddBrandContact((wchar_t*)lpParameter);
	return isSuccess;
}
#endif