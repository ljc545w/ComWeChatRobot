#include "pch.h"

struct GetPublicMsgStruct
{
    DWORD public_id;
    DWORD offset;
};

struct GetPublicMsgRespStruct
{
    DWORD message;
    DWORD length;
};

wstring GetHistoryPublicMsg(DWORD pid, wchar_t *public_id, wchar_t *offset)
{
    WeChatProcess hp(pid);
    if (!hp.m_init)
        return L"";
    DWORD GetHistoryPublicMsgReomteAddr = hp.GetProcAddr(GetHistoryPublicMsgRemote);
    if (GetHistoryPublicMsgReomteAddr == 0)
    {
        return L"";
    }
    GetPublicMsgStruct params = {0};
    WeChatData<wchar_t *> r_public_id(hp.GetHandle(), public_id, TEXTLENGTH(public_id));
    WeChatData<wchar_t *> r_offset(hp.GetHandle(), offset, TEXTLENGTH(offset));

    params.public_id = (DWORD)r_public_id.GetAddr();
    params.offset = (DWORD)r_offset.GetAddr();
    WeChatData<GetPublicMsgStruct *> r_params(hp.GetHandle(), &params, sizeof(params));
    if (!params.public_id || !params.offset || !r_params.GetAddr())
    {
        return L"";
    }
    DWORD dwRet = CallRemoteFunction(hp.GetHandle(), GetHistoryPublicMsgReomteAddr, r_params.GetAddr());
    DWORD dwReadSize;
    GetPublicMsgRespStruct ret_info = {0};
    ReadProcessMemory(hp.GetHandle(), (LPCVOID)dwRet, &ret_info, sizeof(ret_info), &dwReadSize);
    if (ret_info.message == 0)
        return L"";
    char *buffer = new char[ret_info.length + 1];
    ReadProcessMemory(hp.GetHandle(), (LPCVOID)ret_info.message, buffer, ret_info.length, &dwReadSize);
    string result(buffer, ret_info.length);
    wstring wresult = gb2312_to_unicode(result.c_str());
    delete[] buffer;
    return wresult;
}
