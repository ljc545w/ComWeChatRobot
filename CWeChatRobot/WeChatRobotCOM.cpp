// WeChatRobotCOM.cpp: WinMain 的实现


#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "WeChatRobotCOM_i.h"
#include "xdlldata.h"


using namespace ATL;


class CWeChatRobotCOMModule : public ATL::CAtlExeModuleT< CWeChatRobotCOMModule >
{
public :
	DECLARE_LIBID(LIBID_WeChatRobotCOMLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_WECHATROBOTCOM, "{721abb35-141a-4aa2-94f2-762e2833fa6c}")
};

CWeChatRobotCOMModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/,
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

