#include "pch.h"

#define SendAppMsgCall1Offset 0x04421340 - 0x04360000
#define SendAppMsgCall2Offset 0x04AD0980 - 0x04360000
#define SendAppMsgCall3Offset 0x04B1BB30 - 0x04360000

#ifndef USE_SOCKET
struct SendAppMsgStruct {
	wchar_t* wxid;
	wchar_t* appid;
};
#endif

BOOL __stdcall SendAppMsg(wchar_t* wxid,wchar_t* appid) {
	DWORD WeChatWinBase = GetWeChatWinBase();
	DWORD SendAppMsgCall1 = WeChatWinBase + SendAppMsgCall1Offset;
	DWORD SendAppMsgCall2 = WeChatWinBase + SendAppMsgCall2Offset;
	DWORD SendAppMsgCall3 = WeChatWinBase + SendAppMsgCall3Offset;
	vector<WxBaseStruct> receiver;
	VectorStruct* vs = (VectorStruct*)&receiver;
	DWORD preceiver = (DWORD)&vs->v_data;
	WxBaseStruct pwxid(wxid);
	receiver.push_back(pwxid);
	WxBaseStruct pappid(appid);
	char buffer[0x1E0] = { 0 };
	BOOL isSuccess = 0x0;
	__asm {
		pushad;
		pushfd;
		push 0x60966;
		sub esp, 0xC;
		mov esi, dword ptr[SendAppMsgCall3];
		mov ecx, esp;
		mov eax, dword ptr[preceiver];
		push eax;
		call SendAppMsgCall1;
		sub esp, 0x14;
		lea eax, pappid;
		mov ecx, esp;
		push eax;
		call SendAppMsgCall2;
		lea ecx, buffer;
		call SendAppMsgCall3;
		mov isSuccess, eax;
		popfd;
		popad;
	}
	return isSuccess;
}

#ifndef USE_SOCKET
BOOL SendAppMsgRemote(LPVOID lpParameter) {
	SendAppMsgStruct* sams = (SendAppMsgStruct*)lpParameter;
	BOOL isSuccess = SendAppMsg(sams->wxid, sams->appid);
	return isSuccess;
}
#endif