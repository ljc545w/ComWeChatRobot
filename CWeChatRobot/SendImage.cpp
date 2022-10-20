#include "pch.h"

struct ImageParamStruct
{
    DWORD wxid;
    DWORD imagepath;
};

int SendImage(DWORD pid, wchar_t *wxid, wchar_t *imagepath)
{
    WeChatProcess hp(pid);
    if (!hp.m_init)
        return 1;
    DWORD SendImageRemoteAddr = hp.GetProcAddr(SendImageRemote);
    if (SendImageRemoteAddr == 0)
    {
        return 1;
    }
    ImageParamStruct params = {0};
    WeChatData<wchar_t *> r_wxid(hp.GetHandle(), wxid, TEXTLENGTH(wxid));
    WeChatData<wchar_t *> r_imagepath(hp.GetHandle(), imagepath, TEXTLENGTH(imagepath));

    params.wxid = (DWORD)r_wxid.GetAddr();
    params.imagepath = (DWORD)r_imagepath.GetAddr();
    WeChatData<ImageParamStruct *> r_params(hp.GetHandle(), &params, sizeof(params));
    if (!params.wxid || !params.imagepath || !r_params.GetAddr())
    {
        return 1;
    }
    DWORD dwRet = CallRemoteFunction(hp.GetHandle(), SendImageRemoteAddr, r_params.GetAddr());
    return (dwRet != 1);
}
