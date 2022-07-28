#include "pch.h"

struct EditRemarkStruct {
    DWORD wxid;
    DWORD remark;
};

BOOL EditRemark(DWORD pid,wchar_t* wxid, wchar_t* remark) {
    WeChatProcess hp(pid);
    if (!hp.m_init) return 1;
    DWORD EditRemarkRemoteAddr = hp.GetProcAddr(EditRemarkRemote);
    if (EditRemarkRemoteAddr == 0)
        return 1;
    WeChatData<wchar_t*> r_wxid(hp.GetHandle(), wxid, TEXTLENGTH(wxid));
    WeChatData<wchar_t*> r_remark(hp.GetHandle(), remark, TEXTLENGTH(remark));
    EditRemarkStruct params = { 0 };
    params.wxid = (DWORD)r_wxid.GetAddr();
    params.remark = (DWORD)r_remark.GetAddr();
    WeChatData<EditRemarkStruct*> r_params(hp.GetHandle(), &params, sizeof(params));
    if (r_wxid.GetAddr() == 0 || r_params.GetAddr() == 0)
        return 1;
    DWORD ret = CallRemoteFunction(hp.GetHandle(), EditRemarkRemoteAddr, r_params.GetAddr());
    return ret == 0;
}