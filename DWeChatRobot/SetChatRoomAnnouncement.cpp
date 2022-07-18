#include "pch.h"

#define SetChatRoomAnnouncementCallOffset 0x68057ED0 - 0x67C10000
#define SetChatRoomAnnouncementParam1Offset 0x69A911F8 - 0x67C10000
#define SetChatRoomAnnouncementParam2Offset 0x69A90714 - 0x67C10000
#define SetChatRoomAnnouncementParam3Offset 0x69A90768 - 0x67C10000

#ifndef USE_SOCKET
struct SetChatRoomAnnouncementStruct {
	wchar_t* chatroomid;
	wchar_t* announcement;
};
BOOL SetChatRoomAnnouncementRemote(LPVOID lpParameter) {
	SetChatRoomAnnouncementStruct* scras = (SetChatRoomAnnouncementStruct*)lpParameter;
	return SetChatRoomAnnouncement(scras->chatroomid,scras->announcement);
}
#endif

struct SetChatRoomAnnouncementParam1Struct {
	DWORD addr;
	WxString chatroomid;
	WxString announcement;
	WxString selfwxid;
	DWORD fill1 = 0x0;
	DWORD time = 0x0;
	DWORD fill2 = 0x0;
	DWORD fill3 = 0x0;
	WxString xml;
	char nullbuffer[0x2C] = { 0 };
};

struct SetChatRoomAnnouncementParam2Struct {
	DWORD head = 0x1;
	DWORD addr;
	WxString chatroomid;
	WxString announcement;
	WxString selfwxid;
	DWORD time = 0x0;
	DWORD fill1 = 0x0;
	DWORD fill2 = 0x2;
};

BOOL SetChatRoomAnnouncement(wchar_t* chatroomid, wchar_t* announcement) {
	DWORD WeChatWinBase = GetWeChatWinBase();
	DWORD SetChatRoomAnnouncementCall = WeChatWinBase + SetChatRoomAnnouncementCallOffset;
	DWORD SetChatRoomAnnouncementParam1Addr = WeChatWinBase + SetChatRoomAnnouncementParam1Offset;
	DWORD SetChatRoomAnnouncementParam2Addr = WeChatWinBase + SetChatRoomAnnouncementParam2Offset;
	DWORD SetChatRoomAnnouncementParam3Addr = WeChatWinBase + SetChatRoomAnnouncementParam3Offset;

	wstring selfwxid = GetSelfWxid();
	time_t cTime = time(NULL);
	/*wchar_t* xmlbuffer = new wchar_t[0x1000];
	ZeroMemory(xmlbuffer, 0x1000 * 2);
	swprintf_s(xmlbuffer, 0x1000, L"<group_notice_item>\n\t<edittime>%d</edittime>\n\t<datalist count=\"2\">\n\t\t<dataitem dataid=\"f4086b1d1cce7ca08c1ab0ae8b6b50af\" datatype=\"8\" htmlid=\"WeNoteHtmlFile\">\n\t\t\t<datafmt>.htm</datafmt>\n\t\t\t<fullmd5>b1e2c9070ed939767187635505ff4a96</fullmd5>\n\t\t\t<fullsize>64</fullsize>\n\t\t\t<cdn_dataurl>http://wxapp.tc.qq.com/264/20303/stodownload?m=b1e2c9070ed939767187635505ff4a96&amp;filekey=3033020101041f301d02020108040253480410b1e2c9070ed939767187635505ff4a96020140040d00000004627466730000000131&amp;hy=SH&amp;storeid=323032323037313731323039323430303030666233613665663030306233626336626234306230303030303130383030303034663466&amp;bizid=1023</cdn_dataurl>\n\t\t\t<cdn_datakey>3c5e83e10c6b766107c11eed520aaaaa</cdn_datakey>\n\t\t</dataitem>\n\t\t<dataitem dataid=\"2563007f8c87c6ed4ccc40666e8076d4\" datatype=\"1\" htmlid=\" - 1\">\n\t\t\t<datadesc>%ws</datadesc>\n\t\t</dataitem>\n\t</datalist>\n\t<source>\n\t\t<fromusr>%s</fromusr>\n\t\t<tousr>%s</tousr>\n\t\t<sourceid>4918e03e3605937ef668cb588a459806</sourceid>\n\t</source>\n</group_notice_item>",
				(DWORD)cTime,announcement,selfwxid.c_str(),chatroomid);*/
	vector<SetChatRoomAnnouncementParam1Struct> v_param1;
	SetChatRoomAnnouncementParam1Struct param1 = { 0 };
	param1.addr = SetChatRoomAnnouncementParam1Addr;
	param1.chatroomid.buffer = chatroomid;
	param1.chatroomid.length = wcslen(chatroomid);
	param1.chatroomid.maxLength = wcslen(chatroomid) * 2;
	param1.announcement.buffer = announcement;
	param1.announcement.length = announcement ? wcslen(announcement) : 0;
	param1.announcement.maxLength = announcement ? wcslen(announcement) * 2 : 0;
	param1.selfwxid.buffer = (wchar_t*)selfwxid.c_str();
	param1.selfwxid.length = selfwxid.length();
	param1.selfwxid.maxLength = selfwxid.length() * 2;
	/*param1.xml.buffer = xmlbuffer;
	param1.xml.length = wcslen(xmlbuffer);
	param1.xml.maxLength = wcslen(xmlbuffer) * 2;*/
	param1.time = (DWORD)cTime;
	v_param1.push_back(param1);
	VectorStruct* vs = (VectorStruct*)&v_param1;
	DWORD ptrParam1 = (DWORD)&vs->v_data;

	SetChatRoomAnnouncementParam2Struct param2 = { 0 };
	param2.head = 0x1;
	param2.addr = SetChatRoomAnnouncementParam2Addr;
	param2.chatroomid.buffer = chatroomid;
	param2.chatroomid.length = wcslen(chatroomid);
	param2.chatroomid.maxLength = wcslen(chatroomid) * 2;
	param2.announcement.buffer = announcement;
	param2.announcement.length = announcement ? wcslen(announcement) : 0;
	param2.announcement.maxLength = announcement ? wcslen(announcement) * 2 : 0;
	param2.selfwxid.buffer = (wchar_t*)selfwxid.c_str();
	param2.selfwxid.length = selfwxid.length();
	param2.selfwxid.maxLength = selfwxid.length() * 2;
	param2.time = (DWORD)cTime;
	DWORD pnote = 0;
	pnote = (DWORD)&pnote;
	param2.fill1 = pnote;
	param2.fill2 = 0x2;
	__asm {
		pushad;
		pushfd;
		lea eax, param2;
		push eax;
		mov eax, dword ptr[ptrParam1];
		push eax;
		call SetChatRoomAnnouncementCall;
		popfd;
		popad;
	}
	/*delete[] xmlbuffer;*/
	return true;
}