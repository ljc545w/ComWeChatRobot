// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

#ifdef _DEBUG
    int detach_count = 0;
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
#ifdef _DEBUG
        CreateConsole();
        DWORD base = (DWORD)GetModuleHandleA("DWeChatRobot.dll");

        printf("SendImage 0x%08X\n", (DWORD)SendImage);
        printf("SendText 0x%08X\n", (DWORD)SendText);
        printf("SendFile 0x%08X\n", (DWORD)SendFile);
        printf("GetFriendList 0x%08X\n", (DWORD)GetFriendList);
        printf("GetUserInfoByWxId 0x%08X\n", (DWORD)GetUserInfoByWxId);
        printf("SendArticle 0x%08X\n", (DWORD)SendArticle);
        printf("SendCard 0x%08X\n", (DWORD)SendCard);
        printf("CheckFriendStatus 0x%08X\n", (DWORD)CheckFriendStatus);
#endif
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH: {
#ifdef _DEBUG
        detach_count++;
        if (detach_count != 1) {
            FreeConsole();
            // UnHookAll();
        }
#endif
        break;
    }
    }
    return TRUE;
}

