#include "pch.h"

// 发送图片CALL1偏移
#define SendImageCall1Offset (0x5432D730 - 0x54270000)
// 发送图片CALL2偏移
#define SendImageCall2Offset (0x549E0980 - 0x54270000)
// 发送图片CALL3偏移
#define SendImageCall3Offset (0x54791640 - 0x54270000)
// 清空缓存的CALL偏移
#define DeleteSendImageCacheCallOffset (0x54327720 - 0x54270000)

/*
* 外部调用时传递的参数结构
* wxid：保存wxid的地址
* imagepath：保存图片绝对路径的地址
*/
struct ImageParamStruct {
	DWORD wxid;
	DWORD imagepath;
};

/*
* 供外部调用的发送图片消息接口
* lpParamStruct：ImageParamStruct类型结构体指针
* return：void
*/
void SendImageRemote(LPVOID lpParamStruct) {
	ImageParamStruct* params = (ImageParamStruct*)lpParamStruct;
	SendImage((WCHAR*)params->wxid, (WCHAR*)params->imagepath);
}

/*
* 发送图片消息的具体实现
* receiver：接收人wxid
* ImagePath：图片绝对路径
* return：void
*/
void __stdcall SendImage(wchar_t* receiver, wchar_t* ImagePath) {
	DWORD WeChatWinBase = GetWeChatWinBase();
	DWORD SendImageCall1 = WeChatWinBase + SendImageCall1Offset;
	DWORD SendImageCall2 = WeChatWinBase + SendImageCall2Offset;
	DWORD SendImageCall3 = WeChatWinBase + SendImageCall3Offset;
	DWORD DeleteSendImageCacheCall = WeChatWinBase + DeleteSendImageCacheCallOffset;
	char nullbuffer[0x50] = { 0 };
	char buffer[0x3B0] = { 0 };
	WxBaseStruct pReceiver(receiver);
	WxBaseStruct pImagePath(ImagePath);
	WxString nullStruct = { 0 };
	
	__asm {
		pushad;
		call SendImageCall1;
		sub esp, 0x14;
		mov ebx, eax;
		lea eax, nullStruct;
		mov ecx, esp;
		lea edi, pImagePath;
		push eax;
		call SendImageCall2;
		mov ecx, ebx;
		lea eax, pReceiver;
		push edi;
		push eax;
		lea eax, buffer;
		push eax;
		call SendImageCall3;
		lea ecx, buffer;
		call DeleteSendImageCacheCall;
		popad;
	}
}