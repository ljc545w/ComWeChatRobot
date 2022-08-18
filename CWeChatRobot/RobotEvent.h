// RobotEvent.h: CRobotEvent 的声明

#pragma once
#include "resource.h" // 主符号

#include "WeChatRobotCOM_i.h"
#include "_IRobotEventEvents_CP.h"

using namespace ATL;

// CRobotEvent

class ATL_NO_VTABLE CRobotEvent : public CComObjectRootEx<CComMultiThreadModel>,
                                  public CComCoClass<CRobotEvent, &CLSID_RobotEvent>,
                                  public IConnectionPointContainerImpl<CRobotEvent>,
                                  public CProxy_IRobotEventEvents<CRobotEvent>,
                                  public IDispatchImpl<IRobotEvent, &IID_IRobotEvent, &LIBID_WeChatRobotCOMLib, /*wMajor =*/1, /*wMinor =*/0>
{
public:
    CRobotEvent()
    {
    }

    DECLARE_REGISTRY_RESOURCEID(107)
    DECLARE_CLASSFACTORY_SINGLETON(CRobotEvent)

    BEGIN_COM_MAP(CRobotEvent)
    COM_INTERFACE_ENTRY(IRobotEvent)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IConnectionPointContainer)
    END_COM_MAP()

    BEGIN_CONNECTION_POINT_MAP(CRobotEvent)
    CONNECTION_POINT_ENTRY(__uuidof(_IRobotEventEvents))
    END_CONNECTION_POINT_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        return S_OK;
    }

    void FinalRelease()
    {
    }

public:
    STDMETHOD(CPostMessage)
    (DWORD pid, int msgtype, unsigned long long msgid, VARIANT *msg, int *__result);
    STDMETHOD(CRegisterWxPidWithCookie)
    (DWORD pid, DWORD cookie, int *__result);
};

OBJECT_ENTRY_AUTO(__uuidof(RobotEvent), CRobotEvent)
