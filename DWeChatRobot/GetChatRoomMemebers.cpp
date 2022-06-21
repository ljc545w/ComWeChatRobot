#include "pch.h"

// 获取群成员CALL1偏移
#define GetChatRoomMembersCall1Offset 0x54926F80 - 0x54270000
// 获取群成员CALL2偏移
#define GetChatRoomMembersCall2Offset 0x54352990 - 0x54270000
// 获取群成员CALL3偏移
#define GetChatRoomMembersCall3Offset 0x546B88C0 - 0x54270000
// 清空缓存CALL偏移
#define DeleteGetChatRoomMembersCacheCallOffset 0x549271A0 - 0x54270000

/*
* 外部调用的返回类型
* members：群成员wxid字符串，以`^`分隔
* length：members字符串长度
*/
struct ChatRoomInfoStruct {
	wchar_t* members = NULL;
	DWORD length = 0;
};

/*
* 外部调用时的具体返回对象
*/
ChatRoomInfoStruct chatroominfo = { 0 };

/*
* 供外部调用的获取群成员列表接口
* lparameter：保存群聊ID的地址
* return：DWORD，调用成功且群成员数量不为0，返回`chatroominfo`首地址，否则返回0
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
* 获取群成员列表的具体实现
* chatroomid：群聊ID
* return：BOOL，成功返回`1`，失败返回`0`
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