// dlldata.c 的包装器

#ifdef _MERGE_PROXYSTUB // 合并代理存根(stub) DLL

#define REGISTER_PROXY_DLL //DllRegisterServer 等

#define USE_STUBLESS_PROXY	//仅当使用 MIDL 开关 /Oicf 时定义

#pragma comment(lib, "rpcns4.lib")
#pragma comment(lib, "rpcrt4.lib")

#define ENTRY_PREFIX	Prx

#include "dlldata.c"
#include "WeChatRobotCOM_p.c"

#endif //_MERGE_PROXYSTUB
