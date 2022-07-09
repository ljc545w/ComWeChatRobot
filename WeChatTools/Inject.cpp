#include "base.h"
HANDLE hProcess = NULL;

bool isFileExists_stat(string& name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

string wstring2string(wstring wstr)
{
    std::string result;
    //��ȡ��������С��������ռ䣬��������С�°��ֽڼ����  
    int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
    char* buffer = new char[len + 1];
    //���ֽڱ���ת���ɶ��ֽڱ���  
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
    buffer[len] = '\0';
    //ɾ��������������ֵ  
    result.append(buffer);
    delete[] buffer;
    return result;
}

bool InjectDll(DWORD dwId, WCHAR* szPath)//����1��Ŀ�����PID  ����2��DLL·��
{
    if (!hProcess)
        return 1;
    if (GetWeChatRobotBase() != 0) {
        printf("�����ظ�ע��\n");
        return 1;
    }

    /*
    ��1.2 ��Ŀ����̵��ڴ��￪�ٿռ䡿
    */
    LPVOID pRemoteAddress = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);

    //���� ��dll��·��д�뵽Ŀ����̵��ڴ�ռ���
    DWORD dwWriteSize = 0;
    /*
    ��дһ�����ݵ��ղŸ�ָ�����������ٵ��ڴ�ռ��
    */
    if (pRemoteAddress)
    {
        WriteProcessMemory(hProcess, pRemoteAddress, szPath, wcslen(szPath) * 2 + 2, &dwWriteSize);
    }
    else {
        printf("д��ʧ��!\n");
        return 1;
    }

    //���� ����һ��Զ���̣߳���Ŀ����̵���LoadLibrary
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibrary, pRemoteAddress, NULL, NULL);
    if (hThread) {
        WaitForSingleObject(hThread, -1); 
    }
    else {
        printf("����ʧ��!\n");
        return 1;
    }
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pRemoteAddress, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return 0;
}

void Inject(DWORD dwPid, wchar_t* wStr) {
    wchar_t dllpath[MAX_PATH];
    wchar_t workPath[MAX_PATH];
    wchar_t* pworkPath = _wgetcwd(workPath, MAX_PATH);
    swprintf_s(dllpath, MAX_PATH, L"%ws%ws%ws", pworkPath, L"\\", wStr);
    string name = wstring2string((wstring)dllpath);
    if (!isFileExists_stat(name)) {
        wstring info = L"����Ŀ���ļ�·����";
        MessageBox(NULL, info.c_str(), _T("����"), MB_ICONWARNING);
        return;
    }
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    InjectDll(dwPid, dllpath);
}

BOOL RemoveDll(DWORD dwId) {
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwId);
    LPVOID pRemoteAddress = VirtualAllocEx(hProcess, NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    DWORD dwWriteSize = 0;
    HANDLE hThread = NULL;
    DWORD dwHandle, dwID;
    LPVOID pFunc = NULL;
    if(pRemoteAddress)
        WriteProcessMemory(hProcess, pRemoteAddress, dllname, wcslen(dllname) * 2 + 2, &dwWriteSize);
    else {
        printf("д��ʧ��!\n");
        return 1;
    }
    pFunc = GetModuleHandleW;
    hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunc, pRemoteAddress, 0, &dwID);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &dwHandle);
    }
    else {
        printf("GetModuleHandleW����ʧ��!\n");
        return 1;
    }
    CloseHandle(hThread);
    
    // �ͷ�console���ڣ���Ȼ�ر�console��ͬʱ΢��Ҳ���˳�
    pFunc = FreeConsole;
    hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunc, NULL, 0, &dwID);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
    }
    else {
        printf("FreeConsole����ʧ��!\n");
        return 1;
    }
    CloseHandle(hThread);

    // ʹĿ����̵���FreeLibrary��ж��DLL
    pFunc = FreeLibrary;
    hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunc, (LPVOID)dwHandle, 0, &dwID);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
    }
    else {
        printf("FreeLibrary����ʧ��!\n");
        return 1;
    }
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pRemoteAddress, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return 0;
}
