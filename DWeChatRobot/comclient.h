#pragma once
#include <windows.h>
#define USE_COM
#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")

#define WX_MESSAGE 1
#define WX_LOG_MESSAGE 2
BOOL PostComMessage(DWORD pid, int msgtype, unsigned long long msgid, VARIANT *msg);
