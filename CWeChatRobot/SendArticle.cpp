#include "pch.h"

struct SendArticleStruct {
	DWORD wxid;
	DWORD title;
	DWORD abstract;
	DWORD url;
};

BOOL SendArticle(wchar_t* wxid, wchar_t* title, wchar_t* abstract, wchar_t* url) {
	if (!hProcess)
		return 1;
	DWORD WeChatRobotBase = GetWeChatRobotBase();
	DWORD dwId = 0;
	DWORD dwWriteSize = 0;
	SendArticleStruct params;
	ZeroMemory(&params, sizeof(params));
	DWORD SendArticleProcAddr = WeChatRobotBase + SendArticleOffset;
	LPVOID wxidaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
	LPVOID titleaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
	LPVOID abstractaddr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
	LPVOID urladdr = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
	SendArticleStruct* paramAndFunc = (SendArticleStruct*)::VirtualAllocEx(hProcess, 0, sizeof(SendArticleStruct), MEM_COMMIT, PAGE_READWRITE);
	if (!wxidaddr || !titleaddr || !abstractaddr || !urladdr ||
		!paramAndFunc || !WeChatRobotBase)
	{
		return 1;
	}

	if (wxidaddr)
		WriteProcessMemory(hProcess, wxidaddr, wxid, wcslen(wxid) * 2 + 2, &dwWriteSize);
	if (titleaddr)
		WriteProcessMemory(hProcess, titleaddr, title, wcslen(title) * 2 + 2, &dwWriteSize);
	if (abstractaddr)
		WriteProcessMemory(hProcess, abstractaddr, abstract, wcslen(abstract) * 2 + 2, &dwWriteSize);
	if (urladdr)
		WriteProcessMemory(hProcess, urladdr, url, wcslen(url) * 2 + 2, &dwWriteSize);
	params.wxid = (DWORD)wxidaddr;
	params.title = (DWORD)titleaddr;
	params.abstract = (DWORD)abstractaddr;
	params.url = (DWORD)urladdr;

	if (paramAndFunc)
		WriteProcessMemory(hProcess, paramAndFunc, &params, sizeof(params), &dwId);
	HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)SendArticleProcAddr, (LPVOID)paramAndFunc, 0, &dwId);
	if (hThread) {
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	}
	VirtualFreeEx(hProcess, wxidaddr, 0, MEM_RELEASE);
	VirtualFreeEx(hProcess, titleaddr, 0, MEM_RELEASE);
	VirtualFreeEx(hProcess, abstractaddr, 0, MEM_RELEASE);
	VirtualFreeEx(hProcess, urladdr, 0, MEM_RELEASE);
	VirtualFreeEx(hProcess, paramAndFunc, 0, MEM_RELEASE);
	return 0;
}