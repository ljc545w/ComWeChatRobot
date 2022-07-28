#include "pch.h"

struct DelChatRoomMemberStruct
{
    DWORD chatroomid;
    DWORD wxids;
    DWORD length;
};

BOOL DelChatRoomMember(DWORD pid,wchar_t* chatroomid, wchar_t* wxid) {
    WeChatProcess hp(pid);
    if (!hp.m_init) return 1;
    DWORD DelChatRoomMemberRemoteAddr = hp.GetProcAddr(DelChatRoomMemberRemote);
    if (DelChatRoomMemberRemoteAddr == 0)
        return 1;
    WeChatData<wchar_t*> r_chatroomid(hp.GetHandle(), chatroomid, TEXTLENGTH(chatroomid));
    WeChatData<wchar_t*> r_wxid(hp.GetHandle(), wxid, TEXTLENGTH(wxid));
    DelChatRoomMemberStruct params = { 0 };
    params.chatroomid = (DWORD)r_chatroomid.GetAddr();
    params.wxids = (DWORD)r_wxid.GetAddr();
    params.length = 1;
    WeChatData<DelChatRoomMemberStruct*> r_params(hp.GetHandle(), &params, sizeof(params));
    if (r_chatroomid.GetAddr() == 0 || r_wxid.GetAddr() == 0 || r_params.GetAddr() == 0)
        return 1;
    DWORD ret = CallRemoteFunction(hp.GetHandle(), DelChatRoomMemberRemoteAddr, r_params.GetAddr());
    return ret == 0;
}

BOOL DelChatRoomMember(DWORD pid,wchar_t* chatroomid, SAFEARRAY* psaValue) {
    VARIANT rgvar;
    rgvar.vt = VT_BSTR;
    HRESULT hr = S_OK;
    long lLbound = psaValue->rgsabound->lLbound;
    long cElements = psaValue->rgsabound->cElements;
    if (cElements == 1) {
        VariantInit(&rgvar);
        long pIndex = 0;
        hr = SafeArrayGetElement(psaValue, &pIndex, &rgvar);
        return DelChatRoomMember(pid,chatroomid, rgvar.bstrVal);
    }
    DWORD dwWriteSize = 0;
    WeChatProcess hp(pid);
    if (!hp.m_init) return 1;
    DWORD DelChatRoomMemberRemoteAddr = hp.GetProcAddr(DelChatRoomMemberRemote);
    if (DelChatRoomMemberRemoteAddr == 0)
        return 1;
    WeChatData<wchar_t*> r_chatroomid(hp.GetHandle(), chatroomid, TEXTLENGTH(chatroomid));
    vector<void*> wxidptrs;
    for (long i = lLbound; i < lLbound + cElements; i++) {
        VariantInit(&rgvar);
        hr = SafeArrayGetElement(psaValue, &i, &rgvar);
        LPVOID wxidaddr = VirtualAllocEx(hp.GetHandle(), NULL, 1, MEM_COMMIT, PAGE_READWRITE);
        if (wxidaddr) {
            WriteProcessMemory(hp.GetHandle(), wxidaddr, rgvar.bstrVal, wcslen(rgvar.bstrVal) * 2 + 2, &dwWriteSize);
            wxidptrs.push_back(wxidaddr);
        }
    }
    WeChatData<void**> r_wxids(hp.GetHandle(), &wxidptrs[0], wxidptrs.size() * sizeof(void*));
    DelChatRoomMemberStruct params = { 0 };
    params.chatroomid = (DWORD)r_chatroomid.GetAddr();
    params.wxids = (DWORD)r_wxids.GetAddr();
    params.length = wxidptrs.size();
    WeChatData<DelChatRoomMemberStruct*> r_params(hp.GetHandle(), &params, sizeof(params));
    if (r_chatroomid.GetAddr() == 0 || r_wxids.GetAddr() == 0 || r_params.GetAddr() == 0)
        return 1;
    DWORD ret = CallRemoteFunction(hp.GetHandle(), DelChatRoomMemberRemoteAddr, r_params.GetAddr());
    for (unsigned int i = 0; i < wxidptrs.size(); i++) {
        VirtualFreeEx(hp.GetHandle(), wxidptrs[i], 0, MEM_RELEASE);
    }
    return ret == 0;
}