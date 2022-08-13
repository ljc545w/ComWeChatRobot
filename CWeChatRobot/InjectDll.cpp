#include "pch.h"

bool InjectDll(DWORD dwId, WCHAR *szPath)
{
    WeChatProcess hp(dwId);
    if (!hp.m_init)
        return 1;
    if (hp.WeChatRobotBase() != 0)
        return 0;
    WeChatData<wchar_t *> r_dllpath(hp.GetHandle(), szPath, TEXTLENGTH(szPath));
    if (r_dllpath.GetAddr() == 0)
        return 1;
    CallRemoteFunction(hp.GetHandle(), LoadLibraryW, r_dllpath.GetAddr());
    return 0;
}

bool Inject(DWORD dwPid, wchar_t *workPath)
{
    wchar_t *dllpath = new wchar_t[MAX_PATH];
    swprintf_s(dllpath, MAX_PATH, L"%ws%ws%ws", workPath, L"\\", DLLNAME);
    string name = _com_util::ConvertBSTRToString((BSTR)workPath);
    if (!isFileExists_stat(name))
    {
        MessageBox(NULL, dllpath, L"文件不存在", MB_ICONWARNING);
        delete[] dllpath;
        dllpath = NULL;
        return 1;
    }
    bool status = InjectDll(dwPid, dllpath);
    delete[] dllpath;
    dllpath = NULL;
    return status;
}

BOOL RemoveDll(DWORD dwId)
{

    WeChatProcess hp(dwId);
    if (!hp.m_init)
        return 1;
    DWORD WeChatRobotBase = hp.WeChatRobotBase();
    if (WeChatRobotBase == 0)
        return 0;
    CallRemoteFunction(hp.GetHandle(), FreeConsole, NULL);
    CallRemoteFunction(hp.GetHandle(), FreeLibrary, WeChatRobotBase);
    return 0;
}
