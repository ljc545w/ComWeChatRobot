#include "pch.h"
#include "json/json.hpp"
#include <map>
using namespace nlohmann;

#define CheckLoginOffset 0x2366538
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
    DWORD baseAddr = GetWeChatWinBase() + SelfWxidAddrOffset;
    char wxidbuffer[0x100] = {0};
    DWORD SelfWxIdAddr = 0x0;
    sprintf_s(wxidbuffer, "%s", (char *)baseAddr);
    if (strlen(wxidbuffer) < 0x6 || strlen(wxidbuffer) > 0x14)
    {
        SelfWxIdAddr = *(DWORD *)baseAddr;
    }
    else
    {
        SelfWxIdAddr = baseAddr;
    }
    if (SelfWxIdAddr == 0)
    {
        return L"";
    }
    char *sselfwxid = (char *)SelfWxIdAddr;
    wchar_t *wselfwxid = new wchar_t[strlen(sselfwxid) + 1];
    MultiByteToWideChar(CP_ACP, 0, sselfwxid, -1, wselfwxid, strlen(sselfwxid) + 1);
    wstring wxid(wselfwxid);
    delete[] wselfwxid;
    return wxid;
}

/*
* 获取个人信息
*/
wstring GetSelfInfo()
{
    json jData;
    map<string, DWORD> self_info_addr;
    DWORD WeChatWinBase = GetWeChatWinBase();
    self_info_addr["wxId"] = WeChatWinBase + 0x236607C;
    self_info_addr["wxNumber"] = WeChatWinBase + 0x2366548;
    self_info_addr["wxNickName"] = WeChatWinBase + 0x23660F4;
    self_info_addr["Sex"] = WeChatWinBase + 0x23661F8;
    self_info_addr["wxSignature"] = *(DWORD *)(WeChatWinBase + 0x236622C);
    self_info_addr["wxBigAvatar"] = *(DWORD *)(WeChatWinBase + 0x23A111C);
    self_info_addr["wxSmallAvatar"] = *(DWORD *)(WeChatWinBase + 0x23663D4);
    self_info_addr["wxNation"] = WeChatWinBase + 0x23662E8;
    self_info_addr["wxProvince"] = WeChatWinBase + 0x23661FC;
    self_info_addr["wxCity"] = WeChatWinBase + 0x2366214;
    self_info_addr["PhoneNumber"] = WeChatWinBase + 0x2366128;
    for (auto it = self_info_addr.begin(); it != self_info_addr.end(); it++)
    {
        string key = it->first;
        DWORD addr = it->second;
        string temp;
        if (!key.compare("wxNickName"))
        {
            if (*(DWORD *)(addr + 0x14) == 0xF)
            {
                temp = (*((DWORD *)addr) != 0) ? string((char *)addr) : gb2312_to_utf8("null");
            }
            else
            {
                temp = (*((DWORD *)addr) != 0) ? string((char *)(*(DWORD *)addr)) : gb2312_to_utf8("null");
            }
        }
        else if (!key.compare("wxId"))
        {
            char wxidbuffer[0x100] = {0};
            sprintf_s(wxidbuffer, "%s", (char *)addr);
            if (strlen(wxidbuffer) < 0x6 || strlen(wxidbuffer) > 0x14)
            {
                //新的微信号 微信ID用地址保存
                temp = string((char *)(*(DWORD *)addr));
            }
            else
            {
                temp = string((char *)addr);
            }
        }
        else if (!key.compare("Sex"))
        {
            int sex = *(int *)addr;
            switch (sex)
            {
            case 1:
            {
                temp = gb2312_to_utf8("男");
                break;
            }
            case 2:
            {
                temp = gb2312_to_utf8("女");
                break;
            }
            default:
            {
                temp = gb2312_to_utf8("未知");
                break;
            }
            }
        }
        else
        {
            temp = addr != 0 ? string((char *)addr) : gb2312_to_utf8("null");
        }
        jData[key] = temp.c_str();
    }
    wstring selfinfo = utf8_to_unicode(jData.dump().c_str());
    return selfinfo;
}

BOOL isWxLogin()
{
    DWORD CheckLoginAddr = GetWeChatWinBase() + CheckLoginOffset;
    return *(BOOL *)CheckLoginAddr;
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
