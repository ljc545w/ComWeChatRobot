#include "pch.h"

struct ChatRoomInfoStruct {
	DWORD members;
	DWORD length;
};

SAFEARRAY* GetChatRoomMembers(DWORD pid,wchar_t* chatroomid) {
    HRESULT hr = S_OK;
    WeChatProcess hp(pid);
    if (!hp.m_init) return NULL;
    DWORD GetChatRoomMembersRemoteAddr = hp.GetProcAddr(GetChatRoomMembersRemote);
    if (GetChatRoomMembersRemoteAddr == 0)
        return NULL;
    WeChatData<wchar_t*> r_chatroomid(hp.GetHandle(), chatroomid, TEXTLENGTH(chatroomid));
    if (r_chatroomid.GetAddr() == 0)
        return NULL;
    DWORD ret = CallRemoteFunction(hp.GetHandle(), GetChatRoomMembersRemoteAddr, r_chatroomid.GetAddr());
    
    if (ret == 0) {
        return NULL;
    }
    ChatRoomInfoStruct chatroominfo = { 0 };
    ReadProcessMemory(hp.GetHandle(),(LPCVOID)ret,&chatroominfo,sizeof(ChatRoomInfoStruct),0);
    wchar_t* members = new wchar_t[chatroominfo.length + 1];
    ZeroMemory(members, (chatroominfo.length + 1) * 2);
    ReadProcessMemory(hp.GetHandle(), (LPCVOID)chatroominfo.members, members, chatroominfo.length * 2, 0);
    SAFEARRAYBOUND rgsaBound[2] = { {2,0},{2,0} };
    SAFEARRAY* psaValue = SafeArrayCreate(VT_VARIANT, 2, rgsaBound);
    long keyIndex[2] = { 0,0 };
    keyIndex[0] = 0; keyIndex[1] = 0;
    hr = SafeArrayPutElement(psaValue, keyIndex, &(_variant_t)L"chatroomid");
    keyIndex[0] = 0; keyIndex[1] = 1;
    hr = SafeArrayPutElement(psaValue, keyIndex, &(_variant_t)chatroomid);
    keyIndex[0] = 1; keyIndex[1] = 0;
    hr = SafeArrayPutElement(psaValue, keyIndex, &(_variant_t)L"members");
    keyIndex[0] = 1; keyIndex[1] = 1;
    hr = SafeArrayPutElement(psaValue, keyIndex, &(_variant_t)members);
    delete[] members;
    members = NULL;
    return psaValue;
}