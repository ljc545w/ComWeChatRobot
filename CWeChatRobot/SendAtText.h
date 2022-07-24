#pragma once
#include<windows.h>
int SendAtText(DWORD pid,wchar_t* chatroomid, wchar_t* wxid, wchar_t* wxmsg,BOOL AutoNickName);
int SendAtText(DWORD pid,wchar_t* chatroomid, SAFEARRAY* wxid, wchar_t* wxmsg,BOOL AutoNickName);