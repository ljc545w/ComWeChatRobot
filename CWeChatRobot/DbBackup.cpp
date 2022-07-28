#include "pch.h"

// 调用DLL接口时的参数
struct BackupParams {
	DWORD ptrDb;
	DWORD savepath;
};

BOOL BackupSQLiteDB(DWORD pid,DWORD DbHandle, BSTR savepath) {
	WeChatProcess hp(pid);
	if (!hp.m_init) return 1;
	DWORD BackupSQLiteDBRemoteAddr = hp.GetProcAddr(BackupSQLiteDBRemote);
	if (BackupSQLiteDBRemoteAddr == 0)
		return 1;
	char* a_savepath = _com_util::ConvertBSTRToString(savepath);
	WeChatData<char*> r_savepath(hp.GetHandle(), a_savepath, TEXTLENGTHA(a_savepath));
	BackupParams params = { 0 };
	params.ptrDb = DbHandle;
	params.savepath = (DWORD)r_savepath.GetAddr();
	WeChatData<BackupParams*> r_params(hp.GetHandle(), &params, sizeof(params));
	if (r_savepath.GetAddr() == 0 || r_params.GetAddr() == 0)
		return 1;
	DWORD ret = CallRemoteFunction(hp.GetHandle(), BackupSQLiteDBRemoteAddr, r_params.GetAddr());
	return ret == 0;
}