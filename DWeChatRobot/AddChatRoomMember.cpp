#include "pch.h"

#define AddChatRoomMemeberCall1Offset 0x67CF29F0 - 0x67C10000
#define AddChatRoomMemeberCall2Offset 0x68381980 - 0x67C10000
#define AddChatRoomMemeberCall3Offset 0x6804D8D0 - 0x67C10000

#ifndef USE_SOCKET
struct AddChatRoomMemberStruct
{
    DWORD chatroomid;
    DWORD wxidlist;
    DWORD length;
};

BOOL AddChatRoomMemberRemote(LPVOID lpParameter)
{
    AddChatRoomMemberStruct *rp = (AddChatRoomMemberStruct *)lpParameter;
    wchar_t *wsChatRoomId = (WCHAR *)rp->chatroomid;
    if (rp->length == 0)
        return false;
    else if (rp->length == 1)
        return AddChatRoomMember(wsChatRoomId, (wchar_t **)&rp->wxidlist, rp->length);
    else
        return AddChatRoomMember(wsChatRoomId, (wchar_t **)rp->wxidlist, rp->length);
}
#endif // !USE_SOCKET

BOOL AddChatRoomMember(wchar_t *chatroomid, wchar_t **wxids, int length)
{
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD AddChatRoomMemeberCall1 = WeChatWinBase + AddChatRoomMemeberCall1Offset;
    DWORD AddChatRoomMemeberCall2 = WeChatWinBase + AddChatRoomMemeberCall2Offset;
    DWORD AddChatRoomMemeberCall3 = WeChatWinBase + AddChatRoomMemeberCall3Offset;

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
		call AddChatRoomMemeberCall1;
		sub esp, 0x14;
		mov esi, eax;
		mov ecx, esp;
		lea ebx, pchatroomid;
		push ebx;
		call AddChatRoomMemeberCall2;
		mov edi, dword ptr[pmembers];
		push edi;
		mov ecx, esi;
		call AddChatRoomMemeberCall3;
		mov isSuccess, eax;
		popfd;
		popad;
    }
    return isSuccess == 1;
}
