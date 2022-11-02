#include "pch.h"

#define WeChatFilePathOffset 0x2385020
#define DownloadImageCall1Offset 0x591D4010 - 0x590F0000
#define DownloadImageCall2Offset 0x5951AAE0 - 0x590F0000

#define ParserAppXmlCall1Offset 0x588F0C70 - 0x587D0000
#define ParserAppXmlCall2Offset 0x58E51340 - 0x587D0000
#define DownloadFileCall1Offset 0x58893B70 - 0x587D0000
#define DownloadFileCall2Offset 0x58BC7330 - 0x587D0000

#define ParserVideoXmlCallOffset 0x59784620 - 0x590F0000
#define DownloadVideoThumbCallOffset 0x58D3C9B0 - 0x587D0000
#define DownloadVideoCallOffset 0x58C852F0 - 0x587D0000

#define XorKeyOffset 0x5B4569D6 - 0x590F0000
#define GetWeChatCDNCallOffset 0x58C852F0 - 0x587D0000

#define AutoDownloadImageTimeSettingOffset 0x239EC50
#define ImageAutoPatchOffset 0x48D56B
#define AutoDownloadVideoTimeSettingOffset 0x239EBD8
#define VideoAutoPatchOffset 0x48CE1B

static BOOL AutoDownloadTimeSeted = FALSE;

void __stdcall SetDownloadTime()
{
    if (AutoDownloadTimeSeted)
        return;
    DWORD WeChatWinBase = GetWeChatWinBase();
    char settime[] = "00:00-00:00";
    DWORD AutoDownloadTimeSettingAddr1 = GetWeChatWinBase() + AutoDownloadImageTimeSettingOffset;
    DWORD AutoDownloadTimeSettingAddr2 = GetWeChatWinBase() + AutoDownloadVideoTimeSettingOffset;
    WriteProcessMemory(GetCurrentProcess(), (LPVOID)AutoDownloadTimeSettingAddr1, settime, strlen(settime) + 1, 0);
    WriteProcessMemory(GetCurrentProcess(), (LPVOID)AutoDownloadTimeSettingAddr2, settime, strlen(settime) + 1, 0);

    BYTE nopVideo[] = {0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
    DWORD nopVideoAddr = WeChatWinBase + VideoAutoPatchOffset;
    WriteProcessMemory(GetCurrentProcess(), (LPVOID)nopVideoAddr, &nopVideo, sizeof(nopVideo), 0);

    BYTE nopImg[] = {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
    DWORD nopImgAddr = WeChatWinBase + ImageAutoPatchOffset;
    WriteProcessMemory(GetCurrentProcess(), (LPVOID)nopImgAddr, &nopImg, sizeof(nopImg), 0);
    AutoDownloadTimeSeted = TRUE;
}

static BOOL GetWeChatCDN(CHAT_MSG *chat_msg)
{
    DWORD GetWeChatCDNCall = GetWeChatWinBase() + GetWeChatCDNCallOffset;
    int isSuccess = 0x0;
    __asm {
        pushad;
        pushfd;
        mov esi,dword ptr[chat_msg];
        push esi;
        call GetWeChatCDNCall;
        movzx eax,al;
        mov isSuccess,eax;
        popfd;
        popad;
    }
    return (isSuccess == 0x1);
}

BOOL ParserAppXml(wchar_t *xml, char *xml_buf)
{
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD ParserAppXmlCall1 = WeChatWinBase + ParserAppXmlCall1Offset;
    DWORD ParserAppXmlCall2 = WeChatWinBase + ParserAppXmlCall2Offset;
    WxString p_xml(xml);
    int isSuccess = 0x0;
    __asm {
        pushad;
        pushfd;
        mov ecx,dword ptr [xml_buf];
        call ParserAppXmlCall1;
        mov ecx,dword ptr [xml_buf];
        lea eax,p_xml;
        push 0x1;
        push eax;
        call ParserAppXmlCall2;
        movzx eax,al;
        mov isSuccess,eax;
        popfd;
        popad;
    }
    return (isSuccess == 0x1);
}

BOOL ParserVideoXml(wchar_t *xml, char *xml_buf)
{
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD ParserVideoXmlCall = WeChatWinBase + ParserVideoXmlCallOffset;
    string s_xml = unicode_to_utf8(xml).c_str();
    WxStringA p_xml(s_xml);
    int isSuccess = 0x0;
    __asm {
        pushad;
        pushfd;
        mov ecx,dword ptr [xml_buf];
        lea eax,p_xml;
        push eax;
        call ParserVideoXmlCall;
        movzx eax,al;
        mov isSuccess,eax;
        popfd;
        popad;
    }
    return (isSuccess == 0x1);
}

wstring GetWeChatFilePath()
{
    DWORD WeChatWinBase = GetWeChatWinBase();
    WxString *wechat_file_path = (WxString *)(WeChatWinBase + 0x2385020);
    return wstring(wechat_file_path->buffer, wechat_file_path->length);
}

wstring __stdcall GetMsgCDN(ULONG64 msgid)
{
    SetDownloadTime();
    wstring robot_base_dir = GetWeChatFilePath() + L"WeChatRobot\\";
    CHAT_MSG chat_msg = {0};
    if (!FindOrCreateDirectory(robot_base_dir.c_str()) || !GetChatMsgBySvrId(msgid, &chat_msg))
        return L"";
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD param[4] = {0};
    param[0] = (DWORD)&param;
    param[1] = (DWORD)&param;
    param[2] = (DWORD)&param;
    param[3] = 0x101;
    chat_msg.unknown_ptr2 = &param;
    int isSuccess = 0x0;
    wstring abs_path(robot_base_dir);
    switch (chat_msg.type)
    {
    case 0x3:
    {
        abs_path += L"Image\\";
        FindOrCreateDirectory(abs_path.c_str());
        abs_path = abs_path + gb2312_to_unicode(to_string(msgid).c_str()) + L".png";
        if (!_waccess(abs_path.c_str(), 0))
            return abs_path;
        CHAT_MSG image_msg = {0};
        image_msg.handle1 = chat_msg.handle1;
        image_msg.content = chat_msg.content;
        image_msg.unknown_ptr2 = chat_msg.unknown_ptr2;
        image_msg.file_save_path.buffer = (wchar_t *)abs_path.c_str();
        image_msg.file_save_path.length = abs_path.length();
        image_msg.file_save_path.maxLength = abs_path.length() * 2;
        DWORD DownloadImageCall1 = WeChatWinBase + DownloadImageCall1Offset;
        DWORD DownloadImageCall2 = WeChatWinBase + DownloadImageCall2Offset;
        __asm {
            pushad;
            pushfd;
            call DownloadImageCall1;
            push 0x0;
            push 0x0;
            push 0x1;
            lea ecx,image_msg;
            push ecx;
            mov ecx,eax;
            call DownloadImageCall2;
            movzx eax,al;
            mov isSuccess,eax;
            popfd;
            popad;
        }
        break;
    }
    case 0x2B:
    {
        abs_path += L"Video\\";
        FindOrCreateDirectory(abs_path.c_str());
        unique_ptr<char[]> xml_data = make_unique<char[]>(0x680);
        char *xml_buf = xml_data.get();
        if (!ParserVideoXml(chat_msg.content.buffer, xml_buf))
            return L"";
        DWORD DownloadVideoThumbCall = WeChatWinBase + DownloadVideoThumbCallOffset;
        DWORD DownloadVideoCall = WeChatWinBase + DownloadVideoCallOffset;
        wstring thumbnail_path = abs_path + gb2312_to_unicode(to_string(msgid).c_str()) + L".jpg";
        abs_path = abs_path + gb2312_to_unicode(to_string(msgid).c_str()) + L".mp4";
        if (!_waccess(abs_path.c_str(), 0))
            return abs_path;
        chat_msg.file_save_path.buffer = (wchar_t *)abs_path.c_str();
        chat_msg.file_save_path.length = abs_path.length();
        chat_msg.file_save_path.maxLength = abs_path.length() * 2;
        chat_msg.thumbnail.buffer = (wchar_t *)thumbnail_path.c_str();
        chat_msg.thumbnail.length = thumbnail_path.length();
        chat_msg.thumbnail.maxLength = thumbnail_path.length() * 2;
        DWORD param[20] = {0};
        param[17] = 0x1FFF;
        __asm {
            pushad;
            pushfd;
            mov eax,dword ptr [xml_buf];
            push eax;
            lea eax,chat_msg;
            push eax;
            lea eax,param;
            push eax;
            call DownloadVideoThumbCall;
            lea esi,chat_msg;
            push esi;
            call DownloadVideoCall;
            movzx eax,al;
            mov isSuccess,eax;
            popfd;
            popad;
        }
        break;
    }
    case 0x31:
    {
        abs_path += L"Files\\";
        FindOrCreateDirectory(abs_path.c_str());
        unique_ptr<char[]> xml_data = make_unique<char[]>(0xC48);
        char *xml_buf = xml_data.get();
        if (!ParserAppXml(chat_msg.content.buffer, xml_buf))
            return L"";
        DWORD DownloadFileCall1 = WeChatWinBase + DownloadFileCall1Offset;
        DWORD DownloadFileCall2 = WeChatWinBase + DownloadFileCall2Offset;
        WxString *filename = (WxString *)((DWORD)xml_buf + 0x44);
        abs_path = abs_path + gb2312_to_unicode(to_string(msgid).c_str()) + L"_" + wstring(filename->buffer, filename->length);
        if (!_waccess(abs_path.c_str(), 0))
            return abs_path;
        chat_msg.file_save_path.buffer = (wchar_t *)abs_path.c_str();
        chat_msg.file_save_path.length = abs_path.length();
        chat_msg.file_save_path.maxLength = abs_path.length() * 2;
        __asm {
            pushad;
            pushfd;
            call DownloadFileCall1;
            mov ecx,dword ptr [xml_buf];
            push ecx;
            lea ecx,chat_msg;
            push ecx;
            mov ecx,eax;
            call DownloadFileCall2;
            xor al,0x1;
            movzx eax,al;
            mov isSuccess,eax;
            popfd;
            popad;
        }
        break;
    }
    default:
        break;
    }
    return (isSuccess == 0x1) ? abs_path : L"";
}

#ifndef USE_SOCKET
struct GetMsgCDNRespStruct
{
    wchar_t *path = NULL;
    int length = 0;
} cdn_resp;

DWORD GetMsgCDNRemote(ULONG64 *p_msgid)
{
    if (cdn_resp.length != 0)
    {
        delete[] cdn_resp.path;
        cdn_resp.length = 0;
    }
    wstring cdn_path = GetMsgCDN(*p_msgid);
    if (cdn_path.length() == 0)
        return 0;
    cdn_resp.path = new wchar_t[cdn_path.length() + 1]();
    cdn_resp.length = cdn_path.length();
    memcpy(cdn_resp.path, cdn_path.c_str(), cdn_path.length() * 2);
    return (DWORD)&cdn_resp;
}
#endif
