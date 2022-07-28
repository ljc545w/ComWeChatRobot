#include "pch.h"

struct SendArticleStruct {
	DWORD wxid;
	DWORD title;
	DWORD abstract;
	DWORD url;
	DWORD imgpath;
};

BOOL SendArticle(DWORD pid,wchar_t* wxid, wchar_t* title, wchar_t* abstract, wchar_t* url, wchar_t* imgpath) {
    WeChatProcess hp(pid);
    if (!hp.m_init) return 1;
    DWORD SendArticleRemoteAddr = hp.GetProcAddr(SendArticleRemote);
    if (SendArticleRemoteAddr == 0) {
        return 1;
    }
    SendArticleStruct params = { 0 };
    WeChatData<wchar_t*> r_wxid(hp.GetHandle(), wxid, TEXTLENGTH(wxid));
    WeChatData<wchar_t*> r_title(hp.GetHandle(), title, TEXTLENGTH(title));
    WeChatData<wchar_t*> r_abstract(hp.GetHandle(), abstract, TEXTLENGTH(abstract));
    WeChatData<wchar_t*> r_url(hp.GetHandle(), url, TEXTLENGTH(url));
    WeChatData<wchar_t*> r_imgpath(hp.GetHandle(), imgpath, TEXTLENGTH(imgpath));
    params.wxid = (DWORD)r_wxid.GetAddr();
    params.title = (DWORD)r_title.GetAddr();
    params.abstract = (DWORD)r_abstract.GetAddr();
    params.url = (DWORD)r_url.GetAddr();
    params.imgpath = (DWORD)r_imgpath.GetAddr();
    WeChatData<SendArticleStruct*> r_params(hp.GetHandle(), &params, sizeof(params));
    if (!r_wxid.GetAddr() || !r_title.GetAddr() || !r_abstract.GetAddr() || !r_url.GetAddr() || !r_params.GetAddr()) {
        return 1;
    }
    DWORD dwRet = CallRemoteFunction(hp.GetHandle(), SendArticleRemoteAddr, r_params.GetAddr());
	return 0;
}