#include "pch.h"

#define SetChatRoomNameCall1Offset 0x680CD620 - 0x68010000
#define SetChatRoomNameCall2Offset 0x685090A0 - 0x68010000

#ifndef USE_SOCKET
struct SetChatRoomNameStruct
{
    wchar_t *chatroomid;
    wchar_t *chatroomname;
};

BOOL SetChatRoomNameRemote(LPVOID lpParameter)
{
    SetChatRoomNameStruct *scrns = (SetChatRoomNameStruct *)lpParameter;
    return SetChatRoomName(scrns->chatroomid, scrns->chatroomname);
}
#endif // !USE_SOCKET

BOOL SetChatRoomName(wchar_t *chatroomid, wchar_t *chatroomname)
{
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD SetChatRoomNameCall1 = WeChatWinBase + SetChatRoomNameCall1Offset;
    DWORD SetChatRoomNameCall2 = WeChatWinBase + SetChatRoomNameCall2Offset;
    WxString pchatroomid(chatroomid);
    WxString pchatroomname(chatroomname);
    int isSuccess = 0;

    __asm {
		pushad;
		pushfd;
		call SetChatRoomNameCall1;
		lea ebx, pchatroomname;
		push ebx;
		lea esi, pchatroomid;
		push esi;
		mov ecx, eax;
		call SetChatRoomNameCall2;
		movzx eax, al;
		mov isSuccess, eax;
		popfd;
		popad;
    }
    return isSuccess == 0x1;
}
