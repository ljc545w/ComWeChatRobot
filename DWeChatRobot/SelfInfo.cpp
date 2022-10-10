#include "pch.h"
#include "json/json.hpp"
using namespace nlohmann;

// 个人WXID偏移
#define SelfWxidAddrOffset 0x236607C

/*
 * 外部调用时的返回类型
 * message：selfinfo.c_str()
 * length：selfinfo字符串长度
 */
#ifndef USE_SOCKET
struct SelfInfoStruct
{
    DWORD message;
    DWORD length;
} ret;
#endif // !USE_SOCKET

/*
 * 供外部调用的获取个人信息接口
 * return：DWORD，ret的首地址
 */
#ifndef USE_SOCKET
DWORD GetSelfInfoRemote()
{
    ZeroMemory(&ret, sizeof(SelfInfoStruct));
    wstring selfinfo = GetSelfInfo();
    wchar_t *message = new wchar_t[selfinfo.length() + 1];
    memcpy(message, selfinfo.c_str(), (selfinfo.length() + 1) * 2);
    ret.message = (DWORD)message;
    ret.length = selfinfo.length();
    return (DWORD)&ret;
}
#endif

wstring GetSelfWxid()
{
    DWORD addr = GetWeChatWinBase() + SelfWxidAddrOffset;
    string wxid;
    if (*(DWORD *)(addr + 0x14) == 0xF)
    {
        wxid = string((char *)addr, *(int *)(addr + 0x10));
    }
    else
    {
        wxid = string((char *)(*(DWORD *)addr), *(int *)(addr + 0x10));
    }
    return utf8_to_unicode(wxid.c_str());
}

/*
 * 获取个人信息
 */
wstring GetSelfInfo()
{
    if (!isWxLogin())
        return L"请先登录微信.";
    json jData;
    map<string, DWORD> self_info_addr;
    DWORD WeChatWinBase = GetWeChatWinBase();
    self_info_addr["wxId"] = WeChatWinBase + 0x236607C;
    self_info_addr["wxNumber"] = WeChatWinBase + 0x2366548;
    self_info_addr["wxNickName"] = WeChatWinBase + 0x23660F4;
    self_info_addr["Sex"] = WeChatWinBase + 0x23661F8;
    self_info_addr["wxSignature"] = WeChatWinBase + 0x236622C;
    self_info_addr["wxBigAvatar"] = WeChatWinBase + 0x23A111C;
    self_info_addr["wxSmallAvatar"] = WeChatWinBase + 0x23663D4;
    self_info_addr["wxNation"] = WeChatWinBase + 0x23662E8;
    self_info_addr["wxProvince"] = WeChatWinBase + 0x23661FC;
    self_info_addr["wxCity"] = WeChatWinBase + 0x2366214;
    self_info_addr["PhoneNumber"] = WeChatWinBase + 0x2366128;
    self_info_addr["wxFilePath"] = *(DWORD *)(WeChatWinBase + 0x2385020);
    self_info_addr["uin"] = WeChatWinBase + 0x23661C8;
    for (auto it = self_info_addr.begin(); it != self_info_addr.end(); it++)
    {
        string key = it->first;
        DWORD addr = it->second;
        string utf8_str;
        if (key == "Sex")
        {
            int sex = *(int *)addr;
            utf8_str = gb2312_to_utf8(((sex == 1) ? "男" : ((sex == 2) ? "女" : "未知")));
        }
        else if (key == "wxFilePath")
        {
            utf8_str = unicode_to_utf8((wchar_t *)addr);
        }
        else if (key == "uin")
        {
            jData[key] = *(unsigned int *)addr;
            continue;
        }
        else
        {
            if (*(DWORD *)addr == 0)
            {
                utf8_str = "";
            }
            else if (*(DWORD *)(addr + 0x14) == 0xF)
            {
                utf8_str = string((char *)addr, *(int *)(addr + 0x10));
            }
            else
            {
                utf8_str = string((char *)(*(DWORD *)addr), *(int *)(addr + 0x10));
            }
        }
        jData[key] = utf8_str.c_str();
    }
    wstring selfinfo = utf8_to_unicode(jData.dump().c_str());
    return selfinfo;
}

/*
 * 删除个人信息缓存
 * return：void
 */
#ifndef USE_SOCKET
VOID DeleteSelfInfoCacheRemote()
{
    if (ret.length)
    {
        delete[](wchar_t *) ret.message;
        ZeroMemory(&ret, sizeof(SelfInfoStruct));
    }
}
#endif
