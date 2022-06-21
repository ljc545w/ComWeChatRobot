#include "pch.h"
#include <vector>
#include <iostream>

// 接收消息的HOOK地址偏移
#define ReceiveMessageHookOffset 0x547C0F4C - 0x54270000
// 接收消息HOOK的CALL偏移
#define ReceiveMessageNextCallOffset 0x54D04E60 - 0x54270000

// 发送消息的HOOK地址偏移
#define SendMessageHookOffset 0x102C8E32 - 0x0FDE0000
// 发送消息HOOK的CALL偏移
#define SendMessageNextCallOffset 0x101E8170 - 0x0FDE0000

// 保存多条信息的动态数组
vector<ReceiveMsgStruct> messageVector;

// 是否开启接收消息HOOK标志
BOOL ReceiveMessageHooked = false;
// 保存HOOK前的字节码，用于恢复
char OldReceiveMessageAsmCode[5] = { 0 };
char OldSendMessageAsmCode[5] = { 0 };
// 接收消息HOOK地址
DWORD ReceiveMessageHookAddress = GetWeChatWinBase() + ReceiveMessageHookOffset;
// 接收消息HOOK的CALL地址
DWORD ReceiveMessageNextCall = GetWeChatWinBase() + ReceiveMessageNextCallOffset;
// 接收HOOK的跳转地址
DWORD ReceiveMessageJmpBackAddress = ReceiveMessageHookAddress + 0x5;
// 发送消息HOOK地址
DWORD SendMessageHookAddress = GetWeChatWinBase() + SendMessageHookOffset;
// 发送消息HOOK的CALL地址
DWORD SendMessageNextCall = GetWeChatWinBase() + SendMessageNextCallOffset;
// 发送HOOK的跳转地址
DWORD SendMessageJmpBackAddress = SendMessageHookAddress + 0x5;

// 创建广播消息数组
#ifndef USE_SOCKET
static SAFEARRAY* CreateMessageArray(ReceiveMsgStruct* ms) {
	HRESULT hr = S_OK;
	SAFEARRAY* psaValue;
	vector<wstring> MessageInfoKey = {
		L"type",
		L"isSendMessage",
		ms->isSendMessage ? L"sendto" : L"from",
		L"wxid",
		L"message",
		L"filepath",
		L"time"
	};
	SAFEARRAYBOUND rgsaBound[2] = { {MessageInfoKey.size(),0},{2,0} };
	psaValue = SafeArrayCreate(VT_VARIANT, 2, rgsaBound);
	long keyIndex[2] = { 0,0 };
	keyIndex[0] = 0; keyIndex[1] = 0;
	for (unsigned int i = 0; i < MessageInfoKey.size(); i++) {
		keyIndex[0] = i; keyIndex[1] = 0;
		_variant_t key = MessageInfoKey[i].c_str();
		hr = SafeArrayPutElement(psaValue, keyIndex, &key);
		keyIndex[0] = i; keyIndex[1] = 1;
		if (i < 2) {
			_variant_t value = ((DWORD*)ms)[i];
			hr = SafeArrayPutElement(psaValue, keyIndex, &value);
		}
		else {
			_variant_t value = ((wchar_t**)ms)[i * 2 - 2];
			hr = SafeArrayPutElement(psaValue, keyIndex, &value);
		}
	}
	return psaValue;
}
#endif

/*
* 消息处理函数，根据消息缓冲区组装结构并存入容器
* messageAddr：保存消息的缓冲区地址
* return：void
*/
VOID ReceiveMessage(DWORD messageAddr) {
	// 此处用于区别是发送的还是接收的消息
	BOOL isSendMessage = *(BOOL*)(messageAddr + 0x3C);

	ReceiveMsgStruct message = { 0 };
	message.isSendMessage = isSendMessage;
	message.time = GetTimeW();
	message.l_time = wcslen(message.time);
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
#ifdef USE_COM
	// 通过连接点，将消息广播给客户端
	SAFEARRAY* psaValue = CreateMessageArray(&message);
	VARIANT vsaValue;
	vsaValue.vt = VT_ARRAY | VT_VARIANT;
	V_ARRAY(&vsaValue) = psaValue;
	PostComMessage(&vsaValue);
#endif
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
	delete[] messageVector[0].time;
	messageVector[0].time = NULL;
	vector<ReceiveMsgStruct>::iterator k = messageVector.begin();
	messageVector.erase(k);
}

/*
* HOOK的具体实现，接收到消息后调用处理函数
*/
_declspec(naked) void dealReceiveMessage() {
	__asm {
		pushad;
		pushfd;
		mov eax, [edi];
		push eax;
		call ReceiveMessage;
		add esp, 0x4;
		popfd;
		popad;
		call ReceiveMessageNextCall;
		jmp ReceiveMessageJmpBackAddress;
	}
}

/*
* HOOK的具体实现，发送消息后调用处理函数
*/
_declspec(naked) void dealSendMessage() {
	__asm {
		pushad;
		pushfd;
		push edi;
		call ReceiveMessage;
		add esp, 0x4;
		popfd;
		popad;
		call SendMessageNextCall;
		jmp SendMessageJmpBackAddress;
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
	HookAnyAddress(SendMessageHookAddress, (LPVOID)dealSendMessage, OldSendMessageAsmCode);
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
	UnHookAnyAddress(SendMessageHookAddress, OldSendMessageAsmCode);
	ReceiveMessageHooked = FALSE;
}