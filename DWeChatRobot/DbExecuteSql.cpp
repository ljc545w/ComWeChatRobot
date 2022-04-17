#include "pch.h"

#define sqlite3_execOffset 0x66176570 - 0x64E20000

typedef int(*sqlite3_callback)(
	void*,
	int,
	char**,
	char**
);

typedef int(__cdecl* Sqlite3_exec)(
	DWORD,					/* The database on which the SQL executes */
	const char*,            /* The SQL to be executed */
	sqlite3_callback,       /* Invoke this callback routine */
	void*,                  /* First argument to xCallback() */
	char**                  /* Write error messages here */
);

DWORD WeChatWinBase = GetWeChatWinBase();
DWORD sqlite3_execAddr = WeChatWinBase + sqlite3_execOffset;

struct executeParams {
	DWORD ptrDb;
	DWORD ptrSql;
};

struct SQLResultStruct {
	char* ColName;
	DWORD l_ColName;
	char* content;
	DWORD l_content;
};

struct executeResult {
	DWORD SQLResultAddr;
	DWORD length;
};

executeResult result = { 0 };
vector <vector<SQLResultStruct>> SQLResult;

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

int query(void* data, int argc, char** argv, char** azColName) {
	for (int i = 0; i < argc; i++) {
		string content = argv[i] ? UTF8ToGBK(argv[i]) : "NULL";
		cout << azColName[i] << " = " << content << endl;
	}
	printf("\n");
	return 0;
}

int select(void* data, int argc, char** argv, char** azColName) {
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

BOOL ExecuteSQL(DWORD ptrDb,const char* sql,DWORD callback,void* data) {
	Sqlite3_exec p_Sqlite3_exec = (Sqlite3_exec)sqlite3_execAddr;
	int status = p_Sqlite3_exec(ptrDb,sql, (sqlite3_callback)callback,data,0);
	return status == 0;
}

DWORD ExecuteSQLRemote(LPVOID lpParameter){
	ClearResultArray();
	executeParams* sqlparam = (executeParams*)lpParameter;
	BOOL status = ExecuteSQL(sqlparam->ptrDb, (const char*)sqlparam->ptrSql, (DWORD)select, &result);
	
	if (status) {
		result.SQLResultAddr = (DWORD)SQLResult.data();
		return (DWORD)&result;
	}
	else {
		result.length = 0;
	}
	return 0;
}