#include "pch.h"

// ��������CALL1ƫ��
#define SendArticleCall1Offset 0x78758A70 - 0x786A0000
// ��������CALL2ƫ��
#define SendArticleCall2Offset 0x78A8D5E0 - 0x786A0000
// ��������CALL3ƫ��
#define SendArticleCall3Offset 0x787A7F00 - 0x786A0000
// ��������CALL4ƫ��
#define SendArticleCall4Offset 0x78A8D7B0 - 0x786A0000
// ��������CALL����ƫ��
#define SendArticleParamOffset 0x7AA26FE4 - 0x786A0000
// ����WXIDƫ��
#define SelfWxidAddrOffset 0x236607C

// ��ջ���CALL1ƫ��
#define SendArticleClearCacheCall1Offset 0x78D46450 - 0x786A0000
// ��ջ���CALL2ƫ��
#define SendArticleClearCacheCall2Offset 0x78757780 - 0x786A0000

/*
* �ⲿ����ʱ���ݵĲ����ṹ
* wxid�������˵ı����ַ
* title�����±���ı����ַ
* abstract������ժҪ�ı����ַ
* url���������ӵı����ַ
*/
#ifndef USE_SOCKET
struct SendArticleStruct {
	DWORD wxid;
	DWORD title;
	DWORD abstract;
	DWORD url;
	DWORD imgpath;
};
#endif

/*
* ���ⲿ���õķ���������Ϣ�ӿ�
* lparameter��SendArticleStruct���ͽṹ��ָ��
* return��void
*/
#ifndef USE_SOCKET
VOID SendArticleRemote(LPVOID lparameter) {
	SendArticleStruct* sas = (SendArticleStruct*)lparameter;
	wchar_t* wxid = (wchar_t*)sas->wxid;
	wchar_t* title = (wchar_t*)sas->title;
	wchar_t* abstract = (wchar_t*)sas->abstract;
	wchar_t* url = (wchar_t*)sas->url;
	wchar_t* imgpath = sas->imgpath ? (wchar_t*)sas->imgpath : NULL;
	SendArticle(wxid,title,abstract,url, imgpath);
}
#endif

/*
* ��ȡ�Լ���wxid�����ַ
* return��DWORD������wxid�����ַ
*/
DWORD GetSelfWxIdAddr() {
	DWORD baseAddr = GetWeChatWinBase() + SelfWxidAddrOffset;
	char wxidbuffer[0x100] = { 0 };
	DWORD SelfWxIdAddr = 0x0;
	sprintf_s(wxidbuffer, "%s", (char*)baseAddr);
	if (strlen(wxidbuffer) < 0x6 || strlen(wxidbuffer) > 0x14)
	{
		SelfWxIdAddr = *(DWORD*)baseAddr;
	}
	else
	{
		SelfWxIdAddr = baseAddr;
	}
	return SelfWxIdAddr;
}

/*
* ����������Ϣ�ľ���ʵ��
* wxid����Ϣ������wxid
* title�����±���
* abstract������ժҪ
* url����������
* return��BOOL���ɹ�����`1`��ʧ�ܷ���`0`
*/
BOOL __stdcall SendArticle(wchar_t* wxid,wchar_t* title, wchar_t* abstract, wchar_t* url,wchar_t* imgpath) {
	DWORD WeChatWinBase = GetWeChatWinBase();
	DWORD SendArticleCall1 = WeChatWinBase + SendArticleCall1Offset;
	DWORD SendArticleCall2 = WeChatWinBase + SendArticleCall2Offset;
	DWORD SendArticleCall3 = WeChatWinBase + SendArticleCall3Offset;
	DWORD SendArticleCall4 = WeChatWinBase + SendArticleCall4Offset;

	DWORD SendArticleParam = WeChatWinBase + SendArticleParamOffset;

	DWORD SendArticleClearCacheCall1 = WeChatWinBase + SendArticleClearCacheCall1Offset;
	DWORD SendArticleClearCacheCall2 = WeChatWinBase + SendArticleClearCacheCall2Offset;
	// �Լ���wxid��������
	char* sselfwxid = (char*)GetSelfWxIdAddr();
	wchar_t* wselfwxid = new wchar_t[strlen(sselfwxid) + 1];
	MultiByteToWideChar(CP_ACP, 0, sselfwxid, -1, wselfwxid, strlen(sselfwxid) + 1);
	// ����xml����
	wchar_t* xmlbuffer = new wchar_t[0x2000];
	ZeroMemory(xmlbuffer, 0x2000 * 2);
	swprintf_s(xmlbuffer,0x2000, (wchar_t*)L"<msg>\n    <fromusername>%ws</fromusername>\n    <scene>0</scene>\n    <commenturl></commenturl>\n    <appmsg appid=\"\" sdkver=\"0\">\n        <title>%ws</title>\n        <des>%ws</des>\n        <action>view</action>\n        <type>5</type>\n        <showtype>0</showtype>\n        <content></content>\n        <url>%ws</url>\n        <dataurl></dataurl>\n        <lowurl></lowurl>\n        <lowdataurl></lowdataurl>\n        <recorditem>\n            <![CDATA[]]>\n        </recorditem>\n        <thumburl></thumburl>\n        <messageaction></messageaction>\n        <extinfo></extinfo>\n        <sourceusername></sourceusername>\n        <sourcedisplayname></sourcedisplayname>\n        <commenturl></commenturl>\n        <appattach>\n            <totallen>0</totallen>\n            <attachid></attachid>\n            <emoticonmd5></emoticonmd5>\n            <fileext></fileext>\n            <aeskey></aeskey>\n        </appattach>\n        <weappinfo>\n            <pagepath></pagepath>\n            <username></username>\n            <appid></appid>\n            <appservicetype>0</appservicetype>\n        </weappinfo>\n        <websearch />\n    </appmsg>\n    <appinfo>\n        <version>1</version>\n        <appname>Window wechat</appname>\n    </appinfo>\n</msg>",
		wselfwxid,title,abstract,url);

	DWORD sendtype = 0x5;
	WxBaseStruct pSender(wselfwxid);
	char nullbuffer[0x1C] = { 0 };
	WxBaseStruct pXml(xmlbuffer);
	WxBaseStruct pReceiver(wxid);
	WxString imgbuffer = { 0 };
	if (imgpath) {
		imgbuffer.buffer = imgpath;
		imgbuffer.length = wcslen(imgpath);
		imgbuffer.maxLength = wcslen(imgpath) * 2;
	}
	WxString nullStruct = { 0 };
	char buffer[0xFF0] = { 0 };
	DWORD isSuccess = 0x0;
	__asm {
		pushad;
		pushfd;
		lea ecx, buffer;
		call SendArticleCall1;
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
		call SendArticleCall2;
		add esp, 0x14;
		lea eax, nullStruct;
		push eax;
		lea ecx, buffer;
		call SendArticleCall3;
		mov dl, 0x0;
		lea ecx, buffer;
		push SendArticleParam;
		push SendArticleParam;
		call SendArticleCall4;
		mov isSuccess, eax;
		add esp, 0x8;
		lea ecx, buffer;
		call SendArticleClearCacheCall1;
		lea ecx, buffer;
		call SendArticleClearCacheCall2;
		popfd;
		popad;
	}
	delete[] xmlbuffer;
	xmlbuffer = NULL;
	delete[] wselfwxid;
	wselfwxid = NULL;
	return (isSuccess == 0x1);
}