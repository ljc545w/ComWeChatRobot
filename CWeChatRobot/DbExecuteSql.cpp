#include "pch.h"

// 调用DLL接口时的参数
struct executeParams
{
    DWORD ptrDb;
    DWORD ptrSql;
};

// 接收DLL的返回数据，包括动态数组首地址和数组长度
struct executeResult
{
    DWORD SQLResultData;
    DWORD length;
};

// 保存ReadProcessMemory读出的结构体
struct SQLResultAddrStruct
{
    DWORD ColName;
    DWORD l_ColName;
    DWORD content;
    DWORD l_content;
    DWORD isblob;
};

// vector的数据结构
struct VectorStruct
{
#ifdef _DEBUG
    DWORD v_head;
#endif
    DWORD v_data;
    DWORD v_end1;
    DWORD v_end2;
};

// 保存SQL查询结构的基础结构
struct SQLResultStruct
{
    wchar_t *ColName;
    wchar_t *content;
    unsigned char *BlobContent;
    int BlobLength;
};
// 查询结果是一个二维数组
vector<vector<SQLResultStruct>> SQLResult;

// 每次查询前清空前一次查询到的结果
void ClearResultArray()
{
    if (SQLResult.size() == 0)
        return;
    for (unsigned int i = 0; i < SQLResult.size(); i++)
    {
        for (unsigned j = 0; j < SQLResult[i].size(); j++)
        {
            SQLResultStruct *sr = (SQLResultStruct *)&SQLResult[i][j];
            if (sr->ColName)
            {
                delete sr->ColName;
                sr->ColName = NULL;
            }
            if (sr->content)
            {
                delete sr->content;
                sr->content = NULL;
            }
            if (sr->BlobContent)
            {
                delete sr->BlobContent;
                sr->BlobContent = NULL;
            }
        }
        SQLResult[i].clear();
    }
    SQLResult.clear();
}

// 遍历查询结果，构造SAFEARRAY
SAFEARRAY *CreateSQLResultSafeArray()
{
    if (SQLResult.size() == 0 || SQLResult[0].size() == 0)
        return NULL;
    SAFEARRAYBOUND rgsaBound[2] = {{SQLResult.size() + 1, 0}, {SQLResult[0].size(), 0}};
    SAFEARRAY *psaValue = SafeArrayCreate(VT_VARIANT, 2, rgsaBound);
    HRESULT hr = S_OK;
    long Index[2] = {0, 0};
    for (unsigned int i = 0; i < SQLResult.size(); i++)
    {
        for (unsigned int j = 0; j < SQLResult[i].size(); j++)
        {
            SQLResultStruct *ptrResult = (SQLResultStruct *)&SQLResult[i][j];
            if (i == 0)
            {
                Index[0] = 0;
                Index[1] = j;
                hr = SafeArrayPutElement(psaValue, Index, &(_variant_t)ptrResult->ColName);
            }
            Index[0] = i + 1;
            Index[1] = j;
            if (ptrResult->content)
                hr = SafeArrayPutElement(psaValue, Index, &(_variant_t)ptrResult->content);
            else
            {
                VARIANT varChunk;
                SAFEARRAY *bsa;
                BYTE *pByte = NULL;
                SAFEARRAYBOUND rgsabound[1];
                rgsabound[0].cElements = ptrResult->BlobLength;
                rgsabound[0].lLbound = 0;
                bsa = SafeArrayCreate(VT_UI1, 1, rgsabound);
                SafeArrayAccessData(bsa, (void **)&pByte);
                memcpy(pByte, ptrResult->BlobContent, ptrResult->BlobLength);
                SafeArrayUnaccessData(bsa);
                varChunk.vt = VT_ARRAY | VT_UI1;
                varChunk.parray = bsa;
                hr = SafeArrayPutElement(psaValue, Index, &(_variant_t)varChunk);
            }
        }
    }
    return psaValue;
}

// 读出查询结果
VOID ReadSQLResultFromWeChatProcess(HANDLE hProcess, DWORD dwHandle)
{
    executeResult result = {0};
    ReadProcessMemory(hProcess, (LPCVOID)dwHandle, &result, sizeof(executeResult), 0);
    for (unsigned int i = 0; i < result.length; i++)
    {
        VectorStruct v_temp = {0};
        vector<SQLResultStruct> s_temp;
        ReadProcessMemory(hProcess, (LPCVOID)result.SQLResultData, &v_temp, sizeof(VectorStruct), 0);
        while (v_temp.v_data < v_temp.v_end1)
        {
            SQLResultAddrStruct sqlresultAddr = {0};
            SQLResultStruct sqlresult = {0};
            ReadProcessMemory(hProcess, (LPCVOID)v_temp.v_data, &sqlresultAddr, sizeof(SQLResultAddrStruct), 0);
            char *ColName = new char[sqlresultAddr.l_ColName + 1];
            sqlresult.ColName = new wchar_t[sqlresultAddr.l_ColName + 1];
            ReadProcessMemory(hProcess, (LPCVOID)sqlresultAddr.ColName, ColName, sqlresultAddr.l_ColName + 1, 0);
            MultiByteToWideChar(CP_ACP, 0, ColName, -1, sqlresult.ColName, strlen(ColName) + 1);
            char *content = new char[sqlresultAddr.l_content + 1];
            if (!sqlresultAddr.isblob)
            {
                sqlresult.content = new wchar_t[sqlresultAddr.l_content + 1];
                ReadProcessMemory(hProcess, (LPCVOID)sqlresultAddr.content, content, sqlresultAddr.l_content + 1, 0);
                MultiByteToWideChar(CP_UTF8, 0, content, -1, sqlresult.content, strlen(content) + 1);
                sqlresult.BlobContent = NULL;
            }
            else
            {
                sqlresult.BlobContent = new unsigned char[sqlresultAddr.l_content];
                ReadProcessMemory(hProcess, (LPCVOID)sqlresultAddr.content, sqlresult.BlobContent, sqlresultAddr.l_content, 0);
                sqlresult.BlobLength = sqlresultAddr.l_content;
                sqlresult.content = NULL;
            }
            delete[] ColName;
            ColName = NULL;
            delete[] content;
            content = NULL;
            v_temp.v_data += sizeof(SQLResultAddrStruct);
            s_temp.push_back(sqlresult);
        }
        SQLResult.push_back(s_temp);
        result.SQLResultData += sizeof(VectorStruct);
    }
}

SAFEARRAY *ExecuteSQL(DWORD pid, DWORD DbHandle, BSTR sql)
{
    WeChatProcess hp(pid);
    if (!hp.m_init)
        return NULL;
    ClearResultArray();
    // DWORD ExecuteSQLRemoteAddr = hp.GetProcAddr(ExecuteSQLRemote);
    DWORD ExecuteSQLRemoteAddr = hp.GetProcAddr(SelectDataRemote);
    if (ExecuteSQLRemoteAddr == 0)
        return NULL;
    string s_sql = unicode_to_utf8(sql);
    WeChatData<char *> r_sql(hp.GetHandle(), (char *)s_sql.c_str(), s_sql.size());
    executeParams params = {0};
    params.ptrDb = DbHandle;
    params.ptrSql = (DWORD)r_sql.GetAddr();
    WeChatData<executeParams *> r_params(hp.GetHandle(), &params, sizeof(params));
    if (r_sql.GetAddr() == 0 || r_params.GetAddr() == 0)
        return NULL;
    DWORD ret = CallRemoteFunction(hp.GetHandle(), ExecuteSQLRemoteAddr, r_params.GetAddr());
    ReadSQLResultFromWeChatProcess(hp.GetHandle(), ret);
    SAFEARRAY *psaValue = CreateSQLResultSafeArray();
    return psaValue;
}
