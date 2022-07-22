#pragma once
#include<windows.h>

SAFEARRAY* ExecuteSQL(DWORD pid,DWORD DbHandle, BSTR sql);