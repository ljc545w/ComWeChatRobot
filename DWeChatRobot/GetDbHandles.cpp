#include "pch.h"

// 联系人相关库偏移
#define SqlHandleMicroMsgOffset 0x2366934
// 公众号相关库偏移
#define SqlHandlePublicMsgOffset 0x239E3C8
// 聊天记录相关库偏移
#define SqlHandleMSGOffset 0x239FF68

// 保存数据库信息的容器
vector<DbInfoStruct> dbs;
map<wstring, DbInfoStruct> dbmap;

/*
 * 根据数据库名从`dbmap`中检索数据库句柄
 * dbname：数据库名
 * return：DWORD，如果检索成功，返回数据库句柄，否则返回`0`
 */
DWORD GetDbHandleByDbName(wchar_t *dbname)
{
    if (dbmap.size() == 0)
        GetDbHandles();
    if (dbmap.find(dbname) != dbmap.end())
        return dbmap[dbname].handle;
    return 0;
}

unsigned int GetLocalIdByMsgId(ULONG64 msgid, int &dbIndex)
{
    char sql[260] = {0};
    sprintf_s(sql, "select localId from MSG where MsgSvrID=%llu;", msgid);
    wchar_t dbname[20] = {0};
    for (int i = 0;; i++)
    {
        swprintf_s(dbname, L"MSG%d.db", i);
        DWORD handle = GetDbHandleByDbName(dbname);
        if (handle == 0)
            return 0;
        vector<vector<string>> result = SelectData(handle, (const char *)sql);
        if (result.size() == 0)
            continue;
        dbIndex = i + 1;
        return stoi(result[1][0]);
    }
    return 0;
}

/*
 * 供外部调用的获取数据库信息接口
 * return：DWORD，`dbs`首个成员地址
 */
#ifndef USE_SOCKET
DWORD GetDbHandlesRemote()
{
    if (dbs.size() == 0)
        GetDbHandles();
    return (DWORD)dbs.data();
}
#endif

/*
 * 获取数据库信息的具体实现
 * return：void
 */
vector<void *> GetDbHandles()
{
    dbs.clear();
    dbmap.clear();
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD SqlHandleBaseAddr = WeChatWinBase + SqlHandleMicroMsgOffset;
    DWORD SqlHandleBeginAddr = 0x0;
    DWORD SqlHandleEndAddr = 0x0;
    DWORD SqlHandlePublicMsgAddr = *(DWORD *)(WeChatWinBase + SqlHandlePublicMsgOffset);
    DWORD SqlHandleMSGAddr = *(DWORD *)(WeChatWinBase + SqlHandleMSGOffset);
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
    // 获取联系人数据库句柄
    while (SqlHandleBeginAddr < SqlHandleEndAddr)
    {
        dwHandle = *(DWORD *)SqlHandleBeginAddr;
        SqlHandleBeginAddr += 0x4;
        if (SqlHandleBeginAddr == SqlHandleEndAddr)
            break;
        wstring dbname = wstring((wchar_t *)(*(DWORD *)(dwHandle + 0x50)));
        if (dbmap.find(dbname) != dbmap.end())
            continue;
        DbInfoStruct db = {0};
        db.dbname = (wchar_t *)(*(DWORD *)(dwHandle + 0x50));
        db.l_dbname = wcslen(db.dbname);
        db.handle = *(DWORD *)(dwHandle + 0x3C);
        ExecuteSQL(*(DWORD *)(dwHandle + 0x3C), "select * from sqlite_master where type=\"table\";", (DWORD)GetDbInfo, &db);
        dbs.push_back(db);
        dbmap[dbname] = db;
    }
    // 获取公众号数据库句柄
    for (int i = 1; i < 4; i++)
    {
        dwHandle = *((DWORD *)(SqlHandlePublicMsgAddr + i * 0x4));
        wstring dbname = wstring((wchar_t *)(*(DWORD *)(dwHandle + 0x50)));
        if (dbmap.find(dbname) != dbmap.end())
            continue;
        DbInfoStruct db = {0};
        db.dbname = (wchar_t *)(*(DWORD *)(dwHandle + 0x50));
        db.l_dbname = wcslen(db.dbname);
        db.handle = *(DWORD *)(dwHandle + 0x3C);
        ExecuteSQL(*(DWORD *)(dwHandle + 0x3C), "select * from sqlite_master where type=\"table\";", (DWORD)GetDbInfo, &db);
        dbs.push_back(db);
        dbmap[dbname] = db;
    }
    // 获取聊天记录数据库句柄
    int msgdb_count = *(int *)(SqlHandleMSGAddr + 0x4);
    DWORD MsgdwHandle = *(DWORD *)(SqlHandleMSGAddr + 0x1C);
    for (int i = 0; i < msgdb_count; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            dwHandle = *(DWORD *)(MsgdwHandle + 0x14 + j * 4);
            wstring dbname = wstring((wchar_t *)(*(DWORD *)(dwHandle + 0x50)));
            if (dbmap.find(dbname) != dbmap.end())
                continue;
            DbInfoStruct db = {0};
            db.dbname = (wchar_t *)(*(DWORD *)(dwHandle + 0x50));
            db.l_dbname = wcslen(db.dbname);
            db.handle = *(DWORD *)(dwHandle + 0x3C);
            ExecuteSQL(*(DWORD *)(dwHandle + 0x3C), "select * from sqlite_master where type=\"table\";", (DWORD)GetDbInfo, &db);
            dbs.push_back(db);
            dbmap[dbname] = db;
        }
        MsgdwHandle += 0x68;
    }
    // 添加一个空结构体，作为读取结束标志
    DbInfoStruct db_end = {0};
    dbs.push_back(db_end);
#ifdef _DEBUG
    for (unsigned int i = 0; i < dbs.size() - 1; i++)
    {
        printf("dbname = %ws,handle = 0x%08X,table_count:%d\n", dbs[i].dbname, dbs[i].handle, dbs[i].tables.size());
        for (unsigned int j = 0; j < dbs[i].tables.size(); j++)
        {
            cout << "name     = " << dbs[i].tables[j].name << endl;
            cout << "tbl_name = " << dbs[i].tables[j].tbl_name << endl;
            cout << "rootpage = " << dbs[i].tables[j].rootpage << endl;
            cout << "sql      = " << dbs[i].tables[j].sql << endl;
            cout << endl;
        }
        cout << endl;
    }
#endif
    vector<void *> ret_array;
    for (unsigned int i = 0; i < dbs.size() - 1; i++)
        ret_array.push_back(&dbs[i]);
    return ret_array;
}
