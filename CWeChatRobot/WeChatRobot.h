// WeChatRobot.h: CWeChatRobot 的声明

#pragma once
#include "resource.h"       // 主符号



#include "WeChatRobotCOM_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CWeChatRobot

class ATL_NO_VTABLE CWeChatRobot :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWeChatRobot, &CLSID_WeChatRobot>,
	public IDispatchImpl<IWeChatRobot, &IID_IWeChatRobot, &LIBID_WeChatRobotCOMLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
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

	STDMETHODIMP CStartRobotService(int* __result);
	STDMETHODIMP CStopRobotService(int* __result);
	STDMETHODIMP CSendImage(BSTR wxid, BSTR imagepath, int* __result);
	STDMETHODIMP CSendText(BSTR wxid, BSTR wxmsg, int* __result);
	STDMETHODIMP CSendFile(BSTR wxid, BSTR filepath, int* __result);
	STDMETHODIMP CSendArticle(BSTR wxid, BSTR title, BSTR abstract, BSTR url, int* __result);
	STDMETHODIMP CSendCard(BSTR receiver, BSTR sharedwxid, BSTR nickname, int* __result);
	STDMETHODIMP CSendAtText(BSTR chatroomid, VARIANT* wxid, BSTR wxmsg, int* __result);
	STDMETHODIMP CGetFriendList(VARIANT* __result);
	STDMETHODIMP CGetFriendListString(BSTR* __result);
	STDMETHODIMP CGetWxUserInfo(BSTR wxid, BSTR* __result);
	STDMETHODIMP CGetSelfInfo(BSTR* __result);
	STDMETHODIMP CCheckFriendStatusInit(int* __result);
	STDMETHODIMP CCheckFriendStatus(BSTR wxid, int* __result);
	STDMETHODIMP CCheckFriendStatusFinish(int* __result);
	STDMETHODIMP CGetComWorkPath(BSTR* __result);
	STDMETHODIMP CStartReceiveMessage(int* __result);
	STDMETHODIMP CReceiveMessage(VARIANT* __result);
	STDMETHODIMP CStopReceiveMessage(int* __result);
	STDMETHODIMP CGetChatRoomMembers(BSTR chatroomid, VARIANT* __result);
	STDMETHODIMP CGetDbHandles(VARIANT* __result);
	STDMETHODIMP CExecuteSQL(DWORD DbHandle, BSTR sql, VARIANT* __result);
	STDMETHODIMP CBackupSQLiteDB(DWORD DbHandle, BSTR savepath, int* __result);
};

OBJECT_ENTRY_AUTO(__uuidof(WeChatRobot), CWeChatRobot)
