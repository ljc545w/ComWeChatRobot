#include "pch.h"

// ��ϵ����ؿ�ƫ��
#define SqlHandleMicroMsgOffset  0x2366934
// ���ں���ؿ�ƫ��
#define SqlHandlePublicMsgOffset 0x239E3C8
// �����¼��ؿ�ƫ��
#define SqlHandleMSGOffset 0x239FF68

// �������ݿ���Ϣ������
vector<DbInfoStruct> dbs;

/*
* �������ݿ�����`dbs`�м������ݿ���
* dbname�����ݿ���
* return��DWORD����������ɹ����������ݿ��������򷵻�`0`
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
* ���ⲿ���õĻ�ȡ���ݿ���Ϣ�ӿ�
* return��DWORD��`dbs`�׸���Ա��ַ
*/
#ifndef USE_SOCKET
DWORD GetDbHandlesRemote() {
	if (dbs.size() == 0)
		GetDbHandles();
	return (DWORD)dbs.data() ;
}
#endif

/*
* ��ȡ���ݿ���Ϣ�ľ���ʵ��
* return��void
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
	// ��ȡ��ϵ�����ݿ���
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
	// ��ȡ���ں����ݿ���
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
	// ��ȡ�����¼���ݿ���
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
	// ���һ���սṹ�壬��Ϊ��ȡ������־
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