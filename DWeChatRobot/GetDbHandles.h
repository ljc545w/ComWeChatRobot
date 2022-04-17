#pragma once
#include<windows.h>
#include<vector>

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