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