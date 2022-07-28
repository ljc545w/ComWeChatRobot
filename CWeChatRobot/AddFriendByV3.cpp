#include "pch.h"

struct AddFriendByV3Struct {
    DWORD v3;
    DWORD message;
    DWORD AddType;
};

BOOL AddFriendByV3(DWORD pid,wchar_t* v3, wchar_t* message,int AddType) {
    WeChatProcess hp(pid);
    if (!hp.m_init) return 1;
    DWORD AddFriendByV3RemoteAddr = hp.GetProcAddr(AddFriendByV3Remote);
    if (AddFriendByV3RemoteAddr == 0)
        return 1;
    WeChatData<wchar_t*> r_v3(hp.GetHandle(), v3, TEXTLENGTH(v3));
    WeChatData<wchar_t*> r_message(hp.GetHandle(), message, TEXTLENGTH(message));
    AddFriendByV3Struct params = { 0 };
    params.v3 = (DWORD)r_v3.GetAddr();
    params.message = (DWORD)r_message.GetAddr();
    params.AddType = AddType;
    WeChatData<AddFriendByV3Struct*> r_params(hp.GetHandle(), &params, sizeof(params));
    if (r_v3.GetAddr() == 0 || r_params.GetAddr() == 0)
        return 1;
    DWORD ret = CallRemoteFunction(hp.GetHandle(), AddFriendByV3RemoteAddr, r_params.GetAddr());
    return ret == 0;
}