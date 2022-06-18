#pragma once
#include<windows.h>
#define USE_COM
#include<comutil.h>
#pragma comment(lib, "comsuppw.lib")
BOOL PostComMessage(VARIANT* msg);