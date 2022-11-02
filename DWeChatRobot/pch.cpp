// pch.cpp: 与预编译标头对应的源文件

#include "pch.h"
#include "io.h"
#include <functional>

// 当使用预编译的头时，需要使用此源文件，编译才能成功。

/*
 * 创建一个控制台窗口
 * return：BOOL，成功返回`0`，失败返回`1`
 */
BOOL CreateConsole(void)
{
    if (AllocConsole())
    {
        AttachConsole(GetCurrentProcessId());
        FILE *retStream;
        freopen_s(&retStream, "CONOUT$", "w", stdout);
        if (!retStream)
            throw std::runtime_error("Stdout redirection failed.");
        freopen_s(&retStream, "CONOUT$", "w", stderr);
        if (!retStream)
            throw std::runtime_error("Stderr redirection failed.");
        return 0;
    }
    return 1;
}

/*
 * 获取`WeChatWin.dll`基址
 * return：DWORD，`WeChatWin.dll`模块基址
 */
DWORD GetWeChatWinBase()
{
    return (DWORD)GetModuleHandleA("WeChatWin.dll");
}

BOOL FindOrCreateDirectory(const wchar_t *pszPath)
{
    string dir = unicode_to_gb2312((wchar_t *)pszPath);
    char last_char = dir.back();
    if (last_char != '\\')
    {
        dir += '\\';
    }
    if (!_access(dir.c_str(), 0))
        return true;
    return MakeSureDirectoryPathExists(dir.c_str());
}

/*
 * 将宽字节字符串转换成`std::string`
 */
string unicode_to_gb2312(wchar_t *wchar)
{
    wchar_t *wText = wchar;
    DWORD dwNum = WideCharToMultiByte(CP_ACP, NULL, wText, -1, NULL, 0, NULL, FALSE);
    char *psText = new char[dwNum + 1];
    WideCharToMultiByte(CP_ACP, NULL, wText, -1, psText, dwNum, NULL, FALSE);
    psText[dwNum] = '\0';
    string szDst(psText);
    delete[] psText;
    return szDst;
}

/*
 * 将UTF8编码数据转换为GBK编码
 */
string utf8_to_gb2312(const char *strUTF8)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, NULL, 0);
    wchar_t *wszGBK = new wchar_t[len + 1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, wszGBK, len);

    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
    char *szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
    string strTemp(szGBK);
    delete[] szGBK;
    delete[] wszGBK;
    return strTemp;
}

string gb2312_to_utf8(const char *strGB2312)
{
    int len = MultiByteToWideChar(CP_ACP, 0, strGB2312, -1, NULL, 0);
    wchar_t *wszGBK = new wchar_t[len + 1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_ACP, 0, strGB2312, -1, wszGBK, len);

    len = WideCharToMultiByte(CP_UTF8, 0, wszGBK, -1, NULL, 0, NULL, NULL);
    char *szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte(CP_UTF8, 0, wszGBK, -1, szGBK, len, NULL, NULL);
    string strTemp(szGBK);
    delete[] szGBK;
    delete[] wszGBK;
    return strTemp;
}

wstring gb2312_to_unicode(const char *buffer)
{
    int c_size = MultiByteToWideChar(CP_ACP, 0, buffer, -1, 0, 0);
    wchar_t *temp = new wchar_t[c_size + 1];
    MultiByteToWideChar(CP_ACP, 0, buffer, -1, temp, c_size);
    temp[c_size] = L'\0';
    wstring ret(temp);
    delete[] temp;
    temp = NULL;
    return ret;
}

/*
 * 将UTF8编码数据转换为UNICODE编码
 */
wstring utf8_to_unicode(const char *buffer)
{
    int c_size = MultiByteToWideChar(CP_UTF8, 0, buffer, -1, 0, 0);
    wchar_t *temp = new wchar_t[c_size + 1];
    MultiByteToWideChar(CP_UTF8, 0, buffer, -1, temp, c_size);
    temp[c_size] = L'\0';
    wstring ret(temp);
    delete[] temp;
    temp = NULL;
    return ret;
}

string unicode_to_utf8(wchar_t *wstr)
{
    int c_size = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, FALSE);
    char *buffer = new char[c_size + 1];
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, buffer, c_size, NULL, FALSE);
    buffer[c_size] = '\0';
    string str(buffer);
    delete[] buffer;
    buffer = NULL;
    return str;
}

/*
 * 对任意地址添加HOOK
 * dwHookAddr：HOOK的目标地址
 * dwJmpAddress：跳转到的地址
 * originalRecieveCode：保存旧指令的数组
 * return：void
 */
void HookAnyAddress(DWORD dwHookAddr, LPVOID dwJmpAddress, char *originalRecieveCode)
{
    //组装跳转数据
    BYTE jmpCode[5] = {0};
    jmpCode[0] = 0xE9;

    //计算偏移
    *(DWORD *)&jmpCode[1] = (DWORD)dwJmpAddress - dwHookAddr - 5;

    // 保存以前的属性用于还原
    DWORD OldProtext = 0;

    // 因为要往代码段写入数据，又因为代码段是不可写的，所以需要修改属性
    VirtualProtect((LPVOID)dwHookAddr, 5, PAGE_EXECUTE_READWRITE, &OldProtext);
    ReadProcessMemory(GetCurrentProcess(), (LPVOID)dwHookAddr, originalRecieveCode, 5, 0);

    //写入自己的代码
    memcpy((void *)dwHookAddr, jmpCode, 5);

    // 执行完了操作之后需要进行还原
    VirtualProtect((LPVOID)dwHookAddr, 5, OldProtext, &OldProtext);
}

/*
 * 对任意地址取消HOOK
 * dwHookAddr：HOOK的目标地址
 * originalRecieveCode：保存旧指令的数组
 * return：void
 */
void UnHookAnyAddress(DWORD dwHookAddr, char *originalRecieveCode)
{
    DWORD OldProtext = 0;
    VirtualProtect((LPVOID)dwHookAddr, 5, PAGE_EXECUTE_READWRITE, &OldProtext);
    WriteProcessMemory(GetCurrentProcess(), (LPVOID)dwHookAddr, originalRecieveCode, 5, 0);
    VirtualProtect((LPVOID)dwHookAddr, 5, OldProtext, &OldProtext);
}

/*
 * 取消所有HOOK
 * return：void
 */
void UnHookAll()
{
    UnHookLogMsgInfo();
    UnHookReceiveMessage();
    UnHookFriendStatusCode();
    UnHookSearchContact();
    UnHookVoiceMsg();
    UnHookImageMsg();
    UnHookH5ExtBuf();
    UnHookQrcodeImage();
    UnHookA8KeyBuf();
    return;
}

/*
 * 将单字符替换为指定的字符串
 * source：源字符串
 * replaced：被替换的单字符
 * replaceto：替换成的字符串
 * return：std::wstring，替换后的字符串
 */
wstring wreplace(wstring source, wchar_t replaced, wstring replaceto)
{
    wstring temp = L"";
    wchar_t *buffer = (wchar_t *)source.c_str();
    for (unsigned int i = 0; i < source.length(); i++)
    {
        if (buffer[i] == replaced)
        {
            temp += replaceto;
            continue;
        }
        temp += buffer[i];
    }
    return temp;
}

/*
 * 获取当前时间
 */
wstring GetTimeW(long long timestamp)
{
    wchar_t *wstr = new wchar_t[20];
    memset(wstr, 0, 20 * 2);
    // time_t cTime = time(NULL);
    tm tm_out;
    localtime_s(&tm_out, &timestamp);
    swprintf_s(wstr, 20, L"%04d-%02d-%02d %02d:%02d:%02d",
               1900 + tm_out.tm_year, tm_out.tm_mon + 1, tm_out.tm_mday,
               tm_out.tm_hour, tm_out.tm_min, tm_out.tm_sec);
    wstring strTimeW(wstr);
    delete[] wstr;
    return strTimeW;
}

void PrintProcAddr()
{
    CreateConsole();
    printf("WeChatVersion %s\n", GetWeChatVerStr().c_str());
    printf("SendImage 0x%08X\n", (DWORD)SendImage);
    printf("SendText 0x%08X\n", (DWORD)SendText);
    printf("SendFile 0x%08X\n", (DWORD)SendFile);
    printf("SendAtText 0x%08X\n", (DWORD)SendAtText);
    printf("GetFriendList 0x%08X\n", (DWORD)GetFriendList);
    printf("GetUserInfoByWxId 0x%08X\n", (DWORD)GetUserInfoByWxId);
    printf("SendArticle 0x%08X\n", (DWORD)SendArticle);
    printf("SendCard 0x%08X\n", (DWORD)SendCard);
    void(__stdcall * check_friend_status)(wchar_t *) = CheckFriendStatus;
    printf("CheckFriendStatus 0x%08X\n", (DWORD)check_friend_status);
    BOOL(__stdcall * get_chatroom_members)
    (wchar_t *) = GetChatRoomMembers;
    printf("GetChatRoomMembers 0x%08X\n", (DWORD)get_chatroom_members);
    printf("ExecuteSql 0x%08X\n", (DWORD)ExecuteSQL);
    printf("BackupSQLiteDB 0x%08X\n", (DWORD)BackupSQLiteDB);
    printf("VerifyFriendApply 0x%08X\n", (DWORD)VerifyFriendApply);
    printf("AddFriendByV3 0x%08X\n", (DWORD)AddFriendByV3);
    printf("AddFriendByWxid 0x%08X\n", (DWORD)AddFriendByWxid);
    printf("AddBrandContact 0x%08X\n", (DWORD)AddBrandContact);
    void *(*select_data)(DWORD, const char *, void *) = SelectData;
    printf("SelectData 0x%08X\n", (DWORD)select_data);
    void *(__stdcall * search_contact_by_net)(wchar_t *) = SearchContactByNet;
    printf("SearchContactByNet 0x%08X\n", (DWORD)search_contact_by_net);
    printf("AddChatRoomMember 0x%08X\n", (DWORD)AddChatRoomMember);
    printf("DelChatRoomMember 0x%08X\n", (DWORD)DelChatRoomMember);
    printf("SetChatRoomAnnouncement 0x%08X\n", (DWORD)SetChatRoomAnnouncement);
    printf("SetChatRoomSelfNickname 0x%08X\n", (DWORD)SetChatRoomSelfNickname);
    printf("SetChatRoomName 0x%08X\n", (DWORD)SetChatRoomName);
    printf("OpenBrowser 0x%08X\n", (DWORD)OpenBrowser);
    BOOL(__stdcall * get_history_public_msg)
    (wchar_t *, wchar_t *) = GetHistoryPublicMsg;
    printf("GetHistoryPublicMsg 0x%08X\n", (DWORD)get_history_public_msg);
    BOOL(__stdcall * get_a8_key)
    (wchar_t *) = GetA8Key;
    printf("GetA8Key 0x%08X\n", (DWORD)get_a8_key);
}

BOOL ProcessIsWeChat()
{
    char szFileFullPath[MAX_PATH] = {0};
    GetModuleFileNameA(NULL, szFileFullPath, MAX_PATH);
    int length = ::strlen(szFileFullPath);
    string szFile(szFileFullPath);
    size_t pos = szFile.find_last_of('\\');
    string szProcessName = szFile.substr(pos + 1, szFile.length() - pos - 1);
    if (szProcessName == "WeChat.exe")
    {
        return TRUE;
    }
    return FALSE;
}

DWORD OffsetFromIdaAddr(DWORD idaAddr)
{
    return idaAddr - IDA_BASE;
}

void SignalHandler(const char *data, size_t size)
{
#ifdef USE_SOCKET
    string dllname = "SWeChatRobot";
#else
    string dllname = "DWeChatRobot";
#endif // USE_SOCKET
    char szFileFullPath[MAX_PATH] = {0};
    HMODULE hd = GetModuleHandleA(dllname.c_str());
    GetModuleFileNameA(hd, szFileFullPath, MAX_PATH);
    string szFile(szFileFullPath);
    size_t pos = szFile.find_last_of('\\');
    string szFilePath = szFile.substr(0, pos + 1);
    std::string glog_file = szFilePath + "\\log\\error.log";
    std::ofstream fs(glog_file, std::ios::app);
    std::string str = std::string(data, size);
    fs << str;
    fs.close();
    LOG(INFO) << str;
    google::ShutdownGoogleLogging();
}

void gLogInit()
{
#ifdef USE_SOCKET
    string dllname = "SWeChatRobot";
#else
    string dllname = "DWeChatRobot";
#endif // USE_SOCKET
#ifdef _DEBUG
    FLAGS_colorlogtostderr = true; // log信息区分颜色
    FLAGS_logtostderr = 1;
#endif
    google::SetStderrLogging(google::GLOG_INFO);  // 输出log的最低等级是 INFO (可以设置为WARNING或者更高)
    google::InstallFailureSignalHandler();        // 配置安装程序崩溃失败信号处理器
    google::InstallFailureWriter(&SignalHandler); // 安装配置程序失败信号的信息打印过程，设置回调函数
    google::InitGoogleLogging(dllname.c_str());
#ifndef _DEBUG
    char szFileFullPath[MAX_PATH] = {0};
    HMODULE hd = GetModuleHandleA(dllname.c_str());
    GetModuleFileNameA(hd, szFileFullPath, MAX_PATH);
    string szFile(szFileFullPath);
    size_t pos = szFile.find_last_of('\\');
    string szFilePath = szFile.substr(0, pos + 1) + "log\\";
    FindOrCreateDirectory(gb2312_to_unicode(szFilePath.c_str()).c_str());
    google::SetLogDestination(google::GLOG_INFO, szFilePath.c_str());
#endif
}
