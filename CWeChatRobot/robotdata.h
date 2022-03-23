#pragma once
#include "InjertDll.h"
#include "SendImage.h"
#include "SendText.h"
#include "SendFile.h"
#include "SendArticle.h"
#include "SendCard.h"
#include "FriendList.h"
#include "UserInfo.h"
#include "SelfInfo.h"
#include "CheckFriendStatus.h"

extern HANDLE hProcess;
extern DWORD SendImageOffset;
extern DWORD SendTextOffset;
extern DWORD SendFileOffset;
extern DWORD SendArticleOffset;
extern DWORD SendCardOffset;

extern DWORD GetFriendListInitOffset;
extern DWORD GetFriendListRemoteOffset;
extern DWORD GetFriendListFinishOffset;

extern DWORD GetWxUserInfoOffset;
extern DWORD DeleteUserInfoCacheOffset;

extern DWORD GetSelfInfoOffset;
extern DWORD DeleteSelfInfoCacheOffset;
extern wstring SelfInfoString;

extern DWORD CheckFriendStatusInitRemoteOffset;
extern DWORD CheckFriendStatusRemoteOffset;
extern DWORD CheckFriendStatusFinishRemoteOffset;


#define dllname L"DWeChatRobot.dll"

#define SendTextRemote "SendTextRemote"
#define SendImageRemote "SendImageRemote"
#define SendFileRemote "SendFileRemote"
#define SendArticleRemote "SendArticleRemote"
#define SendCardRemote "SendCardRemote"

#define GetFriendListInit "GetFriendListInit"
#define GetFriendListRemote "GetFriendListRemote"
#define GetFriendListFinish "GetFriendListFinish"

#define GetWxUserInfoRemote "GetWxUserInfoRemote"
#define DeleteUserInfoCacheRemote "DeleteUserInfoCacheRemote"

#define GetSelfInfoRemote "GetSelfInfoRemote"
#define DeleteSelfInfoCacheRemote "DeleteSelfInfoCacheRemote"

#define CheckFriendStatusInitRemote "CheckFriendStatusInitRemote"
#define CheckFriendStatusRemote "CheckFriendStatusRemote"
#define CheckFriendStatusFinishRemote "CheckFriendStatusFinishRemote"