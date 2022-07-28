#include "pch.h"

struct AddFriendByWxidStruct {
    DWORD wxid;
    DWORD message;
};

BOOL AddFriendByWxid(DWORD pid,wchar_t* wxid,wchar_t* message) {
    WeChatProcess hp(pid);
    if (!hp.m_init) return 1;
    DWORD AddFriendByWxidRemoteAddr = hp.GetProcAddr(AddFriendByWxidRemote);
    if (AddFriendByWxidRemoteAddr == 0)
        return 1;
    WeChatData<wchar_t*> r_wxid(hp.GetHandle(), wxid, TEXTLENGTH(wxid));
    WeChatData<wchar_t*> r_message(hp.GetHandle(), message, TEXTLENGTH(message));
    AddFriendByWxidStruct params = { 0 };
    params.wxid = (DWORD)r_wxid.GetAddr();
    params.message = (DWORD)r_message.GetAddr();
    WeChatData<AddFriendByWxidStruct*> r_params(hp.GetHandle(), &params, sizeof(params));
    if (r_wxid.GetAddr() == 0 || r_params.GetAddr() == 0)
        return 1;
    DWORD ret = CallRemoteFunction(hp.GetHandle(), AddFriendByWxidRemoteAddr, r_params.GetAddr());
    return ret == 0;
}