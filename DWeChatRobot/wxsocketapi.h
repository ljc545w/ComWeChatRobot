#pragma once
#include "wxsocket.h"
#ifdef WX_SOCKET
#include <signal.h>
#include "mongoose.h"
#pragma comment(lib,"ws2_32.lib")
int HttpStart();
int HttpClose();
#endif