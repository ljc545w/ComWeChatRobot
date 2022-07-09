#include "pch.h"

// �����ļ�CALL1ƫ��
#define SendFileCall1Offset (0x78763B70 - 0x786A0000)
// �����ļ�CALL2ƫ��
#define SendFileCall2Offset (0x78E11980 - 0x786A0000)
// �����ļ�CALL3ƫ��
#define SendFileCall3Offset (0x78A8D8C0 - 0x786A0000)
// ��ջ���CALLƫ��
#define DeleteSendFileCacheCallOffset (0x78757780 - 0x786A0000)

/*
* �ⲿ����ʱ���ݵĲ����ṹ
* wxid��wxid�ı����ַ
* filepath���ļ�����·���ı����ַ
*/
#ifndef USE_SOCKET
struct FileParamStruct {
	DWORD wxid;
	DWORD filepath;
};
#endif

/*
* �ڴ���ʹ�õĲ����ṹ
* type����Ϣ���ͣ��ļ���ϢΪ3
* buffer���ļ�����·��
* length������·���ַ���
* maxLength������·������ֽ���
* fill��ռλ�ÿջ�����
* WxFileStruct��Ĭ�Ϲ��캯��
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
* ���ⲿ���õķ����ļ���Ϣ�ӿ�
* lpParamStruct��FileParamStruct���ͽṹ��ָ��
* return��void
*/
#ifndef USE_SOCKET
void SendFileRemote(LPVOID lpParamStruct) {
	FileParamStruct* params = (FileParamStruct*)lpParamStruct;
	SendFile((WCHAR*)params->wxid, (WCHAR*)params->filepath);
}
#endif

/*
* �����ļ���Ϣ�ľ���ʵ��
* receiver��������wxid
* FilePath���ļ�����·��
* return��void
*/
void __stdcall SendFile(wchar_t* receiver, wchar_t* FilePath) {
	WxBaseStruct pReceiver(receiver);
	WxBaseStruct pFilePath(FilePath);
	WxFileStruct esi_(FilePath);
	WxString nullbuffer = { 0 };

	DWORD WeChatWinBase = GetWeChatWinBase();

	DWORD WxSendFileCall1 = WeChatWinBase + SendFileCall1Offset;
	DWORD WxSendFileCall2 = WeChatWinBase + SendFileCall2Offset;
	DWORD WxSendFileCall3 = WeChatWinBase + SendFileCall3Offset;
	DWORD DeleteSendFileCacheCall = WeChatWinBase + DeleteSendFileCacheCallOffset;
	DWORD WxSendFileParams = 0;
	
	char buffer[0x3B0] = { 0 };

	DWORD edi_ = pReceiver.length;
	DWORD ptrReceiver = (DWORD)pReceiver.buffer;

	__asm {
		pushad;
		pushfd;
		call WxSendFileCall1;
		sub esp, 0x14;
		mov WxSendFileParams, eax;
		lea eax, nullbuffer;
		mov ecx, esp;
		push eax;
		call WxSendFileCall2;
		push 0x00DBE200;
		sub esp, 0x14;
		mov edi, esp;
		mov dword ptr ds : [edi] , 0x0;
		mov dword ptr ds : [edi + 0x4] , 0x0;
		mov dword ptr ds : [edi + 0x8] , 0x0;
		mov dword ptr ds : [edi + 0xC] , 0x0;
		mov dword ptr ds : [edi + 0x10] , 0x0;
		sub esp, 0x14;
		lea eax, pFilePath;
		mov ecx, esp;
		push eax;
		call WxSendFileCall2;
		sub esp, 0x14;
		lea eax, pReceiver;
		mov ecx, esp;
		push eax;
		call WxSendFileCall2;
		mov ecx, dword ptr [WxSendFileParams];
		lea eax, buffer;
		push eax;
		call WxSendFileCall3;
		lea ecx, buffer;
		call DeleteSendFileCacheCall;
		popfd;
		popad;
	}
}