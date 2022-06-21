#pragma once
#include<windows.h>
using namespace std;

/*
* 微信中的基础数据结构
* buffer：UNICODE字符串
* length：`buffer`字符数
* maxLength：`buffer`最大字符数
* fill1：占位成员1，默认为0
* fill2：占位成员2，默认为0
* WxBaseStruct：默认构造函数
*/
struct WxBaseStruct
{
    wchar_t* buffer;
    DWORD length;
    DWORD maxLength;
    DWORD fill1;
    DWORD fill2;

    WxBaseStruct(wchar_t* pStr) {
        buffer = pStr;
        length = wcslen(pStr);
        maxLength = wcslen(pStr) * 2;
        fill1 = 0x0;
        fill2 = 0x0;
    }
};

/*
* 不使用构造函数的微信基础数据结构，使用频率较低
*/
struct WxString
{
    wchar_t* buffer = NULL;
    DWORD length = 0;
    DWORD maxLength = 0;
    DWORD fill1 = 0;
    DWORD fill2 = 0;
};

/*
* 保存单条信息的结构
* messagetype：消息类型
* sender：发送者wxid；l_sender：`sender`字符数
* wxid：如果sender是群聊id，则此成员保存具体发送人wxid，否则与`sender`一致；l_wxid：`wxid`字符数
* message：消息内容，非文本消息是xml格式；l_message：`message`字符数
* filepath：图片、文件及其他资源的保存路径；l_filepath：`filepath`字符数
*/
struct ReceiveMsgStruct {
    DWORD messagetype;
    BOOL isSendMessage;
    wchar_t* sender;
    DWORD l_sender;
    wchar_t* wxid;
    DWORD l_wxid;
    wchar_t* message;
    DWORD l_message;
    wchar_t* filepath;
    DWORD l_filepath;
    wchar_t* time;
    DWORD l_time;
    ~ReceiveMsgStruct() {
        if (this->sender)
            delete[] this->sender;
        if (this->wxid)
            delete[] this->wxid;
        if (this->message)
            delete[] this->message;
        if (this->filepath)
            delete[] this->filepath;
        if (this->time)
            delete[] this->time;
    }
};

// vector在内存中的表现形式
struct VectorStruct {
#ifdef _DEBUG
    DWORD v_head;
#endif
    DWORD v_data;
    DWORD v_end1;
    DWORD v_end2;
};

struct UserInfo {
    int errcode;
    wchar_t* keyword;
    int l_keyword;
    wchar_t* v3;
    int l_v3;
    wchar_t* NickName;
    int l_NickName;
    wchar_t* Signature;
    int l_Signature;
    wchar_t* v2;
    int l_v2;
    wchar_t* Nation;
    int l_Nation;
    wchar_t* Province;
    int l_Province;
    wchar_t* City;
    int l_City;
    wchar_t* BigAvatar;
    int l_BigAvatar;
    wchar_t* SmallAvatar;
    int l_SmallAvatar;
    DWORD sex;
    BOOL over;
};

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

/*
* 保存单个好友信息的结构体
* wxIdAddr：wxid保存地址
* wxNumberAddr：微信号保存地址
* wxNickNameAddr：昵称保存地址
* wxRemarkAddr：备注保存地址
* WxFriendStructW：默认构造函数
*/
struct WxFriendStruct {
    DWORD wxIdAddr;
    DWORD wxNumberAddr;
    DWORD wxNickNameAddr;
    DWORD wxRemarkAddr;
    WxFriendStruct(DWORD wxIdAddr, DWORD wxNumberAddr, DWORD wxNickNameAddr, DWORD wxRemarkAddr) {
        this->wxIdAddr = wxIdAddr;
        this->wxNumberAddr = wxNumberAddr;
        this->wxNickNameAddr = wxNickNameAddr;
        this->wxRemarkAddr = wxRemarkAddr;
    }
};