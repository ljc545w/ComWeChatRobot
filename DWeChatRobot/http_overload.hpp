#pragma once
#include "pch.h"

#ifdef USE_SOCKET
#define WS2LW(wstr) (LPWSTR) wstr.c_str()

BOOL __stdcall SendText(wstring wxid, wstring msg)
{
    return SendText(WS2LW(wxid), WS2LW(msg));
}

BOOL __stdcall SendAtText(wstring wsChatRoomId, vector<wstring> wxids, wstring wsTextMsg, BOOL AutoNickName)
{
    vector<DWORD> wxid_list;
    for (unsigned int i = 0; i < wxids.size(); i++)
        wxid_list.push_back((DWORD)wxids[i].c_str());
    return SendAtText(WS2LW(wsChatRoomId), wxid_list.data(), WS2LW(wsTextMsg), wxid_list.size(), AutoNickName);
}

BOOL __stdcall SendCard(wstring receiver, wstring sharedwxid, wstring nickname)
{
    return SendCard(WS2LW(receiver), WS2LW(sharedwxid), WS2LW(nickname));
}

BOOL __stdcall SendImage(wstring receiver, wstring ImagePath)
{
    return SendImage(WS2LW(receiver), WS2LW(ImagePath));
}

BOOL __stdcall SendFile(wstring receiver, wstring FilePath)
{
    return SendFile(WS2LW(receiver), WS2LW(FilePath));
}

BOOL __stdcall SendArticle(wstring wxid, wstring title, wstring abstract, wstring url, wstring imgpath)
{
    return SendArticle(WS2LW(wxid), WS2LW(title), WS2LW(abstract), WS2LW(url), WS2LW(imgpath));
}

BOOL __stdcall SendAppMsg(wstring wxid, wstring appid)
{
    return SendAppMsg(WS2LW(wxid), WS2LW(appid));
}

BOOL __stdcall DeleteUser(wstring wxid)
{
    return DeleteUser(WS2LW(wxid));
}

wstring __stdcall GetUserInfoByWxId(wstring wxid)
{
    return GetUserInfoByWxId(WS2LW(wxid));
}

wstring __stdcall GetUserNickNameByWxId(wstring wxid)
{
    return GetUserNickNameByWxId(WS2LW(wxid));
}

BOOL __stdcall AddFriendByV3(wstring v3, wstring message, int AddType)
{
    return AddFriendByV3(WS2LW(v3), WS2LW(message), AddType);
}

BOOL __stdcall AddFriendByWxid(wstring wxid, wstring message)
{
    return AddFriendByWxid(WS2LW(wxid), WS2LW(message));
}

BOOL __stdcall AddBrandContact(wstring PublicId)
{
    return AddBrandContact(WS2LW(PublicId));
}

BOOL __stdcall VerifyFriendApply(wstring v3_data, wstring v4_data)
{
    return VerifyFriendApply(WS2LW(v3_data), WS2LW(v4_data));
}

BOOL __stdcall EditRemark(wstring wxid, wstring remark)
{
    return EditRemark(WS2LW(wxid), WS2LW(remark));
}

wstring __stdcall GetChatRoomMemberNickname(wstring chatroomid, wstring wxid)
{
    return GetChatRoomMemberNickname(WS2LW(chatroomid), WS2LW(wxid));
}

BOOL __stdcall DelChatRoomMember(wstring chatroomid, vector<wstring> wxids)
{
    vector<wchar_t *> wxid_list;
    for (unsigned int i = 0; i < wxids.size(); i++)
        wxid_list.push_back(WS2LW(wxids[i]));
    return DelChatRoomMember(WS2LW(chatroomid), wxid_list.data(), wxid_list.size());
}

BOOL __stdcall AddChatRoomMember(wstring chatroomid, vector<wstring> wxids)
{
    vector<wchar_t *> wxid_list;
    for (unsigned int i = 0; i < wxids.size(); i++)
        wxid_list.push_back(WS2LW(wxids[i]));
    return AddChatRoomMember(WS2LW(chatroomid), wxid_list.data(), wxid_list.size());
}

BOOL __stdcall SetChatRoomAnnouncement(wstring chatroomid, wstring announcement)
{
    return SetChatRoomAnnouncement(WS2LW(chatroomid), WS2LW(announcement));
}

BOOL __stdcall SetChatRoomName(wstring chatroomid, wstring chatroomname)
{
    return SetChatRoomName(WS2LW(chatroomid), WS2LW(chatroomname));
}

BOOL __stdcall SetChatRoomSelfNickname(wstring chatroomid, wstring nickname)
{
    return SetChatRoomSelfNickname(WS2LW(chatroomid), WS2LW(nickname));
}

BOOL __stdcall ChangeWeChatVersion(wstring verStr)
{
    return ChangeWeChatVer(WS2LW(verStr));
}

BOOL __stdcall BackupSQLiteDB(DWORD DbHandle, wstring BackupFile)
{
    string filepath = unicode_to_utf8(WS2LW(BackupFile));
    return BackupSQLiteDB(DbHandle, filepath.c_str()) == SQLITE_OK;
}

BOOL __stdcall OpenBrowser(wstring url)
{
    return OpenBrowser(WS2LW(url));
}

BOOL __stdcall ForwardMessage(wstring wxid, ULONG64 msgid)
{
    return ForwardMessage(WS2LW(wxid), msgid);
}

BOOL __stdcall SendXmlMsg(wstring wxid, wstring xml, wstring imgpath)
{
    return SendXmlMsg(WS2LW(wxid), WS2LW(xml), WS2LW(imgpath));
}

BOOL __stdcall GetTransfer(wstring wxid, wstring transcationid, wstring transferid)
{
    return GetTransfer(WS2LW(wxid), WS2LW(transcationid), WS2LW(transferid));
}

BOOL __stdcall SendEmotion(wstring wxid, wstring img_path)
{
    return SendEmotion(WS2LW(wxid), WS2LW(img_path));
}
#endif
