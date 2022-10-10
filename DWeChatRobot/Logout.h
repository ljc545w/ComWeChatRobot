#pragma once
#include <windows.h>
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) BOOL Logout();
#else
BOOL Logout();
#endif
