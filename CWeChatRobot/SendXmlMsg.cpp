#include "pch.h"

struct SendXmlMsgStruct
{
    DWORD wxid;
    DWORD xml;
    DWORD imgpath;
};

BOOL SendXmlMsg(DWORD pid, wchar_t *wxid, wchar_t *xml, wchar_t *imgpath)
{
    WeChatProcess hp(pid);
    if (!hp.m_init)
        return 1;
    DWORD SendXmlMsgRemoteAddr = hp.GetProcAddr(SendXmlMsgRemote);
    if (SendXmlMsgRemoteAddr == 0)
    {
        return 1;
    }
    SendXmlMsgStruct params = {0};
    WeChatData<wchar_t *> r_wxid(hp.GetHandle(), wxid, TEXTLENGTH(wxid));
    WeChatData<wchar_t *> r_xml(hp.GetHandle(), xml, TEXTLENGTH(xml));
    WeChatData<wchar_t *> r_imgpath(hp.GetHandle(), imgpath, TEXTLENGTH(imgpath));
    params.wxid = (DWORD)r_wxid.GetAddr();
    params.xml = (DWORD)r_xml.GetAddr();
    params.imgpath = (DWORD)r_imgpath.GetAddr();
    WeChatData<SendXmlMsgStruct *> r_params(hp.GetHandle(), &params, sizeof(params));
    if (!r_wxid.GetAddr() || !r_xml.GetAddr() || !r_params.GetAddr())
    {
        return 1;
    }
    DWORD dwRet = CallRemoteFunction(hp.GetHandle(), SendXmlMsgRemoteAddr, r_params.GetAddr());
    return dwRet != 1;
}
