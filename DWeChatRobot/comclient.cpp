#include "pch.h"
#ifdef USE_COM
#include <atlbase.h>
#include <atlcom.h>
#include "../CWeChatRobot/WeChatRobotCOM_i.h"
#include "../CWeChatRobot/WeChatRobotCOM_i.c"

#include <iostream>
#include <thread>

// CComModule m_commodule;

class CSink : public CComObjectRoot,
              public _IRobotEventEvents
{
    BEGIN_COM_MAP(CSink)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(_IRobotEventEvents)
    END_COM_MAP()

public:
    virtual ~CSink() {}
    STDMETHODIMP GetTypeInfoCount(UINT *pctinfo) { return E_NOTIMPL; }
    STDMETHODIMP GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo) { return E_NOTIMPL; }
    STDMETHODIMP GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId) { return E_NOTIMPL; }
    STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
    {
#ifdef _DEBUG
        printf("sink, id: %d, parm: %f", dispIdMember, pDispParams->rgvarg[0].fltVal);
#endif
        return S_OK;
    }
};

BOOL PostComMessage(DWORD pid, int msgtype, unsigned long long msgid, VARIANT *msg)
{
    HRESULT hr = S_OK;

    hr = CoInitializeEx(0, COINIT_APARTMENTTHREADED);
    if (FAILED(hr))
        return false;

    CComPtr<IRobotEvent> spRobotEvent;
    hr = ::CoCreateInstance(CLSID_RobotEvent, NULL, CLSCTX_ALL, IID_IRobotEvent, (LPVOID *)&spRobotEvent);
    if (SUCCEEDED(hr))
    {
        /*
        // 这段逻辑用于回调到微信
        CComObject<CSink>* sinkptr = nullptr;
        CComObject<CSink>::CreateInstance(&sinkptr);
        DWORD cookies = 0;
        AtlAdvise(spRobotEvent, sinkptr, __uuidof(_IRobotEventEvents), &cookies);
        */
        int __result = 0;
        spRobotEvent->CPostMessage(pid, msgtype, msgid, msg, &__result);
    }
    else
    {
        CoUninitialize();
        return false;
    }
    CoUninitialize();
    return true;
}
#endif // !USE_COM
