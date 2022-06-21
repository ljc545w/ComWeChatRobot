

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for WeChatRobotCOM.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        EXTERN_C __declspec(selectany) const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif // !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IWeChatRobot,0xa836e359,0x59b2,0x4e01,0x9b,0x0c,0x0e,0x5a,0x5a,0x2e,0xfa,0x22);


MIDL_DEFINE_GUID(IID, IID_IRobotEvent,0x1a9d8d1b,0xd40e,0x4f3a,0x8e,0xd4,0xb7,0x83,0xe6,0xa5,0x98,0x30);


MIDL_DEFINE_GUID(IID, LIBID_WeChatRobotCOMLib,0x721abb35,0x141a,0x4aa2,0x94,0xf2,0x76,0x2e,0x28,0x33,0xfa,0x6c);


MIDL_DEFINE_GUID(CLSID, CLSID_WeChatRobot,0x4ce7f5e1,0x2c93,0x4bfe,0x86,0xac,0x88,0x64,0x52,0xe4,0x74,0x75);


MIDL_DEFINE_GUID(IID, DIID__IRobotEventEvents,0x8fd26fd1,0x9169,0x4e4c,0x8a,0x1d,0xd0,0x50,0x93,0xba,0xcc,0xe5);


MIDL_DEFINE_GUID(CLSID, CLSID_RobotEvent,0x178001e8,0x5e64,0x4cec,0x80,0x32,0x1d,0xe2,0x88,0x01,0xdf,0x55);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



