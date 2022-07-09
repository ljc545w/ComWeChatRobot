#include "pch.h"

// ΢����־HOOK��ַƫ��
#define HookLogMsgInfoAddrOffset 0x78E10444 - 0x786A0000
// HOOK��CALLƫ��
#define HookLogMsgInfoNextCallOffset 0x7A0AF9BE - 0x786A0000
// HOOK����ת��ַƫ��
#define HookLogMsgJmpBackOffset 0x78E10449 - 0x786A0000

// ΢����־HOOK��ַ
DWORD HookLogMsgInfoAddr = GetWeChatWinBase() + HookLogMsgInfoAddrOffset;
// HOOK��CALL��ַ
DWORD NextCallAddr = GetWeChatWinBase() + HookLogMsgInfoNextCallOffset;
// HOOK����ת��ַ
DWORD JmpBackAddr = GetWeChatWinBase() + HookLogMsgJmpBackOffset;

// �Ƿ�����־HOOK��־
BOOL LogMsgHooked = false;
// ����HOOKǰ��ָ�����ڻָ�
char LogOldAsmCode[5] = { 0 };

/*
* ����������ӡ��־��Ϣ
* msg����־��Ϣ
* return��void
*/
VOID PrintMsg(DWORD msg) {
	if (!msg)
		return;
	char* utf8_message = (char*)msg;
	int c_size = MultiByteToWideChar(CP_UTF8, 0, utf8_message, -1, 0, 0);
	wchar_t* wmessage = new wchar_t[c_size + 1];
	memset(wmessage, 0, (c_size + 1) * 2);
	MultiByteToWideChar(CP_UTF8, 0, utf8_message, -1, wmessage, c_size);
	c_size = WideCharToMultiByte(CP_ACP, 0, wmessage, -1, 0, 0, 0, 0);
	char* message = new char[c_size + 1];
	memset(message, 0, c_size + 1);
	WideCharToMultiByte(CP_ACP, 0, wmessage, -1, message, c_size, 0, 0);
	delete[] wmessage;
	wmessage = NULL;
	cout << message;
	delete[] message;
	message = NULL;
	return;
}

/*
* HOOK�ľ���ʵ�֣�������־�����ô�����
*/
__declspec(naked) void doprintmsg(){
	__asm {
		pushad;
		pushfd;
		push eax;
		call PrintMsg;
		add esp, 0x4;
		popfd;
		popad;
		call NextCallAddr;
		jmp JmpBackAddr;
	}
}

/*
* ��ʼHOOK΢����־
* return��void
*/
VOID HookLogMsgInfo() {
	if (LogMsgHooked)
		return;
	HookAnyAddress(HookLogMsgInfoAddr,(LPVOID)doprintmsg, LogOldAsmCode);
	LogMsgHooked = true;
}

/*
* ֹͣHOOK΢����־
* return��void
*/
VOID UnHookLogMsgInfo() {
	if (!LogMsgHooked)
		return;
	UnHookAnyAddress(HookLogMsgInfoAddr, LogOldAsmCode);
	LogMsgHooked = false;
}