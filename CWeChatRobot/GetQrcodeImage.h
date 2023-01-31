#pragma once
#include <windows.h>
#include <atlcomcli.h>
#include <atlsafe.h>
ATL::CComVariant GetQrcodeImage(DWORD pid);
BOOL isWxLogin(DWORD pid);
