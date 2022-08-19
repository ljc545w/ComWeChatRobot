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
#define POST_PARAM(jData, key) utf8_to_unicode(string(jData[key]).c_str())
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
        result = STOI_S((string)jData[key]);
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

void request_event(mg_http_message *hm)
{
    int method = I_METHOD(getMgStrA(hm->method));
    // 第四个参数设置为false，不抛出异常
    json jData = json::parse(hm->body.ptr, hm->body.ptr + hm->body.len, nullptr, false);
    if (jData.is_discarded() == true && method == HTTP_METHOD_POST)
        return;
    int num = STOI_S(getMgVarA(hm, "type"));
    switch (num)
    {
    case WECHAT_MSG_SEND_TEXT:
    {
        wstring wxid = get_http_param_str(hm, jData, "wxid", method);
        wstring msg = get_http_param_str(hm, jData, "msg", method);
        SendText(wxid, msg);
        break;
    }
    case WECHAT_MSG_SEND_AT:
    {
        wstring chatroom = get_http_param_str(hm, jData, "chatroom_id", method);
        vector<wstring> wxids = get_http_param_array(hm, jData, "wxids", method);
        wstring msg = get_http_param_str(hm, jData, "msg", method);
        int auto_nickname = get_http_param_int(hm, jData, "auto_nickname", method);
        SendAtText(chatroom, wxids, msg, auto_nickname);
        break;
    }
    case WECHAT_MSG_SEND_CARD:
    {
        wstring receiver = get_http_param_str(hm, jData, "receiver", method);
        wstring shared_wxid = get_http_param_str(hm, jData, "shared_wxid", method);
        wstring nickname = get_http_param_str(hm, jData, "nickname", method);
        SendCard(receiver, shared_wxid, nickname);
        break;
    }
    case WECHAT_MSG_SEND_IMAGE:
    {
        wstring receiver = get_http_param_str(hm, jData, "receiver", method);
        wstring img_path = get_http_param_str(hm, jData, "img_path", method);
        SendImage(receiver, img_path);
        break;
    }
    case WECHAT_MSG_SEND_FILE:
    {
        wstring receiver = get_http_param_str(hm, jData, "receiver", method);
        wstring file_path = get_http_param_str(hm, jData, "file_path", method);
        SendFile(receiver, file_path);
        break;
    }
    case WECHAT_MSG_SEND_ARTICLE:
    {
        wstring wxid = get_http_param_str(hm, jData, "wxid", method);
        wstring title = get_http_param_str(hm, jData, "title", method);
        wstring abstract = get_http_param_str(hm, jData, "abstract", method);
        wstring url = get_http_param_str(hm, jData, "url", method);
        wstring img_path = get_http_param_str(hm, jData, "img_path", method);
        SendArticle(wxid, title, abstract, url, img_path);
        break;
    }
    case WECHAT_MSG_SEND_APP:
    {
        wstring wxid = get_http_param_str(hm, jData, "wxid", method);
        wstring appid = get_http_param_str(hm, jData, "appid", method);
        SendAppMsg(wxid, appid);
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
    wcout.imbue(locale("chs"));
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
        if (mg_http_match_uri(hm, "/api/"))
        {
            try
            {
                request_event(hm);
                mg_http_reply(c, 200, "", "{\"result\": \"OK\"}", 0, 0);
            }
            catch (json::exception)
            {
                mg_http_reply(c, 200, "", "{\"result\": \"ERROR\"}", 0, 0);
            }
        }
        else if (mg_http_match_uri(hm, "/api/f2/*"))
        {
            mg_printf(c, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
            mg_http_printf_chunk(c, "ID PROTO TYPE      LOCAL           REMOTE\n");
            mg_http_printf_chunk(c, "");
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
