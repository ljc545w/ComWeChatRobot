// WeChatRobot.cpp: CWeChatRobot 的实现

#include "pch.h"
#include "WeChatRobot.h"


// CWeChatRobot
/*
* 参数1：`DWeChatRobot.dll`所在目录
* 参数2：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CStartRobotService(BSTR workPath, int* __result) {
    *__result = StartRobotService(workPath);
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
STDMETHODIMP CWeChatRobot::CSendArticle(BSTR wxid, BSTR title,BSTR abstract,BSTR url, int* __result) {
    *__result = SendArticle(wxid, title,abstract,url);
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
*/
STDMETHODIMP CWeChatRobot::CGetFriendList(BSTR* __result) {
    string smessage = _com_util::ConvertBSTRToString((BSTR)(GetFriendList().c_str()));
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
* 参数1：预返回的值，调用时无需提供
*/
STDMETHODIMP CWeChatRobot::CCheckFriendStatusInit(int* __result) {
    *__result = CheckFriendStatusInit();
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
STDMETHODIMP CWeChatRobot::CCheckFriendStatusFinish(int* __result) {
    *__result = CheckFriendStatusFinish();
    return S_OK;
}