#pragma once
#include<windows.h>
#ifndef USE_SOCKET
extern "C" __declspec(dllexport) VOID SendArticleRemote(LPVOID lparameter);
#endif
BOOL __stdcall SendArticle(wchar_t* wxid, wchar_t* title, wchar_t* abstract, wchar_t* url,wchar_t* imgpath);