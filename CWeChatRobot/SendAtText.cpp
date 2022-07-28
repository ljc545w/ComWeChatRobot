#include "pch.h"

struct SendAtTextStruct
{
    DWORD chatroomid;
    DWORD wxid;
    DWORD wxmsg;
    DWORD length;
    DWORD AutoNickName;
};

int SendAtText(DWORD pid,wchar_t* chatroomid, wchar_t* wxid, wchar_t* wxmsg,BOOL AutoNickName) {
    WeChatProcess hp(pid);
    if (!hp.m_init) return 1;
    DWORD SendAtTextRemoteAddr = hp.GetProcAddr(SendAtTextRemote);
    if (SendAtTextRemoteAddr == 0) {
        return 1;
    }
    SendAtTextStruct params = { 0 };
    WeChatData<wchar_t*> r_wxid(hp.GetHandle(), wxid, TEXTLENGTH(wxid));
    WeChatData<wchar_t*> r_chatroomid(hp.GetHandle(), chatroomid, TEXTLENGTH(chatroomid));
    WeChatData<wchar_t*> r_wxmsg(hp.GetHandle(), wxmsg, TEXTLENGTH(wxmsg));
    params.wxid = (DWORD)r_wxid.GetAddr();
    params.wxmsg = (DWORD)r_wxmsg.GetAddr();
    params.chatroomid = (DWORD)r_chatroomid.GetAddr();
    params.AutoNickName = AutoNickName;
    params.length = 1;
    WeChatData<SendAtTextStruct*> r_params(hp.GetHandle(), &params, sizeof(params));
    if (!params.wxid || !params.wxmsg || !r_params.GetAddr()) {
        return 1;
    }
    DWORD dwRet = CallRemoteFunction(hp.GetHandle(), SendAtTextRemoteAddr, r_params.GetAddr());
    return 0;
}

BOOL SendAtText(DWORD pid,wchar_t* chatroomid, SAFEARRAY* psaValue, wchar_t* wxmsg,BOOL AutoNickName) {
    VARIANT rgvar;
    rgvar.vt = VT_BSTR;
    HRESULT hr = S_OK;
    long lLbound = psaValue->rgsabound->lLbound;
    long cElements = psaValue->rgsabound->cElements;
    if (cElements == 1) {
        VariantInit(&rgvar);
        long pIndex = 0;
        hr = SafeArrayGetElement(psaValue, &pIndex, &rgvar);
        return SendAtText(pid,chatroomid, rgvar.bstrVal, wxmsg,AutoNickName);
    }
    WeChatProcess hp(pid);
    if (!hp.m_init) return 1;
    DWORD SendAtTextRemoteAddr = hp.GetProcAddr(SendAtTextRemote);
    if (SendAtTextRemoteAddr == 0) {
        return 1;
    }
    vector<void*> wxidptrs;
    SendAtTextStruct params = { 0 };
    WeChatData<wchar_t*> r_chatroomid(hp.GetHandle(), chatroomid, TEXTLENGTH(chatroomid));
    WeChatData<wchar_t*> r_wxmsg(hp.GetHandle(), wxmsg, TEXTLENGTH(wxmsg));
    params.wxmsg = (DWORD)r_wxmsg.GetAddr();
    params.chatroomid = (DWORD)r_chatroomid.GetAddr();
    params.AutoNickName = AutoNickName;
    for (long i = lLbound; i < lLbound + cElements; i++) {
        VariantInit(&rgvar);
        hr = SafeArrayGetElement(psaValue, &i, &rgvar);
        LPVOID wxidaddr = WriteWeChatMemory(hp.GetHandle(), rgvar.bstrVal, TEXTLENGTH(rgvar.bstrVal));
        if (wxidaddr) {
            wxidptrs.push_back(wxidaddr);
        }
    }
    WeChatData<void**> r_wxids(hp.GetHandle(), &wxidptrs[0], wxidptrs.size() * sizeof(void*));
    params.wxid = (DWORD)r_wxids.GetAddr();
    params.length = wxidptrs.size();
    WeChatData<SendAtTextStruct*> r_params(hp.GetHandle(), &params, sizeof(params));
    if (!params.chatroomid || !params.wxid || !params.wxmsg || !r_params.GetAddr()) {
        return 1;
    }
    DWORD dwRet = CallRemoteFunction(hp.GetHandle(), SendAtTextRemoteAddr, r_params.GetAddr());
    for (unsigned int i = 0; i < wxidptrs.size(); i++) {
        VirtualFreeEx(hp.GetHandle(), wxidptrs[i], 0, MEM_RELEASE);
    }
    return 0;
}