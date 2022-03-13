#include "pch.h"

/*
Executable modules, 条目 128
 基址=02E50000
 大小=02624000 (39993344.)
 入口=0437C59D WeChatWi.<模块入口点>
 名称=WeChatWi
 文件版本=3.5.0.46
 路径=D:\Tencent\WeChat\[3.5.0.46]\WeChatWin.dll

02FB4485    E8 D696F2FF       call    02EDDB60
02FB448A    83EC 14           sub     esp, 14
02FB448D    8945 B0           mov     dword ptr [ebp-50], eax
02FB4490    8D46 18           lea     eax, dword ptr [esi+18]
02FB4493    8BCC              mov     ecx, esp
02FB4495    8D7E 04           lea     edi, dword ptr [esi+4]                ; 图片路径
02FB4498    50                push    eax
02FB4499    E8 823F5700       call    03528420
02FB449E    8B4D B0           mov     ecx, dword ptr [ebp-50]
02FB44A1    8D85 7CFFFFFF     lea     eax, dword ptr [ebp-84]
02FB44A7    57                push    edi                                   ; 图片路径
02FB44A8    50                push    eax                                   ; 接收人wxid
02FB44A9    8D85 58FCFFFF     lea     eax, dword ptr [ebp-3A8]
02FB44AF    50                push    eax                                   ; 缓冲区，存放xml数据
02FB44B0    E8 DB713300       call    032EB690                              ; 发送图片的CALL
*/

#define WxSendImageCall1offset (0x02EDDB60 - 0x02E50000)
#define WxSendImageCall2offset (0x03528420 - 0x02E50000)
#define WxSendImageCall3offset (0x032EB690 - 0x02E50000)

struct ImageParamStruct {
	DWORD wxid;
	DWORD imagepath;
};

void SendImageRemote(LPVOID lpParamStruct) {
	ImageParamStruct* params = (ImageParamStruct*)lpParamStruct;
	SendImage((WCHAR*)params->wxid, (WCHAR*)params->imagepath);
}

void __stdcall SendImage(wchar_t* receiver, wchar_t* ImagePath) {
	DWORD WxSendImageCall1 = GetWeChatWinBase() + WxSendImageCall1offset;
	DWORD WxSendImageCall2 = GetWeChatWinBase() + WxSendImageCall2offset;
	DWORD WxSendImageCall3 = GetWeChatWinBase() + WxSendImageCall3offset;
	char nullbuffer[0x50] = { 0 };
	char buffer[0x3A8] = { 0 };
	WxBaseStruct pReceiver(receiver);
	WxBaseStruct pImagePath(ImagePath);
	
	__asm {
		pushad;
		call WxSendImageCall1;
		sub esp, 0x14;
		mov dword ptr[ebp - 0x50], eax;
		mov ecx, esp;
		lea edi, pImagePath;
		push eax;
		call WxSendImageCall2;
		mov ecx, dword ptr[ebp - 0x50];
		lea eax, pReceiver;
		push edi;
		push eax;
		lea eax, buffer;
		push eax;
		call WxSendImageCall3;
		popad;
	}
}