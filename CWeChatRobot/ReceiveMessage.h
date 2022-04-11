#pragma once
#include<windows.h>
BOOL StartReceiveMessage();
SAFEARRAY* ReceiveMessage();
BOOL StopReceiveMessage();