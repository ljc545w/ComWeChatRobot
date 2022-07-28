#include "pch.h"

struct VerifyFriendApplyStruct {
	DWORD v3;
	DWORD v4;
};

BOOL VerifyFriendApply(DWORD pid,wchar_t* v3,wchar_t* v4) {
    WeChatProcess hp(pid);
    if (!hp.m_init) return 1;
    DWORD VerifyFriendApplyRemoteAddr = hp.GetProcAddr(VerifyFriendApplyRemote);
    if (VerifyFriendApplyRemoteAddr == 0)
        return 1;
    WeChatData<wchar_t*> r_v3(hp.GetHandle(), v3, TEXTLENGTH(v3));
    WeChatData<wchar_t*> r_v4(hp.GetHandle(), v4, TEXTLENGTH(v4));
    VerifyFriendApplyStruct params = { 0 };
    params.v3 = (DWORD)r_v3.GetAddr();
    params.v4 = (DWORD)r_v4.GetAddr();
    WeChatData<VerifyFriendApplyStruct*> r_params(hp.GetHandle(), &params, sizeof(params));
    if (r_v3.GetAddr() == 0 || r_v4.GetAddr() == 0 || r_params.GetAddr() == 0)
        return 1;
    DWORD ret = CallRemoteFunction(hp.GetHandle(), VerifyFriendApplyRemoteAddr, r_params.GetAddr());
    return ret == 0;
}