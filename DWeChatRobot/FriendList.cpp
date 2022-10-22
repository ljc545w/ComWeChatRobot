#include "pch.h"
#include <vector>

// 通讯录左树偏移
#define LeftTreeOffset 0x23668F4

// 保存所有好友信息的动态数组
vector<WxFriendStruct> WxFriendList;

#ifndef USE_SOCKET
/*
 * 供外部调用的获取好友列表接口1
 * return：int，联系人数量
 */
int GetFriendListInit()
{
    GetFriendList();
#ifdef _DEBUG
    cout << WxFriendList.size() << endl;
#endif
    return WxFriendList.size() == 0 ? 0 : WxFriendList.size() - 1;
}

/*
 * 供外部调用的获取好友列表接口2
 * return：DWORD，WxFriendList第一个成员地址
 */
DWORD GetFriendListRemote()
{
    if (WxFriendList.size() == 0 || WxFriendList.size() - 1 == 0)
        return 0;
#ifdef _DEBUG
    printf("0x%08X\n", (DWORD)&WxFriendList[0]);
    printf("0x%08X\n", (DWORD) & (WxFriendList[0].wxIdAddr));
#endif
    return (DWORD)&WxFriendList[0].wxIdAddr;
}

/*
 * 供外部调用的获取好友列表接口3，清空缓存
 * return：void
 */
void GetFriendListFinish()
{
    WxFriendList.clear();
    cout << WxFriendList.size() << endl;
}
#else
vector<WxFriendStruct> GetWxContact()
{
    WxFriendList.clear();
    GetFriendList();
    return WxFriendList;
}
#endif
/*
 * 获取好友列表的具体实现
 * return：void
 */
WxFriendStruct *__stdcall GetFriendList()
{
#ifdef _DEBUG
    wcout.imbue(locale("chs"));
#endif
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD LeftTreeAddr = 0;
    DWORD RightTreeAddr = 0;
    DWORD LeftTreeHead = 0;
    DWORD baseAddr = WeChatWinBase + LeftTreeOffset;
    __asm {
		pushad;
		mov eax, dword ptr[baseAddr];
		mov eax, dword ptr[eax];
		mov eax, dword ptr[eax + 0x4C];
		mov ecx, dword ptr[eax];
		mov LeftTreeAddr, ecx;
		mov LeftTreeHead, eax;
		mov ecx, dword ptr[eax + 0x4];
		mov RightTreeAddr, ecx;
		popad;
    }

    while (1)
    {
        DWORD wxIdAddr = 0;
        DWORD wxNumberAddr = 0;
        DWORD wxNickNameAddr = 0;
        DWORD wxRemarkAddr = 0;
        DWORD wxTypeAddr = 0;
        DWORD wxVerifyFlagAddr = 0;

        __asm {
			pushad;
			mov eax, dword ptr [LeftTreeAddr];
			mov ecx, eax;
			add ecx, 0x30;
			mov wxIdAddr, ecx;
			mov ecx, eax;
			add ecx, 0x44;
			mov wxNumberAddr, ecx;
			mov ecx, eax;
			add ecx, 0x8C;
			mov wxNickNameAddr, ecx;
			mov ecx, eax;
			add ecx, 0x78;
			mov wxRemarkAddr, ecx;
            mov ecx, eax;
            add ecx, 0x70;
            mov wxTypeAddr,ecx;
            mov ecx, eax;
            add ecx, 0x74;
            mov wxVerifyFlagAddr,ecx;
			mov ecx, dword ptr [eax];
			mov LeftTreeAddr, ecx;
			popad;
        }
        WxFriendStruct p(wxIdAddr, wxNumberAddr, wxNickNameAddr, wxRemarkAddr, wxTypeAddr, wxVerifyFlagAddr);
        WxFriendList.push_back(p);
#ifdef _DEBUG
        wcout << (wchar_t *)(*(DWORD *)p.wxIdAddr) << endl;
#endif
        if (LeftTreeAddr == LeftTreeHead)
        {
            break;
        }
    }
    WxFriendStruct nullp(NULL, NULL, NULL, NULL, NULL, NULL);
    WxFriendList.push_back(nullp);
    return WxFriendList.data();
}
