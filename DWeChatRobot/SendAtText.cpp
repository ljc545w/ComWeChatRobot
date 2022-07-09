#include "pch.h"

// ���Ͱ�����ϢCALLƫ��
#define SendAtTextCallOffset 0x78BC1D30 - 0x786A0000
// ��ջ���CALLƫ��
#define DeleteAtTextCacheCallOffset 0x78757780 - 0x786A0000

/*
* �ⲿ����ʱ���ݵĲ����ṹ
* chatroomid��Ⱥ��ID�ı����ַ
* wxidlist�������б�ı����ַ����ʵ����Ӧ����`wchar_t**`
* wxmsg�����͵����ݱ����ַ
* length�����ص�������������ָʾwxidlist����
*/
#ifndef USE_SOCKET
struct SendAtTextStruct
{
    DWORD chatroomid;
    DWORD wxidlist;
    DWORD wxmsg;
    DWORD length;
    BOOL  AutoNickName;
};
#endif

/*
* �ڴ���ʹ�õĲ����ṹ
* ������Release�汾vector��̬�������
* ��Ա���ͣ�`WxString`
* AtUser������`vector`��`data`���������������׸���Ա�ĵ�ַ
* addr_end1������β��ַ
* addr_end2������β��ַ
*/
struct AtStruct {
    DWORD AtUser;
    DWORD addr_end1;
    DWORD addr_end2;
};

/*
* ���ⲿ���õķ��Ͱ�����Ϣ�ӿ�
* lpParameter��SendAtTextStruct���ͽṹ��ָ��
* return��void
*/
#ifndef USE_SOCKET
void SendAtTextRemote(LPVOID lpParameter) {
    SendAtTextStruct* rp = (SendAtTextStruct*)lpParameter;
    wchar_t* wsChatRoomId = (WCHAR*)rp->chatroomid;
    wchar_t* wsTextMsg = (WCHAR*)rp->wxmsg;
    if (rp->length == 0)
        return;
    else if (rp->length == 1)
        SendAtText(wsChatRoomId, (DWORD*)&rp->wxidlist, wsTextMsg, rp->length, rp->AutoNickName);
    else
        SendAtText(wsChatRoomId, (DWORD*)rp->wxidlist, wsTextMsg, rp->length, rp->AutoNickName);
}
#endif

/*
* ���Ͱ�����Ϣ�ľ���ʵ��
* wsChatRoomId��Ⱥ��ID
* wsWxId�����ص����б�
* wsTextMsg�����͵���Ϣ����
* length�����ص�������
* AutoNickName���Ƿ��Զ���䱻�������ǳ�
* return��void
*/
void __stdcall SendAtText(wchar_t* wsChatRoomId, DWORD wsWxId[], wchar_t* wsTextMsg,int length,BOOL AutoNickName) {
    // +1�������ǲ���һ���սṹ�壬��`AtStruct`β��ַ�趨Ϊ�սṹ���׵�ַ����
    WxString* AtUsers = new WxString[length + 1];
    wstring AtMessage = L"";
    int querySuccess = 0;
    for (int i = 0; i < length; i++) {
        wchar_t* nickname = NULL;
        if (!lstrcmpW((wchar_t*)wsWxId[i], (wchar_t*)L"notify@all")) {
            nickname = (wchar_t*)L"������";
        }
        else
            nickname = GetUserNickNameByWxId((wchar_t*)wsWxId[i]);
        if (!nickname)
            continue;
        WxString temp = { 0 };
        temp.buffer = (wchar_t*)wsWxId[i];
        temp.length = wcslen((wchar_t*)wsWxId[i]);
        temp.maxLength = wcslen((wchar_t*)wsWxId[i]) * 2;
        memcpy(&AtUsers[querySuccess], &temp, sizeof(WxString));
        if (AutoNickName) {
            AtMessage = AtMessage + L"@" + nickname + L" ";
        }
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