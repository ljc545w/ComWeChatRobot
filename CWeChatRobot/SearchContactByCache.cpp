#include "pch.h"

struct GetUserInfoStruct {
	DWORD message;
	DWORD length;
};

std::wstring GetWxUserInfo(DWORD pid,wchar_t* wxid) {
	DWORD dwReadSize = 0;
	wstring info = L"";
	WeChatProcess hp(pid);
	if (!hp.m_init) return L"{}";
	DWORD GetWxUserInfoRemoteAddr = hp.GetProcAddr(GetWxUserInfoRemote);
	DWORD DeleteUserInfoCacheProcAddr = hp.GetProcAddr(DeleteUserInfoCacheRemote);
	if (GetWxUserInfoRemoteAddr == 0)
		return L"{}";
	WeChatData<wchar_t*> r_wxid(hp.GetHandle(), wxid, TEXTLENGTH(wxid));
	if (r_wxid.GetAddr() == 0)
		return L"{}";
	DWORD ret = CallRemoteFunction(hp.GetHandle(), GetWxUserInfoRemoteAddr, r_wxid.GetAddr());
	if (ret == 0)
		return L"{}";
	GetUserInfoStruct userinfo = { 0 };
	ReadProcessMemory(hp.GetHandle(), (LPVOID)ret, &userinfo, sizeof(GetUserInfoStruct), &dwReadSize);
	if (userinfo.length) {
		wchar_t* wmessage = new wchar_t[userinfo.length + 1];
		ZeroMemory(wmessage, (userinfo.length + 1) * 2);
		ReadProcessMemory(hp.GetHandle(), (LPVOID)userinfo.message, wmessage, userinfo.length * 2, &dwReadSize);
		info = (wstring)wmessage;
		delete[] wmessage;
		wmessage = NULL;
	}
	CallRemoteFunction(hp.GetHandle(), DeleteUserInfoCacheProcAddr, NULL);
	return info;
}

