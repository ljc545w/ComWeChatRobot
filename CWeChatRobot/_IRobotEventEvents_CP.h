#pragma once
#include <map>
#include <set>

/**
 * 微信PID与客户端事件ConnectionPoint的cookie的映射，客户端使用自己关心的微信PID和cookie进行注册，
 * 服务端根据PID向订阅该PID的客户端发起事件调用
 */
map<DWORD, set<DWORD>> WxPidToEventCookie;

template<class T>
class CProxy_IRobotEventEvents :
	public ATL::IConnectionPointImpl<T, &__uuidof(_IRobotEventEvents)>
{
public:
    HRESULT Fire_OnGetMessageEvent(DWORD pid, VARIANT* msg)
    {
        HRESULT hr = S_OK;
        T* pThis = static_cast<T*>(this);
        if (WxPidToEventCookie.count(pid)==0)
        {
            return hr;
        }
        const set<DWORD> cookies = WxPidToEventCookie[pid];
        for (DWORD cookie:cookies)
        {
            pThis->Lock();
            ATL::CComPtr<IUnknown> punkConnection=this->m_vec.GetUnknown(cookie);
            pThis->Unlock();
            if (punkConnection)
            {
                IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);
                if (pConnection)
                {
                    ATL::CComVariant varResult;

                    DISPPARAMS params = { msg, nullptr, 1, 0 };
                    hr = pConnection->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, nullptr, nullptr);
                }
                else
                {
                    WxPidToEventCookie[pid].erase(cookie);
                }
            }
            else
            {
                WxPidToEventCookie[pid].erase(cookie);
            }
        }
        return hr;
    }
};
