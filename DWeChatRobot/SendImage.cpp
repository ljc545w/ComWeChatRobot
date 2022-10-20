#include "pch.h"

// 发送图片CALL1偏移
#define SendImageCall1Offset (0x7875D780 - 0x786A0000)
// 发送图片CALL2偏移
#define SendImageCall2Offset (0x78E11980 - 0x786A0000)
// 发送图片CALL3偏移
#define SendImageCall3Offset (0x78BC1640 - 0x786A0000)
// 清空缓存的CALL偏移
#define DeleteSendImageCacheCallOffset (0x78757780 - 0x786A0000)

/*
 * 外部调用时传递的参数结构
 * wxid：保存wxid的地址
 * imagepath：保存图片绝对路径的地址
 */
#ifndef USE_SOCKET
struct ImageParamStruct
{
    DWORD wxid;
    DWORD imagepath;
};
#endif

/*
 * 供外部调用的发送图片消息接口
 * lpParamStruct：ImageParamStruct类型结构体指针
 * return：void
 */
#ifndef USE_SOCKET
BOOL SendImageRemote(LPVOID lpParamStruct)
{
    ImageParamStruct *params = (ImageParamStruct *)lpParamStruct;
    return SendImage((WCHAR *)params->wxid, (WCHAR *)params->imagepath);
}
#endif

/*
 * 发送图片消息的具体实现
 * receiver：接收人wxid
 * ImagePath：图片绝对路径
 * return：BOOL，发送成功返回`1`，发送失败返回`0`
 */
BOOL __stdcall SendImage(wchar_t *receiver, wchar_t *ImagePath)
{
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD SendImageCall1 = WeChatWinBase + SendImageCall1Offset;
    DWORD SendImageCall2 = WeChatWinBase + SendImageCall2Offset;
    DWORD SendImageCall3 = WeChatWinBase + SendImageCall3Offset;
    DWORD DeleteSendImageCacheCall = WeChatWinBase + DeleteSendImageCacheCallOffset;
    char nullbuffer[0x50] = {0};
    char buffer[0x3B0] = {0};
    WxString pReceiver(receiver);
    WxString pImagePath(ImagePath);
    WxString nullStruct = {0};
    DWORD tempeax = 0;
    int isSuccess = 0;
    __asm {
		pushad;
		call SendImageCall1;
		sub esp, 0x14;
		mov tempeax, eax;
		lea eax, nullStruct;
		mov ecx, esp;
		lea edi, pImagePath;
		push eax;
		call SendImageCall2;
		mov ecx, dword ptr [tempeax] ;
		lea eax, pReceiver;
		push edi;
		push eax;
		lea eax, buffer;
		push eax;
		call SendImageCall3;
		mov isSuccess,eax;
		lea ecx, buffer;
		call DeleteSendImageCacheCall;
		popad;
    }
    return isSuccess;
}
