// pch.cpp: 与预编译标头对应的源文件

#include "pch.h"

// 当使用预编译的头时，需要使用此源文件，编译才能成功。
DWORD SendImageOffset = 0x0;
DWORD SendTextOffset = 0x0;
DWORD SendFileOffset = 0x0;
DWORD SendArticleOffset = 0x0;
DWORD SendCardOffset = 0x0;
DWORD SendAtTextOffset = 0x0;

DWORD GetFriendListInitOffset = 0x0;
DWORD GetFriendListRemoteOffset = 0x0;
DWORD GetFriendListFinishOffset = 0x0;

DWORD GetWxUserInfoOffset = 0x0;
DWORD DeleteUserInfoCacheOffset = 0x0;

DWORD GetSelfInfoOffset = 0x0;
DWORD DeleteSelfInfoCacheOffset = 0x0;

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
DWORD BackupSQLiteDBRemoteOffset = 0x0;

wstring SelfInfoString = L"";

HANDLE hProcess = NULL;

bool isFileExists_stat(string& name) {
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

void GetProcOffset(wchar_t* workPath) {
    wchar_t* dllpath = new wchar_t[MAX_PATH];
    swprintf_s(dllpath, MAX_PATH, L"%ws%ws%ws", workPath, L"\\", dllname);
    string name = _com_util::ConvertBSTRToString((BSTR)dllpath);
    if (!isFileExists_stat(name)) {
        MessageBoxA(NULL, name.c_str(), "文件不存在", MB_ICONWARNING);
        return;
    }
    HMODULE hd = LoadLibraryW(dllpath);
    if (!hd)
        return;
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

    DWORD GetSelfInfoProcAddr = (DWORD)GetProcAddress(hd, GetSelfInfoRemote);
    GetSelfInfoOffset = GetSelfInfoProcAddr - WeChatBase;
    DWORD DeleteSelfInfoCacheProcAddr = (DWORD)GetProcAddress(hd, DeleteSelfInfoCacheRemote);
    DeleteSelfInfoCacheOffset = DeleteSelfInfoCacheProcAddr - WeChatBase;

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
    DWORD BackupSQLiteDBRemoteAddr = (DWORD)GetProcAddress(hd, BackupSQLiteDBRemote);
    BackupSQLiteDBRemoteOffset = BackupSQLiteDBRemoteAddr - WeChatBase;

    FreeLibrary(hd);
    delete[] dllpath;
    dllpath = NULL;
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
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, wxPid);
    bool status = Injert(wxPid, workPath);
    if (status == 1) {
        CloseHandle(hProcess);
        return status;
    }
    GetProcOffset(workPath);
    return status;
}

DWORD StopRobotService() {
    DWORD cpid = GetCurrentProcessId();
    if (!hProcess)
        return cpid;
    DWORD wxPid = GetWeChatPid();
    CheckFriendStatusFinish();
    StopReceiveMessage();
    RemoveDll(wxPid);
    ZeroMemory((wchar_t*)SelfInfoString.c_str(), SelfInfoString.length() * 2 + 2);
    CloseHandle(hProcess);
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