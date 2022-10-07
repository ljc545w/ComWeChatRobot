#include "pch.h"

// 发送文章CALL1偏移
#define SendXmlCall1Offset 0x78758A70 - 0x786A0000
// 发送文章CALL2偏移
#define SendXmlCall2Offset 0x78A8D5E0 - 0x786A0000
// 发送文章CALL3偏移
#define SendXmlCall3Offset 0x787A7F00 - 0x786A0000
// 发送文章CALL4偏移
#define SendXmlCall4Offset 0x78A8D7B0 - 0x786A0000
// 发送文章CALL参数偏移
#define SendXmlParamOffset 0x7AA26FE4 - 0x786A0000

// 清空缓存CALL1偏移
#define SendXmlClearCacheCall1Offset 0x78D46450 - 0x786A0000
// 清空缓存CALL2偏移
#define SendXmlClearCacheCall2Offset 0x78757780 - 0x786A0000

/*
 * 供外部调用的发送文章消息接口
 * lparameter：SendXmlStruct类型结构体指针
 * return：void
 */
#ifndef USE_SOCKET
struct SendXmlStruct
{
    DWORD wxid;
    DWORD xml;
    DWORD imgpath;
};

VOID SendXmlMsgRemote(LPVOID lparameter)
{
    SendXmlStruct *sxs = (SendXmlStruct *)lparameter;
    wchar_t *wxid = (wchar_t *)sxs->wxid;
    wchar_t *xml = (wchar_t *)sxs->xml;
    wchar_t *imgpath = (wchar_t *)sxs->imgpath;
    SendXmlMsg(wxid, xml, imgpath);
}
#endif

BOOL __stdcall SendXmlMsg(wchar_t *wxid, wchar_t *xml, wchar_t *imgpath)
{
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD SendXmlCall1 = WeChatWinBase + SendXmlCall1Offset;
    DWORD SendXmlCall2 = WeChatWinBase + SendXmlCall2Offset;
    DWORD SendXmlCall3 = WeChatWinBase + SendXmlCall3Offset;
    DWORD SendXmlCall4 = WeChatWinBase + SendXmlCall4Offset;
    DWORD SendXmlParam = WeChatWinBase + SendXmlParamOffset;

    DWORD SendXmlClearCacheCall1 = WeChatWinBase + SendXmlClearCacheCall1Offset;
    DWORD SendXmlClearCacheCall2 = WeChatWinBase + SendXmlClearCacheCall2Offset;
    // 自己的wxid，发送者
    wstring wselfwxid = GetSelfWxid();

    DWORD sendtype = 0x5;
    WxString pSender((wchar_t *)wselfwxid.c_str());
    char nullbuffer[0x1C] = {0};
    WxString pXml(xml);
    WxString pReceiver(wxid);
    WxString imgbuffer = {0};
    if (imgpath && wcslen(imgpath) != 0)
    {
        imgbuffer.buffer = imgpath;
        imgbuffer.length = wcslen(imgpath);
        imgbuffer.maxLength = wcslen(imgpath) * 2;
    }
    WxString nullStruct = {0};
    char buffer[0xFF0] = {0};
    int isSuccess = 0x0;
    __asm {
		pushad;
		pushfd;
		lea ecx, buffer;
		call SendXmlCall1;
		mov eax, [sendtype];
		push eax;
		lea eax, nullbuffer;
		lea edx, pSender;
		push eax;
		lea eax, imgbuffer;
		push eax;
		lea eax, pXml;
		push eax;
		lea edi, pReceiver;
		push edi;
		lea ecx, buffer;
		call SendXmlCall2;
		add esp, 0x14;
		lea eax, nullStruct;
		push eax;
		lea ecx, buffer;
		call SendXmlCall3;
		mov dl, 0x0;
		lea ecx, buffer;
		push SendXmlParam;
		push SendXmlParam;
		call SendXmlCall4;
		mov isSuccess, eax;
		add esp, 0x8;
		lea ecx, buffer;
		call SendXmlClearCacheCall1;
		lea ecx, buffer;
		call SendXmlClearCacheCall2;
		popfd;
		popad;
    }
    return (isSuccess == 0x1);
}
