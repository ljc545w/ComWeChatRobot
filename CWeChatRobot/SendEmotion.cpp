#include "pch.h"

struct SendEmotionStruct
{
    DWORD wxid;
    DWORD img_path;
};

int SendEmotion(DWORD pid, wchar_t *wxid, wchar_t *img_path)
{
    WeChatProcess hp(pid);
    if (!hp.m_init)
        return 1;
    DWORD SendEmitonRemoteAddr = hp.GetProcAddr(SendEmotionRemote);
    if (SendEmitonRemoteAddr == 0)
    {
        return 1;
    }
    SendEmotionStruct params = {0};
    WeChatData<wchar_t *> r_wxid(hp.GetHandle(), wxid, TEXTLENGTH(wxid));
    WeChatData<wchar_t *> r_img_path(hp.GetHandle(), img_path, TEXTLENGTH(img_path));

    params.wxid = (DWORD)r_wxid.GetAddr();
    params.img_path = (DWORD)r_img_path.GetAddr();
    WeChatData<SendEmotionStruct *> r_params(hp.GetHandle(), &params, sizeof(params));
    if (!params.wxid || !params.img_path || !r_params.GetAddr())
    {
        return 1;
    }
    DWORD dwRet = CallRemoteFunction(hp.GetHandle(), SendEmitonRemoteAddr, r_params.GetAddr());
    return (dwRet != 1);
}
