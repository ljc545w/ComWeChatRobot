#include "pch.h"

static struct UserInfoAddr {
	DWORD errcode;
	DWORD keyword;
	int l_keyword;
	DWORD v3;
	int l_v3;
	DWORD NickName;
	int l_NickName;
	DWORD Signature;
	int l_Signature;
	DWORD v2;
	int l_v2;
	DWORD Nation;
	int l_Nation;
	DWORD Province;
	int l_Province;
	DWORD City;
	int l_City;
	DWORD BigAvatar;
	int l_BigAvatar;
	DWORD SmallAvatar;
	int l_SmallAvatar;
	DWORD sex;
	BOOL over;
} userinfoaddr;

static struct UserInfo {
	wchar_t* keyword;
	wchar_t* v3;
	wchar_t* NickName;
	wchar_t* Signature;
	wchar_t* v2;
	wchar_t* Nation;
	wchar_t* Province;
	wchar_t* City;
	wchar_t* BigAvatar;
	wchar_t* SmallAvatar;
	DWORD sex;
} userinfo;

static vector<wstring> UserInfoKey = {
		L"keyword",
		L"v3",
		L"NickName",
		L"Signature",
		L"v2",
		L"Nation",
		L"Province",
		L"City",
		L"BigAvatar",
		L"SmallAvatar",
		L"Sex"
};

static void ClearUserInfoCache() {
	ZeroMemory(&userinfoaddr, sizeof(UserInfoAddr));
	if (userinfo.keyword) {
		delete userinfo.keyword;
	}
	if (userinfo.v2) {
		delete userinfo.v2;
	}
	if (userinfo.v3) {
		delete userinfo.v3;
	}
	if (userinfo.NickName) {
		delete userinfo.NickName;
	}
	if (userinfo.Nation) {
		delete userinfo.Nation;
	}
	if (userinfo.Province) {
		delete userinfo.Province;
	}
	if (userinfo.City) {
		delete userinfo.City;
	}
	if (userinfo.Signature) {
		delete userinfo.Signature;
	}
	if (userinfo.SmallAvatar) {
		delete userinfo.SmallAvatar;
	}
	if (userinfo.BigAvatar) {
		delete userinfo.BigAvatar;
	}
	ZeroMemory(&userinfo, sizeof(UserInfo));
}

static SAFEARRAY* CreateUserInfoArray() {
	HRESULT hr = S_OK;
	SAFEARRAY* psaValue;
	SAFEARRAYBOUND rgsaBound[2] = { {UserInfoKey.size(),0},{2,0} };
	psaValue = SafeArrayCreate(VT_VARIANT, 2, rgsaBound);
	wchar_t** userinfoArray = (wchar_t**)&userinfo;
	for (unsigned long i = 0; i < UserInfoKey.size(); i++)
	{
		long keyIndex[2] = { (long)i,0 };
		hr = SafeArrayPutElement(psaValue, keyIndex, &(_variant_t)UserInfoKey[i].c_str());
		long valueIndex[2] = { (long)i,1 };
		if(i != UserInfoKey.size() - 1)
			hr = SafeArrayPutElement(psaValue, valueIndex, &(_variant_t)userinfoArray[(int)i]);
		else
			hr = SafeArrayPutElement(psaValue, valueIndex, &(_variant_t)userinfo.sex);
	}
	ClearUserInfoCache();
	return psaValue;
}

static void ReadUserInfoFromMemory(HANDLE hProcess) {
	userinfo.keyword = new wchar_t[userinfoaddr.l_keyword + 1];
	ReadProcessMemory(hProcess, (LPCVOID)userinfoaddr.keyword, userinfo.keyword, (userinfoaddr.l_keyword + 1) * sizeof(wchar_t), 0);
	userinfo.v3 = new wchar_t[userinfoaddr.l_v3 + 1];
	ReadProcessMemory(hProcess, (LPCVOID)userinfoaddr.v3, userinfo.v3, (userinfoaddr.l_v3 + 1) * sizeof(wchar_t), 0);
	userinfo.NickName = new wchar_t[userinfoaddr.l_NickName + 1];
	ReadProcessMemory(hProcess, (LPCVOID)userinfoaddr.NickName, userinfo.NickName, (userinfoaddr.l_NickName + 1) * sizeof(wchar_t), 0);
	userinfo.Signature = new wchar_t[userinfoaddr.l_Signature + 1];
	ReadProcessMemory(hProcess, (LPCVOID)userinfoaddr.Signature, userinfo.Signature, (userinfoaddr.l_Signature + 1) * sizeof(wchar_t), 0);
	userinfo.v2 = new wchar_t[userinfoaddr.l_v2 + 1];
	ReadProcessMemory(hProcess, (LPCVOID)userinfoaddr.v2, userinfo.v2, (userinfoaddr.l_v2 + 1) * sizeof(wchar_t), 0);
	userinfo.Nation = new wchar_t[userinfoaddr.l_Nation + 1];
	ReadProcessMemory(hProcess, (LPCVOID)userinfoaddr.Nation, userinfo.Nation, (userinfoaddr.l_Nation + 1) * sizeof(wchar_t), 0);
	userinfo.Province = new wchar_t[userinfoaddr.l_Province + 1];
	ReadProcessMemory(hProcess, (LPCVOID)userinfoaddr.Province, userinfo.Province, (userinfoaddr.l_Province + 1) * sizeof(wchar_t), 0);
	userinfo.City = new wchar_t[userinfoaddr.l_City + 1];
	ReadProcessMemory(hProcess, (LPCVOID)userinfoaddr.City, userinfo.City, (userinfoaddr.l_City + 1) * sizeof(wchar_t), 0);
	userinfo.BigAvatar = new wchar_t[userinfoaddr.l_BigAvatar + 1];
	ReadProcessMemory(hProcess, (LPCVOID)userinfoaddr.BigAvatar, userinfo.BigAvatar, (userinfoaddr.l_BigAvatar + 1) * sizeof(wchar_t), 0);
	userinfo.SmallAvatar = new wchar_t[userinfoaddr.l_SmallAvatar + 1];
	ReadProcessMemory(hProcess, (LPCVOID)userinfoaddr.SmallAvatar, userinfo.SmallAvatar, (userinfoaddr.l_SmallAvatar + 1) * sizeof(wchar_t), 0);
	userinfo.sex = userinfoaddr.sex;
}

SAFEARRAY* SearchContactByNet(DWORD pid,wchar_t* keyword) {
	DWORD dwReadSize = 0;
	WeChatProcess hp(pid);
	if (!hp.m_init) return NULL;
	DWORD SearchContactByNetRemoteAddr = hp.GetProcAddr(SearchContactByNetRemote);
	if (SearchContactByNetRemoteAddr == 0)
		return NULL;
	WeChatData<wchar_t*> r_keyword(hp.GetHandle(), keyword, TEXTLENGTH(keyword));
	if (r_keyword.GetAddr() == 0)
		return NULL;
	ClearUserInfoCache();
	DWORD ret = CallRemoteFunction(hp.GetHandle(), SearchContactByNetRemoteAddr, r_keyword.GetAddr());
	if (ret == 0)
		return NULL;
	ReadProcessMemory(hp.GetHandle(), (LPCVOID)ret, &userinfoaddr, sizeof(UserInfoAddr), &dwReadSize);
	if (userinfoaddr.errcode == 0) {
		ReadUserInfoFromMemory(hp.GetHandle());
		SAFEARRAY* psa = CreateUserInfoArray();
		return psa;
	}
	return NULL;
}