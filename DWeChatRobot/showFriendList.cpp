#include "pch.h"
#include <vector> 
using namespace std;
#define LeftTreeOffsetA 0x21DD240


struct WxFriendStructA {
	int index;
	string wxid;
	string wxNumber;
	string wxNickName;
	string wxRemark;
	WxFriendStructA(int index_, DWORD wxIdAddr, DWORD wxNumberAddr, DWORD wxNickNameAddr, DWORD wxRemarkAddr) {
		index = index_;
		Wchar_tToString(wxid, (wchar_t*)((LPVOID*)wxIdAddr));
		if (wxNumberAddr)
			Wchar_tToString(wxNumber, (wchar_t*)((LPVOID*)wxNumberAddr));
		else
			wxNumber = "null";
		if (wxNickNameAddr)
			Wchar_tToString(wxNickName, (wchar_t*)((LPVOID*)wxNickNameAddr));
		else
			wxNickName = "null";
		if (wxRemarkAddr)
			Wchar_tToString(wxRemark, (wchar_t*)((LPVOID*)wxRemarkAddr));
		else
			wxRemark = "null";
	}
};


void __stdcall GetFriendListS() {

	DWORD WeChatWinBase = GetWeChatWinBase();
	DWORD LeftTreeAddr = 0;
	DWORD RightTreeAddr = 0;
	DWORD baseAddr = WeChatWinBase + LeftTreeOffsetA;
	vector<WxFriendStructA> WxFriendList;
	__asm {
		pushad;
		mov eax, dword ptr[baseAddr];
		mov eax, dword ptr[eax];
		mov eax, dword ptr[eax + 0x4C];
		mov ecx, dword ptr[eax];
		mov LeftTreeAddr, ecx;
		mov ecx, dword ptr[eax + 0x4];
		mov RightTreeAddr, ecx;
		popad;
	}
	int index = 0;
	// wcout.imbue(locale("chs"));
	while (1) {
		++index;
		DWORD wxIdAddr = 0;
		DWORD wxNumberAddr = 0;
		DWORD wxNickNameAddr = 0;
		DWORD wxRemarkAddr = 0;
		__asm {
			pushad;
			mov eax, dword ptr[LeftTreeAddr];
			mov ecx, dword ptr[eax + 0x30];
			mov wxIdAddr, ecx;
			mov ecx, dword ptr[eax + 0x44];
			mov wxNumberAddr, ecx;
			mov ecx, dword ptr[eax + 0x8C];
			mov wxNickNameAddr, ecx;
			mov ecx, dword ptr[eax + 0x78];
			mov wxRemarkAddr, ecx;
			mov ecx, dword ptr[eax];
			mov LeftTreeAddr, ecx;
			popad;
		}
		if (wxIdAddr == 0xBAADF00D || wxIdAddr == 0x0 || wxIdAddr == -1 || IsBadWritePtr((LPVOID*)wxIdAddr, 10)) {
			break;
		}
		else {
			WxFriendStructA p(index, wxIdAddr, wxNumberAddr, wxNickNameAddr, wxRemarkAddr);
			WxFriendList.push_back(p);

		}
	}

	for (unsigned int i = 0; i < WxFriendList.size(); i++) {
		cout << WxFriendList[i].index << "    ";
		cout << WxFriendList[i].wxid << "    ";
		cout << WxFriendList[i].wxNumber << "    ";
		cout << WxFriendList[i].wxNickName << "    ";
		cout << WxFriendList[i].wxRemark << "    ";
		cout << endl;
	}
	WxFriendList.clear();
}