#include "pch.h"

#define SearchContactByNetCall1Offset 0x638BF150 - 0x635C0000
#define SearchContactByNetCall2Offset 0x63ACF9C0 - 0x635C0000

void __stdcall SearchContactByNet(wchar_t* keyword) {
	DWORD SearchContactByNetCall1 = GetWeChatWinBase() + SearchContactByNetCall1Offset;
	DWORD SearchContactByNetCall2 = GetWeChatWinBase() + SearchContactByNetCall2Offset;
	WxBaseStruct pkeyword(keyword);

	__asm {
		pushad;
		pushfd;
		call SearchContactByNetCall1;
		lea ebx, pkeyword;
		push ebx;
		mov ecx, eax;
		call SearchContactByNetCall2;
		popfd;
		popad;
	}
}