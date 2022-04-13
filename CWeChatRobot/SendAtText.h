#pragma once
#include<windows.h>
int SendAtText(wchar_t* chatroomid, wchar_t* wxid, wchar_t* wxmsg);
int SendAtText(wchar_t* chatroomid, SAFEARRAY* wxid, wchar_t* wxmsg);