#include "pch.h"

struct ChatRoomSelfNicknameStruct
{
    DWORD chatroomid;
    DWORD nickname;
};

BOOL SetChatRoomSelfNickname(DWORD pid,wchar_t* chatroomid, wchar_t* nickname) {
    WeChatProcess hp(pid);
    if (!hp.m_init) return 1;
    DWORD SetChatRoomSelfNicknameRemoteAddr = hp.GetProcAddr(SetChatRoomSelfNicknameRemote);
    if (SetChatRoomSelfNicknameRemoteAddr == 0)
        return 1;
    WeChatData<wchar_t*> r_chatroomid(hp.GetHandle(), chatroomid, TEXTLENGTH(chatroomid));
    WeChatData<wchar_t*> r_nickname(hp.GetHandle(), nickname, TEXTLENGTH(nickname));
    ChatRoomSelfNicknameStruct params = { 0 };
    params.chatroomid = (DWORD)r_chatroomid.GetAddr();
    params.nickname = (DWORD)r_nickname.GetAddr();
    WeChatData<ChatRoomSelfNicknameStruct*> r_params(hp.GetHandle(), &params, sizeof(params));
    if (r_chatroomid.GetAddr() == 0 || r_params.GetAddr() == 0)
        return 1;
    DWORD ret = CallRemoteFunction(hp.GetHandle(), SetChatRoomSelfNicknameRemoteAddr, r_params.GetAddr());
    return ret == 0;
}