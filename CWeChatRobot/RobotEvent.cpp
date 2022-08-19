// RobotEvent.cpp: CRobotEvent 的实现

#include "pch.h"
#include "RobotEvent.h"

#define WX_MESSAGE 1
#define WX_LOG_MESSAGE 2

// CRobotEvent

STDMETHODIMP CRobotEvent::CPostMessage(DWORD pid, int msgtype, unsigned long long msgid, VARIANT *msg, int *__result)
{
    // TODO: 在此处添加实现代码

    // 将收到的消息广播给所有用户
    switch (msgtype)
    {
    case WX_MESSAGE:
    {
        Fire_OnGetMessageEvent(pid, msgid, msg);
        break;
    }
    case WX_LOG_MESSAGE:
    {
        break;
    }
    default:
        break;
    }
    *__result = 0;
    return S_OK;
}

/**
 * \param pid 微信PID
 * \param cookie Cookie
 */
STDMETHODIMP CRobotEvent::CRegisterWxPidWithCookie(DWORD pid, DWORD cookie, int *__result)
{
    WxPidToEventCookie[pid].insert(cookie);
    *__result = 0;
    return S_OK;
}
