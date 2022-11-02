// WeChatRobot.cpp: CWeChatRobot 的实现

#include "pch.h"
#include "WeChatRobot.h"

// CWeChatRobot
/*
 * 参数0：目标进程pid
 * 参数1：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CStartRobotService(DWORD pid, int *__result)
{
    *__result = StartRobotService(pid);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CStopRobotService(DWORD pid, int *__result)
{
    *__result = StopRobotService(pid);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：接收人wxid
 * 参数2：文本消息内容
 * 参数3：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CSendText(DWORD pid, BSTR wxid, BSTR wxmsg, int *__result)
{
    *__result = SendText(pid, wxid, wxmsg);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：群聊id
 * 参数2：艾特的人wxid
 * 参数3：文本消息内容
 * 参数4：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CSendAtText(DWORD pid, BSTR chatroomid, VARIANT *wxid, BSTR wxmsg, BOOL AutoNickName, int *__result)
{
    *__result = 0;
    if (wxid->vt == VT_BSTR)
    {
        *__result = SendAtText(pid, chatroomid, wxid->bstrVal, wxmsg, AutoNickName);
    }
    else if (wxid->vt == (VT_ARRAY | VT_VARIANT))
    {
        SAFEARRAY *psaValue = wxid->parray;
        *__result = SendAtText(pid, chatroomid, psaValue, wxmsg, AutoNickName);
    }
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：接收人wxid
 * 参数2：图片绝对路径
 * 参数3：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CSendImage(DWORD pid, BSTR wxid, BSTR imagepath, int *__result)
{
    *__result = SendImage(pid, wxid, imagepath);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：接收人wxid
 * 参数2：文件绝对路径
 * 参数3：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CSendFile(DWORD pid, BSTR wxid, BSTR filepath, int *__result)
{
    *__result = SendFile(pid, wxid, filepath);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：接收人wxid
 * 参数2：文章标题
 * 参数3：文章摘要
 * 参数4：文章链接
 * 参数5：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CSendArticle(DWORD pid, BSTR wxid, BSTR title, BSTR abstract, BSTR url, BSTR imgpath, int *__result)
{
    *__result = SendArticle(pid, wxid, title, abstract, url, imgpath);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：接收人wxid
 * 参数2：被分享人wxid
 * 参数3：显示的名字
 * 参数4：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CSendCard(DWORD pid, BSTR receiver, BSTR sharedwxid, BSTR nickname, int *__result)
{
    *__result = SendCard(pid, receiver, sharedwxid, nickname);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：预返回的值，调用时无需提供
 * 返回一个三维数组，python的comtypes包会将其解析为元组
 */
STDMETHODIMP CWeChatRobot::CGetFriendList(DWORD pid, VARIANT *__result)
{
    VARIANT vsaValue;
    vsaValue.vt = VT_ARRAY | VT_VARIANT;
    V_ARRAY(&vsaValue) = GetFriendList(pid);
    *__result = vsaValue;
    return S_OK;
}

/*
* 参数0：目标进程pid
* 参数1：预返回的值，调用时无需提供
  返回构造好的json串，在反序列化时需考虑好友信息中是否存在json字符
  （考虑到从SAFEARRAY转换到适当变量可能较为繁琐，故保留此接口）
*/
STDMETHODIMP CWeChatRobot::CGetFriendListString(DWORD pid, BSTR *__result)
{
    wstring info = GetFriendListString(pid);
    *__result = (_bstr_t)info.c_str();
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：要查询的wxid
 * 参数2：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CGetWxUserInfo(DWORD pid, BSTR wxid, BSTR *__result)
{
    wstring info = GetWxUserInfo(pid, wxid);
    *__result = (_bstr_t)info.c_str();
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CGetSelfInfo(DWORD pid, BSTR *__result)
{
    wstring info = GetSelfInfo(pid);
    *__result = (_bstr_t)info.c_str();
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：查询的wxid
 * 参数2：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CCheckFriendStatus(DWORD pid, BSTR wxid, int *__result)
{
    *__result = CheckFriendStatus(pid, (wchar_t *)wxid);
    return S_OK;
}

/*
 * 参数1：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CGetComWorkPath(BSTR *__result)
{
    wstring path = GetComWorkPath();
    *__result = (_bstr_t)path.c_str();
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CStartReceiveMessage(DWORD pid, int port, int *__result)
{
    *__result = StartReceiveMessage(pid, port);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CStopReceiveMessage(DWORD pid, int *__result)
{
    *__result = StopReceiveMessage(pid);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：群聊ID
 * 参数2：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CGetChatRoomMembers(DWORD pid, BSTR chatroomid, VARIANT *__result)
{
    VARIANT vsaValue;
    vsaValue.vt = VT_ARRAY | VT_VARIANT;
    V_ARRAY(&vsaValue) = GetChatRoomMembers(pid, chatroomid);
    *__result = vsaValue;
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CGetDbHandles(DWORD pid, VARIANT *__result)
{
    VARIANT vsaValue;
    vsaValue.vt = VT_ARRAY | VT_VARIANT;
    V_ARRAY(&vsaValue) = GetDbHandles(pid);
    *__result = vsaValue;
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：数据库句柄
 * 参数2：要执行的SQL语句
 * 参数3：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CExecuteSQL(DWORD pid, DWORD DbHandle, BSTR sql, VARIANT *__result)
{
    VARIANT vsaValue;
    vsaValue.vt = VT_ARRAY | VT_VARIANT;
    V_ARRAY(&vsaValue) = ExecuteSQL(pid, DbHandle, sql);
    *__result = vsaValue;
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：数据库句柄
 * 参数2：备份保存路径
 * 参数3：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CBackupSQLiteDB(DWORD pid, DWORD DbHandle, BSTR savepath, int *__result)
{
    *__result = BackupSQLiteDB(pid, DbHandle, savepath);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：v3数据
 * 参数2：v4数据
 * 参数3：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CVerifyFriendApply(DWORD pid, BSTR v3, BSTR v4, int *__result)
{
    *__result = VerifyFriendApply(pid, v3, v4);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：wxid
 * 参数2：附加信息
 * 参数3：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CAddFriendByWxid(DWORD pid, BSTR wxid, BSTR message, int *__result)
{
    *__result = AddFriendByWxid(pid, wxid, message);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：v3数据
 * 参数2：附加信息
 * 参数3：添加方式
 * 参数4：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CAddFriendByV3(DWORD pid, BSTR v3, BSTR message, int AddType, int *__result)
{
    *__result = AddFriendByV3(pid, v3, message, AddType);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CGetWeChatVer(BSTR *__result)
{
    wstring path = GetWeChatVerStr();
    *__result = (_bstr_t)path.c_str();
    return S_OK;
}

/*
 * 参数1：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CStartWeChat(int *__result)
{
    *__result = StartWeChat();
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：搜索关键字
 * 参数2：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CSearchContactByNet(DWORD pid, BSTR keyword, VARIANT *__result)
{
    VARIANT vsaValue;
    vsaValue.vt = VT_ARRAY | VT_VARIANT;
    V_ARRAY(&vsaValue) = SearchContactByNet(pid, keyword);
    *__result = vsaValue;
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：公众号id
 * 参数2：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CAddBrandContact(DWORD pid, BSTR PublicId, int *__result)
{
    *__result = AddBrandContact(pid, PublicId);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：保存路径
 * 参数2：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CHookVoiceMsg(DWORD pid, BSTR savepath, int *__result)
{
    *__result = HookVoiceMsg(pid, savepath);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CUnHookVoiceMsg(DWORD pid, int *__result)
{
    UnHookVoiceMsg(pid);
    *__result = 0;
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：保存路径
 * 参数2：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CHookImageMsg(DWORD pid, BSTR savepath, int *__result)
{
    *__result = HookImageMsg(pid, savepath);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CUnHookImageMsg(DWORD pid, int *__result)
{
    UnHookImageMsg(pid);
    *__result = 0;
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：版本号
 * 参数2：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CChangeWeChatVer(DWORD pid, BSTR verStr, int *__result)
{
    *__result = ChangeWeChatVer(pid, verStr);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：接收人wxid
 * 参数2：小程序id
 * 参数3：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CSendAppMsg(DWORD pid, BSTR wxid, BSTR appid, int *__result)
{
    *__result = SendAppMsg(pid, wxid, appid);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：要删除的人wxid
 * 参数2：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CDeleteUser(DWORD pid, BSTR wxid, int *__result)
{
    *__result = DeleteUser(pid, wxid);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CIsWxLogin(DWORD pid, int *__result)
{
    *__result = isWxLogin(pid);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：wxid或群聊id
 * 参数2：备注内容
 * 参数3：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CEditRemark(DWORD pid, BSTR wxid, BSTR remark, int *__result)
{
    *__result = EditRemark(pid, wxid, remark);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：群聊id
 * 参数2：群聊名称
 * 参数3：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CSetChatRoomName(DWORD pid, BSTR chatroomid, BSTR name, int *__result)
{
    *__result = SetChatRoomName(pid, chatroomid, name);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：群聊id
 * 参数2：公告内容
 * 参数3：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CSetChatRoomAnnouncement(DWORD pid, BSTR chatroomid, BSTR announcement, int *__result)
{
    *__result = SetChatRoomAnnouncement(pid, chatroomid, announcement);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：群聊id
 * 参数2：个人昵称
 * 参数3：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CSetChatRoomSelfNickname(DWORD pid, BSTR chatroomid, BSTR nickname, int *__result)
{
    *__result = SetChatRoomSelfNickname(pid, chatroomid, nickname);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：群聊id
 * 参数2：群成员wxid
 * 参数3：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CGetChatRoomMemberNickname(DWORD pid, BSTR chatroomid, BSTR wxid, BSTR *__result)
{
    wstring nickname = GetChatRoomMemberNickname(pid, chatroomid, wxid);
    *__result = (_bstr_t)nickname.c_str();
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：群聊id
 * 参数2：wxid列表
 * 参数3：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CDelChatRoomMember(DWORD pid, BSTR chatroomid, VARIANT *wxids, int *__result)
{
    if (wxids->vt == VT_BSTR)
    {
        *__result = DelChatRoomMember(pid, chatroomid, wxids->bstrVal);
    }
    else if (wxids->vt == (VT_ARRAY | VT_VARIANT))
    {
        SAFEARRAY *psaValue = wxids->parray;
        *__result = DelChatRoomMember(pid, chatroomid, psaValue);
    }
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：群聊id
 * 参数2：wxid列表
 * 参数3：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CAddChatRoomMember(DWORD pid, BSTR chatroomid, VARIANT *wxids, int *__result)
{
    if (wxids->vt == VT_BSTR)
    {
        *__result = AddChatRoomMember(pid, chatroomid, wxids->bstrVal);
    }
    else if (wxids->vt == (VT_ARRAY | VT_VARIANT))
    {
        SAFEARRAY *psaValue = wxids->parray;
        *__result = AddChatRoomMember(pid, chatroomid, psaValue);
    }
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：要打开的网页url
 * 参数2：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::COpenBrowser(DWORD pid, BSTR url, int *__result)
{
    *__result = OpenBrowser(pid, url);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：公众号ID
 * 参数2：推送消息的偏移
 * 参数3：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CGetHistoryPublicMsg(DWORD pid, BSTR PublicId, BSTR Offset, VARIANT *__result)
{
    HRESULT hr = S_OK;
    wstring result = GetHistoryPublicMsg(pid, PublicId, Offset);
    VARIANT vsaValue;
    vsaValue.vt = VT_ARRAY | VT_VARIANT;
    SAFEARRAYBOUND rgsaBound = {1, 0};
    SAFEARRAY *psaValue = SafeArrayCreate(VT_VARIANT, 1, &rgsaBound);
    long index = 0;
    // 数据大小超过16382个字符，客户端调用可能出现异常，因此将数据放入安全数组中传递
    hr = SafeArrayPutElement(psaValue, &index, &(_variant_t)result.c_str());
    V_ARRAY(&vsaValue) = psaValue;
    *__result = vsaValue;
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：消息接收人wxid
 * 参数2：要转发的消息id
 * 参数3：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CForwardMessage(DWORD pid, BSTR wxid, ULONG64 msgid, int *__result)
{
    *__result = ForwardMessage(pid, wxid, msgid);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CGetQrcodeImage(DWORD pid, VARIANT *__result)
{
    *__result = GetQrcodeImage(pid);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：公众号文章链接
 * 参数2：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CGetA8Key(DWORD pid, BSTR url, BSTR *__result)
{
    *__result = (_bstr_t)GetA8Key(pid, url).c_str();
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：消息接收人wxid
 * 参数2：xml内容
 * 参数3：图片绝对路径
 * 参数4：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CSendXmlMsg(DWORD pid, BSTR wxid, BSTR xml, BSTR imgpath, int *__result)
{
    *__result = SendXmlMsg(pid, wxid, xml, imgpath);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CLogout(DWORD pid, int *__result)
{
    *__result = Logout(pid);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：转账人wxid
 * 参数2：从xml中获取的transcationid
 * 参数3：从xml中获取的transferid
 * 参数4：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CGetTransfer(DWORD pid, BSTR wxid, BSTR transcationid, BSTR transferid, int *__result)
{
    *__result = GetTransfer(pid, wxid, transcationid, transferid);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：接收人wxid
 * 参数2：表情绝对路径
 * 参数3：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CSendEmotion(DWORD pid, BSTR wxid, BSTR img_path, int *__result)
{
    *__result = SendEmotion(pid, wxid, img_path);
    return S_OK;
}

/*
 * 参数0：目标进程pid
 * 参数1：消息id
 * 参数2：预返回的值，调用时无需提供
 */
STDMETHODIMP CWeChatRobot::CGetMsgCDN(DWORD pid, ULONG64 msgid, BSTR *__result)
{
    _bstr_t path = (_bstr_t)GetMsgCND(pid, msgid).c_str();
    *__result = path;
    return S_OK;
}
