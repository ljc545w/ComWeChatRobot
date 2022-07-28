#include "pch.h"

struct ChatRoomNameStruct
{
    DWORD chatroomid;
    DWORD name;
};

BOOL SetChatRoomName(DWORD pid,wchar_t* chatroomid, wchar_t* name) {
    WeChatProcess hp(pid);
    if (!hp.m_init) return 1;
    DWORD SetChatRoomNameRemoteAddr = hp.GetProcAddr(SetChatRoomNameRemote);
    if (SetChatRoomNameRemoteAddr == 0)
        return 1;
    WeChatData<wchar_t*> r_chatroomid(hp.GetHandle(), chatroomid, TEXTLENGTH(chatroomid));
    WeChatData<wchar_t*> r_name(hp.GetHandle(), name, TEXTLENGTH(name));
    ChatRoomNameStruct params = { 0 };
    params.chatroomid = (DWORD)r_chatroomid.GetAddr();
    params.name = (DWORD)r_name.GetAddr();
    WeChatData<ChatRoomNameStruct*> r_params(hp.GetHandle(), &params, sizeof(params));
    if (r_chatroomid.GetAddr() == 0 || r_params.GetAddr() == 0)
        return 1;
    DWORD ret = CallRemoteFunction(hp.GetHandle(), SetChatRoomNameRemoteAddr, r_params.GetAddr());
    return ret == 0;
}