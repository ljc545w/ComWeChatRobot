#include "pch.h"

#define GetChatRoomMemberNicknameCall1Offset 0x78F629F0 - 0x78E80000
#define GetChatRoomMemberNicknameCall2Offset 0x792C89A0 - 0x78E80000

#ifndef USE_SOCKET
struct ChatRoomMemberNicknameStruct
{
    wchar_t *chatroomid;
    wchar_t *wxid;
    wchar_t *nickname;
};

BOOL GetChatRoomMemberNicknameRemote(LPVOID lpParameter)
{
    ChatRoomMemberNicknameStruct *crmns = (ChatRoomMemberNicknameStruct *)lpParameter;
    wstring nickname = GetChatRoomMemberNickname(crmns->chatroomid, crmns->wxid);
    memcpy(crmns->nickname, nickname.c_str(), nickname.length() * 2);
    return nickname.length() != 0;
}
#endif // !USE_SOCKET

wstring GetChatRoomMemberNickname(wchar_t *chatroomid, wchar_t *wxid)
{
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD GetChatRoomMemberNicknameCall1 = WeChatWinBase + GetChatRoomMemberNicknameCall1Offset;
    DWORD GetChatRoomMemberNicknameCall2 = WeChatWinBase + GetChatRoomMemberNicknameCall2Offset;
    WxString pchatroomid(chatroomid);
    WxString pwxid(wxid);
    WxString pnickname(NULL);
    __asm {
		pushad;
		pushfd;
		call GetChatRoomMemberNicknameCall1;
		lea edx, pnickname;
		push edx;
		lea edi, pwxid;
		push edi;
		lea ecx, pchatroomid;
		push ecx;
		mov ecx, eax;
		call GetChatRoomMemberNicknameCall2;
		popfd;
		popad;
    }
    wstring nickname = L"";
    if (pnickname.buffer)
    {
        nickname += wstring(pnickname.buffer);
    }
    else
    {
        nickname += GetUserNickNameByWxId(wxid);
    }
    return nickname;
}
