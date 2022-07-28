#include "pch.h"

struct ChatRoomMemberNicknameStruct
{
    DWORD chatroomid;
    DWORD wxid;
    DWORD nickname;
};

wstring GetChatRoomMemberNickname(DWORD pid,wchar_t* chatroomid, wchar_t* wxid) {
    wchar_t buffer[33] = { 0 };
    WeChatProcess hp(pid);
    if (!hp.m_init) return L"";
    DWORD GetChatRoomMemberNicknameRemoteAddr = hp.GetProcAddr(GetChatRoomMemberNicknameRemote);
    if (GetChatRoomMemberNicknameRemoteAddr == 0)
        return L"";
    WeChatData<wchar_t*> r_chatroomid(hp.GetHandle(), chatroomid, TEXTLENGTH(chatroomid));
    WeChatData<wchar_t*> r_wxid(hp.GetHandle(), wxid, TEXTLENGTH(wxid));
    WeChatData<wchar_t*> r_nickname(hp.GetHandle(), buffer, 33 * 2);
    ChatRoomMemberNicknameStruct params = { 0 };
    params.chatroomid = (DWORD)r_chatroomid.GetAddr();
    params.wxid = (DWORD)r_wxid.GetAddr();
    params.nickname = (DWORD)r_nickname.GetAddr();
    WeChatData<ChatRoomMemberNicknameStruct*> r_params(hp.GetHandle(), &params, sizeof(params));
    if (r_chatroomid.GetAddr() == 0 || r_wxid.GetAddr() == 0 || r_params.GetAddr() == 0 || r_nickname.GetAddr() == 0)
        return L"";
    DWORD ret = CallRemoteFunction(hp.GetHandle(), GetChatRoomMemberNicknameRemoteAddr, r_params.GetAddr());
    ReadProcessMemory(hp.GetHandle(), r_nickname.GetAddr(), buffer, 32 * 2, 0);
    wstring nickname(buffer);
    return nickname;
}