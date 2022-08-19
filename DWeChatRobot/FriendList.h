#pragma once
#include <vector>
#include "wxdata.h"
using namespace std;
WxFriendStruct *__stdcall GetFriendList();

#ifndef USE_SOCKET
extern "C" __declspec(dllexport) DWORD GetFriendListRemote();
extern "C" __declspec(dllexport) int GetFriendListInit();
extern "C" __declspec(dllexport) void GetFriendListFinish();
#else
vector<WxFriendStruct> GetWxContact();
#endif
