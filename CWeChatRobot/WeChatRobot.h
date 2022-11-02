// WeChatRobot.h: CWeChatRobot 的声明

#pragma once
#include "resource.h" // 主符号

#include "WeChatRobotCOM_i.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;

// CWeChatRobot

class ATL_NO_VTABLE CWeChatRobot : public CComObjectRootEx<CComSingleThreadModel>,
                                   public CComCoClass<CWeChatRobot, &CLSID_WeChatRobot>,
                                   public IDispatchImpl<IWeChatRobot, &IID_IWeChatRobot, &LIBID_WeChatRobotCOMLib, /*wMajor =*/1, /*wMinor =*/0>
{
public:
    CWeChatRobot()
    {
    }

    DECLARE_REGISTRY_RESOURCEID(106)

    BEGIN_COM_MAP(CWeChatRobot)
    COM_INTERFACE_ENTRY(IWeChatRobot)
    COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        return S_OK;
    }

    void FinalRelease()
    {
    }

public:
    STDMETHODIMP CStartRobotService(DWORD pid, int *__result);
    STDMETHODIMP CStopRobotService(DWORD pid, int *__result);
    STDMETHODIMP CSendImage(DWORD pid, BSTR wxid, BSTR imagepath, int *__result);
    STDMETHODIMP CSendText(DWORD pid, BSTR wxid, BSTR wxmsg, int *__result);
    STDMETHODIMP CSendFile(DWORD pid, BSTR wxid, BSTR filepath, int *__result);
    STDMETHODIMP CSendArticle(DWORD pid, BSTR wxid, BSTR title, BSTR abstract, BSTR url, BSTR imgpath, int *__result);
    STDMETHODIMP CSendCard(DWORD pid, BSTR receiver, BSTR sharedwxid, BSTR nickname, int *__result);
    STDMETHODIMP CSendAtText(DWORD pid, BSTR chatroomid, VARIANT *wxid, BSTR wxmsg, BOOL AutoNickName, int *__result);
    STDMETHODIMP CGetFriendList(DWORD pid, VARIANT *__result);
    STDMETHODIMP CGetFriendListString(DWORD pid, BSTR *__result);
    STDMETHODIMP CGetWxUserInfo(DWORD pid, BSTR wxid, BSTR *__result);
    STDMETHODIMP CGetSelfInfo(DWORD pid, BSTR *__result);
    STDMETHODIMP CCheckFriendStatus(DWORD pid, BSTR wxid, int *__result);
    STDMETHODIMP CGetComWorkPath(BSTR *__result);
    STDMETHODIMP CStartReceiveMessage(DWORD pid, int port, int *__result);
    STDMETHODIMP CStopReceiveMessage(DWORD pid, int *__result);
    STDMETHODIMP CGetChatRoomMembers(DWORD pid, BSTR chatroomid, VARIANT *__result);
    STDMETHODIMP CGetDbHandles(DWORD pid, VARIANT *__result);
    STDMETHODIMP CExecuteSQL(DWORD pid, DWORD DbHandle, BSTR sql, VARIANT *__result);
    STDMETHODIMP CBackupSQLiteDB(DWORD pid, DWORD DbHandle, BSTR savepath, int *__result);
    STDMETHODIMP CVerifyFriendApply(DWORD pid, BSTR v3, BSTR v4, int *__result);
    STDMETHODIMP CAddFriendByWxid(DWORD pid, BSTR wxid, BSTR message, int *__result);
    STDMETHODIMP CAddFriendByV3(DWORD pid, BSTR v3, BSTR message, int AddType, int *__result);
    STDMETHODIMP CGetWeChatVer(BSTR *__result);
    STDMETHODIMP CStartWeChat(int *__result);
    STDMETHODIMP CSearchContactByNet(DWORD pid, BSTR keyword, VARIANT *__result);
    STDMETHODIMP CAddBrandContact(DWORD pid, BSTR PublicId, int *__result);
    STDMETHODIMP CHookVoiceMsg(DWORD pid, BSTR savepath, int *__result);
    STDMETHODIMP CUnHookVoiceMsg(DWORD pid, int *__result);
    STDMETHODIMP CHookImageMsg(DWORD pid, BSTR savepath, int *__result);
    STDMETHODIMP CUnHookImageMsg(DWORD pid, int *__result);
    STDMETHODIMP CChangeWeChatVer(DWORD pid, BSTR verStr, int *__result);
    STDMETHODIMP CSendAppMsg(DWORD pid, BSTR wxid, BSTR appid, int *__result);
    STDMETHODIMP CDeleteUser(DWORD pid, BSTR wxid, int *__result);
    STDMETHODIMP CIsWxLogin(DWORD pid, int *__result);
    STDMETHODIMP CEditRemark(DWORD pid, BSTR wxid, BSTR remark, int *__result);
    STDMETHODIMP CSetChatRoomName(DWORD pid, BSTR chatroomid, BSTR name, int *__result);
    STDMETHODIMP CSetChatRoomAnnouncement(DWORD pid, BSTR chatroomid, BSTR announcement, int *__result);
    STDMETHODIMP CSetChatRoomSelfNickname(DWORD pid, BSTR chatroomid, BSTR nickname, int *__result);
    STDMETHODIMP CGetChatRoomMemberNickname(DWORD pid, BSTR chatroomid, BSTR wxid, BSTR *__result);
    STDMETHODIMP CDelChatRoomMember(DWORD pid, BSTR chatroomid, VARIANT *wxids, int *__result);
    STDMETHODIMP CAddChatRoomMember(DWORD pid, BSTR chatroomid, VARIANT *wxids, int *__result);
    STDMETHODIMP COpenBrowser(DWORD pid, BSTR url, int *__result);
    STDMETHODIMP CGetHistoryPublicMsg(DWORD pid, BSTR PublicId, BSTR Offset, VARIANT *__result);
    STDMETHODIMP CForwardMessage(DWORD pid, BSTR wxid, ULONG64 msgid, int *__result);
    STDMETHODIMP CGetQrcodeImage(DWORD pid, VARIANT *__result);
    STDMETHODIMP CGetA8Key(DWORD pid, BSTR url, BSTR *__result);
    STDMETHODIMP CSendXmlMsg(DWORD pid, BSTR wxid, BSTR xml, BSTR imgpath, int *__result);
    STDMETHODIMP CLogout(DWORD pid, int *__result);
    STDMETHODIMP CGetTransfer(DWORD pid, BSTR wxid, BSTR transcationid, BSTR transferid, int *__result);
    STDMETHODIMP CSendEmotion(DWORD pid, BSTR wxid, BSTR img_path, int *__result);
    STDMETHODIMP CWeChatRobot::CGetMsgCDN(DWORD pid, ULONG64 msgid, BSTR *__result);
};

OBJECT_ENTRY_AUTO(__uuidof(WeChatRobot), CWeChatRobot)
