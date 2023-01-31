#include "pch.h"

#define SendEmotionCall1Offset 0x5FEC1980 - 0x5F750000
#define SendEmotionCall2Offset 0x5FBC77E0 - 0x5F750000
#define SendEmotionHandleOffset 0x61AEE888 - 0x5F750000

#ifndef USE_SOCKET
struct SendEmotionStruct
{
    wchar_t *wxid;
    wchar_t *img_path;
};
BOOL SendEmotionRemote(LPVOID lparameter)
{
    SendEmotionStruct *ses = (SendEmotionStruct *)lparameter;
    BOOL status = SendEmotion(ses->wxid, ses->img_path);
    return status;
}
#endif

BOOL __stdcall SendEmotion(wchar_t *wxid, wchar_t *img_path)
{
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD SendEmotionCall1 = WeChatWinBase + SendEmotionCall1Offset;
    DWORD SendEmotionCall2 = WeChatWinBase + SendEmotionCall2Offset;
    DWORD SendEmotionHandle = WeChatWinBase + SendEmotionHandleOffset;
    WxString p_wxid(wxid), p_img_path(img_path), p_null(NULL);
    char buf[0x1C] = {0};
    int isSuccess = 0x0;
    __asm {
        pushad;
        pushfd;
        mov ebx,dword ptr[SendEmotionHandle];
        lea eax,buf;
        push eax;
        push 0x0;
        sub esp,0x14;
        mov esi,esp;
        mov dword ptr [esi], 0x0;
        mov dword ptr [esi+0x4], 0x0;
        mov dword ptr [esi+0x8], 0x0;
        mov dword ptr [esi+0xC], 0x0;
        mov dword ptr [esi+0x10], 0x0;
        push 0x2;
        lea eax,p_wxid;
        sub esp,0x14;
        mov ecx,esp;
        push eax;
        call SendEmotionCall1;
        sub esp,0x14;
        mov esi,esp;
        mov dword ptr [esi], 0x0;
        mov dword ptr [esi+0x4], 0x0;
        mov dword ptr [esi+0x8], 0x0;
        mov dword ptr [esi+0xC], 0x0;
        mov dword ptr [esi+0x10], 0x0;
        sub esp,0x14;
        mov ecx,esp;
        lea eax,p_img_path;
        push eax;
        call SendEmotionCall1;
        mov ecx,ebx;
        call SendEmotionCall2;
        movzx eax,al;
        mov isSuccess,eax;
        popfd;
        popad;
    }
    return (isSuccess == 0x1);
}
