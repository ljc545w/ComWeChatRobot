#pragma once
#include <windows.h>
BOOL OpenBrowser(wchar_t *url);

#ifndef USE_SOCKET
extern "C" __declspec(dllexport) BOOL OpenBrowserRemote(LPVOID url);
#endif
