#include "pch.h"

// ������״̬CALL1ƫ��
#define CheckFriendStatusCall1Offset 0x78784540 - 0x786A0000
// ������״̬CALL2ƫ��
#define CheckFriendStatusCall2Offset 0x78E11890 - 0x786A0000
// ������״̬CALL3ƫ��
#define CheckFriendStatusCall3Offset 0x78E11980 - 0x786A0000
// ������״̬CALL4ƫ��
#define CheckFriendStatusCall4Offset 0x78B03970 - 0x786A0000
// ������״̬����ƫ��
#define CheckFriendStatusParamOffset 0x7AA068F4 - 0x786A0000

// ����״̬��HOOK��ַƫ��
#define CheckFriendStatusHookOffset 0x10587373 - 0x10000000
// HOOK��CALLƫ��
#define CheckFriendStatusNextCallOffset 0x10587410 - 0x10000000
// HOOK��ת�ĵ�ַƫ��
#define CheckFriendStatusHookJmpBackOffset 0x10587378 - 0x10000000

// HOOK��CALL��ַ
DWORD CheckFriendStatusNextCallAddress = GetWeChatWinBase() + CheckFriendStatusNextCallOffset;
// HOOK��ת�ĵ�ַ
DWORD CheckFriendStatusHookJmpBackAddress = GetWeChatWinBase() + CheckFriendStatusHookJmpBackOffset;

// ����HOOKǰ���ֽ��룬���ڻָ�
char OldAsmCode[5] = { 0 };
// �Ƿ�HOOK��־
BOOL CheckFriendStatusHooked = false;
// �������״̬�벢��Ϊ���÷���
DWORD LocalFriendStatus = 0x0;

/*
* �����ڴ���ƽ���ջ
*/
struct FriendStatusParamStruct {
	DWORD fill0 = 0x0;
	DWORD fill1 = 0x0;
	DWORD fill2 = -0x1;
	DWORD fill3 = 0x0;
	DWORD fill4 = 0x0;
	DWORD fill5 = 0xF;
	char nullbuffer[0xC] = { 0 };
};

/*
* ����������������״̬�뷶Χ�򲻴���
* result������״̬��
* return��void
*/
void dealVerifyUserResult(DWORD result) {
	if (result < 0xB0 || result > 0xB5)
		return;
	LocalFriendStatus = result;
}

/*
* HOOK�ľ���ʵ�֣���¼״̬�벢��ת��������
*/
__declspec(naked) void doHookVerifyUserResult() {
	__asm {
		pushfd;
		pushad;
		mov eax, [esi];
		push eax;
		call dealVerifyUserResult;
		add esp, 0x4;
		popad;
		popfd;
		call CheckFriendStatusNextCallAddress;
		jmp CheckFriendStatusHookJmpBackAddress;
	}
}

/*
* ��ʼHOOK����״̬
* return��void
*/
VOID HookFriendStatusCode(){
	if (CheckFriendStatusHooked)
		return;
	DWORD WeChatWinBase = GetWeChatWinBase();
	DWORD dwHookAddress = WeChatWinBase + CheckFriendStatusHookOffset;
	HookAnyAddress(dwHookAddress, doHookVerifyUserResult, OldAsmCode);
	CheckFriendStatusHooked = true;
}

/*
* ȡ��HOOK����״̬
* return��void
*/
VOID UnHookFriendStatusCode() {
	if (!CheckFriendStatusHooked)
		return;
	DWORD WeChatWinBase = GetWeChatWinBase();
	DWORD dwHookAddress = WeChatWinBase + CheckFriendStatusHookOffset;
	UnHookAnyAddress(dwHookAddress,OldAsmCode);
	CheckFriendStatusHooked = false;
}

#ifndef USE_SOCKET
/*
* ���ⲿ���õļ�����״̬�ӿڣ���鲢����״̬��
* lparameter��Ҫ������ϵ��wxid�����ַ
* return��DWORD������״̬��
*/
DWORD CheckFriendStatusRemote(LPVOID lparameter) {
	CheckFriendStatus((wchar_t*)lparameter);
	return LocalFriendStatus;
}
#endif

/*
* ������״̬�ľ���ʵ��
* wxid��Ҫ������ϵ��wxid
* return��void
*/
VOID __stdcall CheckFriendStatus(wchar_t* wxid) {
	if (!CheckFriendStatusHooked)
		HookFriendStatusCode();
	LocalFriendStatus = 0x0;
	DWORD WeChatWinBase = GetWeChatWinBase();
	DWORD CheckFriendStatusCall1 = WeChatWinBase + CheckFriendStatusCall1Offset;
	DWORD CheckFriendStatusCall2 = WeChatWinBase + CheckFriendStatusCall2Offset;
	DWORD CheckFriendStatusCall3 = WeChatWinBase + CheckFriendStatusCall3Offset;
	DWORD CheckFriendStatusCall4 = WeChatWinBase + CheckFriendStatusCall4Offset;
	DWORD CheckFriendStatusParam = WeChatWinBase + CheckFriendStatusParamOffset;

	WxBaseStruct pwxid(wxid);
	FriendStatusParamStruct FriendStatusParam;

	char* swxid = new char[wcslen(wxid) + 1];
	ZeroMemory(swxid, wcslen(wxid) + 1);
	WideCharToMultiByte(CP_ACP, 0, wxid, -1, swxid, wcslen(wxid), NULL, NULL);
	pwxid.fill1 = (DWORD)swxid;
	pwxid.fill2 = wcslen(wxid);

	wchar_t* message = (WCHAR*)L"����";

	__asm {
		pushad;
		pushfd;
		mov edi, 0x6;
		mov esi, 0x0;
		sub esp, 0x18;
		mov ecx, esp;
		mov dword ptr[ecx], 0x0;
		mov dword ptr[ecx + 0x14], 0xF;
		mov dword ptr[ecx + 0x10], 0x0;
		sub esp, 0x18;
		lea eax, FriendStatusParam;
		mov ecx, esp;
		push eax;
		call CheckFriendStatusCall1;
		push esi;
		push edi;
		mov edi, message;
		sub esp, 0x14;
		mov ecx, esp;
		push -0x1;
		mov eax, edi;
		push eax;
		call CheckFriendStatusCall2;
		// ����ĳ�0x2������Ӻ��ѣ�0x1���������״̬
		push 0x1;
		lea eax, pwxid;
		sub esp, 0x14;
		mov ecx, esp;
		push eax;
		call CheckFriendStatusCall3;
		mov eax, [CheckFriendStatusParam];
		mov eax, [eax];
		mov ecx, eax;
		call CheckFriendStatusCall4;
		popfd;
		popad;
	}
	while (!LocalFriendStatus && CheckFriendStatusHooked) {
		Sleep(10);
	}
#ifdef _DEBUG
	printf("wxid:%ws,status:0x%02X\n", wxid,LocalFriendStatus);
#endif
	delete[] swxid;
	swxid = NULL;
	return;
}