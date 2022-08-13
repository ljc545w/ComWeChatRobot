#include "pch.h"
#include <vector>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <map>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define CLTIP "127.0.0.1"

// 接收消息的HOOK地址偏移
#define ReceiveMessageHookOffset 0x78BF0F4C - 0x786A0000
// 接收消息HOOK的CALL偏移
#define ReceiveMessageNextCallOffset 0x79136350 - 0x786A0000

// 发送消息的HOOK地址偏移
#define SendMessageHookOffset 0x78B88E42 - 0x786A0000
// 发送消息HOOK的CALL偏移
#define SendMessageNextCallOffset 0x78AA8170 - 0x786A0000

#define READ_WSTRING(addr, offset) wstring((wchar_t *)(*(DWORD *)(addr + offset)), *(DWORD *)(addr + offset + 0x4))

static int SRVPORT = 0;

struct ScoketMsgStruct
{
    DWORD pid;
    int messagetype;
    BOOL isSendMessage;
    unsigned long long msgid;
    wchar_t sender[80];
    wchar_t wxid[80];
    wchar_t message[0x1000B];
    wchar_t filepath[MAX_PATH];
    wchar_t time[30];
};

// 是否开启接收消息HOOK标志
BOOL ReceiveMessageHooked = false;
// 保存HOOK前的字节码，用于恢复
char OldReceiveMessageAsmCode[5] = {0};
char OldSendMessageAsmCode[5] = {0};
static DWORD WeChatWinBase = GetWeChatWinBase();
// 接收消息HOOK地址
static DWORD ReceiveMessageHookAddress = WeChatWinBase + ReceiveMessageHookOffset;
// 接收消息HOOK的CALL地址
static DWORD ReceiveMessageNextCall = WeChatWinBase + ReceiveMessageNextCallOffset;
// 接收HOOK的跳转地址
static DWORD ReceiveMessageJmpBackAddress = ReceiveMessageHookAddress + 0x5;
// 发送消息HOOK地址
static DWORD SendMessageHookAddress = WeChatWinBase + SendMessageHookOffset;
// 发送消息HOOK的CALL地址
static DWORD SendMessageNextCall = WeChatWinBase + SendMessageNextCallOffset;
// 发送HOOK的跳转地址
static DWORD SendMessageJmpBackAddress = SendMessageHookAddress + 0x5;

// 通过socket将消息发送给服务端
BOOL SendSocketMessage(ReceiveMsgStruct *ms)
{
    shared_ptr<ReceiveMsgStruct> shared_ms(ms);
    if (SRVPORT == 0)
    {
        return false;
    }
    SOCKET clientsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientsocket < 0)
    {
#ifdef _DEBUG
        cout << "create socket error,"
             << " errno:" << errno << endl;
#endif
        return false;
    }
    BOOL status = false;
    sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons((u_short)SRVPORT);
    InetPtonA(AF_INET, CLTIP, &clientAddr.sin_addr.s_addr);

    if (connect(clientsocket, reinterpret_cast<sockaddr *>(&clientAddr), sizeof(sockaddr)) < 0)
    {
#ifdef _DEBUG
        cout << "connect error,"
             << " errno:" << errno << endl;
#endif
        return false;
    }
    char recvbuf[1024] = {0};
    auto sms = std::make_shared<ScoketMsgStruct>();
    ZeroMemory(sms.get(), sizeof(ScoketMsgStruct));
    sms->pid = shared_ms->pid;
    sms->messagetype = shared_ms->messagetype;
    sms->isSendMessage = shared_ms->isSendMessage;
    sms->msgid = shared_ms->msgid;
    memcpy(sms->wxid, shared_ms->wxid.c_str(), shared_ms->wxid.length() * 2);
    memcpy(sms->sender, shared_ms->sender.c_str(), shared_ms->sender.length() * 2);
    memcpy(sms->message, shared_ms->message.c_str(), shared_ms->message.length() * 2);
    memcpy(sms->filepath, shared_ms->filepath.c_str(), shared_ms->filepath.length() * 2);
    memcpy(sms->time, shared_ms->time.c_str(), shared_ms->time.length() * 2);
#ifdef _DEBUG
    wcout << sms->time << endl;
#endif
    int ret = send(clientsocket, (char *)sms.get(), sizeof(ScoketMsgStruct), 0);
    if (ret == -1 || ret == 0)
    {
#ifdef _DEBUG
        cout << "send fail,"
             << " errno:" << errno << endl;
#endif
        closesocket(clientsocket);
        return false;
    }
    memset(recvbuf, 0, sizeof(recvbuf));
    ret = recv(clientsocket, recvbuf, sizeof(recvbuf), 0);
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

// 创建广播消息数组
#ifndef USE_SOCKET
static SAFEARRAY *CreateMessageArray(map<wstring, _variant_t> msg)
{
    HRESULT hr = S_OK;
    SAFEARRAY *psaValue;
    vector<wstring> MessageInfoKey = {
        L"pid",
        L"type",
        L"isSendMessage",
        L"msgid",
        msg[L"isSendMessage"].boolVal ? L"sendto" : L"from",
        L"wxid",
        L"message",
        L"filepath",
        L"time"};
    SAFEARRAYBOUND rgsaBound[2] = {{MessageInfoKey.size(), 0}, {2, 0}};
    psaValue = SafeArrayCreate(VT_VARIANT, 2, rgsaBound);
    long keyIndex[2] = {0, 0};
    keyIndex[0] = 0;
    keyIndex[1] = 0;
    for (unsigned int i = 0; i < MessageInfoKey.size(); i++)
    {
        keyIndex[0] = i;
        keyIndex[1] = 0;
        _variant_t key = MessageInfoKey[i].c_str();
        hr = SafeArrayPutElement(psaValue, keyIndex, &key);
        keyIndex[0] = i;
        keyIndex[1] = 1;
        hr = SafeArrayPutElement(psaValue, keyIndex, &msg[MessageInfoKey[i]]);
    }
    return psaValue;
}
#endif

static void dealMessage(DWORD messageAddr)
{
    BOOL isSendMessage = *(BOOL *)(messageAddr + 0x3C);
    ReceiveMsgStruct *message = new ReceiveMsgStruct;
    ZeroMemory(message, sizeof(ReceiveMsgStruct));
    message->pid = GetCurrentProcessId();
    message->isSendMessage = isSendMessage;
    message->time = GetTimeW(*(DWORD *)(messageAddr + 0x44));
    message->messagetype = *(DWORD *)(messageAddr + 0x38);
    message->msgid = *(unsigned long long *)(messageAddr + 0x30);
    message->sender = READ_WSTRING(messageAddr, 0x48);
    int length = *(DWORD *)(messageAddr + 0x170 + 0x4);
    if (length == 0)
    {
        message->wxid = message->sender;
    }
    else
    {
        message->wxid = READ_WSTRING(messageAddr, 0x170);
    }
    message->message = READ_WSTRING(messageAddr, 0x70);
    message->filepath = READ_WSTRING(messageAddr, 0x1AC);
#ifdef USE_COM
    // 通过连接点，将消息广播给客户端
    map<wstring, _variant_t> msg_map;
    msg_map[L"pid"] = message->pid;
    msg_map[L"isSendMessage"] = isSendMessage;
    msg_map[L"time"] = message->time.c_str();
    msg_map[L"type"] = message->messagetype;
    msg_map[L"msgid"] = message->msgid;
    msg_map[L"sendto"] = message->sender.c_str();
    msg_map[L"from"] = message->sender.c_str();
    msg_map[L"wxid"] = message->wxid.c_str();
    msg_map[L"message"] = message->message.c_str();
    msg_map[L"filepath"] = message->filepath.c_str();
    SAFEARRAY *psaValue = CreateMessageArray(msg_map);
    VARIANT vsaValue;
    vsaValue.vt = VT_ARRAY | VT_VARIANT;
    V_ARRAY(&vsaValue) = psaValue;
    PostComMessage(message->pid, WX_MESSAGE, &vsaValue);
#endif
    HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SendSocketMessage, message, NULL, 0);
    if (hThread)
    {
        CloseHandle(hThread);
    }
}

/*
* 消息处理函数，根据消息缓冲区组装结构并存入容器
* messageAddr：保存消息的缓冲区地址
* return：void
*/
VOID ReceiveMessage(DWORD messagesAddr)
{
    // 此处用于区别是发送的还是接收的消息
    DWORD *messages = (DWORD *)messagesAddr;
    for (DWORD messageAddr = messages[0]; messageAddr < messages[1]; messageAddr += 0x298)
    {
        dealMessage(messageAddr);
    }
}

/*
* HOOK的具体实现，接收到消息后调用处理函数
*/
_declspec(naked) void dealReceiveMessage()
{
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
* HOOK的具体实现，发送消息后调用处理函数
*/
_declspec(naked) void dealSendMessage()
{
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
* 开始接收消息HOOK
* return：void
*/
VOID HookReceiveMessage(int port)
{
    SRVPORT = port;
    WeChatWinBase = GetWeChatWinBase();
    if (ReceiveMessageHooked || !WeChatWinBase)
        return;
    ReceiveMessageHookAddress = WeChatWinBase + ReceiveMessageHookOffset;
    ReceiveMessageNextCall = WeChatWinBase + ReceiveMessageNextCallOffset;
    ReceiveMessageJmpBackAddress = ReceiveMessageHookAddress + 0x5;
    SendMessageHookAddress = WeChatWinBase + SendMessageHookOffset;
    SendMessageNextCall = WeChatWinBase + SendMessageNextCallOffset;
    SendMessageJmpBackAddress = SendMessageHookAddress + 0x5;
    HookAnyAddress(ReceiveMessageHookAddress, (LPVOID)dealReceiveMessage, OldReceiveMessageAsmCode);
    HookAnyAddress(SendMessageHookAddress, (LPVOID)dealSendMessage, OldSendMessageAsmCode);
    ReceiveMessageHooked = TRUE;
}

/*
* 停止接收消息HOOK
* return：void
*/
VOID UnHookReceiveMessage()
{
    SRVPORT = 0;
    if (!ReceiveMessageHooked)
        return;
    UnHookAnyAddress(ReceiveMessageHookAddress, OldReceiveMessageAsmCode);
    UnHookAnyAddress(SendMessageHookAddress, OldSendMessageAsmCode);
    ReceiveMessageHooked = FALSE;
}
