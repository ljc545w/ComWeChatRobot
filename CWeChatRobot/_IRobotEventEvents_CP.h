#pragma once

template<class T>
class CProxy_IRobotEventEvents :
	public ATL::IConnectionPointImpl<T, &__uuidof(_IRobotEventEvents)>
{
public:
    HRESULT Fire_OnGetMessageEvent(VARIANT* msg)
    {
        HRESULT hr = S_OK;
        T* pThis = static_cast<T*>(this);
        int cConnections = m_vec.GetSize();

        for (int iConnection = 0; iConnection < cConnections; iConnection++)
        {
            pThis->Lock();
            CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
            pThis->Unlock();

            IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

            if (pConnection)
            {
                /*CComVariant avarParams[1];
                avarParams[0] = msg;
                avarParams[0].vt = msg->vt;*/
                CComVariant varResult;

                DISPPARAMS params = { msg, NULL, 1, 0 };
                hr = pConnection->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
            }
        }
        return hr;
    }
};
