#include "pch.h"
#include <direct.h>
#include <io.h>

/*
Executable modules, 条目 126
 基址=78BD0000
 大小=02624000 (39993344.)
 入口=7A0FC59D WeChatWi.<模块入口点>
 名称=WeChatWi
 文件版本=3.5.0.46
 路径=D:\Tencent\WeChat\[3.5.0.46]\WeChatWin.dll

78FC8980    E8 FB730000     call    78FCFD80                         ; 表情位置：eax-3C，大小：eax-48
*/

#define SaveGifHookOffset (0x5D9A2E83 - 0x5D5B0000)
#define NextCallOffset (0x5D9A8910 - 0x5D5B0000)
CHAR originalRecieveCode[5] = { 0 };

BYTE bjmpcode[5] = { 0 };
DWORD SaveGifHookAddress = GetWeChatWinBase() + SaveGifHookOffset;
DWORD dwReternAddress = GetWeChatWinBase() + SaveGifHookOffset + 5;
DWORD NextCallAddr = GetWeChatWinBase() + NextCallOffset;
bool SaveGifHooked = false;

void  __declspec(naked) ExtractExpression()
{
	__asm
	{
		pushad;
		push eax;
		call OutputExpression;
		popad;
		call NextCallAddr;
		//跳回返回地址
		jmp dwReternAddress;
	}
}

void HookExtractExpression()
{
	HookAnyAddress(SaveGifHookAddress, ExtractExpression,originalRecieveCode);
	SaveGifHooked = true;
}

void CreateDir(const char* dir)
{
	int m = 0, n;
	string str1, str2;

	str1 = dir;
	str2 = str1.substr(0, 2);
	str1 = str1.substr(3, str1.size());

	while (m >= 0)
	{
		m = str1.find('\\');
		str2 += '\\' + str1.substr(0, m);
		n = _access(str2.c_str(), 0); //判断该目录是否存在
		if (n == -1)
		{
			int status = _mkdir(str2.c_str());     //创建目录
		}
		str1 = str1.substr(m + 1, str1.size());
	}
}

void CreateFileWithCurrentTime(char* filedir, char* filepostfix, DWORD filedata, DWORD filedatalen)
{
	//获取当前时间作为文件名
	time_t rawtime;
	struct tm* ptminfo = new struct tm;
	time(&rawtime);
	localtime_s(ptminfo, &rawtime);
	char currenttime[30] = { 0 };
	sprintf_s(currenttime, "%02d%02d%02d%02d%02d%02d", ptminfo->tm_year + 1900,
		ptminfo->tm_mon + 1, ptminfo->tm_mday, ptminfo->tm_hour, ptminfo->tm_min, ptminfo->tm_sec);

	//拼接完整的路径
	char filepath[MAX_PATH] = { 0 };
	sprintf_s(filepath, "%s%s%s", filedir, currenttime, filepostfix);
	//创建文件
	HANDLE hFile = CreateFileA(filepath, GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBoxA(NULL, "创建文件失败", "错误", 0);
		return;
	}
	//写入文件
	DWORD dwWrite = 0;
	WriteFile(hFile, (LPCVOID)filedata, filedatalen, &dwWrite, 0);
	//关闭句柄
	CloseHandle(hFile);
}

void __stdcall OutputExpression(DWORD dwExpressionAddr)
{
	//取出图片长度
	DWORD dwImageLen = *((DWORD*)(dwExpressionAddr + 0x4));
	//取出图片内容
	DWORD dwImage = *((DWORD*)(dwExpressionAddr));
	if (dwImageLen == 0) {
		return;
	}
	unsigned char magic_head[4] = { 0 };
	char postfix[5] = { 0 };
	memcpy(magic_head, (void*)dwImage, 3);
	//根据MAGIC头判断后缀
	if (magic_head[0] == 137 && magic_head[1] == 80 && magic_head[2] == 78)
	{
		strcpy_s(postfix, 5, ".png");
	}
	else if (magic_head[0] == 71 && magic_head[1] == 73 && magic_head[2] == 70)
	{
		strcpy_s(postfix, 5, ".gif");
	}
	else if (magic_head[0] == 255 && magic_head[1] == 216 && magic_head[2] == 255)
	{
		strcpy_s(postfix, 5, ".jpg");
	}
	//获取临时文件夹目录
	char temppath[MAX_PATH] = { 0 };
	GetTempPathA(MAX_PATH, temppath);
	char imagedir[25] = { "WeChatRecordExpressions" };

	//拼接存放微信表情的目录
	char WeChatExpressionsPath[MAX_PATH] = { 0 };
	sprintf_s(WeChatExpressionsPath, "%s%s\\", temppath, imagedir);
	//创建目录存放图片
	CreateDir(WeChatExpressionsPath);

	//保存表情图片
	CreateFileWithCurrentTime(WeChatExpressionsPath, postfix, dwImage, dwImageLen);
}

void UnHookExtractExpression()
{
	// 回复被覆盖的指令
	if (!SaveGifHooked)
		return;
	UnHookAnyAddress(SaveGifHookAddress, originalRecieveCode);
	PostMessage(HWND_BROADCAST, NULL, 0, 0);
	SaveGifHooked = false;
}