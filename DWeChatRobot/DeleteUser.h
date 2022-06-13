#pragma once
#include<windows.h>
BOOL __stdcall DeleteUser(wchar_t* wxid);
extern "C" __declspec(dllexport) BOOL DeleteUserRemote(LPVOID lpParameter);