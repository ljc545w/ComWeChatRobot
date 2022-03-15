#pragma once
#include<windows.h>
extern "C" __declspec(dllexport) VOID SendArticleRemote(LPVOID lparameter);
BOOL __stdcall SendArticle(wchar_t* wxid, wchar_t* title, wchar_t* abstract, wchar_t* url);