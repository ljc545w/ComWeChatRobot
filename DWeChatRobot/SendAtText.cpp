#include "pch.h"

#define SendAtTextCallOffset 0x6782E7B0 - 0x67370000
#define DeleteAtTextCacheCallOffset 0x67404200 - 0x67370000

struct SendAtTextStruct
{
    DWORD chatroomid;
    DWORD wxid;
    DWORD wxmsg;
    DWORD length;
};

struct AtStruct {
    DWORD AtUser;
    DWORD addr_end1;
    DWORD addr_end2;
};

void SendAtTextRemote(LPVOID lpParameter) {
    SendAtTextStruct* rp = (SendAtTextStruct*)lpParameter;
    wchar_t* wsChatRoomId = (WCHAR*)rp->chatroomid;
    wchar_t* wsTextMsg = (WCHAR*)rp->wxmsg;
    if (rp->length == 0)
        return;
    else if(rp->length == 1)
        SendAtText(wsChatRoomId, (DWORD*)&rp->wxid, wsTextMsg,rp->length);
    else
        SendAtText(wsChatRoomId, (DWORD*)rp->wxid, wsTextMsg, rp->length);
}

void __stdcall SendAtText(wchar_t* wsChatRoomId, DWORD wsWxId[], wchar_t* wsTextMsg,int length) {
    WxString* AtUsers = new WxString[length + 1];
    wstring AtMessage = L"";
    int querySuccess = 0;
    for (int i = 0; i < length; i++) {
        wchar_t* nickname = NULL;
        if (!lstrcmpW((wchar_t*)wsWxId[i], (wchar_t*)L"notify@all")) {
            nickname = (wchar_t*)L"ËùÓÐÈË";
        }
        else
            nickname = GetUserNickNameByWxId((wchar_t*)wsWxId[i]);
        if (!nickname)
            continue;
        WxString temp = { 0 };
        temp.buffer = (wchar_t*)wsWxId[i];
        temp.length = wcslen((wchar_t*)wsWxId[i]);
        temp.maxLength = wcslen((wchar_t*)wsWxId[i]) * 2;
        memcpy(&AtUsers[querySuccess],&temp,sizeof(WxString));
        AtMessage = AtMessage + L"@" + nickname + L" ";
        querySuccess++;
    }
    AtMessage += wsTextMsg;
    if (!querySuccess)
        return;
    WxBaseStruct wxChatRoomId(wsChatRoomId);
    WxBaseStruct wxTextMsg((wchar_t*)AtMessage.c_str());
    AtStruct at = { 0 };
    at.AtUser = (DWORD)AtUsers;
    at.addr_end1 = (DWORD)&AtUsers[querySuccess];
    at.addr_end2 = (DWORD)&AtUsers[querySuccess];

    wchar_t** pWxmsg = &wxTextMsg.buffer;
    char buffer[0x3B0] = { 0 };

    DWORD dllBaseAddress = GetWeChatWinBase();
    DWORD callAddress = dllBaseAddress + SendAtTextCallOffset;
    DWORD DeleteTextCacheCall = dllBaseAddress + DeleteAtTextCacheCallOffset;

    __asm {
        lea eax, at;
        push 0x1;
        push eax;
        mov edi, pWxmsg;
        push edi;
        lea edx, wxChatRoomId;
        lea ecx, buffer;
        call callAddress;
        add esp, 0xC;
        lea ecx, buffer;
        call DeleteTextCacheCall;
    }
    delete[] AtUsers;
    AtUsers = NULL;
}