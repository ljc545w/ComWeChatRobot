#include "pch.h"

struct ChatRoomAnnouncementStruct
{
    DWORD chatroomid;
    DWORD announcement;
};

BOOL SetChatRoomAnnouncement(DWORD pid,wchar_t* chatroomid, wchar_t* announcement) {
    WeChatProcess hp(pid);
    if (!hp.m_init) return 1;
    DWORD SetChatRoomAnnouncementRemoteAddr = hp.GetProcAddr(SetChatRoomAnnouncementRemote);
    if (SetChatRoomAnnouncementRemoteAddr == 0)
        return 1;
    WeChatData<wchar_t*> r_chatroomid(hp.GetHandle(), chatroomid, TEXTLENGTH(chatroomid));
    WeChatData<wchar_t*> r_announcement(hp.GetHandle(), announcement, TEXTLENGTH(announcement));
    ChatRoomAnnouncementStruct params = { 0 };
    params.chatroomid = (DWORD)r_chatroomid.GetAddr();
    params.announcement = (DWORD)r_announcement.GetAddr();
    WeChatData<ChatRoomAnnouncementStruct*> r_params(hp.GetHandle(), &params, sizeof(params));
    if (r_chatroomid.GetAddr() == 0 || r_params.GetAddr() == 0)
        return 1;
    DWORD ret = CallRemoteFunction(hp.GetHandle(), SetChatRoomAnnouncementRemoteAddr, r_params.GetAddr());
    return ret == 0;
}