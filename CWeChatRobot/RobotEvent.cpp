// RobotEvent.cpp: CRobotEvent 的实现

#include "pch.h"
#include "RobotEvent.h"


// CRobotEvent

STDMETHODIMP CRobotEvent::CPostMessage(VARIANT* msg, int* __result)
{
    // TODO: 在此处添加实现代码

    // 将收到的消息广播给所有用户
    Fire_OnGetMessageEvent(msg);
    *__result = 0;
    return S_OK;
}
