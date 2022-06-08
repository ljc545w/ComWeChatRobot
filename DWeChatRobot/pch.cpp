// pch.cpp: 与预编译标头对应的源文件

#include "pch.h"

// 当使用预编译的头时，需要使用此源文件，编译才能成功。

/*
* 创建一个控制台窗口
* return：BOOL，成功返回`0`，失败返回`1`
*/
BOOL CreateConsole(void) {
    if (AllocConsole()) {
        AttachConsole(GetCurrentProcessId());
        FILE* retStream;
        freopen_s(&retStream, "CONOUT$", "w", stdout);
        if (!retStream) throw std::runtime_error("Stdout redirection failed.");
        freopen_s(&retStream, "CONOUT$", "w", stderr);
        if (!retStream) throw std::runtime_error("Stderr redirection failed.");
        return 0;
    }
    return 1;
}

/*
* 获取`WeChatWin.dll`基址
* return：DWORD，`WeChatWin.dll`模块基址
*/
DWORD GetWeChatWinBase() {
    return (DWORD)GetModuleHandleA("WeChatWin.dll");
}

/*
* 将宽字节字符串转换成`std::string`
*/
void Wchar_tToString(std::string& szDst, wchar_t* wchar)
{
    wchar_t* wText = wchar;
    DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte的运用
    char* psText; // psText为char*的临时数组，作为赋值给std::string的中间变量
    psText = new char[dwNum];
    WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte的再次运用
    szDst = psText;// std::string赋值
    delete[]psText;// psText的清除
}

/*
* 将UTF8编码数据转换为GBK编码
*/
string UTF8ToGBK(const std::string& strUTF8)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
    wchar_t* wszGBK = new wchar_t[len + 1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, wszGBK, len);

    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
    char* szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
    //strUTF8 = szGBK;  
    std::string strTemp(szGBK);
    delete[]szGBK;
    delete[]wszGBK;
    return strTemp;
}

/*
* 对任意地址添加HOOK
* dwHookAddr：HOOK的目标地址
* dwJmpAddress：跳转到的地址
* originalRecieveCode：保存旧指令的数组
* return：void
*/
void HookAnyAddress(DWORD dwHookAddr, LPVOID dwJmpAddress,char* originalRecieveCode)
{
    //组装跳转数据
    BYTE jmpCode[5] = { 0 };
    jmpCode[0] = 0xE9;

    //计算偏移
    *(DWORD*)&jmpCode[1] = (DWORD)dwJmpAddress - dwHookAddr - 5;

    // 保存以前的属性用于还原
    DWORD OldProtext = 0;

    // 因为要往代码段写入数据，又因为代码段是不可写的，所以需要修改属性
    VirtualProtect((LPVOID)dwHookAddr, 5, PAGE_EXECUTE_READWRITE, &OldProtext);
    ReadProcessMemory(GetCurrentProcess(), (LPVOID)dwHookAddr, originalRecieveCode, 5, 0);

    //写入自己的代码
    memcpy((void*)dwHookAddr, jmpCode, 5);

    // 执行完了操作之后需要进行还原
    VirtualProtect((LPVOID)dwHookAddr, 5, OldProtext, &OldProtext);
}

/*
* 对任意地址取消HOOK
* dwHookAddr：HOOK的目标地址
* originalRecieveCode：保存旧指令的数组
* return：void
*/
void UnHookAnyAddress(DWORD dwHookAddr, char* originalRecieveCode)
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
void UnHookAll() {
    UnHookLogMsgInfo();
    UnHookReceiveMessage();
    StopSearchContactHook();
    return;
}

/*
* 将单字符替换为指定的字符串
* source：源字符串
* replaced：被替换的单字符
* replaceto：替换成的字符串
* return：std::wstring，替换后的字符串
*/
wstring wreplace(wstring source, wchar_t replaced, wstring replaceto) {
    wstring temp = L"";
    wchar_t* buffer = (wchar_t*)source.c_str();
    for (unsigned int i = 0; i < source.length(); i++) {
        if (buffer[i] == replaced) {
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
wchar_t* GetTimeW() {
    wchar_t* wstr = new wchar_t[20];
    memset(wstr, 0, 20 * 2);
    time_t cTime = time(NULL);
    tm tm_out;
    localtime_s(&tm_out, &cTime);
    swprintf_s(wstr,20, L"%04d-%02d-%02d %02d:%02d:%02d",
        1900 + tm_out.tm_year, tm_out.tm_mon + 1, tm_out.tm_mday,
        tm_out.tm_hour, tm_out.tm_min, tm_out.tm_sec);
    return wstr;
}

void PrintProcAddr() {
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
    printf("CheckFriendStatus 0x%08X\n", (DWORD)CheckFriendStatus);
    printf("GetChatRoomMembers 0x%08X\n", (DWORD)GetChatRoomMembers);
    printf("ExecuteSql 0x%08X\n", (DWORD)ExecuteSQL);
    printf("BackupSQLiteDB 0x%08X\n", (DWORD)BackupSQLiteDBRemote);
    printf("VerifyFriendApply 0x%08X\n", (DWORD)VerifyFriendApply);
    printf("AddFriendByV3 0x%08X\n", (DWORD)AddFriendByV3);
    printf("AddFriendByWxid 0x%08X\n", (DWORD)AddFriendByWxid);
    printf("SelectDataRemote 0x%08X\n", (DWORD)SelectDataRemote);
    printf("SearchContactByNet 0x%08X\n", (DWORD)SearchContactByNet);
    printf("SearchContactByNetRemote 0x%08X\n", (DWORD)SearchContactByNetRemote);
}

BOOL ProcessIsWeChat()
{
    char szFileFullPath[MAX_PATH] = { 0 }, szProcessName[MAX_PATH] = { 0 };
    GetModuleFileNameA(NULL, szFileFullPath, MAX_PATH);
    int length = ::strlen(szFileFullPath);
    for (int i = length - 1; i >= 0; i--)
    {
        if (szFileFullPath[i] == '\\')
        {
            i++;
            for (int j = 0; i <= length; j++)
            {
                szProcessName[j] = szFileFullPath[i++];
            }
            break;
        }
    }

    if (::strcmp(szProcessName, "WeChat.exe") != 0)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

DWORD GetWeChatVerInt()
{
    WCHAR VersionFilePath[MAX_PATH];
    BYTE WeChatVersion[4] = { 0 };
    if (GetModuleFileName((HMODULE)GetWeChatWinBase(), VersionFilePath, MAX_PATH) == 0)
    {
        return 0;
    }
    
    VS_FIXEDFILEINFO* pVsInfo;
    unsigned int iFileInfoSize = sizeof(VS_FIXEDFILEINFO);
    int iVerInfoSize = GetFileVersionInfoSize(VersionFilePath, NULL);
    if (iVerInfoSize != 0) {
        char* pBuf = new char[iVerInfoSize];
        if (GetFileVersionInfo(VersionFilePath, 0, iVerInfoSize, pBuf)) {
            if (VerQueryValue(pBuf, TEXT("\\"), (void**)&pVsInfo, &iFileInfoSize)) {
                WeChatVersion[3] = (BYTE)(0x60 + (pVsInfo->dwFileVersionMS >> 16) & 0x0000FFFF);
                WeChatVersion[2] = (BYTE)(pVsInfo->dwFileVersionMS & 0x0000FFFF);
                WeChatVersion[1] = (BYTE)((pVsInfo->dwFileVersionLS >> 16) & 0x0000FFFF);
                WeChatVersion[0] = (BYTE)(pVsInfo->dwFileVersionLS & 0x0000FFFF);
            }
        }
        delete[] pBuf;
    }
    return *(DWORD*)WeChatVersion;
}

string GetWeChatVerStr() {
    DWORD WeChatVersion = GetWeChatVerInt();
    if (WeChatVersion == 0)
        return "null";
    string wxver = "";
    BYTE* pWxVer = (BYTE*)&WeChatVersion;
    strstream wxVer;
    wxVer << (int)pWxVer[3] - 0x60 << "." << (int)pWxVer[2] << "." << (int)pWxVer[1] << "." << (int)pWxVer[0];
    wxVer >> wxver;
    return wxver;
}

DWORD OffsetFromIdaAddr(DWORD idaAddr) {
    return idaAddr - IDA_BASE;
}