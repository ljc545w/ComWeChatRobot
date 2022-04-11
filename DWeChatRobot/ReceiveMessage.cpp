#include "pch.h"
#include <vector>

#define ReceiveMessageHookOffset 0x034A4F60 - 0x02FE0000
#define ReceiveMessageNextCallOffset 0x034A0CE0 - 0x02FE0000

struct messageStruct {
	DWORD messagetype;
	wchar_t* sender;
	DWORD l_sender;
	wchar_t* wxid;
	DWORD l_wxid;
	wchar_t* message;
	DWORD l_message;
};

vector<messageStruct> messageVector;

BOOL ReceiveMessageHooked = false;
char OldReceiveMessageAsmCode[5] = { 0 };

DWORD ReceiveMessageHookAddress = GetWeChatWinBase() + ReceiveMessageHookOffset;
DWORD ReceiveMessageNextCall = GetWeChatWinBase() + ReceiveMessageNextCallOffset;
DWORD JmpBackAddress = ReceiveMessageHookAddress + 0x5;

VOID ReceiveMessage(DWORD messageAddr) {
	messageStruct message = { 0 };
	message.messagetype = *(DWORD*)(messageAddr + 0x38);
	
	DWORD length = *(DWORD*)(messageAddr + 0x48 + 0x4);
	message.sender = new wchar_t[length + 1];
	ZeroMemory(message.sender, (length + 1) * 2);
	memcpy(message.sender,(wchar_t*)(*(DWORD*)(messageAddr + 0x48)),length * 2);
	message.l_sender = length;
	
	length = *(DWORD*)(messageAddr + 0x170 + 0x4);
	if (length == 0) {
		message.wxid = new wchar_t[message.l_sender + 1];
		ZeroMemory(message.wxid, (message.l_sender + 1) * 2);
		memcpy(message.wxid, (wchar_t*)(*(DWORD*)(messageAddr + 0x48)), message.l_sender * 2);
		message.l_wxid = message.l_sender;
	}
	else {
		message.wxid = new wchar_t[length + 1];
		ZeroMemory(message.wxid, (length + 1) * 2);
		memcpy(message.wxid, (wchar_t*)(*(DWORD*)(messageAddr + 0x170)), length * 2);
		message.l_wxid = length;
	}
	
	length = *(DWORD*)(messageAddr + 0x70 + 0x4);
	message.message = new wchar_t[length + 1];
	ZeroMemory(message.message, (length + 1) * 2);
	memcpy(message.message, (wchar_t*)(*(DWORD*)(messageAddr + 0x70)), length * 2);
	message.l_message = length;

	messageVector.push_back(message);
}

DWORD GetHeadMessage() {
	if (messageVector.size() == 0)
		return 0;
	return (DWORD)&messageVector[0].messagetype;
}

VOID PopHeadMessage() {
	if (messageVector.size() == 0)
		return;
	vector<messageStruct>::iterator k = messageVector.begin();
	messageVector.erase(k);
}

_declspec(naked) void dealReceiveMessage() {
	__asm {
		pushad;
		pushfd;
		mov edi, [eax];
		push edi;
		call ReceiveMessage;
		add esp, 0x4;
		popfd;
		popad;
		call ReceiveMessageNextCall;
		jmp JmpBackAddress;
	}
}

VOID HookReceiveMessage() {
	if (ReceiveMessageHooked)
		return;
	HookAnyAddress(ReceiveMessageHookAddress,(LPVOID)dealReceiveMessage,OldReceiveMessageAsmCode);
	ReceiveMessageHooked = TRUE;
}

VOID UnHookReceiveMessage() {
	if (!ReceiveMessageHooked)
		return;
	UnHookAnyAddress(ReceiveMessageHookAddress,OldReceiveMessageAsmCode);
	ReceiveMessageHooked = FALSE;
}