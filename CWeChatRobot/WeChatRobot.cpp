﻿// WeChatRobot.cpp: CWeChatRobot 的实现

#include "pch.h"
#include "WeChatRobot.h"


// CWeChatRobot
/*
* 参数1：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CStartRobotService(int* __result) {
    *__result = StartRobotService();
    return S_OK;
}

/*
* 参数1：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CStopRobotService(int* __result) {
    *__result = StopRobotService();
    return S_OK;
}

/*
* 参数1：接收人wxid
* 参数2：文本消息内容
* 参数3：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CSendText(BSTR wxid, BSTR wxmsg, int* __result) {
    *__result = SendText(wxid, wxmsg);
    return S_OK;
}

/*
* 参数1：群聊id
* 参数2：艾特的人wxid
* 参数3：文本消息内容
* 参数4：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CSendAtText(BSTR chatroomid, VARIANT* wxid, BSTR wxmsg, BOOL AutoNickName, int* __result) {
    *__result = 0;
    if (wxid->vt == VT_BSTR) {
        *__result = SendAtText(chatroomid, wxid->bstrVal, wxmsg, AutoNickName);
    }
    else if (wxid->vt == (VT_ARRAY | VT_VARIANT)) {
        SAFEARRAY* psaValue = wxid->parray;
        *__result = SendAtText(chatroomid, psaValue, wxmsg, AutoNickName);
    }
    return S_OK;
}

/*
* 参数1：接收人wxid
* 参数2：图片绝对路径
* 参数3：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CSendImage(BSTR wxid, BSTR imagepath, int* __result) {
    *__result = SendImage(wxid, imagepath);
    return S_OK;
}

/*
* 参数1：接收人wxid
* 参数2：文件绝对路径
* 参数3：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CSendFile(BSTR wxid, BSTR filepath, int* __result) {
    *__result = SendFile(wxid, filepath);
    return S_OK;
}

/*
* 参数1：接收人wxid
* 参数2：文章标题
* 参数3：文章摘要
* 参数4：文章链接
* 参数5：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CSendArticle(BSTR wxid, BSTR title,BSTR abstract,BSTR url, BSTR imgpath, int* __result) {
    *__result = SendArticle(wxid, title,abstract,url,imgpath);
    return S_OK;
}

/*
* 参数1：接收人wxid
* 参数2：被分享人wxid
* 参数3：显示的名字
* 参数4：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CSendCard(BSTR receiver, BSTR sharedwxid, BSTR nickname, int* __result) {
    *__result = SendCard(receiver, sharedwxid, nickname);
    return S_OK;
}

/*
* 参数1：预返回的值，调用时无需提供
* 返回一个三维数组，python的comtypes包会将其解析为元组
*/
STDMETHODIMP CWeChatRobot::CGetFriendList(VARIANT* __result) {
    VARIANT vsaValue;
    vsaValue.vt = VT_ARRAY | VT_VARIANT;
    V_ARRAY(&vsaValue) = GetFriendList();
    *__result = vsaValue;
    return S_OK;
}

/*
* 参数1：预返回的值，调用时无需提供
  返回构造好的json串，在反序列化时需考虑好友信息中是否存在json字符
  （考虑到从SAFEARRAY转换到适当变量可能较为繁琐，故保留此接口）
*/
STDMETHODIMP CWeChatRobot::CGetFriendListString(BSTR* __result) {
    string smessage = _com_util::ConvertBSTRToString((BSTR)(GetFriendListString().c_str()));
    *__result = _com_util::ConvertStringToBSTR(smessage.c_str());
    return S_OK;
}

/*
* 参数1：要查询的wxid
* 参数2：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CGetWxUserInfo(BSTR wxid,BSTR* __result) {
    string smessage = _com_util::ConvertBSTRToString((BSTR)(GetWxUserInfo(wxid).c_str()));
    *__result = _com_util::ConvertStringToBSTR(smessage.c_str());
    return S_OK;
}

/*
* 参数1：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CGetSelfInfo(BSTR* __result) {
    string smessage = _com_util::ConvertBSTRToString((BSTR)(GetSelfInfo().c_str()));
    *__result = _com_util::ConvertStringToBSTR(smessage.c_str());
    return S_OK;
}

/*
* 参数1：查询的wxid
* 参数2：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CCheckFriendStatus(BSTR wxid,int* __result) {
    *__result = CheckFriendStatus((wchar_t*)wxid);
    return S_OK;
}

/*
* 参数1：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CGetComWorkPath(BSTR* __result) {
    string path = _com_util::ConvertBSTRToString((BSTR)(GetComWorkPath().c_str()));
    *__result = _com_util::ConvertStringToBSTR(path.c_str());
    return S_OK;
}

/*
* 参数1：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CStartReceiveMessage(int port,int* __result) {
    *__result = StartReceiveMessage(port);
    return S_OK;
}

/*
* 参数1：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CStopReceiveMessage(int* __result) {
    *__result = StopReceiveMessage();
    return S_OK;
}

/*
* 参数1：群聊ID
* 参数2：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CGetChatRoomMembers(BSTR chatroomid,VARIANT* __result) {
    VARIANT vsaValue;
    vsaValue.vt = VT_ARRAY | VT_VARIANT;
    V_ARRAY(&vsaValue) = GetChatRoomMembers(chatroomid);
    *__result = vsaValue;
    return S_OK;
}

/*
* 参数1：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CGetDbHandles(VARIANT* __result) {
    VARIANT vsaValue;
    vsaValue.vt = VT_ARRAY | VT_VARIANT;
    V_ARRAY(&vsaValue) = GetDbHandles();
    *__result = vsaValue;
    return S_OK;
}

/*
* 参数1：数据库句柄
* 参数2：要执行的SQL语句
* 参数3：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CExecuteSQL(DWORD DbHandle,BSTR sql,VARIANT* __result) {
    VARIANT vsaValue;
    vsaValue.vt = VT_ARRAY | VT_VARIANT;
    V_ARRAY(&vsaValue) = ExecuteSQL(DbHandle, sql);
    *__result = vsaValue;
    return S_OK;
}

/*
* 参数1：数据库句柄
* 参数2：备份保存路径
* 参数3：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CBackupSQLiteDB(DWORD DbHandle, BSTR savepath, int* __result) {
    *__result = BackupSQLiteDB(DbHandle, savepath);
    return S_OK;
}

/*
* 参数1：v3数据
* 参数2：v4数据
* 参数3：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CVerifyFriendApply(BSTR v3, BSTR v4, int* __result) {
    *__result = VerifyFriendApply(v3, v4);
    return S_OK;
}

/*
* 参数1：wxid
* 参数2：附加信息
* 参数3：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CAddFriendByWxid(BSTR wxid, BSTR message, int* __result) {
    *__result = AddFriendByWxid(wxid, message);
    return S_OK;
}

/*
* 参数1：v3数据
* 参数2：附加信息
* 参数3：添加方式
* 参数4：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CAddFriendByV3(BSTR v3, BSTR message,int AddType, int* __result) {
    *__result = AddFriendByV3(v3, message,AddType);
    return S_OK;
}

/*
* 参数1：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CGetWeChatVer(BSTR* __result) {
    string path = _com_util::ConvertBSTRToString((BSTR)(GetWeChatVerStr().c_str()));
    *__result = _com_util::ConvertStringToBSTR(path.c_str());
    return S_OK;
}

/*
* 参数1：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CStartWeChat(int* __result) {
    StartWeChat();
    *__result = 0;
    return S_OK;
}

/*
* 参数1：搜索关键字
* 参数2：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CSearchContactByNet(BSTR keyword, VARIANT* __result) {
    VARIANT vsaValue;
    vsaValue.vt = VT_ARRAY | VT_VARIANT;
    V_ARRAY(&vsaValue) = SearchContactByNet(keyword);
    *__result = vsaValue;
    return S_OK;
}

/*
* 参数1：公众号id
* 参数2：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CAddBrandContact(BSTR PublicId, int* __result) {
    *__result = AddBrandContact(PublicId);
    return S_OK;
}

/*
* 参数1：保存路径
* 参数2：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CHookVoiceMsg(BSTR savepath, int* __result) {
    *__result = HookVoiceMsg(savepath);
    return S_OK;
}

/*
* 参数1：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CUnHookVoiceMsg(int* __result) {
    UnHookVoiceMsg();
    *__result = 0;
    return S_OK;
}

/*
* 参数1：保存路径
* 参数2：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CHookImageMsg(BSTR savepath, int* __result) {
    *__result = HookImageMsg(savepath);
    return S_OK;
}

/*
* 参数1：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CUnHookImageMsg(int* __result) {
    UnHookImageMsg();
    *__result = 0;
    return S_OK;
}

/*
* 参数1：版本号
* 参数2：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CChangeWeChatVer(BSTR verStr, int* __result) {
    *__result = ChangeWeChatVer(verStr);
    return S_OK;
}

/*
* 参数1：接收人wxid
* 参数2：小程序id
* 参数3：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CSendAppMsg(BSTR wxid,BSTR appid,int* __result) {
    *__result = SendAppMsg(wxid,appid);
    return S_OK;
}

/*
* 参数1：要删除的人wxid
* 参数2：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CDeleteUser(BSTR wxid, int* __result) {
    *__result = DeleteUser(wxid);
    return S_OK;
}

/*
* 参数1：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CIsWxLogin(int* __result) {
    *__result = isWxLogin();
    return S_OK;
}