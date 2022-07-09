#pragma once
#include "InjectDll.h"
#include "SendImage.h"
#include "SendText.h"
#include "SendFile.h"
#include "SendArticle.h"
#include "SendCard.h"
#include "SendAtText.h"
#include "FriendList.h"
#include "SearchContact.h"
#include "SelfInfo.h"
#include "CheckFriendStatus.h"
#include "ReceiveMessage.h"
#include "GetChatRoomMembers.h"
#include "GetDbHandles.h"
#include "DbExecuteSql.h"
#include "DbBackup.h"
#include "VerifyFriendApply.h"
#include "AddFriend.h"
#include "wechatver.h"
#include "DeleteUser.h"
#include "SendAppMsg.h"

extern HANDLE hProcess;
extern DWORD SendImageOffset;
extern DWORD SendTextOffset;
extern DWORD SendFileOffset;
extern DWORD SendArticleOffset;
extern DWORD SendCardOffset;
extern DWORD SendAtTextOffset;
extern DWORD SendAppMsgRemoteOffset;

extern DWORD GetFriendListInitOffset;
extern DWORD GetFriendListRemoteOffset;
extern DWORD GetFriendListFinishOffset;

extern DWORD GetWxUserInfoOffset;
extern DWORD DeleteUserInfoCacheOffset;
extern DWORD SearchContactByNetRemoteOffset;

extern DWORD VerifyFriendApplyOffset;

extern DWORD GetSelfInfoOffset;
extern DWORD DeleteSelfInfoCacheOffset;
extern wstring SelfInfoString;
extern DWORD isWxLoginOffset;

extern DWORD CheckFriendStatusRemoteOffset;

extern DWORD HookReceiveMessageRemoteOffset;
extern DWORD UnHookReceiveMessageRemoteOffset;

extern DWORD GetChatRoomMembersRemoteOffset;

extern DWORD GetDbHandlesRemoteOffset;
extern DWORD ExecuteSQLRemoteOffset;
extern DWORD SelectDataRemoteOffset;
extern DWORD BackupSQLiteDBRemoteOffset;

extern DWORD AddFriendByWxidRemoteOffset;
extern DWORD AddFriendByV3RemoteOffset;
extern DWORD DeleteUserRemoteOffset;
extern DWORD AddBrandContactRemoteOffset;

extern DWORD HookImageMsgRemoteOffset;
extern DWORD UnHookImageMsgRemoteOffset;
extern DWORD HookVoiceMsgRemoteOffset;
extern DWORD UnHookVoiceMsgRemoteOffset;

extern DWORD ChangeWeChatVerRemoteOffset;


#define dllname							L"DWeChatRobot.dll"

#define SendTextRemote					"SendTextRemote"
#define SendImageRemote					"SendImageRemote"
#define SendFileRemote					"SendFileRemote"
#define SendArticleRemote				"SendArticleRemote"
#define SendCardRemote					"SendCardRemote"
#define SendAtTextRemote				"SendAtTextRemote"
#define SendAppMsgRemote				"SendAppMsgRemote"

#define GetFriendListInit				"GetFriendListInit"
#define GetFriendListRemote				"GetFriendListRemote"
#define GetFriendListFinish				"GetFriendListFinish"

#define GetWxUserInfoRemote				"GetWxUserInfoRemote"
#define DeleteUserInfoCacheRemote		"DeleteUserInfoCacheRemote"

#define GetSelfInfoRemote				"GetSelfInfoRemote"
#define DeleteSelfInfoCacheRemote		"DeleteSelfInfoCacheRemote"
#define SearchContactByNetRemote		"SearchContactByNetRemote"
#define isWxLoginRemote					"isWxLogin"

#define VerifyFriendApplyRemote			"VerifyFriendApplyRemote"

#define CheckFriendStatusRemote			"CheckFriendStatusRemote"

#define HookReceiveMessageRemote		"HookReceiveMessage"
#define UnHookReceiveMessageRemote		"UnHookReceiveMessage"

#define GetChatRoomMembersRemote		"GetChatRoomMembersRemote"

#define GetDbHandlesRemote				"GetDbHandlesRemote"
#define ExecuteSQLRemote				"ExecuteSQLRemote"
#define SelectDataRemote				"SelectDataRemote"
#define BackupSQLiteDBRemote			"BackupSQLiteDBRemote"

#define AddFriendByWxidRemote			"AddFriendByWxidRemote"
#define AddFriendByV3Remote				"AddFriendByV3Remote"
#define DeleteUserRemote				"DeleteUserRemote"
#define AddBrandContactRemote			"AddBrandContactRemote"

#define HookImageMsgRemote				"HookImageMsgRemote"
#define UnHookImageMsgRemote			"UnHookImageMsg"
#define HookVoiceMsgRemote				"HookVoiceMsgRemote"
#define UnHookVoiceMsgRemote			"UnHookVoiceMsg"

#define ChangeWeChatVerRemote			"ChangeWeChatVerRemote"