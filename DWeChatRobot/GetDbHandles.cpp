#include "pch.h"

// 联系人相关库偏移
#define SqlHandleMicroMsgOffset  0x2366934
// 公众号相关库偏移
#define SqlHandlePublicMsgOffset 0x239E3C8
// 聊天记录相关库偏移
#define SqlHandleMSGOffset 0x239FF68

// 保存数据库信息的容器
vector<DbInfoStruct> dbs;

/*
* 根据数据库名从`dbs`中检索数据库句柄
* dbname：数据库名
* return：DWORD，如果检索成功，返回数据库句柄，否则返回`0`
*/
DWORD GetDbHandleByDbName(wchar_t* dbname) {
	if (dbs.size() == 0)
		GetDbHandles();
	for (unsigned int i = 0; i < dbs.size() - 1; i++) {
		if (!lstrcmpW(dbs[i].dbname, dbname))
			return dbs[i].handle;
	}
	return 0;
}

/*
* 供外部调用的获取数据库信息接口
* return：DWORD，`dbs`首个成员地址
*/
#ifndef USE_SOCKET
DWORD GetDbHandlesRemote() {
	if (dbs.size() == 0)
		GetDbHandles();
	return (DWORD)dbs.data() ;
}
#endif

/*
* 获取数据库信息的具体实现
* return：void
*/
void* GetDbHandles() {
	dbs.clear();
	DWORD WeChatWinBase = GetWeChatWinBase();
	DWORD SqlHandleBaseAddr = WeChatWinBase + SqlHandleMicroMsgOffset;
	DWORD SqlHandleBeginAddr = 0x0;
	DWORD SqlHandleEndAddr = 0x0;
	DWORD SqlHandlePublicMsgAddr = *(DWORD*)(WeChatWinBase + SqlHandlePublicMsgOffset);
	DWORD SqlHandleMSGAddr = *(DWORD*)(WeChatWinBase + SqlHandleMSGOffset);
	__asm {
		mov eax, [SqlHandleBaseAddr];
		mov ecx, [eax];
		add ecx, 0x1428;
		mov eax, [ecx];
		mov SqlHandleBeginAddr, eax;
		mov eax, [ecx + 0x4];
		mov SqlHandleEndAddr, eax;
	}
	DWORD dwHandle = 0x0;
	wstring dbnames = L"";
	// 获取联系人数据库句柄
	while (SqlHandleBeginAddr < SqlHandleEndAddr) {
		dwHandle = *(DWORD*)SqlHandleBeginAddr;
		SqlHandleBeginAddr += 0x4;
		if (SqlHandleBeginAddr == SqlHandleEndAddr)
			break;
		if(dbnames.find(L"|" + (wstring)(wchar_t*)(*(DWORD*)(dwHandle + 0x50)) + L"|",0) != wstring::npos)
			continue;
		DbInfoStruct db = { 0 };
		dbnames = dbnames + L"|" + (wchar_t*)(*(DWORD*)(dwHandle + 0x50)) + L"|";
		db.dbname = (wchar_t*)(*(DWORD*)(dwHandle + 0x50));
		db.l_dbname = wcslen(db.dbname);
		db.handle = *(DWORD*)(dwHandle + 0x3C);
		ExecuteSQL(*(DWORD*)(dwHandle + 0x3C), "select * from sqlite_master where type=\"table\";",(DWORD)GetDbInfo,&db);
		dbs.push_back(db);
	}
	// 获取公众号数据库句柄
	for (int i = 1; i < 4; i++) {
		dwHandle = *((DWORD*)(SqlHandlePublicMsgAddr + i * 0x4));
		if (dbnames.find(L"|" + (wstring)(wchar_t*)(*(DWORD*)(dwHandle + 0x50)) + L"|", 0) != wstring::npos)
			continue;
		DbInfoStruct db = { 0 };
		dbnames = dbnames + L"|" + (wchar_t*)(*(DWORD*)(dwHandle + 0x50)) + L"|";
		db.dbname = (wchar_t*)(*(DWORD*)(dwHandle + 0x50));
		db.l_dbname = wcslen(db.dbname);
		db.handle = *(DWORD*)(dwHandle + 0x3C);
		ExecuteSQL(*(DWORD*)(dwHandle + 0x3C), "select * from sqlite_master where type=\"table\";", (DWORD)GetDbInfo, &db);
		dbs.push_back(db);
	}
	// 获取聊天记录数据库句柄
	int msgdb_count = *(int*)(SqlHandleMSGAddr + 0x4);
	DWORD MsgdwHandle = *(DWORD*)(SqlHandleMSGAddr + 0x1C);
	for (int i = 0; i < msgdb_count; i++) {
		for (int j = 0; j < 4; j++) {
			dwHandle = *(DWORD*)(MsgdwHandle + 0x14 + j * 4);
			if (dbnames.find(L"|" + (wstring)(wchar_t*)(*(DWORD*)(dwHandle + 0x50)) + L"|", 0) != wstring::npos)
				continue;
			DbInfoStruct db = { 0 };
			dbnames = dbnames + L"|" + (wchar_t*)(*(DWORD*)(dwHandle + 0x50)) + L"|";
			db.dbname = (wchar_t*)(*(DWORD*)(dwHandle + 0x50));
			db.l_dbname = wcslen(db.dbname);
			db.handle = *(DWORD*)(dwHandle + 0x3C);
			ExecuteSQL(*(DWORD*)(dwHandle + 0x3C), "select * from sqlite_master where type=\"table\";", (DWORD)GetDbInfo, &db);
			dbs.push_back(db);
		}
		MsgdwHandle += 0x68;
	}
	// 添加一个空结构体，作为读取结束标志
	DbInfoStruct db_end = { 0 };
	dbs.push_back(db_end);
#ifdef _DEBUG
	for (unsigned int i = 0; i < dbs.size() - 1; i++) {
		printf("dbname = %ws,handle = 0x%08X,table_count:%d\n",dbs[i].dbname,dbs[i].handle,dbs[i].tables.size());
		for (unsigned int j = 0; j < dbs[i].tables.size();j++) {
			cout << "name     = " << dbs[i].tables[j].name << endl;
			cout << "tbl_name = " << dbs[i].tables[j].tbl_name << endl;
			cout << "rootpage = " << dbs[i].tables[j].rootpage << endl;
			cout << "sql      = " << dbs[i].tables[j].sql << endl;
			cout << endl;
		}
		cout << endl;
	}
#endif
	return dbs.data();
}