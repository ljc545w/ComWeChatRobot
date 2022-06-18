#pragma once
#ifdef USE_SOCKET
#include <signal.h>
// mongoose: https://github.com/Automattic/mongoose
#include "mongoose/mongoose.h"
#pragma comment(lib,"ws2_32.lib")
extern "C" __declspec(dllexport) void HttpStart();
extern "C" __declspec(dllexport) int HttpClose();
#endif