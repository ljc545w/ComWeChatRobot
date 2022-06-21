#pragma once
#include<windows.h>
bool Inject(DWORD dwPid, wchar_t* workPath);
BOOL RemoveDll(DWORD dwId);