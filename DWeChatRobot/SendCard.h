#pragma once
#include<windows.h>
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) VOID SendCardRemote(LPVOID lparameter);
#endif
BOOL __stdcall SendCard(wchar_t* receiver, wchar_t* sharedwxid, wchar_t* nickname);