#include "pch.h"

#define GetChatRoomMembersCall1Offset 0x6246BBB0 - 0x61E20000
#define GetChatRoomMembersCall2Offset 0x61EDF550 - 0x61E20000
#define GetChatRoomMembersCall3Offset 0x622046D0 - 0x61E20000
#define DeleteGetChatRoomMembersCacheCallOffset 0x6246BDD0 - 0x61E20000

struct ChatRoomInfoStruct {
	wchar_t* members = NULL;
	DWORD length = 0;
};

ChatRoomInfoStruct chatroominfo = { 0 };

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

BOOL __stdcall GetChatRoomMembers(wchar_t* chatroomid) {
	DWORD WeChatWinBase = GetWeChatWinBase();
	DWORD GetChatRoomMembersCall1 = WeChatWinBase + GetChatRoomMembersCall1Offset;
	DWORD GetChatRoomMembersCall2 = WeChatWinBase + GetChatRoomMembersCall2Offset;
	DWORD GetChatRoomMembersCall3 = WeChatWinBase + GetChatRoomMembersCall3Offset;
	DWORD DeleteGetChatRoomMembersCacheCall = WeChatWinBase + DeleteGetChatRoomMembersCacheCallOffset;

	WxBaseStruct wsChatRoomId(chatroomid);
	char buffer[0x1B0] = { 0 };
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