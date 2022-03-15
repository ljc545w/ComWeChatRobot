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
        printf("HookExtractExpression 0x%08X\n", (DWORD)HookExtractExpression);
        printf("GetWxUserInfoByWxid 0x%08X\n", (DWORD)GetWxUserInfoByWxid);
        printf("GetUserDetailInfoByWxId 0x%08X\n", (DWORD)GetUserDetailInfoByWxId);
        printf("GetWxUserInfoRemote 0x%08X\n", (DWORD)GetWxUserInfoRemote);
        system("pause");
        wchar_t* wxid = (wchar_t*)L"wxid_ltedgfwcw7yu22";
        GetWxUserInfoRemote(wxid);
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

