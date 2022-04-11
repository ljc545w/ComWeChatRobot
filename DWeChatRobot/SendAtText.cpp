#include "pch.h"

#define SendAtTextCallOffset 0x6782E7B0 - 0x67370000
#define DeleteAtTextCacheCallOffset 0x67404200 - 0x67370000

struct SendAtTextStruct
{
    DWORD chatroomid;
    DWORD wxid;
    DWORD wxmsg;
};

struct AtUserStruct {
    wchar_t* wxid;
    DWORD Length;
    DWORD maxLength;
    DWORD fill1 = 0;
    DWORD fill2 = 0;
    DWORD fill3 = 0;
};

struct AtStruct {
    AtUserStruct* AtUser = NULL;
    DWORD addr_end1;
    DWORD addr_end2;
};

void SendAtTextRemote(LPVOID lpParameter) {
    SendAtTextStruct* rp = (SendAtTextStruct*)lpParameter;
    wchar_t* wsChatRoomId = (WCHAR*)rp->chatroomid;
    wchar_t* wsWxId = (WCHAR*)rp->wxid;
    wchar_t* wsTextMsg = (WCHAR*)rp->wxmsg;
    SendAtText(wsChatRoomId,wsWxId, wsTextMsg);
}

void __stdcall SendAtText(wchar_t* wsChatRoomId, wchar_t* wsWxId, wchar_t* wsTextMsg) {
    wchar_t* NickName = GetUserNickNameByWxId(wsWxId);
    if (!NickName)
        return;
    wchar_t* SendTextMsg = new wchar_t[wcslen(wsTextMsg) + wcslen(NickName) + 3];
    ZeroMemory(SendTextMsg, (wcslen(wsTextMsg) + wcslen(NickName) + 3) * 2);
    swprintf_s(SendTextMsg, (wcslen(wsTextMsg) + wcslen(NickName) + 2) * 2,L"@%ws %ws",NickName,wsTextMsg);

    WxBaseStruct wxChatRoomId(wsChatRoomId);
    WxBaseStruct wxTextMsg(SendTextMsg);
    AtStruct at = { 0 };
    AtUserStruct AtUser = { 0 };
    AtUser.wxid = wsWxId;
    AtUser.Length = wcslen(wsWxId);
    AtUser.maxLength = wcslen(wsWxId) * 2;
    at.AtUser = &AtUser;
    at.addr_end1 = (DWORD)&AtUser.fill3;
    at.addr_end2 = (DWORD)&AtUser.fill3;

    wchar_t** pWxmsg = &wxTextMsg.buffer;
    char buffer[0x3B0] = { 0 };

    DWORD dllBaseAddress = GetWeChatWinBase();
    DWORD callAddress = dllBaseAddress + SendAtTextCallOffset;
    DWORD DeleteTextCacheCall = dllBaseAddress + DeleteAtTextCacheCallOffset;

    __asm {
        pushad;
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
        popad;
    }
}