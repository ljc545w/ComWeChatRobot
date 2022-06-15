// pch.cpp: 与预编译标头对应的源文件

#include "pch.h"

// 当使用预编译的头时，需要使用此源文件，编译才能成功。
DWORD SendImageOffset = 0x0;
DWORD SendTextOffset = 0x0;
DWORD SendFileOffset = 0x0;
DWORD SendArticleOffset = 0x0;
DWORD SendCardOffset = 0x0;
DWORD SendAtTextOffset = 0x0;
DWORD SendAppMsgRemoteOffset = 0x0;

DWORD GetFriendListInitOffset = 0x0;
DWORD GetFriendListRemoteOffset = 0x0;
DWORD GetFriendListFinishOffset = 0x0;

DWORD GetWxUserInfoOffset = 0x0;
DWORD DeleteUserInfoCacheOffset = 0x0;

DWORD GetSelfInfoOffset = 0x0;
DWORD DeleteSelfInfoCacheOffset = 0x0;
DWORD SearchContactByNetRemoteOffset = 0x0;

DWORD VerifyFriendApplyOffset = 0x0;

DWORD CheckFriendStatusInitRemoteOffset = 0x0;
DWORD CheckFriendStatusRemoteOffset = 0x0;
DWORD CheckFriendStatusFinishRemoteOffset = 0x0;

DWORD HookReceiveMessageRemoteOffset = 0x0;
DWORD UnHookReceiveMessageRemoteOffset = 0x0;
DWORD GetHeadMessageRemoteOffset = 0x0;
DWORD PopHeadMessageRemoteOffset = 0x0;

DWORD GetChatRoomMembersRemoteOffset = 0x0;

DWORD GetDbHandlesRemoteOffset = 0x0;
DWORD ExecuteSQLRemoteOffset = 0x0;
DWORD SelectDataRemoteOffset = 0x0;
DWORD BackupSQLiteDBRemoteOffset = 0x0;

DWORD AddFriendByWxidRemoteOffset = 0x0;
DWORD AddFriendByV3RemoteOffset = 0x0;
DWORD DeleteUserRemoteOffset = 0x0;
DWORD AddBrandContactRemoteOffset = 0x0;

DWORD HookImageMsgRemoteOffset = 0x0;
DWORD UnHookImageMsgRemoteOffset = 0x0;
DWORD HookVoiceMsgRemoteOffset = 0x0;
DWORD UnHookVoiceMsgRemoteOffset = 0x0;

DWORD ChangeWeChatVerRemoteOffset = 0x0;

wstring SelfInfoString = L"";

HANDLE hProcess = NULL;

BOOL isFileExists_stat(string& name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

BOOL CreateConsole() {
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

DWORD GetWeChatRobotBase() {
    if (!hProcess)
        return 0;
    DWORD dwWriteSize = 0;
    LPVOID pRemoteAddress = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    if (pRemoteAddress)
        WriteProcessMemory(hProcess, pRemoteAddress, dllname, wcslen(dllname) * 2 + 2, &dwWriteSize);
    else
        return 0;
    DWORD dwHandle, dwID;
    LPVOID pFunc = GetModuleHandleW;
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunc, pRemoteAddress, 0, &dwID);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &dwHandle);
    }
    else {
        return 0;
    }
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pRemoteAddress, 0, MEM_RELEASE);
    return dwHandle;
}

BOOL GetProcOffset(wchar_t* workPath) {
    wchar_t* dllpath = new wchar_t[MAX_PATH];
    memset(dllpath, 0, MAX_PATH * 2);
    swprintf_s(dllpath, MAX_PATH, L"%ws%ws%ws", workPath, L"\\", dllname);
    string name = _com_util::ConvertBSTRToString((BSTR)dllpath);
    if (!isFileExists_stat(name)) {
        MessageBoxA(NULL, name.c_str(), "文件不存在", MB_ICONWARNING);
        return 0;
    }
    HMODULE hd = LoadLibrary(dllpath);
    if (!hd) {
        return 0;
    }
    DWORD WeChatBase = (DWORD)GetModuleHandleW(dllname);
    DWORD SendImageProcAddr = (DWORD)GetProcAddress(hd, SendImageRemote);
    SendImageOffset = SendImageProcAddr - WeChatBase;
    DWORD SendTextProcAddr = (DWORD)GetProcAddress(hd, SendTextRemote);
    SendTextOffset = SendTextProcAddr - WeChatBase;
    DWORD SendFileProcAddr = (DWORD)GetProcAddress(hd, SendFileRemote);
    SendFileOffset = SendFileProcAddr - WeChatBase;
    DWORD SendArticleProcAddr = (DWORD)GetProcAddress(hd, SendArticleRemote);
    SendArticleOffset = SendArticleProcAddr - WeChatBase;
    DWORD SendCardProcAddr = (DWORD)GetProcAddress(hd, SendCardRemote);
    SendCardOffset = SendCardProcAddr - WeChatBase;
    DWORD SendAtTextProcAddr = (DWORD)GetProcAddress(hd, SendAtTextRemote);
    SendAtTextOffset = SendAtTextProcAddr - WeChatBase;
    DWORD SendAppMsgProcAddr = (DWORD)GetProcAddress(hd, SendAppMsgRemote);
    SendAppMsgRemoteOffset = SendAppMsgProcAddr - WeChatBase;

    DWORD GetFriendListInitProcAddr = (DWORD)GetProcAddress(hd, GetFriendListInit);
    GetFriendListInitOffset = GetFriendListInitProcAddr - WeChatBase;
    DWORD GetFriendListRemoteProcAddr = (DWORD)GetProcAddress(hd, GetFriendListRemote);
    GetFriendListRemoteOffset = GetFriendListRemoteProcAddr - WeChatBase;
    DWORD GetFriendListFinishProcAddr = (DWORD)GetProcAddress(hd, GetFriendListFinish);
    GetFriendListFinishOffset = GetFriendListFinishProcAddr - WeChatBase;

    DWORD GetWxUserInfoProcAddr = (DWORD)GetProcAddress(hd, GetWxUserInfoRemote);
    GetWxUserInfoOffset = GetWxUserInfoProcAddr - WeChatBase;
    DWORD DeleteUserInfoCacheProcAddr = (DWORD)GetProcAddress(hd, DeleteUserInfoCacheRemote);
    DeleteUserInfoCacheOffset = DeleteUserInfoCacheProcAddr - WeChatBase;

    DWORD VerifyFriendApplyProcAddr = (DWORD)GetProcAddress(hd, VerifyFriendApplyRemote);
    VerifyFriendApplyOffset = VerifyFriendApplyProcAddr - WeChatBase;

    DWORD GetSelfInfoProcAddr = (DWORD)GetProcAddress(hd, GetSelfInfoRemote);
    GetSelfInfoOffset = GetSelfInfoProcAddr - WeChatBase;
    DWORD DeleteSelfInfoCacheProcAddr = (DWORD)GetProcAddress(hd, DeleteSelfInfoCacheRemote);
    DeleteSelfInfoCacheOffset = DeleteSelfInfoCacheProcAddr - WeChatBase;
    DWORD SearchContactByNetRemoteAddr = (DWORD)GetProcAddress(hd, SearchContactByNetRemote);
    SearchContactByNetRemoteOffset = SearchContactByNetRemoteAddr - WeChatBase;

    DWORD CheckFriendStatusInitRemoteAddr = (DWORD)GetProcAddress(hd, CheckFriendStatusInitRemote);
    CheckFriendStatusInitRemoteOffset = CheckFriendStatusInitRemoteAddr - WeChatBase;
    DWORD CheckFriendStatusRemoteAddr = (DWORD)GetProcAddress(hd, CheckFriendStatusRemote);
    CheckFriendStatusRemoteOffset = CheckFriendStatusRemoteAddr - WeChatBase;
    DWORD CheckFriendStatusFinishRemoteAddr = (DWORD)GetProcAddress(hd, CheckFriendStatusFinishRemote);
    CheckFriendStatusFinishRemoteOffset = CheckFriendStatusFinishRemoteAddr - WeChatBase;

    DWORD HookReceiveMessageRemoteAddr = (DWORD)GetProcAddress(hd, HookReceiveMessageRemote);
    HookReceiveMessageRemoteOffset = HookReceiveMessageRemoteAddr - WeChatBase;
    DWORD UnHookReceiveMessageRemoteAddr = (DWORD)GetProcAddress(hd, UnHookReceiveMessageRemote);
    UnHookReceiveMessageRemoteOffset = UnHookReceiveMessageRemoteAddr - WeChatBase;
    DWORD GetHeadMessageRemoteAddr = (DWORD)GetProcAddress(hd, GetHeadMessageRemote);
    GetHeadMessageRemoteOffset = GetHeadMessageRemoteAddr - WeChatBase;
    DWORD PopHeadMessageRemoteAddr = (DWORD)GetProcAddress(hd, PopHeadMessageRemote);
    PopHeadMessageRemoteOffset = PopHeadMessageRemoteAddr - WeChatBase;

    DWORD GetChatRoomMembersRemoteAddr = (DWORD)GetProcAddress(hd, GetChatRoomMembersRemote);
    GetChatRoomMembersRemoteOffset = GetChatRoomMembersRemoteAddr - WeChatBase;

    DWORD GetDbHandlesRemoteAddr = (DWORD)GetProcAddress(hd, GetDbHandlesRemote);
    GetDbHandlesRemoteOffset = GetDbHandlesRemoteAddr - WeChatBase;
    DWORD ExecuteSQLRemoteAddr = (DWORD)GetProcAddress(hd, ExecuteSQLRemote);
    ExecuteSQLRemoteOffset = ExecuteSQLRemoteAddr - WeChatBase;
    DWORD SelectDataRemoteAddr = (DWORD)GetProcAddress(hd, SelectDataRemote);
    SelectDataRemoteOffset = SelectDataRemoteAddr - WeChatBase;
    DWORD BackupSQLiteDBRemoteAddr = (DWORD)GetProcAddress(hd, BackupSQLiteDBRemote);
    BackupSQLiteDBRemoteOffset = BackupSQLiteDBRemoteAddr - WeChatBase;

    DWORD AddFriendByWxidRemoteAddr = (DWORD)GetProcAddress(hd, AddFriendByWxidRemote);
    AddFriendByWxidRemoteOffset = AddFriendByWxidRemoteAddr - WeChatBase;
    DWORD AddFriendByV3RemoteAddr = (DWORD)GetProcAddress(hd, AddFriendByV3Remote);
    AddFriendByV3RemoteOffset = AddFriendByV3RemoteAddr - WeChatBase;
    DWORD DeleteUserRemoteAddr = (DWORD)GetProcAddress(hd, DeleteUserRemote);
    DeleteUserRemoteOffset = DeleteUserRemoteAddr - WeChatBase;
    DWORD AddBrandContactRemoteAddr = (DWORD)GetProcAddress(hd, AddBrandContactRemote);
    AddBrandContactRemoteOffset = AddBrandContactRemoteAddr - WeChatBase;

    DWORD HookImageMsgRemoteAddr = (DWORD)GetProcAddress(hd, HookImageMsgRemote);
    HookImageMsgRemoteOffset = HookImageMsgRemoteAddr - WeChatBase;
    DWORD UnHookImageMsgAddr = (DWORD)GetProcAddress(hd, UnHookImageMsgRemote);
    UnHookImageMsgRemoteOffset = UnHookImageMsgAddr - WeChatBase;
    DWORD HookVoiceMsgRemoteAddr = (DWORD)GetProcAddress(hd, HookVoiceMsgRemote);
    HookVoiceMsgRemoteOffset = HookVoiceMsgRemoteAddr - WeChatBase;
    DWORD UnHookVoiceMsgAddr = (DWORD)GetProcAddress(hd, UnHookVoiceMsgRemote);
    UnHookVoiceMsgRemoteOffset = UnHookVoiceMsgAddr - WeChatBase;

    DWORD ChangeWeChatVerRemoteAddr = (DWORD)GetProcAddress(hd, ChangeWeChatVerRemote);
    ChangeWeChatVerRemoteOffset = ChangeWeChatVerRemoteAddr - WeChatBase;

    FreeLibrary(hd);
    delete[] dllpath;
    dllpath = NULL;
    return 1;
}

DWORD GetWeChatPid() {
    HWND hCalc = FindWindow(NULL, L"微信");
    DWORD wxPid = 0;
    GetWindowThreadProcessId(hCalc, &wxPid);
    if (wxPid == 0) {
        hCalc = FindWindow(NULL, L"微信测试版");
        GetWindowThreadProcessId(hCalc, &wxPid);
    }
    return wxPid;
}

DWORD StartRobotService() {
    DWORD wxPid = GetWeChatPid();
    if (!wxPid) {
        MessageBoxA(NULL, "请先启动目标程序", "提示", MB_ICONWARNING);
        return 1;
    }
    wstring wworkPath = GetComWorkPath();
    wchar_t* workPath = (wchar_t*)wworkPath.c_str();
    if (!GetProcOffset(workPath)) {
        wchar_t info[200] = { 0 };
        swprintf_s(info, 200, L"COM无法加载位于%ws的%ws!", workPath, dllname);
        MessageBox(NULL, info, L"致命错误!", MB_ICONWARNING);
        return 1;
    };
    if(!hProcess)
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, wxPid);
    bool status = Injert(wxPid, workPath);
    if (status == 1) {
        CloseHandle(hProcess);
        hProcess = NULL;
        return status;
    }
    return status;
}

DWORD StopRobotService() {
    DWORD cpid = GetCurrentProcessId();
    DWORD wxPid = GetWeChatPid();
    if (!wxPid) {
        hProcess = NULL;
        return cpid;
    }
    if (!hProcess)
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, wxPid);
    RemoveDll(wxPid);
    ZeroMemory((wchar_t*)SelfInfoString.c_str(), SelfInfoString.length() * 2 + 2);
    CloseHandle(hProcess);
    hProcess = NULL;
    StopReceiveMessage();
    return cpid;
}

wstring GetComWorkPath() {
    wchar_t szFilePath[MAX_PATH + 1] = { 0 };
    GetModuleFileName(NULL, szFilePath, MAX_PATH);
    wstring wpath = szFilePath;
    int pos = wpath.find_last_of(L"\\");
    wpath = wpath.substr(0,pos);
    return wpath;
}

static BOOL GetWeChatInstallInfo(TCHAR* lpValueName, VOID* Value, DWORD lpcbData) {
    HKEY hKey = NULL;
    ZeroMemory(Value, lpcbData);
    LSTATUS lRet = RegOpenKeyEx(HKEY_CURRENT_USER, _T("SOFTWARE\\Tencent\\WeChat"), 0, KEY_QUERY_VALUE, &hKey);
    if (lRet != 0) {
        return false;
    }
    lRet = RegQueryValueEx(hKey, lpValueName, NULL, NULL, (LPBYTE)Value, &lpcbData);
    RegCloseKey(hKey);
    if (lRet != 0) {
        return false;
    }
    return true;
}

tstring GetWeChatInstallDir() {
    TCHAR* szProductType = new TCHAR[MAX_PATH];
    GetWeChatInstallInfo((TCHAR*)TEXT("InstallPath"), (void*)szProductType, MAX_PATH);
    tstring wxdir(szProductType);
    delete[] szProductType;
    szProductType = NULL;
    return wxdir.length() == 0 ? TEXT("") : wxdir;
}

DWORD GetWeChatVerInt() {
    DWORD version = 0x0;
    GetWeChatInstallInfo((TCHAR*)TEXT("CrashVersion"), (void*)&version, sizeof(DWORD));
    return version;
}

tstring GetWeChatVerStr() {
    BYTE pversion[4] = { 0 };
    GetWeChatInstallInfo((TCHAR*)TEXT("CrashVersion"), (void*)pversion, sizeof(DWORD));
    TCHAR* temp = new TCHAR[20];
    _stprintf_s(temp, 20, _T("%d.%d.%d.%d\0"), (int)(pversion[3] - 0x60), (int)pversion[2], (int)pversion[1], (int)pversion[0]);
    tstring verStr(temp);
    delete[] temp;
    temp = NULL;
    return verStr;
}

VOID StartWeChat()
{
    tstring szAppName = GetWeChatInstallDir();
    if (szAppName.length() == 0)
        return;
    szAppName += TEXT("\\WeChat.exe");
    STARTUPINFO StartInfo;
    ZeroMemory(&StartInfo, sizeof(StartInfo));
    PROCESS_INFORMATION procStruct;
    ZeroMemory(&procStruct, sizeof(procStruct));
    StartInfo.cb = sizeof(STARTUPINFO);
    if (CreateProcess((LPCTSTR)szAppName.c_str(), NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &StartInfo, &procStruct))
    {
        CloseHandle(procStruct.hProcess);
        CloseHandle(procStruct.hThread);
    }
}