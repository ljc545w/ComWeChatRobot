#include "pch.h"

struct SendArticleStruct {
	DWORD title;
	DWORD abstract;
	DWORD wxid;
	DWORD url;
};

struct WxSendXmlStruct
{
	wchar_t* buffer;
	DWORD length;
	DWORD maxLength;
	DWORD fill1;
	DWORD fill2;
	char nullbuffer[0x3C] = { 0 };

	WxSendXmlStruct(wchar_t* pStr) {
		buffer = pStr;
		length = wcslen(pStr);
		maxLength = wcslen(pStr) * 2;
		fill1 = 0x0;
		fill2 = 0x0;
	}
};

struct WxSenderStruct
{
	wchar_t* buffer;
	DWORD length;
	DWORD maxLength;
	DWORD fill1;
	DWORD fill2;
	char nullbuffer[0x64] = { 0 };

	WxSenderStruct(wchar_t* pStr) {
		buffer = pStr;
		length = wcslen(pStr);
		maxLength = wcslen(pStr) * 2;
		fill1 = 0x0;
		fill2 = 0x0;
	}
};

BOOL SendArticle(wchar_t* wxid,wchar_t* title, wchar_t* abstract, wchar_t* url) {
	DWORD WeChatWinBase = GetWeChatWinBase();
	DWORD SendArticleCall = WeChatWinBase + 0x03297840 - 0x02F20000;
	char* sselfwxid = (char*)(*(DWORD*)(WeChatWinBase + 0x21DC9C4));
	wchar_t* wselfwxid = new wchar_t[strlen(sselfwxid) + 1];
	MultiByteToWideChar(CP_ACP, MB_COMPOSITE, sselfwxid, -1, wselfwxid, strlen(sselfwxid) + 1);

	wchar_t* xmlbuffer = new wchar_t[0x2000];
	ZeroMemory(xmlbuffer, 0x2000 * 2);
	swprintf_s(xmlbuffer,0x2000, (wchar_t*)L"<msg>\n    <fromusername>%ws</fromusername>\n    <scene>0</scene>\n    <commenturl></commenturl>\n    <appmsg appid=\"\" sdkver=\"0\">\n        <title>%ws</title>\n        <des>%ws</des>\n        <action>view</action>\n        <type>5</type>\n        <showtype>0</showtype>\n        <content></content>\n        <url>%ws</url>\n        <dataurl></dataurl>\n        <lowurl></lowurl>\n        <lowdataurl></lowdataurl>\n        <recorditem>\n            <![CDATA[]]>\n        </recorditem>\n        <thumburl></thumburl>\n        <messageaction></messageaction>\n        <extinfo></extinfo>\n        <sourceusername></sourceusername>\n        <sourcedisplayname></sourcedisplayname>\n        <commenturl></commenturl>\n        <appattach>\n            <totallen>0</totallen>\n            <attachid></attachid>\n            <emoticonmd5></emoticonmd5>\n            <fileext></fileext>\n            <aeskey></aeskey>\n        </appattach>\n        <weappinfo>\n            <pagepath></pagepath>\n            <username></username>\n            <appid></appid>\n            <appservicetype>0</appservicetype>\n        </weappinfo>\n        <websearch />\n    </appmsg>\n    <appinfo>\n        <version>1</version>\n        <appname>Window wechat</appname>\n    </appinfo>\n</msg>",
		wselfwxid,title,abstract,url);
	DWORD sendtype = 0x5;
	WxSenderStruct pSender(wselfwxid);
	char nullbuffer[0x1C] = { 0 };
	char imgbuffer[0x3C] = { 0 };
	WxSendXmlStruct pXml(xmlbuffer);
	WxSenderStruct pReceiver(wxid);
	char buffer[0xF70] = { 0 };
	DWORD isSuccess = 0x0;
	__asm {
		pushad;
		pushfd;
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
		call SendArticleCall;
		add esp, 0x14;
		mov isSuccess, eax;
		popfd;
		popad;
	}
	delete[] xmlbuffer;
	xmlbuffer = NULL;
	delete[] wselfwxid;
	wselfwxid = NULL;
	return isSuccess;
}