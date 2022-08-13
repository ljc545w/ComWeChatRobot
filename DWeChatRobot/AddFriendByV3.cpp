#include "pch.h"

// Ìí¼ÓºÃÓÑCALL1Æ«ÒÆ
#define AddFriendByV3Call1Offset 0x78784540 - 0x786A0000
// Ìí¼ÓºÃÓÑCALL2Æ«ÒÆ
#define AddFriendByV3Call2Offset 0x78E11890 - 0x786A0000
// Ìí¼ÓºÃÓÑCALL3Æ«ÒÆ
#define AddFriendByV3Call3Offset 0x78E11980 - 0x786A0000
// Ìí¼ÓºÃÓÑCALL4Æ«ÒÆ
#define AddFriendByV3Call4Offset 0x78B03970 - 0x786A0000
// Ìí¼ÓºÃÓÑ²ÎÊýÆ«ÒÆ
#define AddFriendByV3ParamOffset 0x7AA068F4 - 0x786A0000

struct AddFriendByV3ParamStruct
{
    DWORD fill0 = 0x0;
    DWORD fill1 = 0x0;
    DWORD fill2 = -0x1;
    DWORD fill3 = 0x0;
    DWORD fill4 = 0x0;
    DWORD fill5 = 0xF;
    char nullbuffer[0xC] = {0};
};

#ifndef USE_SOCKET
struct AddFriendByV3Struct
{
    wchar_t *wxid;
    wchar_t *message;
    int AddType;
};
#endif

#ifndef USE_SOCKET
BOOL AddFriendByV3Remote(LPVOID lpParameter)
{
    AddFriendByV3Struct *afbvs = (AddFriendByV3Struct *)lpParameter;
    BOOL isSuccess = AddFriendByV3(afbvs->wxid, afbvs->message, afbvs->AddType);
    return isSuccess;
}
#endif

BOOL __stdcall AddFriendByV3(wchar_t *v3, wchar_t *message, int AddType)
{
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD AddFriendByV3Call1 = WeChatWinBase + AddFriendByV3Call1Offset;
    DWORD AddFriendByV3Call2 = WeChatWinBase + AddFriendByV3Call2Offset;
    DWORD AddFriendByV3Call3 = WeChatWinBase + AddFriendByV3Call3Offset;
    DWORD AddFriendByV3Call4 = WeChatWinBase + AddFriendByV3Call4Offset;
    DWORD AddFriendByV3ParamAddr = WeChatWinBase + AddFriendByV3ParamOffset;

    WxString pv3(v3);
    AddFriendByV3ParamStruct AddFriendParam;

    char *sv3 = new char[wcslen(v3) + 1];
    ZeroMemory(sv3, wcslen(v3) + 1);
    WideCharToMultiByte(CP_ACP, 0, v3, -1, sv3, wcslen(v3), NULL, NULL);
    pv3.fill1 = (DWORD)sv3;
    pv3.fill2 = wcslen(v3);
    wchar_t *pmessage = message ? message : (wchar_t *)L"";
    BOOL isSuccess = 0x0;

    __asm {
		pushad;
		pushfd;
        // ÊÖ»úºÅ 0xF£¬Î¢ÐÅºÅ 0x3£¬QQºÅ 0x1£¬wxid 0x6
		mov edi, [AddType];
		mov esi, 0x0;
		sub esp, 0x18;
		mov eax, esp;
		mov dword ptr[eax], 0x0;
		mov dword ptr[eax + 0x14], 0xF;
		mov dword ptr[eax + 0x10], 0x0;
		sub esp, 0x18;
		lea eax, AddFriendParam;
		mov ecx, esp;
		push eax;
		call AddFriendByV3Call1;
		push esi;
		push edi;
		mov edi, pmessage;
		sub esp, 0x14;
		mov ecx, esp;
		push -0x1;
		mov eax, edi;
		push eax;
		call AddFriendByV3Call2;
		push 0x2;
		lea eax, pv3;
		sub esp, 0x14;
		mov ecx, esp;
		push eax;
		call AddFriendByV3Call3;
		mov eax, [AddFriendByV3ParamAddr];
		mov eax, [eax];
		mov ecx, eax;
		call AddFriendByV3Call4;
		mov isSuccess, eax;
		popfd;
		popad;
    }
    return isSuccess;
}
