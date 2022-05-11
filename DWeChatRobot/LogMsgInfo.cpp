#include "pch.h"

// 微信日志HOOK地址偏移
#define HookLogMsgInfoAddrOffset 0x103408A4 - 0x0FC40000
// HOOK的CALL偏移
#define HookLogMsgInfoNextCallOffset 0x11586DFC - 0x0FC40000
// HOOK的跳转地址偏移
#define HookLogMsgJmpBackOffset 0x103408A9 - 0x0FC40000

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
	string message = (char*)msg;
	cout << message;
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