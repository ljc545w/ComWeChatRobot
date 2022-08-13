#include "pch.h"

#define SetChatRoomSelfNicknameCall1Offset 0x680CD620 - 0x68010000
#define SetChatRoomSelfNicknameCall2Offset 0x68781980 - 0x68010000
#define SetChatRoomSelfNicknameCall3Offset 0x68509FE0 - 0x68010000

#ifndef USE_SOCKET
struct SetChatRoomSelfNicknameStruct
{
    wchar_t *chatroomid;
    wchar_t *nickname;
};
BOOL SetChatRoomSelfNicknameRemote(LPVOID lpParameter)
{
    SetChatRoomSelfNicknameStruct *scrsns = (SetChatRoomSelfNicknameStruct *)lpParameter;
    if (!scrsns->nickname)
        return false;
    return SetChatRoomSelfNickname(scrsns->chatroomid, scrsns->nickname);
}
#endif // !USE_SOCKET

BOOL SetChatRoomSelfNickname(wchar_t *chatroomid, wchar_t *nickname)
{
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD SetChatRoomSelfNicknameCall1 = WeChatWinBase + SetChatRoomSelfNicknameCall1Offset;
    DWORD SetChatRoomSelfNicknameCall2 = WeChatWinBase + SetChatRoomSelfNicknameCall2Offset;
    DWORD SetChatRoomSelfNicknameCall3 = WeChatWinBase + SetChatRoomSelfNicknameCall3Offset;
    wstring selfwxid = GetSelfWxid();
    WxString pchatroomid(chatroomid);
    WxString pselfwxid((wchar_t *)selfwxid.c_str());
    WxString pnickname(nickname);

    int isSuccess = 0x0;
    __asm {
		pushad;
		pushfd;
		call SetChatRoomSelfNicknameCall1;
		sub esp, 0x14;
		mov esi, eax;
		lea eax, pnickname;
		mov ecx, esp;
		push eax;
		call SetChatRoomSelfNicknameCall2;
		sub esp, 0x14;
		lea eax, pselfwxid;
		mov ecx, esp;
		push eax;
		call SetChatRoomSelfNicknameCall2;
		sub esp, 0x14;
		lea eax, pchatroomid;
		mov ecx, esp;
		push eax;
		call SetChatRoomSelfNicknameCall2;
		mov ecx, esi;
		call SetChatRoomSelfNicknameCall3;
		mov isSuccess, eax;
		popfd;
		popad;
    }
    return isSuccess == 0x1;
}
