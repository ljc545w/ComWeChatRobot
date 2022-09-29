// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        gLogInit();
        LOG(INFO) << "Robot Start, isWxLogin: " << isWxLogin() << ", pid: " << GetCurrentProcessId() << endl;
        if (ProcessIsWeChat())
        {
#ifndef USE_SOCKET
#ifdef _DEBUG
            PrintProcAddr();
            HookLogMsgInfo();
#endif
#endif
        }
        break;
    }
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
    {
        UnHookAll();
        LOG(INFO) << "Robot Stop";
        google::ShutdownGoogleLogging();
        break;
    }
    }
    return TRUE;
}
