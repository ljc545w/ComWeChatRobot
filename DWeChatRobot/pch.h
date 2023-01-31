// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。
#pragma once
#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include "framework.h"
#include <iostream>
#include <vector>
#include <map>
#include <strstream>
#include <string>
#include <sstream>
#include <fstream>
#include <imagehlp.h>
#include <time.h>
#include "wxdata.h"
#include "wxapi.h"
#include "base64/base64.h"
#include <glog/logging.h>
#endif // PCH_H

#ifndef _DEBUG
#pragma comment(lib, "glog.lib")
#else
#pragma comment(lib, "glogd.lib")
#endif

#ifdef USE_SOCKET
#include "wxsocketapi.h"
#else
#include "comclient.h"
#endif
