#include "pch.h"
#include <vector>
#include <winsock2.h>
#include <Ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define CLTIP "127.0.0.1"

// ������Ϣ��HOOK��ַƫ��
#define ReceiveMessageHookOffset 0x78BF0F4C - 0x786A0000
// ������ϢHOOK��CALLƫ��
#define ReceiveMessageNextCallOffset 0x79136350 - 0x786A0000

// ������Ϣ��HOOK��ַƫ��
#define SendMessageHookOffset 0x78B88E42 - 0x786A0000
// ������ϢHOOK��CALLƫ��
#define SendMessageNextCallOffset 0x78AA8170 - 0x786A0000

static int SRVPORT = 0;

struct ScoketMsgStruct {
	int messagetype;
	BOOL isSendMessage;
	wchar_t sender[80];
	wchar_t wxid[80];
	wchar_t message[0x1000B];
	wchar_t filepath[MAX_PATH];
	wchar_t time[30];
};

// �Ƿ���������ϢHOOK��־
BOOL ReceiveMessageHooked = false;
// ����HOOKǰ���ֽ��룬���ڻָ�
char OldReceiveMessageAsmCode[5] = { 0 };
char OldSendMessageAsmCode[5] = { 0 };
// ������ϢHOOK��ַ
DWORD ReceiveMessageHookAddress = GetWeChatWinBase() + ReceiveMessageHookOffset;
// ������ϢHOOK��CALL��ַ
DWORD ReceiveMessageNextCall = GetWeChatWinBase() + ReceiveMessageNextCallOffset;
// ����HOOK����ת��ַ
DWORD ReceiveMessageJmpBackAddress = ReceiveMessageHookAddress + 0x5;
// ������ϢHOOK��ַ
DWORD SendMessageHookAddress = GetWeChatWinBase() + SendMessageHookOffset;
// ������ϢHOOK��CALL��ַ
DWORD SendMessageNextCall = GetWeChatWinBase() + SendMessageNextCallOffset;
// ����HOOK����ת��ַ
DWORD SendMessageJmpBackAddress = SendMessageHookAddress + 0x5;

// ͨ��socket����Ϣ���͸������
BOOL SendSocketMessage(ReceiveMsgStruct* ms)
{
	if (SRVPORT == 0) {
		delete ms;
		return false;
	}
	SOCKET clientsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientsocket < 0)
	{
#ifdef _DEBUG
		cout << "create socket error," << " errno:" << errno << endl;
#endif
		return false;
	}
	BOOL status = false;
	sockaddr_in clientAddr;
	memset(&clientAddr, 0, sizeof(clientAddr));
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons((u_short)SRVPORT);
	InetPtonA(AF_INET,CLTIP,&clientAddr.sin_addr.s_addr);

	if (connect(clientsocket, reinterpret_cast<sockaddr*>(&clientAddr), sizeof(sockaddr)) < 0)
	{
#ifdef _DEBUG
		cout << "connect error,"<< " errno:" << errno << endl;
#endif
		delete ms;
		return false;
	}
	char recvbuf[1024] = { 0 };
	ScoketMsgStruct* sms = new ScoketMsgStruct;
	ZeroMemory(sms, sizeof(ScoketMsgStruct));
	sms->messagetype = ms->messagetype;
	sms->isSendMessage = ms->isSendMessage;
	memcpy(sms->wxid, ms->wxid, ms->l_wxid * 2);
	memcpy(sms->sender, ms->sender, ms->l_sender * 2);
	memcpy(sms->message, ms->message, ms->l_message * 2);
	memcpy(sms->filepath, ms->filepath, ms->l_filepath * 2);
	memcpy(sms->time, ms->time, ms->l_time * 2);
	wcout << sms->time << endl;
	int ret = send(clientsocket, (char*)sms, sizeof(ScoketMsgStruct), 0);
	if (ret == -1 || ret == 0)
	{
#ifdef _DEBUG
		cout << "send fail," << " errno:" << errno << endl;
#endif
		delete ms;
		delete sms;
		closesocket(clientsocket);
		return false;
	}
	memset(recvbuf, 0, sizeof(recvbuf));
	ret = recv(clientsocket, recvbuf, sizeof(recvbuf), 0);
	delete ms;
	delete sms;
	closesocket(clientsocket);
	if (ret == -1 || ret == 0)
	{
#ifdef _DEBUG
		cout << "the server close" << endl;
#endif
		return false;
	}
	return true;
}

// �����㲥��Ϣ����
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

static void dealMessage(DWORD messageAddr) {
	BOOL isSendMessage = *(BOOL*)(messageAddr + 0x3C);
	ReceiveMsgStruct* message = new ReceiveMsgStruct;
	ZeroMemory(message, sizeof(ReceiveMsgStruct));
	message->isSendMessage = isSendMessage;
	message->time = GetTimeW(*(DWORD*)(messageAddr + 0x44));
	message->l_time = wcslen(message->time);
	message->messagetype = *(DWORD*)(messageAddr + 0x38);

	DWORD length = *(DWORD*)(messageAddr + 0x48 + 0x4);
	message->sender = new wchar_t[length + 1];
	ZeroMemory(message->sender, (length + 1) * 2);
	memcpy(message->sender, (wchar_t*)(*(DWORD*)(messageAddr + 0x48)), length * 2);
	message->l_sender = length;

	length = *(DWORD*)(messageAddr + 0x170 + 0x4);
	if (length == 0) {
		message->wxid = new wchar_t[message->l_sender + 1];
		ZeroMemory(message->wxid, (message->l_sender + 1) * 2);
		memcpy(message->wxid, (wchar_t*)(*(DWORD*)(messageAddr + 0x48)), message->l_sender * 2);
		message->l_wxid = message->l_sender;
	}
	else {
		message->wxid = new wchar_t[length + 1];
		ZeroMemory(message->wxid, (length + 1) * 2);
		memcpy(message->wxid, (wchar_t*)(*(DWORD*)(messageAddr + 0x170)), length * 2);
		message->l_wxid = length;
	}

	length = *(DWORD*)(messageAddr + 0x70 + 0x4);
	message->message = new wchar_t[length + 1];
	ZeroMemory(message->message, (length + 1) * 2);
	memcpy(message->message, (wchar_t*)(*(DWORD*)(messageAddr + 0x70)), length * 2);
	message->l_message = length;

	length = *(DWORD*)(messageAddr + 0x1AC + 0x4);
	message->filepath = new wchar_t[length + 1];
	ZeroMemory(message->filepath, (length + 1) * 2);
	memcpy(message->filepath, (wchar_t*)(*(DWORD*)(messageAddr + 0x1AC)), length * 2);
	message->l_filepath = length;
#ifdef USE_COM
	// ͨ�����ӵ㣬����Ϣ�㲥���ͻ���
	SAFEARRAY* psaValue = CreateMessageArray(message);
	VARIANT vsaValue;
	vsaValue.vt = VT_ARRAY | VT_VARIANT;
	V_ARRAY(&vsaValue) = psaValue;
	PostComMessage(&vsaValue);
#endif
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SendSocketMessage, message, NULL, 0);
	if (hThread) {
		CloseHandle(hThread);
	}
}

/*
* ��Ϣ��������������Ϣ��������װ�ṹ����������
* messageAddr��������Ϣ�Ļ�������ַ
* return��void
*/
VOID ReceiveMessage(DWORD messagesAddr) {
	// �˴����������Ƿ��͵Ļ��ǽ��յ���Ϣ
	DWORD* messages = (DWORD*)messagesAddr;
	for (DWORD messageAddr = messages[0]; messageAddr < messages[1]; messageAddr += 0x298) {
		dealMessage(messageAddr);
	}
}


/*
* HOOK�ľ���ʵ�֣����յ���Ϣ����ô�����
*/
_declspec(naked) void dealReceiveMessage() {
	__asm {
		pushad;
		pushfd;
		// mov eax, [edi];
		push edi;
		call ReceiveMessage;
		add esp, 0x4;
		popfd;
		popad;
		call ReceiveMessageNextCall;
		jmp ReceiveMessageJmpBackAddress;
	}
}

/*
* HOOK�ľ���ʵ�֣�������Ϣ����ô�����
*/
_declspec(naked) void dealSendMessage() {
	__asm {
		pushad;
		pushfd;
		push edi;
		call dealMessage;
		add esp, 0x4;
		popfd;
		popad;
		call SendMessageNextCall;
		jmp SendMessageJmpBackAddress;
	}
}

/*
* ��ʼ������ϢHOOK
* return��void
*/
VOID HookReceiveMessage(int port) {
	SRVPORT = port;
	if (ReceiveMessageHooked)
		return;
	HookAnyAddress(ReceiveMessageHookAddress,(LPVOID)dealReceiveMessage,OldReceiveMessageAsmCode);
	HookAnyAddress(SendMessageHookAddress, (LPVOID)dealSendMessage, OldSendMessageAsmCode);
	ReceiveMessageHooked = TRUE;
}

/*
* ֹͣ������ϢHOOK
* return��void
*/
VOID UnHookReceiveMessage() {
	SRVPORT = 0;
	if (!ReceiveMessageHooked)
		return;
	UnHookAnyAddress(ReceiveMessageHookAddress,OldReceiveMessageAsmCode);
	UnHookAnyAddress(SendMessageHookAddress, OldSendMessageAsmCode);
	ReceiveMessageHooked = FALSE;
}