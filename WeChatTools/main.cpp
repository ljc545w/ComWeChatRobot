#include "main.h"


int _tmain(int nargv, WCHAR* argvs[])
{
    DWORD dwId = 0;
    HWND hCalc = FindWindow(NULL, L"΢��");
    DWORD dwPid = 0;
    DWORD dwRub = GetWindowThreadProcessId(hCalc, &dwPid);
    if (!dwPid) {
        wstring info = L"��������Ŀ����̣�";
        MessageBox(NULL, info.c_str(), _T("����"), MB_ICONWARNING);
        return 1;
    }
    wchar_t* wStr = NULL;
    
    if (nargv == 1) {
        return 1;
    }
    else if (nargv == 2) {
        wStr = argvs[1];
        Inject(dwPid, argvs[1]);
    }
    else if (nargv == 3 && !((wstring)argvs[1]).compare(L"-r")) {
        wStr = argvs[2];
        RemoveDll(dwPid);
    }
    return 0;
}