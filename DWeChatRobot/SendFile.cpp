#include "pch.h"

// 发送文件CALL1偏移
#define SendFileCall1Offset (0x67A71DC0 - 0x67370000)
// 发送文件CALL2偏移
#define SendFileCall2Offset (0x68D81C83 - 0x67370000)
// 发送文件CALL3偏移
#define SendFileCall3Offset (0x68D8047A - 0x67370000)
// 发送文件CALL4偏移
#define SendFileCall4Offset (0x67702260 - 0x67370000)
// 发送文件参数偏移
#define SendFileParamsOffset (0x6959F170 - 0x67370000)
// 清空缓存CALL偏移
#define DeleteSendFileCacheCallOffset (0x67404200 - 0x67370000)

/*
* 外部调用时传递的参数结构
* wxid：wxid的保存地址
* filepath：文件绝对路径的保存地址
*/
struct FileParamStruct {
	DWORD wxid;
	DWORD filepath;
};

/*
* 内存中使用的参数结构
* type：消息类型，文件消息为3
* buffer：文件绝对路径
* length：绝对路径字符数
* maxLength：绝对路径最大字节数
* fill：占位用空缓冲区
* WxFileStruct：默认构造函数
*/
struct WxFileStruct {
	int type = 3;
	wchar_t* buffer;
	DWORD length;
	DWORD maxLength;
	char fill[0x34] = { 0 };

	WxFileStruct(wchar_t* pStr) {
		buffer = pStr;
		length = wcslen(pStr);
		maxLength = wcslen(pStr) * 2;
	}
};

/*
* 供外部调用的发送文件消息接口
* lpParamStruct：FileParamStruct类型结构体指针
* return：void
*/
void SendFileRemote(LPVOID lpParamStruct) {
	FileParamStruct* params = (FileParamStruct*)lpParamStruct;
	SendFile((WCHAR*)params->wxid, (WCHAR*)params->filepath);
}

/*
* 发送文件消息的具体实现
* receiver：接收人wxid
* FilePath：文件绝对路径
* return：void
*/
void __stdcall SendFile(wchar_t* receiver, wchar_t* FilePath) {
	WxBaseStruct pReceiver(receiver);
	WxBaseStruct pFilePath(FilePath);
	WxFileStruct esi_(FilePath);

	DWORD WeChatWinBase = GetWeChatWinBase();

	DWORD WxSendFileCall1 = WeChatWinBase + SendFileCall1Offset;
	DWORD WxSendFileCall2 = WeChatWinBase + SendFileCall2Offset;
	DWORD WxSendFileCall3 = WeChatWinBase + SendFileCall3Offset;
	DWORD WxSendFileCall4 = WeChatWinBase + SendFileCall4Offset;
	DWORD WxSendFileParams = WeChatWinBase + SendFileParamsOffset;
	DWORD DeleteSendFileCacheCall = WeChatWinBase + DeleteSendFileCacheCallOffset;

	char buffer[0x3B0] = { 0 };

	DWORD edi_ = pReceiver.length;
	DWORD ptrReceiver = (DWORD)pReceiver.buffer;

	DWORD tempecx = 0;

	__asm {
		pushad;
		pushfd;
		lea esi, esi_;
		push 0;
		push 0;
		push 0;
		push 0;
		push 0;
		push 0x005A0000;
		sub esp, 0x14;
		mov edi, esp;
		mov dword ptr ds : [edi] , 0x0;
		mov dword ptr ds : [edi + 0x4] , 0x0;
		mov dword ptr ds : [edi + 0x8] , 0x0;
		mov dword ptr ds : [edi + 0xC] , 0x0;
		mov dword ptr ds : [edi + 0x10] , 0x0;

		sub esp, 0x14;
		lea eax, dword ptr ds : [esi + 0x4] ;
		mov ecx, esp;
		push eax;
		call WxSendFileCall1;

		sub esp, 0x14;
		mov ecx, esp;
		mov dword ptr ds : [ecx] , 0x0;
		mov dword ptr ds : [ecx + 0x4] , 0x0;
		mov dword ptr ds : [ecx + 0x8] , 0x0;
		mov dword ptr ds : [ecx + 0xC] , 0x0;
		mov dword ptr ds : [ecx + 0x10] , 0x0;

		lea edx, pReceiver;
		mov eax, [edx];

		mov edi, edi_;
		lea eax, dword ptr ds : [edi * 2 + 0x2] ;
		mov tempecx, ecx;
		push eax;
		push dword ptr ds : [ecx] ;
		call WxSendFileCall2;
		mov ecx, [tempecx];
		mov edx, eax;
		add esp, 0x8;

		mov eax, dword ptr ds : [ecx + 0x4] ;
		mov dword ptr ds : [ecx] , edx;
		mov dword ptr ds : [ecx + 0x8] , edi;
		mov tempecx, ecx;
		push edi;
		push ptrReceiver;
		push dword ptr ds : [ecx] ;
		call WxSendFileCall3;
		mov ecx, [tempecx];
		add esp, 0xC;

		mov edx, 0x0;
		mov eax, dword ptr ds : [ecx] ;
		mov word ptr ds : [eax + edi * 2] , 0x0;
		mov dword ptr ds : [ecx + 0x4] , edi;

		mov ecx, WxSendFileParams;
		lea eax, buffer;
		push eax;
		call WxSendFileCall4;
		lea ecx, buffer;
		call DeleteSendFileCacheCall;
		popfd;
		popad;
	}
}