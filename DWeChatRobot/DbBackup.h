#pragma once
#include<windows.h>
/*
* 外部调用时传递的参数类型
* DbHandle：要备份的数据库句柄
* BackupFile：备份的保存位置
*/
struct BackupStruct {
	DWORD DbHandle;
	char* BackupFile;
};
int BackupSQLiteDB(DWORD DbHandle, const char* BackupFile);
extern "C" __declspec(dllexport) int BackupSQLiteDBRemote(LPVOID lpParameter);