#pragma once
#include <windows.h>
#include "wxdata.h"
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) VOID HookReceiveMessage(int port);
extern "C" __declspec(dllexport) VOID UnHookReceiveMessage();
extern "C" __declspec(dllexport) void UnHookVoiceMsg();
extern "C" __declspec(dllexport) BOOL HookVoiceMsgRemote(LPVOID lpParameter);
extern "C" __declspec(dllexport) void UnHookImageMsg();
extern "C" __declspec(dllexport) BOOL HookImageMsgRemote(LPVOID lpParameter);
extern "C" __declspec(dllexport) DWORD GetMsgCDNRemote(ULONG64 *p_msgid);
#else
VOID HookReceiveMessage(int port);
VOID UnHookReceiveMessage();
void UnHookImageMsg();
void UnHookVoiceMsg();
BOOL __stdcall HookVoiceMsg(wstring save_path);
BOOL __stdcall HookImageMsg(wstring save_path);
#endif

void __stdcall HookVoiceMsg();
void __stdcall HookImageMsg();
BOOL __stdcall GetChatMsgBySvrId(ULONG64 msgid, PCHAT_MSG p_chat_msg);
BOOL __stdcall ReleaseChatMsg(PCHAT_MSG p_chat_msg);
wstring __stdcall GetMsgCDN(ULONG64 msgid);
void __stdcall SetDownloadTime();

typedef enum MSG_SOURCE_TYPETag
{
    MSG_SYNC,
    MSG_UPDATE,
    MSG_SEND,
} MSG_SOURCE,
    *PMSG_SOURCE;
