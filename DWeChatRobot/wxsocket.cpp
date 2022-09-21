#include "pch.h"

#ifdef USE_SOCKET
// mongoose:    https://github.com/cesanta/mongoose
#include "mongoose/mongoose.c"
// json:        https://github.com/nlohmann/json
#include "json/json.hpp"
#include "http_overload.hpp"
using namespace nlohmann;

static struct mg_mgr mgr;
static bool httpruning = false;
static HANDLE http_thread = NULL;
static const char *s_root_dir = ".";

json methods = {{"GET", HTTP_METHOD_GET}, {"POST", HTTP_METHOD_POST}};
#define I_METHOD(method) methods[method]

#define getMgStrW(str) utf8_to_unicode(getMgStrA(str).c_str())
#define getMgVarW(hm, name) utf8_to_unicode(getMgVarA(hm, name).c_str())

#ifdef UNICODE
#define getMgStr getMgStrW
#define getMgVar getMgVarW
#else
#define getMgStr getMgStrA
#define getMgVar getMgVarA
#endif

#define STOI_S(str) (is_digit_number(str) ? stoi(str) : 0)
#define POST_PARAM(jData, key) utf8_to_unicode(jData[key].get<string>().c_str())
#define GET_PARAM(hm, name) getMgVarW(hm, name)

bool is_digit_number(string str)
{
    if (str.length() == 0)
        return false;
    for (auto it : str)
        if (it < '0' || it > '9')
            return false;
    return true;
}

string getMgStrA(struct mg_str str)
{
    return string(str.ptr, str.len);
}

string getMgVarA(mg_http_message *hm, string name)
{
    string ret;
    char *buffer = new char[hm->query.len + 1];
    ZeroMemory(buffer, hm->query.len + 1);
    int len = mg_http_get_var(&hm->query, name.c_str(), buffer, hm->query.len);
    if (len > 0)
        ret = string(buffer, len);
    delete[] buffer;
    buffer = NULL;
    return ret;
}

static wstring get_http_param_str(mg_http_message *hm, json jData, string key, int method)
{
    wstring result;
    switch (method)
    {
    case HTTP_METHOD_GET:
    {
        result = GET_PARAM(hm, key);
        break;
    }
    case HTTP_METHOD_POST:
    {
        result = POST_PARAM(jData, key);
        break;
    }
    default:
        break;
    }
    return result;
}

static int get_http_param_int(mg_http_message *hm, json jData, string key, int method)
{
    int result = 0;
    switch (method)
    {
    case HTTP_METHOD_GET:
    {
        result = STOI_S(getMgVarA(hm, key));
        break;
    }
    case HTTP_METHOD_POST:
    {
        try
        {
            result = jData[key].get<int>();
        }
        catch (json::exception)
        {
            result = STOI_S(jData[key].get<string>());
        }
        break;
    }
    default:
        break;
    }
    return result;
}

static unsigned long long get_http_param_ulong64(mg_http_message *hm, json jData, string key, int method)
{
    unsigned long long result = 0;
    switch (method)
    {
    case HTTP_METHOD_GET:
    {
        string value = getMgVarA(hm, key);
        istringstream is(value);
        is >> result;
        break;
    }
    case HTTP_METHOD_POST:
    {
        try
        {
            result = jData[key].get<ULONG64>();
        }
        catch (json::exception)
        {
            string value = jData[key].get<string>();
            istringstream is(value);
            is >> result;
        }
        break;
    }
    default:
        break;
    }
    return result;
}

static vector<wstring> get_http_param_array(mg_http_message *hm, json jData, string key, int method)
{
    vector<wstring> result;
    switch (method)
    {
    case HTTP_METHOD_GET:
    {
        result = split(GET_PARAM(hm, key), L',');
        break;
    }
    case HTTP_METHOD_POST:
    {
        result = split(POST_PARAM(jData, key), L',');
        break;
    }
    default:
        break;
    }
    return result;
}

void request_event(mg_http_message *hm, string &ret, struct mg_connection *c)
{
    int method = I_METHOD(getMgStrA(hm->method));
    // 第四个参数设置为false，不抛出异常
    json jData = json::parse(hm->body.ptr, hm->body.ptr + hm->body.len, nullptr, false);
    if (hm->body.len != 0 && jData.is_discarded() == true && method == HTTP_METHOD_POST)
    {
        json ret_data = {{"result", "ERROR"},
                         {"err_msg", "json string is invalid."}};
        ret = ret_data.dump();
        return;
    }
    int api_number = STOI_S(getMgVarA(hm, "type"));
    switch (api_number)
    {
    case WECHAT_IS_LOGIN:
    {
        BOOL logined = isWxLogin();
        json ret_data = {{"is_login", logined}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_GET_SELF_INFO:
    {
        json ret_data;
        string self_info = unicode_to_utf8(WS2LW(GetSelfInfo()));
        json j_info = json::parse(self_info.c_str(), self_info.c_str() + self_info.size(), nullptr, false);
        if (j_info.is_discarded() != true)
            ret_data = {{"data", j_info}, {"result", "OK"}};
        else
            ret_data = {{"data", self_info}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_MSG_SEND_TEXT:
    {
        wstring wxid = get_http_param_str(hm, jData, "wxid", method);
        wstring msg = get_http_param_str(hm, jData, "msg", method);
        BOOL status = SendText(wxid, msg);
        json ret_data = {{"msg", 1}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_MSG_SEND_AT:
    {
        wstring chatroom = get_http_param_str(hm, jData, "chatroom_id", method);
        vector<wstring> wxids = get_http_param_array(hm, jData, "wxids", method);
        wstring msg = get_http_param_str(hm, jData, "msg", method);
        int auto_nickname = get_http_param_int(hm, jData, "auto_nickname", method);
        BOOL status = SendAtText(chatroom, wxids, msg, auto_nickname);
        json ret_data = {{"msg", status}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_MSG_SEND_CARD:
    {
        wstring receiver = get_http_param_str(hm, jData, "receiver", method);
        wstring shared_wxid = get_http_param_str(hm, jData, "shared_wxid", method);
        wstring nickname = get_http_param_str(hm, jData, "nickname", method);
        BOOL status = SendCard(receiver, shared_wxid, nickname);
        json ret_data = {{"msg", status}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_MSG_SEND_IMAGE:
    {
        wstring receiver = get_http_param_str(hm, jData, "receiver", method);
        wstring img_path = get_http_param_str(hm, jData, "img_path", method);
        BOOL status = SendImage(receiver, img_path);
        json ret_data = {{"msg", status}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_MSG_SEND_FILE:
    {
        wstring receiver = get_http_param_str(hm, jData, "receiver", method);
        wstring file_path = get_http_param_str(hm, jData, "file_path", method);
        BOOL status = SendFile(receiver, file_path);
        json ret_data = {{"msg", status}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_MSG_SEND_ARTICLE:
    {
        wstring wxid = get_http_param_str(hm, jData, "wxid", method);
        wstring title = get_http_param_str(hm, jData, "title", method);
        wstring abstract = get_http_param_str(hm, jData, "abstract", method);
        wstring url = get_http_param_str(hm, jData, "url", method);
        wstring img_path = get_http_param_str(hm, jData, "img_path", method);
        BOOL status = SendArticle(wxid, title, abstract, url, img_path);
        json ret_data = {{"msg", status}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_MSG_SEND_APP:
    {
        wstring wxid = get_http_param_str(hm, jData, "wxid", method);
        wstring appid = get_http_param_str(hm, jData, "appid", method);
        BOOL status = SendAppMsg(wxid, appid);
        json ret_data = {{"msg", status}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_MSG_START_HOOK:
    {
        int port = get_http_param_int(hm, jData, "port", method);
        HookReceiveMessage(port);
        break;
    }
    case WECHAT_MSG_STOP_HOOK:
    {
        UnHookReceiveMessage();
        break;
    }
    case WECHAT_MSG_START_IMAGE_HOOK:
    {
        wstring savepath = get_http_param_str(hm, jData, "save_path", method);
        BOOL status = HookImageMsg(savepath);
        json ret_data = {{"msg", status}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_MSG_STOP_IMAGE_HOOK:
    {
        UnHookImageMsg();
        break;
    }
    case WECHAT_MSG_START_VOICE_HOOK:
    {
        wstring savepath = get_http_param_str(hm, jData, "save_path", method);
        BOOL status = HookVoiceMsg(savepath);
        json ret_data = {{"msg", status}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_MSG_STOP_VOICE_HOOK:
    {
        UnHookVoiceMsg();
        break;
    }
    case WECHAT_CONTACT_GET_LIST:
    {
        vector<WxFriendStruct> friend_list = GetWxContact();
        json ret_data = {{"data", json::array()}, {"result", "OK"}};
        for (unsigned int i = 0; i < friend_list.size() - 1; i++)
        {
            json f_j;
            WxFriendStruct f = friend_list[i];
            DWORD buffer_addr = *(DWORD *)f.wxIdAddr;
            wstring wxid = buffer_addr != 0 ? wstring((wchar_t *)buffer_addr) : L"";
            buffer_addr = *(DWORD *)f.wxNumberAddr;
            wstring wxNumber = buffer_addr != 0 ? wstring((wchar_t *)buffer_addr) : L"";
            buffer_addr = *(DWORD *)f.wxNickNameAddr;
            wstring wxNickName = buffer_addr != 0 ? wstring((wchar_t *)buffer_addr) : L"";
            buffer_addr = *(DWORD *)f.wxRemarkAddr;
            wstring wxRemark = buffer_addr != 0 ? wstring((wchar_t *)buffer_addr) : L"";
            f_j["wxid"] = unicode_to_utf8(WS2LW(wxid));
            f_j["wxNumber"] = unicode_to_utf8(WS2LW(wxNumber));
            f_j["wxNickName"] = unicode_to_utf8(WS2LW(wxNickName));
            f_j["wxRemark"] = unicode_to_utf8(WS2LW(wxRemark));
            ret_data["data"].push_back(f_j);
        }
        ret = ret_data.dump();
        break;
    }
    case WECHAT_CONTACT_CHECK_STATUS:
    {
        wstring wxid = get_http_param_str(hm, jData, "wxid", method);
        int status_code = CheckFriendStatus(wxid);
        json ret_data = {{"status", status_code}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_CONTACT_DEL:
    {
        wstring wxid = get_http_param_str(hm, jData, "wxid", method);
        BOOL status = DeleteUser(wxid);
        json ret_data = {{"msg", status}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_CONTACT_SEARCH_BY_CACHE:
    {
        wstring wxid = get_http_param_str(hm, jData, "wxid", method);
        string userinfo = unicode_to_utf8(WS2LW(GetUserInfoByWxId(wxid)));
        json ret_data;
        json j_info = json::parse(userinfo.c_str(), userinfo.c_str() + userinfo.size(), nullptr, false);
        if (j_info.is_discarded() != true)
            ret_data = {{"data", j_info}, {"result", "OK"}};
        else
            ret_data = {{"data", userinfo}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_CONTACT_SEARCH_BY_NET:
    {
        wstring keyword = get_http_param_str(hm, jData, "keyword", method);
        map<wstring, wstring> userinfo = SearchContactByNet(keyword);
        json ret_data;
        for (auto it : userinfo)
        {
            string key = unicode_to_utf8(WS2LW(it.first));
            ret_data[key] = unicode_to_utf8(WS2LW(it.second));
        }
        ret = ret_data.dump();
        break;
    }
    case WECHAT_CONTACT_ADD_BY_WXID:
    {
        wstring wxid = get_http_param_str(hm, jData, "wxid", method);
        wstring msg = get_http_param_str(hm, jData, "msg", method);
        BOOL status = AddFriendByWxid(wxid, msg);
        json ret_data = {{"msg", status}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_CONTACT_ADD_BY_V3:
    {
        wstring v3 = get_http_param_str(hm, jData, "v3", method);
        wstring msg = get_http_param_str(hm, jData, "msg", method);
        int add_type = get_http_param_int(hm, jData, "add_type", method);
        BOOL status = AddFriendByV3(v3, msg, add_type);
        json ret_data = {{"msg", status}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_CONTACT_ADD_BY_PUBLIC_ID:
    {
        wstring public_id = get_http_param_str(hm, jData, "public_id", method);
        BOOL status = AddBrandContact(public_id);
        json ret_data = {{"msg", status}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_CONTACT_VERIFY_APPLY:
    {
        wstring v3 = get_http_param_str(hm, jData, "v3", method);
        wstring v4 = get_http_param_str(hm, jData, "v4", method);
        BOOL status = VerifyFriendApply(v3, v4);
        json ret_data = {{"msg", status}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_CONTACT_EDIT_REMARK:
    {
        wstring wxid = get_http_param_str(hm, jData, "wxid", method);
        wstring remark = get_http_param_str(hm, jData, "remark", method);
        BOOL status = EditRemark(wxid, remark);
        json ret_data = {{"msg", status}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_CHATROOM_GET_MEMBER_LIST:
    {
        wstring chatroom_id = get_http_param_str(hm, jData, "chatroom_id", method);
        wstring members = GetChatRoomMembers(chatroom_id);
        json ret_data = {{"members", unicode_to_utf8(WS2LW(members))}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_CHATROOM_GET_MEMBER_NICKNAME:
    {
        wstring chatroom_id = get_http_param_str(hm, jData, "chatroom_id", method);
        wstring wxid = get_http_param_str(hm, jData, "wxid", method);
        wstring nickname = GetChatRoomMemberNickname(chatroom_id, wxid);
        json ret_data = {{"nickname", unicode_to_utf8(WS2LW(nickname))}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_CHATROOM_DEL_MEMBER:
    {
        wstring chatroom_id = get_http_param_str(hm, jData, "chatroom_id", method);
        vector<wstring> wxids = get_http_param_array(hm, jData, "wxids", method);
        BOOL status = DelChatRoomMember(chatroom_id, wxids);
        json ret_data = {{"msg", status}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_CHATROOM_ADD_MEMBER:
    {
        wstring chatroom_id = get_http_param_str(hm, jData, "chatroom_id", method);
        vector<wstring> wxids = get_http_param_array(hm, jData, "wxids", method);
        BOOL status = AddChatRoomMember(chatroom_id, wxids);
        json ret_data = {{"msg", status}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_CHATROOM_SET_ANNOUNCEMENT:
    {
        wstring chatroom_id = get_http_param_str(hm, jData, "chatroom_id", method);
        wstring announcement = get_http_param_str(hm, jData, "announcement", method);
        BOOL status = SetChatRoomAnnouncement(chatroom_id, announcement);
        json ret_data = {{"msg", status}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_CHATROOM_SET_CHATROOM_NAME:
    {
        wstring chatroom_id = get_http_param_str(hm, jData, "chatroom_id", method);
        wstring new_name = get_http_param_str(hm, jData, "chatroom_name", method);
        BOOL status = SetChatRoomName(chatroom_id, new_name);
        json ret_data = {{"msg", status}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_CHATROOM_SET_SELF_NICKNAME:
    {
        wstring chatroom_id = get_http_param_str(hm, jData, "chatroom_id", method);
        wstring nickname = get_http_param_str(hm, jData, "nickname", method);
        BOOL status = SetChatRoomSelfNickname(chatroom_id, nickname);
        json ret_data = {{"msg", status}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_DATABASE_GET_HANDLES:
    {
        vector<void *> v_ptr = GetDbHandles();
        json ret_data = {{"data", json::array()}, {"result", "OK"}};
        for (unsigned int i = 0; i < v_ptr.size(); i++)
        {
            json db_info;
            db_info["tables"] = json::array();
            DbInfoStruct *db = reinterpret_cast<DbInfoStruct *>(v_ptr[i]);
            db_info["handle"] = db->handle;
            db_info["db_name"] = unicode_to_utf8(db->dbname);
            for (auto table : db->tables)
            {
                json table_info = {{"name", table.name}, {"tbl_name", table.tbl_name}, {"sql", table.sql}, {"rootpage", table.rootpage}};
                db_info["tables"].push_back(table_info);
            }
            ret_data["data"].push_back(db_info);
        }
        ret = ret_data.dump();
        break;
    }
    case WECHAT_DATABASE_BACKUP:
    {
        DWORD db_handle = get_http_param_int(hm, jData, "db_handle", method);
        wstring savepath = get_http_param_str(hm, jData, "save_path", method);
        BOOL status = BackupSQLiteDB(db_handle, savepath);
        json ret_data = {{"msg", status}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_DATABASE_QUERY:
    {
        DWORD db_handle = get_http_param_int(hm, jData, "db_handle", method);
        wstring sql = get_http_param_str(hm, jData, "sql", method);
        string a_sql = unicode_to_utf8(WS2LW(sql));
        vector<vector<string>> items = SelectData(db_handle, a_sql.c_str());
        json ret_data = {{"data", json::array()}, {"result", "OK"}};
        for (auto it : items)
        {
            json temp_arr = json::array();
            for (size_t i = 0; i < it.size(); i++)
            {
                temp_arr.push_back(it[i]);
            }
            ret_data["data"].push_back(temp_arr);
        }
        ret = ret_data.dump();
        break;
    }
    case WECHAT_SET_VERSION:
    {
        wstring versionStr = get_http_param_str(hm, jData, "version", method);
        BOOL status = ChangeWeChatVersion(versionStr);
        json ret_data = {{"msg", status}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_LOG_START_HOOK:
    {
#ifndef _DEBUG
        CreateConsole();
#endif
        HookLogMsgInfo();
        break;
    }
    case WECHAT_LOG_STOP_HOOK:
    {
#ifndef _DEBUG
        FreeConsole();
#endif
        UnHookLogMsgInfo();
        break;
    }
    case WECHAT_BROWSER_OPEN_WITH_URL:
    {
        wstring url = get_http_param_str(hm, jData, "url", method);
        OpenBrowser(url);
        break;
    }
    case WECHAT_GET_PUBLIC_MSG:
    {
        wstring public_id = get_http_param_str(hm, jData, "public_id", method);
        wstring offset = get_http_param_str(hm, jData, "offset", method);
        string resp = GetHistoryPublicMsg(public_id, offset);
        json ret_data = {{"msg", resp}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_MSG_FORWARD_MESSAGE:
    {
        wstring wxid = get_http_param_str(hm, jData, "wxid", method);
        ULONG64 msgid = get_http_param_ulong64(hm, jData, "msgid", method);
        BOOL status = ForwardMessage(wxid, msgid);
        json ret_data = {{"msg", status}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    case WECHAT_GET_QRCODE_IMAGE:
    {
        int size = 0;
        BYTE *image = GetQrcodeImage(size);
        if (image != NULL)
        {
            mg_printf(c, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\nContent-Type: image/png\r\n\r\n");
            mg_http_write_chunk(c, (const char *)image, size);
            mg_http_printf_chunk(c, "");
            ret = "";
        }
        else
        {
            string message;
            if (isWxLogin())
                message = "获取失败，微信已登录.";
            else
                message = "获取失败.";
            json ret_data = {{"msg", gb2312_to_utf8(message.c_str())}, {"result", "OK"}};
            ret = ret_data.dump();
        }
        break;
    }
    case WECHAT_GET_A8KEY:
    {
        wstring url = get_http_param_str(hm, jData, "url", method);
        string response = GetA8Key(url);
        json ret_data = {{"msg", response}, {"result", "OK"}};
        ret = ret_data.dump();
        break;
    }
    default:
        // char* wxid = mg_json_get_str(hm->body, "$.wxid");
        break;
    }
}

// Event handler for the listening connection.
// Simply serve static files from `s_root_dir`
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
#ifdef _DEBUG
    wcout.imbue(locale("chs"));
#endif
    switch (ev)
    {
    case MG_EV_HTTP_MSG:
    {
        struct mg_http_message *hm = (struct mg_http_message *)ev_data, tmp = {0};
#ifdef _DEBUG
        struct mg_str unknown = mg_str_n("?", 1), *cl;
        mg_http_parse((char *)c->send.buf, c->send.len, &tmp);
        cl = mg_http_get_header(&tmp, "Content-Length");
        if (cl == NULL)
            cl = &unknown;
        MG_INFO(("%.*s %.*s %.*s %.*s", (int)hm->method.len, hm->method.ptr,
                 (int)hm->uri.len, hm->uri.ptr, (int)tmp.uri.len, tmp.uri.ptr,
                 (int)cl->len, cl->ptr));
#endif
        string ret = R"({"result":"OK"})";
        if (mg_http_match_uri(hm, "/api/"))
        {
            try
            {
                request_event(hm, ret, c);
            }
            catch (json::exception &e)
            {
                json res = {{"result", "ERROR"}, {"err_msg", e.what()}};
                ret = res.dump();
            }
            if (ret != "")
                mg_http_reply(c, 200, "", ret.c_str(), 0, 0);
        }
        else
        {
            struct mg_http_serve_opts opts = {0};
            opts.root_dir = s_root_dir;
            mg_http_serve_dir(c, (struct mg_http_message *)ev_data, &opts);
        }
        break;
    }
    }
    (void)fn_data;
}

void http_poll(int port)
{
    mg_log_set("2");
    mg_mgr_init(&mgr);
    string s_http_addr = "http://0.0.0.0:" + to_string(port);
    mg_http_listen(&mgr, s_http_addr.c_str(), fn, NULL);
    while (httpruning)
        mg_mgr_poll(&mgr, 1000);
}

void http_start(int port)
{
    if (httpruning)
        return;
#ifdef _DEBUG
    CreateConsole();
#endif
    httpruning = true;
    http_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)http_poll, (LPVOID)port, NULL, 0);
}

int http_close()
{
    if (!httpruning)
    {
        return 0;
    }
    httpruning = false;
    if (http_thread)
    {
        WaitForSingleObject(http_thread, -1);
        CloseHandle(http_thread);
        http_thread = NULL;
    }
    mg_mgr_free(&mgr);
    UnHookAll();
    return 0;
}
#endif // !USE_SOCKET
