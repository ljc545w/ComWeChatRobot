#pragma once
#include "InjertDll.h"
#include "SendImage.h"
#include "SendText.h"
#include "SendFile.h"
#include "FriendList.h"

extern HANDLE hProcess;
extern DWORD SendImageOffset;
extern DWORD SendTextOffset;
extern DWORD SendFileOffset;

extern DWORD GetFriendListInitOffset;
extern DWORD GetFriendListRemoteOffset;
extern DWORD GetFriendListFinishOffset;

#define dllname L"DWeChatRobot.dll"

#define SendTextRemote "SendTextRemote"
#define SendImageRemote "SendImageRemote"
#define SendFileRemote "SendFileRemote"

#define GetFriendListInit "GetFriendListInit"
#define GetFriendListRemote "GetFriendListRemote"
#define GetFriendListFinish "GetFriendListFinish"