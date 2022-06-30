#include "pch.h"

// 微信日志HOOK地址偏移
#define HookLogMsgInfoAddrOffset 0x78E10444 - 0x786A0000
// HOOK的CALL偏移
#define HookLogMsgInfoNextCallOffset 0x7A0AF9BE - 0x786A0000
// HOOK的跳转地址偏移
#define HookLogMsgJmpBackOffset 0x78E10449 - 0x786A0000

// 微信日志HOOK地址
DWORD HookLogMsgInfoAddr = GetWeChatWinBase() + HookLogMsgInfoAddrOffset;
// HOOK的CALL地址
DWORD NextCallAddr = GetWeChatWinBase() + HookLogMsgInfoNextCallOffset;
// HOOK的跳转地址
DWORD JmpBackAddr = GetWeChatWinBase() + HookLogMsgJmpBackOffset;

// 是否开启日志HOOK标志
BOOL LogMsgHooked = false;
// 保存HOOK前的指令用于恢复
char LogOldAsmCode[5] = { 0 };

/*
* 处理函数，打印日志信息
* msg：日志信息
* return：void
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
* HOOK的具体实现，拦截日志并调用处理函数
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
* 开始HOOK微信日志
* return：void
*/
VOID HookLogMsgInfo() {
	if (LogMsgHooked)
		return;
	HookAnyAddress(HookLogMsgInfoAddr,(LPVOID)doprintmsg, LogOldAsmCode);
	LogMsgHooked = true;
}

/*
* 停止HOOK微信日志
* return：void
*/
VOID UnHookLogMsgInfo() {
	if (!LogMsgHooked)
		return;
	UnHookAnyAddress(HookLogMsgInfoAddr, LogOldAsmCode);
	LogMsgHooked = false;
}