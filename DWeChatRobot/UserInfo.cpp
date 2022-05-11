#include "pch.h"
#include <typeinfo>
#include <string>
#include <vector>

// 获取好友信息CALL0偏移
#define GetUserInfoCall0Offset 0x6740A000 - 0x67370000
// 获取好友信息CALL1偏移
#define GetUserInfoCall1Offset 0x679C9840 - 0x67370000
// 获取好友信息CALL2偏移
#define GetUserInfoCall2Offset 0x67A71DC0 - 0x67370000
// 获取好友信息CALL3偏移
#define GetUserInfoCall3Offset 0x677724A0 - 0x67370000

// 清空缓存CALL1偏移
#define DeleteUserInfoCacheCall1Offset 0x67775990 - 0x67370000
// 清空缓存CALL2偏移
#define DeleteUserInfoCacheCall2Offset 0x679CA340 - 0x67370000

/*
* 外部调用时的返回类型
* message：wUserInfo.c_str()
* length：wUserInfo字符串长度
*/
struct GetUserInfoStruct {
	DWORD message;
	DWORD length;
};

// 保存好友信息的字符串
wstring wUserInfo = L"";
// 外部调用时的具体返回对象
GetUserInfoStruct ret = { 0 };

/*
* 根据缓冲区内容拼接好友信息
* address：缓冲区地址
* return：void
*/
VOID WxUserInfo(DWORD address) {
	vector<DWORD> InfoType{
		address + 0x10,
		address + 0x24,
		address + 0x38,
		address + 0x58,
		address + 0x6C,
		address + 0xFC,
		address + 0x110,
		address + 0x19C,
		address + 0x1B0,
		address + 0x1C4,
		address + 0x1D8,
		address + 0x27C
	};
	vector<wchar_t*> InfoTypeName{
		(WCHAR*)L"\"wxId\"",
		(WCHAR*)L"\"wxNumber\"",
		(WCHAR*)L"\"wxV3\"",
		(WCHAR*)L"\"wxRemark\"",
		(WCHAR*)L"\"wxNickName\"",
		(WCHAR*)L"\"wxBigAvatar\"",
		(WCHAR*)L"\"wxSmallAvatar\"",
		(WCHAR*)L"\"wxSignature\"",
		(WCHAR*)L"\"wxNation\"",
		(WCHAR*)L"\"wxProvince\"",
		(WCHAR*)L"\"wxCity\"",
		(WCHAR*)L"\"wxBackground\"",
	};
	wUserInfo += L"{";
	for (unsigned int i = 0; i < InfoType.size(); i++) {
		wchar_t* wstemp = ((*((DWORD*)InfoType[i])) != 0) ? (WCHAR*)(*((LPVOID*)InfoType[i])) : (WCHAR*)L"null";
		wstring wsrtemp = wreplace(wstemp,L'\"',L"\\\"");
		wUserInfo = wUserInfo + InfoTypeName[i] + L":\"" + wsrtemp + L"\"";
		if (i != InfoType.size() - 1) {
			wUserInfo += L",";
		}
	}
	wUserInfo += L"}";
#ifdef _DEBUG
	wcout.imbue(locale("chs"));
	wcout << wUserInfo.c_str() << endl;
#endif
}

/*
* 供外部调用的获取好友信息接口
* lparamter：保存好友wxid的地址
* return：DWORD，`ret`的首地址
*/
DWORD GetWxUserInfoRemote(LPVOID lparamter) {
	wchar_t* userwxid = (wchar_t*)lparamter;
	
	if (!GetUserInfoByWxId(userwxid)) {
		return 0;
	}
	ret.message = (DWORD)wUserInfo.c_str();
	ret.length = (DWORD)wUserInfo.length();
	return (DWORD)&ret;
}

/*
* 供外部调用的清空好友信息缓存的接口
* return：void
*/
VOID DeleteUserInfoCacheRemote() {
	if (ret.length) {
		ZeroMemory((wchar_t*)ret.message, ret.length * 2 + 2);
		ret.length = 0;
		wUserInfo = L"";
	}
}

/*
* 根据wxid获取好友信息的具体实现
* wxid：好友wxid
* return：BOOL，成功返回`1`，失败返回`0`
*/
BOOL __stdcall GetUserInfoByWxId(wchar_t* wxid) {
	DWORD WeChatWinBase = GetWeChatWinBase();
	DWORD WxGetUserInfoCall0 = WeChatWinBase + GetUserInfoCall0Offset;
	DWORD WxGetUserInfoCall1 = WeChatWinBase + GetUserInfoCall1Offset;
	DWORD WxGetUserInfoCall2 = WeChatWinBase + GetUserInfoCall2Offset;
	DWORD WxGetUserInfoCall3 = WeChatWinBase + GetUserInfoCall3Offset;
	DWORD DeleteUserInofCacheCall1 = WeChatWinBase + DeleteUserInfoCacheCall1Offset;
	DWORD DeleteUserInofCacheCall2 = WeChatWinBase + DeleteUserInfoCacheCall2Offset;
	char buffer[0x3FC] = { 0 };
	WxBaseStruct pWxid(wxid);
	DWORD address = 0;
	DWORD isSuccess = 0;
	__asm
	{
		pushad;
		call WxGetUserInfoCall0;
		mov edi, eax;
		lea ecx, buffer;
		call WxGetUserInfoCall1;
		lea eax, buffer;
		mov address, eax;
		push eax;
		sub esp, 0x14;
		mov ecx, esp;
		lea esi, pWxid;
		push esi;
		call WxGetUserInfoCall2;
		mov ecx, edi;
		call WxGetUserInfoCall3;
		mov isSuccess, eax;
		popad;
	}
	if(isSuccess)
		WxUserInfo(address);
	__asm {
		pushad;
		lea eax, buffer;
		push eax;
		call DeleteUserInofCacheCall1;
		lea ecx, buffer;
		mov esi, eax;
		call DeleteUserInofCacheCall2;
		popad;
	}
	return isSuccess;
}

/*
* 根据wxid获取联系人昵称，主要用于发送艾特消息接口
* wxid：联系人wxid
* return：wchar_t*，获取到的wxid
*/
wchar_t* __stdcall GetUserNickNameByWxId(wchar_t* wxid) {
	DWORD WeChatWinBase = GetWeChatWinBase();
	DWORD WxGetUserInfoCall0 = WeChatWinBase + GetUserInfoCall0Offset;
	DWORD WxGetUserInfoCall1 = WeChatWinBase + GetUserInfoCall1Offset;
	DWORD WxGetUserInfoCall2 = WeChatWinBase + GetUserInfoCall2Offset;
	DWORD WxGetUserInfoCall3 = WeChatWinBase + GetUserInfoCall3Offset;
	DWORD DeleteUserInofCacheCall1 = WeChatWinBase + DeleteUserInfoCacheCall1Offset;
	DWORD DeleteUserInofCacheCall2 = WeChatWinBase + DeleteUserInfoCacheCall2Offset;
	char buffer[0x3FC] = { 0 };
	WxBaseStruct pWxid(wxid);
	DWORD address = 0;
	DWORD isSuccess = 0;
	__asm
	{
		pushad;
		call WxGetUserInfoCall0;
		mov edi, eax;
		lea ecx, buffer;
		call WxGetUserInfoCall1;
		lea eax, buffer;
		mov address, eax;
		push eax;
		sub esp, 0x14;
		mov ecx, esp;
		lea esi, pWxid;
		push esi;
		call WxGetUserInfoCall2;
		mov ecx, edi;
		call WxGetUserInfoCall3;
		mov isSuccess, eax;
		popad;
	}
	wchar_t* NickName = NULL;
	if (isSuccess) {
		DWORD length = *(DWORD*)(address + 0x6C + 0x4);
		NickName = new wchar_t[length + 1];
		ZeroMemory(NickName, (length + 1) * 2);
		memcpy(NickName, (wchar_t*)(*(DWORD*)(address + 0x6C)), length * 2);
	}
	__asm {
		pushad;
		lea eax, buffer;
		push eax;
		call DeleteUserInofCacheCall1;
		lea ecx, buffer;
		mov esi, eax;
		call DeleteUserInofCacheCall2;
		popad;
	}
	return NickName;
}