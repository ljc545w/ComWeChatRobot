#include "pch.h"

struct FileParamStruct {
    DWORD wxid;
    DWORD filepath;
};

int SendFile(DWORD pid,wchar_t* wxid, wchar_t* filepath) {
    WeChatProcess hp(pid);
    if (!hp.m_init) return 1;
    DWORD SendFileRemoteAddr = hp.GetProcAddr(SendFileRemote);
    if (SendFileRemoteAddr == 0) {
        return 1;
    }
    FileParamStruct params = { 0 };
    WeChatData<wchar_t*> r_wxid(hp.GetHandle(), wxid, TEXTLENGTH(wxid));
    WeChatData<wchar_t*> r_filepath(hp.GetHandle(), filepath, TEXTLENGTH(filepath));

    params.wxid = (DWORD)r_wxid.GetAddr();
    params.filepath = (DWORD)r_filepath.GetAddr();
    WeChatData<FileParamStruct*> r_params(hp.GetHandle(), &params, sizeof(params));
    if (!params.wxid || !params.filepath || !r_params.GetAddr()) {
        return 1;
    }
    DWORD dwRet = CallRemoteFunction(hp.GetHandle(), SendFileRemoteAddr, r_params.GetAddr());
    return 0;
}
