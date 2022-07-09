#pragma once
#include<windows.h>
BOOL __stdcall DeleteUser(wchar_t* wxid);
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) BOOL DeleteUserRemote(LPVOID lpParameter);
#endif