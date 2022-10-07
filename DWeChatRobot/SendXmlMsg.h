#pragma once
#include <windows.h>
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) VOID SendXmlMsgRemote(LPVOID lparameter);
#endif
BOOL __stdcall SendXmlMsg(wchar_t *wxid, wchar_t *xml, wchar_t *imgpath);
