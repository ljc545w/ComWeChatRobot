#include "pch.h"
#include <vector>

// 接收消息的HOOK地址偏移
#define ReceiveMessageHookOffset 0x034A4F60 - 0x02FE0000
// HOOK的CALL偏移
#define ReceiveMessageNextCallOffset 0x034A0CE0 - 0x02FE0000

/*
* 保存单条信息的结构
* messagetype：消息类型
* sender：发送者wxid；l_sender：`sender`字符数
* wxid：如果sender是群聊id，则此成员保存具体发送人wxid，否则与`sender`一致；l_wxid：`wxid`字符数
* message：消息内容，非文本消息是xml格式；l_message：`message`字符数
* filepath：图片、文件及其他资源的保存路径；l_filepath：`filepath`字符数
*/
struct messageStruct {
	DWORD messagetype;
	wchar_t* sender;
	DWORD l_sender;
	wchar_t* wxid;
	DWORD l_wxid;
	wchar_t* message;
	DWORD l_message;
	wchar_t* filepath;
	DWORD l_filepath;
};

// 保存多条信息的动态数组
vector<messageStruct> messageVector;

// 是否开启接收消息HOOK标志
BOOL ReceiveMessageHooked = false;
// 保存HOOK前的字节码，用于恢复
char OldReceiveMessageAsmCode[5] = { 0 };
// 接收消息HOOK地址
DWORD ReceiveMessageHookAddress = GetWeChatWinBase() + ReceiveMessageHookOffset;
// HOOK的CALL地址
DWORD ReceiveMessageNextCall = GetWeChatWinBase() + ReceiveMessageNextCallOffset;
// HOOK的跳转地址
DWORD JmpBackAddress = ReceiveMessageHookAddress + 0x5;

/*
* 消息处理函数，根据消息缓冲区组装结构并存入容器
* messageAddr：保存消息的缓冲区地址
* return：void
*/
VOID ReceiveMessage(DWORD messageAddr) {
	// 此处用于区别是发送的还是接收的消息，发送的消息会被过滤
	DWORD isSendMessage = *(DWORD*)(messageAddr + 0x3C);
	if (isSendMessage)
		return;
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

	length = *(DWORD*)(messageAddr + 0x1AC + 0x4);
	message.filepath = new wchar_t[length + 1];
	ZeroMemory(message.filepath, (length + 1) * 2);
	memcpy(message.filepath, (wchar_t*)(*(DWORD*)(messageAddr + 0x1AC)), length * 2);
	message.l_filepath = length;

	messageVector.push_back(message);
}

/*
* 供外部调用的获取消息接口，优先返回较早消息
* return：DWORD，messageVector第一个成员地址
*/
DWORD GetHeadMessage() {
	if (messageVector.size() == 0)
		return 0;
	return (DWORD)&messageVector[0].messagetype;
}

/*
* 供外部调用的删除消息接口，用于删除messageVector第一个成员，每读一条需要执行一次
* return：void
*/
VOID PopHeadMessage() {
	if (messageVector.size() == 0)
		return;
	delete[] messageVector[0].message;
	messageVector[0].message = NULL;
	delete[] messageVector[0].sender;
	messageVector[0].sender = NULL;
	delete[] messageVector[0].wxid;
	messageVector[0].wxid = NULL;
	delete[] messageVector[0].filepath;
	messageVector[0].filepath = NULL;
	vector<messageStruct>::iterator k = messageVector.begin();
	messageVector.erase(k);
}

/*
* HOOK的具体实现，接收到消息后调用处理函数
*/
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

/*
* 开始接收消息HOOK
* return：void
*/
VOID HookReceiveMessage() {
	if (ReceiveMessageHooked)
		return;
	HookAnyAddress(ReceiveMessageHookAddress,(LPVOID)dealReceiveMessage,OldReceiveMessageAsmCode);
	ReceiveMessageHooked = TRUE;
}

/*
* 停止接收消息HOOK
* return：void
*/
VOID UnHookReceiveMessage() {
	if (!ReceiveMessageHooked)
		return;
	UnHookAnyAddress(ReceiveMessageHookAddress,OldReceiveMessageAsmCode);
	ReceiveMessageHooked = FALSE;
}