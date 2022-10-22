#include "pch.h"

// 发送文件CALL1偏移
#define SendFileCall1Offset (0x78763B70 - 0x786A0000)
// 发送文件CALL2偏移
#define SendFileCall2Offset (0x78E11980 - 0x786A0000)
// 发送文件CALL3偏移
#define SendFileCall3Offset (0x78A8D8C0 - 0x786A0000)
// 清空缓存CALL偏移
#define DeleteSendFileCacheCallOffset (0x78757780 - 0x786A0000)

/*
 * 外部调用时传递的参数结构
 * wxid：wxid的保存地址
 * filepath：文件绝对路径的保存地址
 */
#ifndef USE_SOCKET
struct FileParamStruct
{
    DWORD wxid;
    DWORD filepath;
};
#endif

/*
 * 内存中使用的参数结构
 * type：消息类型，文件消息为3
 * buffer：文件绝对路径
 * length：绝对路径字符数
 * maxLength：绝对路径最大字节数
 * fill：占位用空缓冲区
 * WxFileStruct：默认构造函数
 */
struct WxFileStruct
{
    int type = 3;
    wchar_t *buffer;
    DWORD length;
    DWORD maxLength;
    char fill[0x34] = {0};

    WxFileStruct(wchar_t *pStr)
    {
        buffer = pStr;
        length = wcslen(pStr);
        maxLength = wcslen(pStr) * 2;
    }
};

/*
 * 供外部调用的发送文件消息接口
 * lpParamStruct：FileParamStruct类型结构体指针
 * return：void
 */
#ifndef USE_SOCKET
void SendFileRemote(LPVOID lpParamStruct)
{
    FileParamStruct *params = (FileParamStruct *)lpParamStruct;
    SendFile((WCHAR *)params->wxid, (WCHAR *)params->filepath);
}
#endif

/*
 * 发送文件消息的具体实现
 * receiver：接收人wxid
 * FilePath：文件绝对路径
 * return：BOOL，发送成功返回`1`，发送失败返回`0`
 */
BOOL __stdcall SendFile(wchar_t *receiver, wchar_t *FilePath)
{
    WxString pReceiver(receiver);
    WxString pFilePath(FilePath);
    // WxFileStruct esi_(FilePath);
    WxString nullbuffer = {0};

    DWORD WeChatWinBase = GetWeChatWinBase();

    DWORD WxSendFileCall1 = WeChatWinBase + SendFileCall1Offset;
    DWORD WxSendFileCall2 = WeChatWinBase + SendFileCall2Offset;
    DWORD WxSendFileCall3 = WeChatWinBase + SendFileCall3Offset;
    DWORD DeleteSendFileCacheCall = WeChatWinBase + DeleteSendFileCacheCallOffset;
    DWORD WxSendFileParams = 0;

    char buffer[0x3B0] = {0};

    // DWORD edi_ = pReceiver.length;
    DWORD ptrReceiver = (DWORD)pReceiver.buffer;
    int isSuccess = 0;
    __asm {
		pushad;
		pushfd;
		call WxSendFileCall1;
		sub esp, 0x14;
		mov WxSendFileParams, eax;
		lea eax, nullbuffer;
		mov ecx, esp;
		push eax;
		call WxSendFileCall2;
		push 0x00DBE200;
		sub esp, 0x14;
		mov edi, esp;
		mov dword ptr ds : [edi] , 0x0;
		mov dword ptr ds : [edi + 0x4] , 0x0;
		mov dword ptr ds : [edi + 0x8] , 0x0;
		mov dword ptr ds : [edi + 0xC] , 0x0;
		mov dword ptr ds : [edi + 0x10] , 0x0;
		sub esp, 0x14;
		lea eax, pFilePath;
		mov ecx, esp;
		push eax;
		call WxSendFileCall2;
		sub esp, 0x14;
		lea eax, pReceiver;
		mov ecx, esp;
		push eax;
		call WxSendFileCall2;
		mov ecx, dword ptr [WxSendFileParams];
		lea eax, buffer;
		push eax;
		call WxSendFileCall3;
		mov al,byte ptr [eax + 0x38];
		movzx eax,al;
		mov isSuccess,eax;
        lea ecx, buffer;
        call DeleteSendFileCacheCall;
		popfd;
		popad;
    }
    return (isSuccess == 0x31);
}
