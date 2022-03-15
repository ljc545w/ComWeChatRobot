#include "pch.h"

struct SendArticleStruct {
	DWORD wxid;
	DWORD title;
	DWORD abstract;
	DWORD url;
};

VOID SendArticleRemote(LPVOID lparameter) {
	SendArticleStruct* sas = (SendArticleStruct*)lparameter;
	wchar_t* wxid = (wchar_t*)sas->wxid;
	wchar_t* title = (wchar_t*)sas->title;
	wchar_t* abstract = (wchar_t*)sas->abstract;
	wchar_t* url = (wchar_t*)sas->url;
	SendArticle(wxid,title,abstract,url);
}

BOOL __stdcall SendArticle(wchar_t* wxid,wchar_t* title, wchar_t* abstract, wchar_t* url) {
	DWORD WeChatWinBase = GetWeChatWinBase();
	DWORD SendArticleCall1 = WeChatWinBase + 0x5BC68E80 - 0x5BBE0000;
	DWORD SendArticleCall2 = WeChatWinBase + 0x03297840 - 0x02F20000;
	DWORD SendArticleCall3 = WeChatWinBase + 0x5BC9C570 - 0x5BBE0000;
	DWORD SendArticleCall4 = WeChatWinBase + 0x5BF57A10 - 0x5BBE0000;

	DWORD SendArticleParam = WeChatWinBase + 0x5DDCCD1C - 0x5BBE0000;

	DWORD SendArticleClearCacheCall1 = WeChatWinBase + 0x5C1F40D0 - 0x5BBE0000;
	DWORD SendArticleClearCacheCall2 = WeChatWinBase + 0x59637BA0 - 0x595B0000;
	// 自己的wxid，发送者
	char* sselfwxid = (char*)(*(DWORD*)(WeChatWinBase + 0x21DC9C4));
	wchar_t* wselfwxid = new wchar_t[strlen(sselfwxid) + 1];
	MultiByteToWideChar(CP_ACP, MB_COMPOSITE, sselfwxid, -1, wselfwxid, strlen(sselfwxid) + 1);
	// 构造xml数据
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
	WxString nullStruct = { 0 };
	char buffer[0xF70] = { 0 };
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