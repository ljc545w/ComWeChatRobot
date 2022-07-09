#include "pch.h"

#ifdef USE_SOCKET
// mongoose: https://github.com/cesanta/mongoose
#include "mongoose/mongoose.c"
#include<thread>
static struct mg_mgr mgr;
static bool httpruning = false;
static const char* s_http_addr = "http://0.0.0.0:8000";    // HTTP port
static const char* s_root_dir = ".";
static bool allclosed = false;

// Event handler for the listening connection.
// Simply serve static files from `s_root_dir`
static void fn(struct mg_connection* c, int ev, void* ev_data, void* fn_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message* hm = (struct mg_http_message*)ev_data, tmp = { 0 };
        if (mg_http_match_uri(hm, "/api/test/")) {
            mg_http_reply(c, 200, "", "{\"result\": \"OK\"}", 0, 0);
            // cout << hm->method.ptr << endl;
            char* wxid = mg_json_get_str(hm->body, "$.wxid");
            char* msg = mg_json_get_str(hm->body, "$.msg");
            SendText((wchar_t*)L"filehelper", (wchar_t*)L"≤‚ ‘œ˚œ¢");
        }
        else if (mg_http_match_uri(hm, "/api/f2/*")) {
            mg_printf(c, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
            mg_http_printf_chunk(c, "ID PROTO TYPE      LOCAL           REMOTE\n");
            mg_http_printf_chunk(c, "");
        }
        else {
            struct mg_http_serve_opts opts = { 0 };
            opts.root_dir = s_root_dir;
            mg_http_serve_dir(c, (struct mg_http_message*)ev_data, &opts);
        }
    }
    (void)fn_data;
}

void HttpStart() {
    CreateConsole();
    mg_log_set("2");
    mg_mgr_init(&mgr);
    mg_http_listen(&mgr, s_http_addr, fn, NULL);
    httpruning = true;
    while(httpruning)
        mg_mgr_poll(&mgr, 1000);
    mg_mgr_free(&mgr);
    allclosed = true;
}

int HttpClose() {
    if (!httpruning) {
        return 0;
    }
    httpruning = false;
    while (!allclosed)
        Sleep(100);
    allclosed = false;
    return 0;
}
#endif // !USE_SOCKET