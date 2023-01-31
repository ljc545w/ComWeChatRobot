#include "pch.h"

struct TableInfoAddrStruct
{
    DWORD name;
    DWORD l_name;
    DWORD tbl_name;
    DWORD l_tbl_name;
    DWORD sql;
    DWORD l_sql;
    DWORD rootpage;
    DWORD l_rootpage;
};

struct DbInfoAddrStruct
{
    DWORD handle;
    DWORD dbname;
    DWORD l_dbname;
    // vector的数据结构
#ifdef _DEBUG
    DWORD v_head;
#endif
    DWORD v_data;
    DWORD v_end1;
    DWORD v_end2;
    // 表的数量
    DWORD count;
    DWORD extrainfo;
};

struct TableInfoStruct
{
    std::string name;
    std::string tbl_name;
    std::string sql;
    std::string rootpage;
};

struct DbInfoStruct
{
    DWORD handle;
    std::wstring dbname;
    vector<TableInfoStruct> tables;
    DWORD count;
};

vector<DbInfoStruct> dbs;

SAFEARRAY *CreateDbInfoSafeArray()
{
    SAFEARRAY *psaValue;
    ULONG count = 0;
    HRESULT hr = S_OK;
    for (unsigned int i = 0; i < dbs.size(); i++)
    {
        count += dbs[i].count;
    }
    SAFEARRAYBOUND rgsaBound[3] = {{count, 0}, {6, 0}, {2, 0}};
    psaValue = SafeArrayCreate(VT_VARIANT, 3, rgsaBound);
    long index = 0;
    for (unsigned int i = 0; i < dbs.size(); i++)
    {
        for (unsigned int j = 0; j < dbs[i].tables.size(); j++)
        {
            LONG ArrayIndex[3] = {index, 0, 0};
            ArrayIndex[1] = 0;
            ArrayIndex[2] = {0};
            hr = SafeArrayPutElement(psaValue, ArrayIndex, &(_variant_t)L"dbname");
            ArrayIndex[1] = 0;
            ArrayIndex[2] = {1};
            hr = SafeArrayPutElement(psaValue, ArrayIndex, &(_variant_t)dbs[i].dbname.c_str());
            ArrayIndex[1] = 1;
            ArrayIndex[2] = {0};
            hr = SafeArrayPutElement(psaValue, ArrayIndex, &(_variant_t)L"Handle");
            ArrayIndex[1] = 1;
            ArrayIndex[2] = {1};
            hr = SafeArrayPutElement(psaValue, ArrayIndex, &(_variant_t)dbs[i].handle);
            ArrayIndex[1] = 2;
            ArrayIndex[2] = {0};
            hr = SafeArrayPutElement(psaValue, ArrayIndex, &(_variant_t)L"name");
            ArrayIndex[1] = 2;
            ArrayIndex[2] = {1};
            hr = SafeArrayPutElement(psaValue, ArrayIndex, &(_variant_t)dbs[i].tables[j].name.c_str());
            ArrayIndex[1] = 3;
            ArrayIndex[2] = {0};
            hr = SafeArrayPutElement(psaValue, ArrayIndex, &(_variant_t)L"tbl_name");
            ArrayIndex[1] = 3;
            ArrayIndex[2] = {1};
            hr = SafeArrayPutElement(psaValue, ArrayIndex, &(_variant_t)dbs[i].tables[j].tbl_name.c_str());
            ArrayIndex[1] = 4;
            ArrayIndex[2] = {0};
            hr = SafeArrayPutElement(psaValue, ArrayIndex, &(_variant_t)L"rootpage");
            ArrayIndex[1] = 4;
            ArrayIndex[2] = {1};
            hr = SafeArrayPutElement(psaValue, ArrayIndex, &(_variant_t)dbs[i].tables[j].rootpage.c_str());
            ArrayIndex[1] = 5;
            ArrayIndex[2] = {0};
            hr = SafeArrayPutElement(psaValue, ArrayIndex, &(_variant_t)L"sql");
            ArrayIndex[1] = 5;
            ArrayIndex[2] = {1};
            hr = SafeArrayPutElement(psaValue, ArrayIndex, &(_variant_t)dbs[i].tables[j].sql.c_str());
            index++;
        }
    }
    return psaValue;
}

SAFEARRAY *GetDbHandles(DWORD pid)
{
    dbs.clear();
    WeChatProcess hp(pid);
    if (!hp.m_init)
        return NULL;
    DWORD GetDbHandlesRemoteAddr = hp.GetProcAddr(GetDbHandlesRemote);
    if (GetDbHandlesRemoteAddr == 0)
        return NULL;
    DWORD ret = CallRemoteFunction(hp.GetHandle(), GetDbHandlesRemoteAddr, NULL);
    while (1)
    {
        DbInfoAddrStruct dbaddr = {0};
        ReadProcessMemory(hp.GetHandle(), (LPCVOID)ret, &dbaddr, sizeof(DbInfoAddrStruct), 0);
        if (dbaddr.handle == 0)
            break;
        DbInfoStruct db = {0};
        db.handle = dbaddr.handle;
        db.count = dbaddr.count;
        wchar_t *wbuf = new wchar_t[dbaddr.l_dbname + 1];
        ReadProcessMemory(hp.GetHandle(), (LPCVOID)dbaddr.dbname, wbuf, sizeof(wchar_t) * (dbaddr.l_dbname + 1), 0);
        db.dbname = std::wstring(wbuf);
        delete[] wbuf;
        DWORD db_table_start_addr = dbaddr.v_data;
        while (db_table_start_addr < dbaddr.v_end1)
        {
            char *buf = NULL;
            TableInfoAddrStruct tbaddr = {0};
            TableInfoStruct tb;
            ReadProcessMemory(hp.GetHandle(), (LPCVOID)db_table_start_addr, &tbaddr, sizeof(TableInfoAddrStruct), 0);
            buf = new char[tbaddr.l_name + 1];
            ReadProcessMemory(hp.GetHandle(), (LPCVOID)tbaddr.name, buf, tbaddr.l_name + 1, 0);
            tb.name = std::string(buf);
            delete[] buf;
            buf = new char[tbaddr.l_tbl_name + 1];
            ReadProcessMemory(hp.GetHandle(), (LPCVOID)tbaddr.tbl_name, buf, tbaddr.l_tbl_name + 1, 0);
            tb.tbl_name = std::string(buf);
            delete[] buf;
            buf = new char[tbaddr.l_rootpage + 1];
            ReadProcessMemory(hp.GetHandle(), (LPCVOID)tbaddr.rootpage, buf, tbaddr.l_rootpage + 1, 0);
            tb.rootpage = std::string(buf);
            delete[] buf;
            buf = new char[tbaddr.l_sql + 1];
            ReadProcessMemory(hp.GetHandle(), (LPCVOID)tbaddr.sql, buf, tbaddr.l_sql + 1, 0);
            tb.sql = std::string(buf);
            delete[] buf;
            db.tables.push_back(tb);
            db_table_start_addr += sizeof(TableInfoAddrStruct);
        }
        dbs.push_back(db);
        ret += sizeof(DbInfoAddrStruct);
    }
    SAFEARRAY *psaValue = CreateDbInfoSafeArray();
    return psaValue;
}
