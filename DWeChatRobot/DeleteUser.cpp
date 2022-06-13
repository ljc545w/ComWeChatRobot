#include "pch.h"

#define DeleteUserCall1Offset 0x04417570 - 0x04360000
#define DeleteUserCall2Offset 0x0441D570 - 0x04360000
#define DeleteUserCall3Offset 0x047C8570 - 0x04360000

BOOL __stdcall DeleteUser(wchar_t* wxid) {
	DWORD WeChatWinBase = GetWeChatWinBase();
	DWORD DeleteUserCall1 = WeChatWinBase + DeleteUserCall1Offset;
	DWORD DeleteUserCall2 = WeChatWinBase + DeleteUserCall2Offset;
	DWORD DeleteUserCall3 = WeChatWinBase + DeleteUserCall3Offset;

	WxBaseStruct pwxid(wxid);
	char buffer[0x70] = { 0 };
	BOOL isSuccess = 0x0;
	__asm {
		pushad;
		pushfd;
		lea ecx, buffer;
		call DeleteUserCall1;
		call DeleteUserCall2;
		push ecx;
		lea esi, pwxid;
		push esi;
		mov ecx, eax;
		call DeleteUserCall3;
		mov isSuccess, eax;
		popfd;
		popad;
	}
	return isSuccess;
}

BOOL DeleteUserRemote(LPVOID lpParameter) {
	BOOL isSuccess = DeleteUser((wchar_t*)lpParameter);
	return isSuccess;
}