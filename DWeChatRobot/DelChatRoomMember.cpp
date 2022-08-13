#include "pch.h"

#define DelChatRoomMemeberCall1Offset 0x67CF29F0 - 0x67C10000
#define DelChatRoomMemeberCall2Offset 0x68381980 - 0x67C10000
#define DelChatRoomMemeberCall3Offset 0x6804DC10 - 0x67C10000

#ifndef USE_SOCKET
struct DelChatRoomMemberStruct
{
    DWORD chatroomid;
    DWORD wxidlist;
    DWORD length;
};

BOOL DelChatRoomMemberRemote(LPVOID lpParameter)
{
    DelChatRoomMemberStruct *rp = (DelChatRoomMemberStruct *)lpParameter;
    wchar_t *wsChatRoomId = (WCHAR *)rp->chatroomid;
    if (rp->length == 0)
        return false;
    else if (rp->length == 1)
        return DelChatRoomMember(wsChatRoomId, (wchar_t **)&rp->wxidlist, rp->length);
    else
        return DelChatRoomMember(wsChatRoomId, (wchar_t **)rp->wxidlist, rp->length);
}
#endif // !USE_SOCKET

BOOL DelChatRoomMember(wchar_t *chatroomid, wchar_t **wxids, int length)
{
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD DelChatRoomMemeberCall1 = WeChatWinBase + DelChatRoomMemeberCall1Offset;
    DWORD DelChatRoomMemeberCall2 = WeChatWinBase + DelChatRoomMemeberCall2Offset;
    DWORD DelChatRoomMemeberCall3 = WeChatWinBase + DelChatRoomMemeberCall3Offset;

    WxString pchatroomid(chatroomid);
    vector<WxString> members;
    VectorStruct *vs = (VectorStruct *)&members;
    DWORD pmembers = (DWORD)&vs->v_data;
    for (int i = 0; i < length; i++)
    {
        WxString pwxid(wxids[i]);
        members.push_back(pwxid);
    }
    int isSuccess = 0;
    __asm {
		pushad;
		pushfd;
		call DelChatRoomMemeberCall1;
		sub esp, 0x14;
		mov esi, eax;
		mov ecx, esp;
		lea ebx, pchatroomid;
		push ebx;
		call DelChatRoomMemeberCall2;
		mov edi, dword ptr[pmembers];
		push edi;
		mov ecx, esi;
		call DelChatRoomMemeberCall3;
		mov isSuccess, eax;
		popfd;
		popad;
    }
    return isSuccess == 1;
}
