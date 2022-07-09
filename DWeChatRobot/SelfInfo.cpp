#include "pch.h"
#include <vector>

#define CheckLoginOffset 0x2366538

/*
* �ⲿ����ʱ�ķ�������
* message��selfinfo.c_str()
* length��selfinfo�ַ�������
*/
#ifndef USE_SOCKET
struct SelfInfoStruct {
	DWORD message;
	DWORD length;
} ret;
#endif // !USE_SOCKET


/*
* ���ⲿ���õĻ�ȡ������Ϣ�ӿ�
* return��DWORD��ret���׵�ַ
*/
#ifndef USE_SOCKET
DWORD GetSelfInfoRemote() {
	ZeroMemory(&ret, sizeof(SelfInfoStruct));
	wstring selfinfo = GetSelfInfo();
	wchar_t* message = new wchar_t[selfinfo.length() + 1];
	memcpy(message,selfinfo.c_str(),(selfinfo.length() + 1) * 2);
	ret.message = (DWORD)message;
	ret.length = selfinfo.length();
	return (DWORD)&ret;
}
#endif

/*
* ��ȡ������Ϣ
*/
wstring GetSelfInfo() {
	wstring selfinfo = L"";
	DWORD WeChatWinBase = GetWeChatWinBase();
	vector<DWORD> SelfInfoAddr = {
		WeChatWinBase + 0x236607C,
		WeChatWinBase + 0x2366548,
		WeChatWinBase + 0x23660F4,
		WeChatWinBase + 0x23661F8,
		*(DWORD*)(WeChatWinBase + 0x236622C),
		*(DWORD*)(WeChatWinBase + 0x23A111C),
		*(DWORD*)(WeChatWinBase + 0x23663D4),
		WeChatWinBase + 0x23662E8,
		WeChatWinBase + 0x23661FC,
		WeChatWinBase + 0x2366214,
		WeChatWinBase + 0x2366128
	};

	vector<wstring> SelfInfoKey = {
		L"\"wxId\"",
		L"\"wxNumber\"",
		L"\"wxNickName\"",
		L"\"Sex\"",
		L"\"wxSignature\"",
		L"\"wxBigAvatar\"",
		L"\"wxSmallAvatar\"",
		L"\"wxNation\"",
		L"\"wxProvince\"",
		L"\"wxCity\"",
		L"\"PhoneNumber\""
	};

	selfinfo = selfinfo + L"{";
	for (unsigned int i = 0; i < SelfInfoAddr.size(); i++) {
		selfinfo = selfinfo + SelfInfoKey[i] + L":";
		selfinfo = selfinfo + L"\"";
		char* temp = NULL;
		if (!SelfInfoKey[i].compare(L"\"wxNickName\"")) {
			if (*(DWORD*)(SelfInfoAddr[i] + 0x14) == 0xF) {
				temp = (*((DWORD*)SelfInfoAddr[i]) != 0) ? (char*)SelfInfoAddr[i] : (char*)"null";
			}
			else {
				temp = (*((DWORD*)SelfInfoAddr[i]) != 0) ? (char*)(*(DWORD*)SelfInfoAddr[i]) : (char*)"null";
			}
		}
		else if (!SelfInfoKey[i].compare(L"\"wxId\"")) {
			char wxidbuffer[0x100] = { 0 };
			sprintf_s(wxidbuffer, "%s", (char*)SelfInfoAddr[i]);
			if (strlen(wxidbuffer) < 0x6 || strlen(wxidbuffer) > 0x14)
			{
				//�µ�΢�ź� ΢��ID�õ�ַ����
				temp = (char*)(*(DWORD*)SelfInfoAddr[i]);
			}
			else
			{
				temp = (char*)SelfInfoAddr[i];
			}
		}
		else if (!SelfInfoKey[i].compare(L"\"Sex\"")) {
			int sex = *(int*)SelfInfoAddr[i];
			switch (sex) {
			case 1: {
				selfinfo = selfinfo + L"��\",";
				break;
			}
			case 2: {
				selfinfo = selfinfo + L"Ů\",";
				break;
			}
			default: {
				selfinfo = selfinfo + L"δ֪\",";
				break;
			}
			}
			continue;
		}
		else {
			temp = (char*)SelfInfoAddr[i];
			if (temp == NULL || strlen(temp) == 0) {
				temp = (char*)"null";
			}
		}
		wchar_t* wtemp = new wchar_t[strlen(temp) + 1];
		ZeroMemory(wtemp, (strlen(temp) + 1) * 2);
		MultiByteToWideChar(CP_UTF8, 0, temp, -1, wtemp, strlen(temp) + 1);
		wstring wrtemp = wreplace(wtemp,L'\"',L"\\\"");
		selfinfo = selfinfo + wrtemp;
		selfinfo = selfinfo + L"\"";
		if(i!= SelfInfoAddr.size() - 1)
			selfinfo = selfinfo + L",";
		delete[] wtemp;
		wtemp = NULL;
	}
	selfinfo = selfinfo + L"}";
#ifdef _DEBUG
	wcout.imbue(locale("chs"));
	wcout << selfinfo << endl;
#endif
	return selfinfo;
}

BOOL isWxLogin() {
	DWORD CheckLoginAddr = GetWeChatWinBase() + CheckLoginOffset;
	return *(BOOL*)CheckLoginAddr;
}

/*
* ɾ��������Ϣ����
* return��void
*/
#ifndef USE_SOCKET
VOID DeleteSelfInfoCacheRemote() {
	if (ret.length) {
		delete[] (wchar_t*)ret.message;
		ZeroMemory(&ret, sizeof(SelfInfoStruct));
	}
}
#endif