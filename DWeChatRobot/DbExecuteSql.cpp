#include "pch.h"

// sqlite3_callback����ָ��
typedef int(*sqlite3_callback)(
	void*,
	int,
	char**,
	char**
);

// sqlite3_exec����ָ��
typedef int(__cdecl* Sqlite3_exec)(
	DWORD,					/* The database on which the SQL executes */
	const char*,            /* The SQL to be executed */
	sqlite3_callback,       /* Invoke this callback routine */
	void*,                  /* First argument to xCallback() */
	char**                  /* Write error messages here */
);

DWORD WeChatWinBase = GetWeChatWinBase();
// sqlite3_exec������ַ
DWORD sqlite3_execAddr = WeChatWinBase + OffsetFromIdaAddr(IDA_SQLITE3_EXEC_ADDRESS);

/*
* �ⲿ����ʱ���ݵĲ����ṹ
* ptrDb�����ݿ���
* ptrSql������sql�ĵ�ַ
*/
#ifndef USE_SOCKET
struct executeParams {
	DWORD ptrDb;
	DWORD ptrSql;
};
#endif

/*
* �����ѯ����Ľṹ
* ColName���ֶ�����l_ColName��`ColName`�ַ���
* content���ֶ�ֵ��l_content��`content`�ַ���
*/
struct SQLResultStruct {
	char* ColName;
	DWORD l_ColName;
	char* content;
	DWORD l_content;
	BOOL  isblob;
};

/*
* �ⲿ����ʱ�ķ�������
* SQLResultAddr��`SQLResult`�׳�Ա��ַ
* length����ѯ�������
*/
struct executeResult {
	DWORD SQLResultAddr;
	DWORD length;
};

// �ⲿ����ʱ�ľ��巵�ض���
executeResult result = { 0 };
// �����ѯ����Ķ�ά��̬����
vector <vector<SQLResultStruct>> SQLResult;

/*
* ��ȡ���ݿ���Ϣ�Ļص�����
*/
int GetDbInfo(void* data,int argc,char** argv,char** azColName) {
	DbInfoStruct* pdata = (DbInfoStruct*)data;
	TableInfoStruct tb = { 0 };
	if (argv[1])
	{
		tb.name = new char[strlen(argv[1]) + 1];
		memcpy(tb.name,argv[1],strlen(argv[1]) + 1);
	}
	else {
		tb.name = (char*)"NULL";
	}
	if (argv[2])
	{
		tb.tbl_name = new char[strlen(argv[2]) + 1];
		memcpy(tb.tbl_name, argv[2], strlen(argv[2]) + 1);
	}
	else {
		tb.tbl_name = (char*)"NULL";
	}
	if (argv[3])
	{
		tb.rootpage = new char[strlen(argv[3]) + 1];
		memcpy(tb.rootpage, argv[3], strlen(argv[3]) + 1);
	}
	else {
		tb.rootpage = (char*)"NULL";
	}
	if (argv[4])
	{
		tb.sql = new char[strlen(argv[4]) + 1];
		memcpy(tb.sql, argv[4], strlen(argv[4]) + 1);
	}
	else {
		tb.sql = (char*)"NULL";
	}
	tb.l_name = strlen(tb.name);
	tb.l_tbl_name = strlen(tb.tbl_name);
	tb.l_sql = strlen(tb.sql);
	tb.l_rootpage = strlen(tb.rootpage);
	pdata->tables.push_back(tb);
	pdata->count = pdata->tables.size();
	return 0;
}

/*
* DLL�ڲ���ѯ�õĻص�������ֱ����ʾ��ѯ������ô�����
*/
int query(void* data, int argc, char** argv, char** azColName) {
	for (int i = 0; i < argc; i++) {
		string content = argv[i] ? UTF8ToGBK(argv[i]) : "NULL";
		cout << azColName[i] << " = " << content << endl;
	}
	printf("\n");
	return 0;
}

/*
* �ⲿ����ʱʹ�õĻص����������������`SQLResult`��
* return��int��ִ�гɹ�����`0`��ִ��ʧ�ܷ��ط�0ֵ
*/
int selectdbinfo(void* data, int argc, char** argv, char** azColName) {
	executeResult* pdata = (executeResult*)data;
	vector<SQLResultStruct> tempStruct;
	for (int i = 0; i < argc; i++) {
		SQLResultStruct temp = { 0 };
		temp.ColName = new char[strlen(azColName[i]) + 1];
		memcpy(temp.ColName, azColName[i], strlen(azColName[i]) + 1);
		temp.l_ColName = strlen(azColName[i]);
		if (argv[i]) {
			temp.content = new char[strlen(argv[i]) + 1];
			memcpy(temp.content, argv[i], strlen(argv[i]) + 1);
			temp.l_content = strlen(argv[i]);
		}
		else {
			temp.content = new char[2];
			ZeroMemory(temp.content, 2);
			temp.l_content = 0;
		}
		tempStruct.push_back(temp);
	}
	SQLResult.push_back(tempStruct);
	pdata->length++;
	return 0;
}

/*
* ��ղ�ѯ������ͷ��ڴ�
* return��void
*/
void ClearResultArray() {
	if (SQLResult.size() == 0)
		return;
	for(unsigned int i = 0; i < SQLResult.size(); i++) {
		for (unsigned j = 0; j < SQLResult[i].size(); j++) {
			SQLResultStruct* sr = (SQLResultStruct*)&SQLResult[i][j];
			if (sr->ColName) {
				delete sr->ColName;
				sr->ColName = NULL;
			}
			if (sr->content) {
				delete sr->content;
				sr->content = NULL;
			}
		}
		SQLResult[i].clear();
	}
	SQLResult.clear();
	result.SQLResultAddr = 0;
	result.length = 0;
}

/*
* ִ��SQL����ں���
* ptrDb�����ݿ���
* sql��Ҫִ�е�SQL
* callback���ص�������ַ
* data�����ݸ��ص������Ĳ���
* return��void*��ִ�гɹ���������ָ�룬ִ��ʧ�ܷ���`0`
*/
void* ExecuteSQL(DWORD ptrDb,const char* sql,DWORD callback,void* data) {
	Sqlite3_exec p_Sqlite3_exec = (Sqlite3_exec)sqlite3_execAddr;
	int status = p_Sqlite3_exec(ptrDb,sql, (sqlite3_callback)callback,data,0);
	if (status != SQLITE_OK)
		return NULL;
	return SQLResult.data();
}

/*
* ���ⲿ���õ�ִ��SQL�ӿ�
* lpParameter��`executeParams`���ͽṹ��ָ��
* return��DWORD�����SQLִ�гɹ�������`SQLResult`�׳�Ա��ַ�����򷵻�0
*/
#ifndef USE_SOCKET
DWORD ExecuteSQLRemote(LPVOID lpParameter){
	ClearResultArray();
	executeParams* sqlparam = (executeParams*)lpParameter;
	void* status = ExecuteSQL(sqlparam->ptrDb, (const char*)sqlparam->ptrSql, (DWORD)selectdbinfo, &result);
	
	if (status != NULL) {
		result.SQLResultAddr = (DWORD)SQLResult.data();
		return (DWORD)&result;
	}
	else {
		result.length = 0;
	}
	return 0;
}
#endif

void* SelectData(DWORD db,const char* sql,void* data)
{
	executeResult* pdata = (executeResult*)data;
	DWORD wxBaseAddress = GetWeChatWinBase();
	Sqlite3_prepare p_Sqlite3_prepare = (Sqlite3_prepare)(wxBaseAddress + OffsetFromIdaAddr(IDA_SQLITE3_PREPARE_ADDRESS));
	Sqlite3_step p_Sqlite3_step = (Sqlite3_step)(wxBaseAddress + OffsetFromIdaAddr(IDA_SQLITE3_STEP_ADDRESS));
	Sqlite3_column_count p_Sqlite3_column_count = (Sqlite3_column_count)(wxBaseAddress + OffsetFromIdaAddr(IDA_SQLITE3_COLUMN_COUNT_ADDRESS));
	Sqlite3_column_name p_Sqlite3_column_name = (Sqlite3_column_name)(wxBaseAddress + OffsetFromIdaAddr(IDA_SQLITE3_COLUMN_NAME_ADDRESS));
	Sqlite3_column_type p_Sqlite3_column_type = (Sqlite3_column_type)(wxBaseAddress + OffsetFromIdaAddr(IDA_SQLITE3_COLUMN_TYPE_ADDRESS));
	Sqlite3_column_blob p_Sqlite3_column_blob = (Sqlite3_column_blob)(wxBaseAddress + OffsetFromIdaAddr(IDA_SQLITE3_COLUMN_BLOB_ADDRESS));
	Sqlite3_column_bytes p_Sqlite3_column_bytes = (Sqlite3_column_bytes)(wxBaseAddress + OffsetFromIdaAddr(IDA_SQLITE3_COLUMN_BYTES_ADDRESS));
	Sqlite3_finalize p_Sqlite3_finalize = (Sqlite3_finalize)(wxBaseAddress + OffsetFromIdaAddr(IDA_SQLITE3_FINALIZE_ADDRESS));
	DWORD* stmt;
	int rc = p_Sqlite3_prepare(db, sql, -1, &stmt, 0);
	if (rc != SQLITE_OK)
		return NULL;
	while (p_Sqlite3_step(stmt) == SQLITE_ROW)
	{
		int col_count = p_Sqlite3_column_count(stmt);
		vector<SQLResultStruct> tempStruct;
		for (int i = 0; i < col_count; i++) {
			SQLResultStruct temp = { 0 };
			const char* ColName = p_Sqlite3_column_name(stmt, i);
			int nType = p_Sqlite3_column_type(stmt, i);
			const void* pReadBlobData = p_Sqlite3_column_blob(stmt, i);
			int nLength = p_Sqlite3_column_bytes(stmt, i);
			temp.ColName = new char[strlen(ColName) + 1];
			memcpy(temp.ColName, ColName, strlen(ColName) + 1);
			temp.l_ColName = strlen(ColName);
			temp.l_content = nLength;
			switch (nType)
			{
			case SQLITE_BLOB: {
				temp.content = new char[nLength];
				memcpy(temp.content, pReadBlobData, nLength);
				temp.isblob = true;
				break;
			}
			default: {
				if (nLength != 0) {
					temp.content = new char[nLength + 1];
					memcpy(temp.content, pReadBlobData, nLength + 1);
				}
				else {
					temp.content = new char[2];
					ZeroMemory(temp.content, 2);
				}
				temp.isblob = false;
				break;
			}
			}
			tempStruct.push_back(temp);
		}
		SQLResult.push_back(tempStruct);
		pdata->length++;
	}
	p_Sqlite3_finalize(stmt);
	return SQLResult.data();
}

#ifndef USE_SOCKET
int SelectDataRemote(LPVOID lpParameter) {
	ClearResultArray();
	executeParams* sqlparam = (executeParams*)lpParameter;
	void* status = SelectData(sqlparam->ptrDb, (const char*)sqlparam->ptrSql, &result);

	if (status != NULL) {
		result.SQLResultAddr = (DWORD)SQLResult.data();
		return (DWORD)&result;
	}
	else {
		result.length = 0;
	}
	return 0;
}
#endif