#pragma once
#include "InjertDll.h"
#include "SendImage.h"
#include "SendText.h"
#include "SendFile.h"
#include "FriendList.h"
#include "UserInfo.h"
#include "SelfInfo.h"

extern HANDLE hProcess;
extern DWORD SendImageOffset;
extern DWORD SendTextOffset;
extern DWORD SendFileOffset;

extern DWORD GetFriendListInitOffset;
extern DWORD GetFriendListRemoteOffset;
extern DWORD GetFriendListFinishOffset;

extern DWORD GetWxUserInfoOffset;
extern DWORD DeleteUserInfoCacheOffset;

extern DWORD GetSelfInfoOffset;
extern DWORD DeleteSelfInfoCacheOffset;
extern wstring SelfInfoString;

#define dllname L"DWeChatRobot.dll"

#define SendTextRemote "SendTextRemote"
#define SendImageRemote "SendImageRemote"
#define SendFileRemote "SendFileRemote"

#define GetFriendListInit "GetFriendListInit"
#define GetFriendListRemote "GetFriendListRemote"
#define GetFriendListFinish "GetFriendListFinish"

#define GetWxUserInfoRemote "GetWxUserInfoRemote"
#define DeleteUserInfoCacheRemote "DeleteUserInfoCacheRemote"

#define GetSelfInfoRemote "GetSelfInfoRemote"
#define DeleteSelfInfoCacheRemote "DeleteSelfInfoCacheRemote"