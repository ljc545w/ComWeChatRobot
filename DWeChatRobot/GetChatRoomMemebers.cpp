#include "pch.h"

// ��ȡȺ��ԱCALL1ƫ��
#define GetChatRoomMembersCall1Offset 0x106B6F80 - 0x10000000
// ��ȡȺ��ԱCALL2ƫ��
#define GetChatRoomMembersCall2Offset 0x100E29F0 - 0x10000000
// ��ȡȺ��ԱCALL3ƫ��
#define GetChatRoomMembersCall3Offset 0x104488C0 - 0x10000000
// ��ջ���CALLƫ��
#define DeleteGetChatRoomMembersCacheCallOffset 0x106B71A0 - 0x10000000

/*
* �ⲿ���õķ�������
* members��Ⱥ��Աwxid�ַ�������`^`�ָ�
* length��members�ַ�������
*/
struct ChatRoomInfoStruct {
	wchar_t* members = NULL;
	DWORD length = 0;
};

/*
* �ⲿ����ʱ�ľ��巵�ض���
*/
ChatRoomInfoStruct chatroominfo = { 0 };

/*
* ���ⲿ���õĻ�ȡȺ��Ա�б�ӿ�
* lparameter������Ⱥ��ID�ĵ�ַ
* return��DWORD�����óɹ���Ⱥ��Ա������Ϊ0������`chatroominfo`�׵�ַ�����򷵻�0
*/
#ifndef USE_SOCKET
DWORD GetChatRoomMembersRemote(LPVOID lparameter) {
	wchar_t* chatroomid = (WCHAR*)lparameter;
	if (chatroominfo.members != NULL) {
		delete[] chatroominfo.members;
		chatroominfo.members = NULL;
		chatroominfo.length = 0;
	}
	if (GetChatRoomMembers(chatroomid)) {
#ifdef _DEBUG
		wcout << chatroominfo.members << endl;
#endif
		return (DWORD)&chatroominfo.members;
	}
	else {
		return 0;
	}
	return 0;
}
#endif

/*
* ��ȡȺ��Ա�б�ľ���ʵ��
* chatroomid��Ⱥ��ID
* return��BOOL���ɹ�����`1`��ʧ�ܷ���`0`
*/
BOOL __stdcall GetChatRoomMembers(wchar_t* chatroomid) {
	DWORD WeChatWinBase = GetWeChatWinBase();
	DWORD GetChatRoomMembersCall1 = WeChatWinBase + GetChatRoomMembersCall1Offset;
	DWORD GetChatRoomMembersCall2 = WeChatWinBase + GetChatRoomMembersCall2Offset;
	DWORD GetChatRoomMembersCall3 = WeChatWinBase + GetChatRoomMembersCall3Offset;
	DWORD DeleteGetChatRoomMembersCacheCall = WeChatWinBase + DeleteGetChatRoomMembersCacheCallOffset;

	WxBaseStruct wsChatRoomId(chatroomid);
	char buffer[0x1E0] = { 0 };
	DWORD isSuccess = 0x0;
	DWORD DataAddr = 0x0;

	__asm {
		pushad;
		pushfd;
		lea ecx, buffer;
		mov DataAddr, ecx;
		call GetChatRoomMembersCall1;
		call GetChatRoomMembersCall2;
		lea eax, buffer;
		push eax;
		lea edi, wsChatRoomId;
		push edi;
		call GetChatRoomMembersCall3;
		mov isSuccess, eax;
		popfd;
		popad;
	}
	if (isSuccess) {
		char* members = (char*)(*(DWORD*)(DataAddr + 0x1C));
#ifdef _DEBUG
		cout << members << endl;
#endif
		wchar_t* wmembers = new wchar_t[strlen(members) + 1];
		ZeroMemory(wmembers, (strlen(members) + 1) * 2);
		MultiByteToWideChar(CP_ACP,0,members,-1,wmembers, strlen(members) + 1);
		chatroominfo.members = wmembers;
		chatroominfo.length = wcslen(wmembers);
	}
	__asm {
		pushad;
		pushfd;
		lea ecx, buffer;
		call DeleteGetChatRoomMembersCacheCall;
		popfd;
		popad;
	}
	return isSuccess;
}