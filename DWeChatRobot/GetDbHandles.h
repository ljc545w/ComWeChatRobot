#pragma once
#include<windows.h>
#include<vector>

/*
* 保存数据库单个表信息的结构体
* name：表名；l_name：`name`字符数
* tbl_name：表名；l_tbl_name：`tbl_name`字符数
* sql：建表语句；l_sql：`sql`字符数
* rootpage：表编号；l_rootpage：`rootpage`字符数
*/
struct TableInfoStruct {
    char* name;
    DWORD l_name;
    char* tbl_name;
    DWORD l_tbl_name;
    char* sql;
    DWORD l_sql;
    char* rootpage;
    DWORD l_rootpage;
};

/*
* 保存数据库信息的结构体
* handle：数据库句柄
* dbname：数据库名
* l_dbname：`dbname`字符数
* tables：保存库中所有表信息的容器
* count：库中表的数量
*/
struct DbInfoStruct {
    DWORD handle;
    wchar_t* dbname;
    DWORD l_dbname;
    vector<TableInfoStruct> tables;
    DWORD count;
};

void GetDbHandles();
extern "C" __declspec(dllexport) DWORD GetDbHandlesRemote();
DWORD GetDbHandleByDbName(wchar_t* dbname);