#include "pch.h"

// sqlite3_exec函数偏移
#define sqlite3_execOffset 0x66176570 - 0x64E20000

// sqlite3_callback函数指针
typedef int(*sqlite3_callback)(
	void*,
	int,
	char**,
	char**
);

// sqlite3_exec函数指针
typedef int(__cdecl* Sqlite3_exec)(
	DWORD,					/* The database on which the SQL executes */
	const char*,            /* The SQL to be executed */
	sqlite3_callback,       /* Invoke this callback routine */
	void*,                  /* First argument to xCallback() */
	char**                  /* Write error messages here */
);

DWORD WeChatWinBase = GetWeChatWinBase();
// sqlite3_exec函数地址
DWORD sqlite3_execAddr = WeChatWinBase + sqlite3_execOffset;

/*
* 外部调用时传递的参数结构
* ptrDb：数据库句柄
* ptrSql：保存sql的地址
*/
struct executeParams {
	DWORD ptrDb;
	DWORD ptrSql;
};

/*
* 保存查询结果的结构
* ColName：字段名；l_ColName：`ColName`字符数
* content：字段值；l_content：`content`字符数
*/
struct SQLResultStruct {
	char* ColName;
	DWORD l_ColName;
	char* content;
	DWORD l_content;
};

/*
* 外部调用时的返回类型
* SQLResultAddr：`SQLResult`首成员地址
* length：查询结果条数
*/
struct executeResult {
	DWORD SQLResultAddr;
	DWORD length;
};

// 外部调用时的具体返回对象
executeResult result = { 0 };
// 保存查询结果的二维动态数组
vector <vector<SQLResultStruct>> SQLResult;

/*
* 获取数据库信息的回调函数
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
* DLL内部查询用的回调函数，直接显示查询结果，用处不大
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
* 外部调用时使用的回调函数，将结果存入`SQLResult`中
* return：int，执行成功返回`0`，执行失败返回非0值
*/
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

/*
* 清空查询结果，释放内存
* return：void
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
* 执行SQL的入口函数
* ptrDb：数据库句柄
* sql：要执行的SQL
* callback：回调函数地址
* data：传递给回调函数的参数
* return：BOOL，执行成功返回`1`，执行失败返回`0`
*/
BOOL ExecuteSQL(DWORD ptrDb,const char* sql,DWORD callback,void* data) {
	Sqlite3_exec p_Sqlite3_exec = (Sqlite3_exec)sqlite3_execAddr;
	int status = p_Sqlite3_exec(ptrDb,sql, (sqlite3_callback)callback,data,0);
	return status == 0;
}

/*
* 供外部调用的执行SQL接口
* lpParameter：`executeParams`类型结构体指针
* return：DWORD，如果SQL执行成功，返回`SQLResult`首成员地址，否则返回0
*/
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