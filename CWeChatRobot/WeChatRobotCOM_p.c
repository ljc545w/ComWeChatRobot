

/* this ALWAYS GENERATED file contains the proxy stub code */


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

#if !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_)


#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif /* __RPCPROXY_H_VERSION__ */


#include "WeChatRobotCOM_i.h"

#define TYPE_FORMAT_STRING_SIZE   1239                              
#define PROC_FORMAT_STRING_SIZE   967                               
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   2            

typedef struct _WeChatRobotCOM_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } WeChatRobotCOM_MIDL_TYPE_FORMAT_STRING;

typedef struct _WeChatRobotCOM_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } WeChatRobotCOM_MIDL_PROC_FORMAT_STRING;

typedef struct _WeChatRobotCOM_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } WeChatRobotCOM_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const WeChatRobotCOM_MIDL_TYPE_FORMAT_STRING WeChatRobotCOM__MIDL_TypeFormatString;
extern const WeChatRobotCOM_MIDL_PROC_FORMAT_STRING WeChatRobotCOM__MIDL_ProcFormatString;
extern const WeChatRobotCOM_MIDL_EXPR_FORMAT_STRING WeChatRobotCOM__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IWeChatRobot_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IWeChatRobot_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif
#if !(TARGET_IS_NT60_OR_LATER)
#error You need Windows Vista or later to run this stub because it uses these features:
#error   forced complex structure or array, new range semantics, compiled for Windows Vista.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const WeChatRobotCOM_MIDL_PROC_FORMAT_STRING WeChatRobotCOM__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure CStartRobotService */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
/*  8 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x24 ),	/* 36 */
/* 14 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 16 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter __result */

/* 24 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 26 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 28 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 30 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 32 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 34 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CStopRobotService */

/* 36 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 38 */	NdrFcLong( 0x0 ),	/* 0 */
/* 42 */	NdrFcShort( 0x8 ),	/* 8 */
/* 44 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 46 */	NdrFcShort( 0x0 ),	/* 0 */
/* 48 */	NdrFcShort( 0x24 ),	/* 36 */
/* 50 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 52 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 54 */	NdrFcShort( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0x0 ),	/* 0 */
/* 58 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter __result */

/* 60 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 62 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 64 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 66 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 68 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 70 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CSendText */

/* 72 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 74 */	NdrFcLong( 0x0 ),	/* 0 */
/* 78 */	NdrFcShort( 0x9 ),	/* 9 */
/* 80 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 82 */	NdrFcShort( 0x0 ),	/* 0 */
/* 84 */	NdrFcShort( 0x24 ),	/* 36 */
/* 86 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 88 */	0x8,		/* 8 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 90 */	NdrFcShort( 0x0 ),	/* 0 */
/* 92 */	NdrFcShort( 0x1 ),	/* 1 */
/* 94 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter wxid */

/* 96 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 98 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 100 */	NdrFcShort( 0x2a ),	/* Type Offset=42 */

	/* Parameter wxmsg */

/* 102 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 104 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 106 */	NdrFcShort( 0x2a ),	/* Type Offset=42 */

	/* Parameter __result */

/* 108 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 110 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 112 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 114 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 116 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 118 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CSendImage */

/* 120 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 122 */	NdrFcLong( 0x0 ),	/* 0 */
/* 126 */	NdrFcShort( 0xa ),	/* 10 */
/* 128 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 130 */	NdrFcShort( 0x0 ),	/* 0 */
/* 132 */	NdrFcShort( 0x24 ),	/* 36 */
/* 134 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 136 */	0x8,		/* 8 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 138 */	NdrFcShort( 0x0 ),	/* 0 */
/* 140 */	NdrFcShort( 0x1 ),	/* 1 */
/* 142 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter wxid */

/* 144 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 146 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 148 */	NdrFcShort( 0x2a ),	/* Type Offset=42 */

	/* Parameter imagepath */

/* 150 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 152 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 154 */	NdrFcShort( 0x2a ),	/* Type Offset=42 */

	/* Parameter __result */

/* 156 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 158 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 160 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 162 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 164 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 166 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CSendFile */

/* 168 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 170 */	NdrFcLong( 0x0 ),	/* 0 */
/* 174 */	NdrFcShort( 0xb ),	/* 11 */
/* 176 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 178 */	NdrFcShort( 0x0 ),	/* 0 */
/* 180 */	NdrFcShort( 0x24 ),	/* 36 */
/* 182 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 184 */	0x8,		/* 8 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 186 */	NdrFcShort( 0x0 ),	/* 0 */
/* 188 */	NdrFcShort( 0x1 ),	/* 1 */
/* 190 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter wxid */

/* 192 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 194 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 196 */	NdrFcShort( 0x2a ),	/* Type Offset=42 */

	/* Parameter filepath */

/* 198 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 200 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 202 */	NdrFcShort( 0x2a ),	/* Type Offset=42 */

	/* Parameter __result */

/* 204 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 206 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 208 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 210 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 212 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 214 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CSendArticle */

/* 216 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 218 */	NdrFcLong( 0x0 ),	/* 0 */
/* 222 */	NdrFcShort( 0xc ),	/* 12 */
/* 224 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 226 */	NdrFcShort( 0x0 ),	/* 0 */
/* 228 */	NdrFcShort( 0x24 ),	/* 36 */
/* 230 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 232 */	0x8,		/* 8 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 234 */	NdrFcShort( 0x0 ),	/* 0 */
/* 236 */	NdrFcShort( 0x1 ),	/* 1 */
/* 238 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter wxid */

/* 240 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 242 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 244 */	NdrFcShort( 0x2a ),	/* Type Offset=42 */

	/* Parameter title */

/* 246 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 248 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 250 */	NdrFcShort( 0x2a ),	/* Type Offset=42 */

	/* Parameter abstract */

/* 252 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 254 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 256 */	NdrFcShort( 0x2a ),	/* Type Offset=42 */

	/* Parameter url */

/* 258 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 260 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 262 */	NdrFcShort( 0x2a ),	/* Type Offset=42 */

	/* Parameter __result */

/* 264 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 266 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 268 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 270 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 272 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 274 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CSendCard */

/* 276 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 278 */	NdrFcLong( 0x0 ),	/* 0 */
/* 282 */	NdrFcShort( 0xd ),	/* 13 */
/* 284 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 286 */	NdrFcShort( 0x0 ),	/* 0 */
/* 288 */	NdrFcShort( 0x24 ),	/* 36 */
/* 290 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 292 */	0x8,		/* 8 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 294 */	NdrFcShort( 0x0 ),	/* 0 */
/* 296 */	NdrFcShort( 0x1 ),	/* 1 */
/* 298 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter receiver */

/* 300 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 302 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 304 */	NdrFcShort( 0x2a ),	/* Type Offset=42 */

	/* Parameter sharedwxid */

/* 306 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 308 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 310 */	NdrFcShort( 0x2a ),	/* Type Offset=42 */

	/* Parameter nickname */

/* 312 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 314 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 316 */	NdrFcShort( 0x2a ),	/* Type Offset=42 */

	/* Parameter __result */

/* 318 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 320 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 322 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 324 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 326 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 328 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CGetFriendList */

/* 330 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 332 */	NdrFcLong( 0x0 ),	/* 0 */
/* 336 */	NdrFcShort( 0xe ),	/* 14 */
/* 338 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 340 */	NdrFcShort( 0x0 ),	/* 0 */
/* 342 */	NdrFcShort( 0x8 ),	/* 8 */
/* 344 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 346 */	0x8,		/* 8 */
			0x43,		/* Ext Flags:  new corr desc, clt corr check, has range on conformance */
/* 348 */	NdrFcShort( 0x1 ),	/* 1 */
/* 350 */	NdrFcShort( 0x0 ),	/* 0 */
/* 352 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter __result */

/* 354 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
/* 356 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 358 */	NdrFcShort( 0x4ac ),	/* Type Offset=1196 */

	/* Return value */

/* 360 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 362 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 364 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CGetFriendListString */

/* 366 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 368 */	NdrFcLong( 0x0 ),	/* 0 */
/* 372 */	NdrFcShort( 0xf ),	/* 15 */
/* 374 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 376 */	NdrFcShort( 0x0 ),	/* 0 */
/* 378 */	NdrFcShort( 0x8 ),	/* 8 */
/* 380 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 382 */	0x8,		/* 8 */
			0x43,		/* Ext Flags:  new corr desc, clt corr check, has range on conformance */
/* 384 */	NdrFcShort( 0x1 ),	/* 1 */
/* 386 */	NdrFcShort( 0x0 ),	/* 0 */
/* 388 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter __result */

/* 390 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 392 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 394 */	NdrFcShort( 0x4ba ),	/* Type Offset=1210 */

	/* Return value */

/* 396 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 398 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 400 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CGetWxUserInfo */

/* 402 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 404 */	NdrFcLong( 0x0 ),	/* 0 */
/* 408 */	NdrFcShort( 0x10 ),	/* 16 */
/* 410 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 412 */	NdrFcShort( 0x0 ),	/* 0 */
/* 414 */	NdrFcShort( 0x8 ),	/* 8 */
/* 416 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 418 */	0x8,		/* 8 */
			0x47,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, has range on conformance */
/* 420 */	NdrFcShort( 0x1 ),	/* 1 */
/* 422 */	NdrFcShort( 0x1 ),	/* 1 */
/* 424 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter wxid */

/* 426 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 428 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 430 */	NdrFcShort( 0x2a ),	/* Type Offset=42 */

	/* Parameter __result */

/* 432 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 434 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 436 */	NdrFcShort( 0x4ba ),	/* Type Offset=1210 */

	/* Return value */

/* 438 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 440 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 442 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CGetSelfInfo */

/* 444 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 446 */	NdrFcLong( 0x0 ),	/* 0 */
/* 450 */	NdrFcShort( 0x11 ),	/* 17 */
/* 452 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 454 */	NdrFcShort( 0x0 ),	/* 0 */
/* 456 */	NdrFcShort( 0x8 ),	/* 8 */
/* 458 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 460 */	0x8,		/* 8 */
			0x43,		/* Ext Flags:  new corr desc, clt corr check, has range on conformance */
/* 462 */	NdrFcShort( 0x1 ),	/* 1 */
/* 464 */	NdrFcShort( 0x0 ),	/* 0 */
/* 466 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter __result */

/* 468 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 470 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 472 */	NdrFcShort( 0x4ba ),	/* Type Offset=1210 */

	/* Return value */

/* 474 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 476 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 478 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CCheckFriendStatusInit */

/* 480 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 482 */	NdrFcLong( 0x0 ),	/* 0 */
/* 486 */	NdrFcShort( 0x12 ),	/* 18 */
/* 488 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 490 */	NdrFcShort( 0x0 ),	/* 0 */
/* 492 */	NdrFcShort( 0x24 ),	/* 36 */
/* 494 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 496 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 498 */	NdrFcShort( 0x0 ),	/* 0 */
/* 500 */	NdrFcShort( 0x0 ),	/* 0 */
/* 502 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter __result */

/* 504 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 506 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 508 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 510 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 512 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 514 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CCheckFriendStatus */

/* 516 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 518 */	NdrFcLong( 0x0 ),	/* 0 */
/* 522 */	NdrFcShort( 0x13 ),	/* 19 */
/* 524 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 526 */	NdrFcShort( 0x0 ),	/* 0 */
/* 528 */	NdrFcShort( 0x24 ),	/* 36 */
/* 530 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 532 */	0x8,		/* 8 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 534 */	NdrFcShort( 0x0 ),	/* 0 */
/* 536 */	NdrFcShort( 0x1 ),	/* 1 */
/* 538 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter wxid */

/* 540 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 542 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 544 */	NdrFcShort( 0x2a ),	/* Type Offset=42 */

	/* Parameter __result */

/* 546 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 548 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 550 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 552 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 554 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 556 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CCheckFriendStatusFinish */

/* 558 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 560 */	NdrFcLong( 0x0 ),	/* 0 */
/* 564 */	NdrFcShort( 0x14 ),	/* 20 */
/* 566 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 568 */	NdrFcShort( 0x0 ),	/* 0 */
/* 570 */	NdrFcShort( 0x24 ),	/* 36 */
/* 572 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 574 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 576 */	NdrFcShort( 0x0 ),	/* 0 */
/* 578 */	NdrFcShort( 0x0 ),	/* 0 */
/* 580 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter __result */

/* 582 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 584 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 586 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 588 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 590 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 592 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CGetComWorkPath */

/* 594 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 596 */	NdrFcLong( 0x0 ),	/* 0 */
/* 600 */	NdrFcShort( 0x15 ),	/* 21 */
/* 602 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 604 */	NdrFcShort( 0x0 ),	/* 0 */
/* 606 */	NdrFcShort( 0x8 ),	/* 8 */
/* 608 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 610 */	0x8,		/* 8 */
			0x43,		/* Ext Flags:  new corr desc, clt corr check, has range on conformance */
/* 612 */	NdrFcShort( 0x1 ),	/* 1 */
/* 614 */	NdrFcShort( 0x0 ),	/* 0 */
/* 616 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter __result */

/* 618 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 620 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 622 */	NdrFcShort( 0x4ba ),	/* Type Offset=1210 */

	/* Return value */

/* 624 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 626 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 628 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CStartReceiveMessage */

/* 630 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 632 */	NdrFcLong( 0x0 ),	/* 0 */
/* 636 */	NdrFcShort( 0x16 ),	/* 22 */
/* 638 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 640 */	NdrFcShort( 0x0 ),	/* 0 */
/* 642 */	NdrFcShort( 0x24 ),	/* 36 */
/* 644 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 646 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 648 */	NdrFcShort( 0x0 ),	/* 0 */
/* 650 */	NdrFcShort( 0x0 ),	/* 0 */
/* 652 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter __result */

/* 654 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 656 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 658 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 660 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 662 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 664 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CReceiveMessage */

/* 666 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 668 */	NdrFcLong( 0x0 ),	/* 0 */
/* 672 */	NdrFcShort( 0x17 ),	/* 23 */
/* 674 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 676 */	NdrFcShort( 0x0 ),	/* 0 */
/* 678 */	NdrFcShort( 0x8 ),	/* 8 */
/* 680 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 682 */	0x8,		/* 8 */
			0x43,		/* Ext Flags:  new corr desc, clt corr check, has range on conformance */
/* 684 */	NdrFcShort( 0x1 ),	/* 1 */
/* 686 */	NdrFcShort( 0x0 ),	/* 0 */
/* 688 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter __result */

/* 690 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
/* 692 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 694 */	NdrFcShort( 0x4ac ),	/* Type Offset=1196 */

	/* Return value */

/* 696 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 698 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 700 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CStopReceiveMessage */

/* 702 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 704 */	NdrFcLong( 0x0 ),	/* 0 */
/* 708 */	NdrFcShort( 0x18 ),	/* 24 */
/* 710 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 712 */	NdrFcShort( 0x0 ),	/* 0 */
/* 714 */	NdrFcShort( 0x24 ),	/* 36 */
/* 716 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 718 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 720 */	NdrFcShort( 0x0 ),	/* 0 */
/* 722 */	NdrFcShort( 0x0 ),	/* 0 */
/* 724 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter __result */

/* 726 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 728 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 730 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 732 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 734 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 736 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CSendAtText */

/* 738 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 740 */	NdrFcLong( 0x0 ),	/* 0 */
/* 744 */	NdrFcShort( 0x19 ),	/* 25 */
/* 746 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 748 */	NdrFcShort( 0x0 ),	/* 0 */
/* 750 */	NdrFcShort( 0x24 ),	/* 36 */
/* 752 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 754 */	0x8,		/* 8 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 756 */	NdrFcShort( 0x0 ),	/* 0 */
/* 758 */	NdrFcShort( 0x1 ),	/* 1 */
/* 760 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter chatroomid */

/* 762 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 764 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 766 */	NdrFcShort( 0x2a ),	/* Type Offset=42 */

	/* Parameter wxid */

/* 768 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 770 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 772 */	NdrFcShort( 0x4cc ),	/* Type Offset=1228 */

	/* Parameter wxmsg */

/* 774 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 776 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 778 */	NdrFcShort( 0x2a ),	/* Type Offset=42 */

	/* Parameter __result */

/* 780 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 782 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 784 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 786 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 788 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 790 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CGetChatRoomMembers */

/* 792 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 794 */	NdrFcLong( 0x0 ),	/* 0 */
/* 798 */	NdrFcShort( 0x1a ),	/* 26 */
/* 800 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 802 */	NdrFcShort( 0x0 ),	/* 0 */
/* 804 */	NdrFcShort( 0x8 ),	/* 8 */
/* 806 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 808 */	0x8,		/* 8 */
			0x47,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, has range on conformance */
/* 810 */	NdrFcShort( 0x1 ),	/* 1 */
/* 812 */	NdrFcShort( 0x1 ),	/* 1 */
/* 814 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter chatroomid */

/* 816 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 818 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 820 */	NdrFcShort( 0x2a ),	/* Type Offset=42 */

	/* Parameter __result */

/* 822 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
/* 824 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 826 */	NdrFcShort( 0x4ac ),	/* Type Offset=1196 */

	/* Return value */

/* 828 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 830 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 832 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CGetDbHandles */

/* 834 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 836 */	NdrFcLong( 0x0 ),	/* 0 */
/* 840 */	NdrFcShort( 0x1b ),	/* 27 */
/* 842 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 844 */	NdrFcShort( 0x0 ),	/* 0 */
/* 846 */	NdrFcShort( 0x8 ),	/* 8 */
/* 848 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 850 */	0x8,		/* 8 */
			0x43,		/* Ext Flags:  new corr desc, clt corr check, has range on conformance */
/* 852 */	NdrFcShort( 0x1 ),	/* 1 */
/* 854 */	NdrFcShort( 0x0 ),	/* 0 */
/* 856 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter __result */

/* 858 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
/* 860 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 862 */	NdrFcShort( 0x4ac ),	/* Type Offset=1196 */

	/* Return value */

/* 864 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 866 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 868 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CExecuteSQL */

/* 870 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 872 */	NdrFcLong( 0x0 ),	/* 0 */
/* 876 */	NdrFcShort( 0x1c ),	/* 28 */
/* 878 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 880 */	NdrFcShort( 0x8 ),	/* 8 */
/* 882 */	NdrFcShort( 0x8 ),	/* 8 */
/* 884 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 886 */	0x8,		/* 8 */
			0x47,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, has range on conformance */
/* 888 */	NdrFcShort( 0x1 ),	/* 1 */
/* 890 */	NdrFcShort( 0x1 ),	/* 1 */
/* 892 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter DbHandle */

/* 894 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 896 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 898 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter sql */

/* 900 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 902 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 904 */	NdrFcShort( 0x2a ),	/* Type Offset=42 */

	/* Parameter __result */

/* 906 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
/* 908 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 910 */	NdrFcShort( 0x4ac ),	/* Type Offset=1196 */

	/* Return value */

/* 912 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 914 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 916 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CBackupSQLiteDB */

/* 918 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 920 */	NdrFcLong( 0x0 ),	/* 0 */
/* 924 */	NdrFcShort( 0x1d ),	/* 29 */
/* 926 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 928 */	NdrFcShort( 0x8 ),	/* 8 */
/* 930 */	NdrFcShort( 0x24 ),	/* 36 */
/* 932 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 934 */	0x8,		/* 8 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 936 */	NdrFcShort( 0x0 ),	/* 0 */
/* 938 */	NdrFcShort( 0x1 ),	/* 1 */
/* 940 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter DbHandle */

/* 942 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 944 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 946 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter savepath */

/* 948 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 950 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 952 */	NdrFcShort( 0x2a ),	/* Type Offset=42 */

	/* Parameter __result */

/* 954 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 956 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 958 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 960 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 962 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 964 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const WeChatRobotCOM_MIDL_TYPE_FORMAT_STRING WeChatRobotCOM__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/*  4 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/*  6 */	
			0x12, 0x0,	/* FC_UP */
/*  8 */	NdrFcShort( 0x18 ),	/* Offset= 24 (32) */
/* 10 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 12 */	NdrFcShort( 0x2 ),	/* 2 */
/* 14 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 16 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 18 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 20 */	0x0 , 
			0x0,		/* 0 */
/* 22 */	NdrFcLong( 0x0 ),	/* 0 */
/* 26 */	NdrFcLong( 0x0 ),	/* 0 */
/* 30 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 32 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 34 */	NdrFcShort( 0x8 ),	/* 8 */
/* 36 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (10) */
/* 38 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 40 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 42 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 44 */	NdrFcShort( 0x0 ),	/* 0 */
/* 46 */	NdrFcShort( 0x4 ),	/* 4 */
/* 48 */	NdrFcShort( 0x0 ),	/* 0 */
/* 50 */	NdrFcShort( 0xffd4 ),	/* Offset= -44 (6) */
/* 52 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 54 */	NdrFcShort( 0x476 ),	/* Offset= 1142 (1196) */
/* 56 */	
			0x13, 0x0,	/* FC_OP */
/* 58 */	NdrFcShort( 0x45e ),	/* Offset= 1118 (1176) */
/* 60 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 62 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 64 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 66 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 68 */	0x0 , 
			0x0,		/* 0 */
/* 70 */	NdrFcLong( 0x0 ),	/* 0 */
/* 74 */	NdrFcLong( 0x0 ),	/* 0 */
/* 78 */	NdrFcShort( 0x2 ),	/* Offset= 2 (80) */
/* 80 */	NdrFcShort( 0x10 ),	/* 16 */
/* 82 */	NdrFcShort( 0x2f ),	/* 47 */
/* 84 */	NdrFcLong( 0x14 ),	/* 20 */
/* 88 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 90 */	NdrFcLong( 0x3 ),	/* 3 */
/* 94 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 96 */	NdrFcLong( 0x11 ),	/* 17 */
/* 100 */	NdrFcShort( 0x8001 ),	/* Simple arm type: FC_BYTE */
/* 102 */	NdrFcLong( 0x2 ),	/* 2 */
/* 106 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 108 */	NdrFcLong( 0x4 ),	/* 4 */
/* 112 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 114 */	NdrFcLong( 0x5 ),	/* 5 */
/* 118 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 120 */	NdrFcLong( 0xb ),	/* 11 */
/* 124 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 126 */	NdrFcLong( 0xa ),	/* 10 */
/* 130 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 132 */	NdrFcLong( 0x6 ),	/* 6 */
/* 136 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (368) */
/* 138 */	NdrFcLong( 0x7 ),	/* 7 */
/* 142 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 144 */	NdrFcLong( 0x8 ),	/* 8 */
/* 148 */	NdrFcShort( 0xe2 ),	/* Offset= 226 (374) */
/* 150 */	NdrFcLong( 0xd ),	/* 13 */
/* 154 */	NdrFcShort( 0xe0 ),	/* Offset= 224 (378) */
/* 156 */	NdrFcLong( 0x9 ),	/* 9 */
/* 160 */	NdrFcShort( 0xec ),	/* Offset= 236 (396) */
/* 162 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 166 */	NdrFcShort( 0xf8 ),	/* Offset= 248 (414) */
/* 168 */	NdrFcLong( 0x24 ),	/* 36 */
/* 172 */	NdrFcShort( 0x3a2 ),	/* Offset= 930 (1102) */
/* 174 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 178 */	NdrFcShort( 0x39c ),	/* Offset= 924 (1102) */
/* 180 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 184 */	NdrFcShort( 0x39a ),	/* Offset= 922 (1106) */
/* 186 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 190 */	NdrFcShort( 0x398 ),	/* Offset= 920 (1110) */
/* 192 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 196 */	NdrFcShort( 0x396 ),	/* Offset= 918 (1114) */
/* 198 */	NdrFcLong( 0x4014 ),	/* 16404 */
/* 202 */	NdrFcShort( 0x394 ),	/* Offset= 916 (1118) */
/* 204 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 208 */	NdrFcShort( 0x392 ),	/* Offset= 914 (1122) */
/* 210 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 214 */	NdrFcShort( 0x390 ),	/* Offset= 912 (1126) */
/* 216 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 220 */	NdrFcShort( 0x37a ),	/* Offset= 890 (1110) */
/* 222 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 226 */	NdrFcShort( 0x378 ),	/* Offset= 888 (1114) */
/* 228 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 232 */	NdrFcShort( 0x382 ),	/* Offset= 898 (1130) */
/* 234 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 238 */	NdrFcShort( 0x378 ),	/* Offset= 888 (1126) */
/* 240 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 244 */	NdrFcShort( 0x37a ),	/* Offset= 890 (1134) */
/* 246 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 250 */	NdrFcShort( 0x378 ),	/* Offset= 888 (1138) */
/* 252 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 256 */	NdrFcShort( 0x376 ),	/* Offset= 886 (1142) */
/* 258 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 262 */	NdrFcShort( 0x374 ),	/* Offset= 884 (1146) */
/* 264 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 268 */	NdrFcShort( 0x372 ),	/* Offset= 882 (1150) */
/* 270 */	NdrFcLong( 0x10 ),	/* 16 */
/* 274 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 276 */	NdrFcLong( 0x12 ),	/* 18 */
/* 280 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 282 */	NdrFcLong( 0x13 ),	/* 19 */
/* 286 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 288 */	NdrFcLong( 0x15 ),	/* 21 */
/* 292 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 294 */	NdrFcLong( 0x16 ),	/* 22 */
/* 298 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 300 */	NdrFcLong( 0x17 ),	/* 23 */
/* 304 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 306 */	NdrFcLong( 0xe ),	/* 14 */
/* 310 */	NdrFcShort( 0x350 ),	/* Offset= 848 (1158) */
/* 312 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 316 */	NdrFcShort( 0x354 ),	/* Offset= 852 (1168) */
/* 318 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 322 */	NdrFcShort( 0x352 ),	/* Offset= 850 (1172) */
/* 324 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 328 */	NdrFcShort( 0x30e ),	/* Offset= 782 (1110) */
/* 330 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 334 */	NdrFcShort( 0x30c ),	/* Offset= 780 (1114) */
/* 336 */	NdrFcLong( 0x4015 ),	/* 16405 */
/* 340 */	NdrFcShort( 0x30a ),	/* Offset= 778 (1118) */
/* 342 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 346 */	NdrFcShort( 0x300 ),	/* Offset= 768 (1114) */
/* 348 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 352 */	NdrFcShort( 0x2fa ),	/* Offset= 762 (1114) */
/* 354 */	NdrFcLong( 0x0 ),	/* 0 */
/* 358 */	NdrFcShort( 0x0 ),	/* Offset= 0 (358) */
/* 360 */	NdrFcLong( 0x1 ),	/* 1 */
/* 364 */	NdrFcShort( 0x0 ),	/* Offset= 0 (364) */
/* 366 */	NdrFcShort( 0xffff ),	/* Offset= -1 (365) */
/* 368 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 370 */	NdrFcShort( 0x8 ),	/* 8 */
/* 372 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 374 */	
			0x13, 0x0,	/* FC_OP */
/* 376 */	NdrFcShort( 0xfea8 ),	/* Offset= -344 (32) */
/* 378 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 380 */	NdrFcLong( 0x0 ),	/* 0 */
/* 384 */	NdrFcShort( 0x0 ),	/* 0 */
/* 386 */	NdrFcShort( 0x0 ),	/* 0 */
/* 388 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 390 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 392 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 394 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 396 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 398 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 402 */	NdrFcShort( 0x0 ),	/* 0 */
/* 404 */	NdrFcShort( 0x0 ),	/* 0 */
/* 406 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 408 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 410 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 412 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 414 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 416 */	NdrFcShort( 0x2 ),	/* Offset= 2 (418) */
/* 418 */	
			0x13, 0x0,	/* FC_OP */
/* 420 */	NdrFcShort( 0x298 ),	/* Offset= 664 (1084) */
/* 422 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x49,		/* 73 */
/* 424 */	NdrFcShort( 0x18 ),	/* 24 */
/* 426 */	NdrFcShort( 0xa ),	/* 10 */
/* 428 */	NdrFcLong( 0x8 ),	/* 8 */
/* 432 */	NdrFcShort( 0x64 ),	/* Offset= 100 (532) */
/* 434 */	NdrFcLong( 0xd ),	/* 13 */
/* 438 */	NdrFcShort( 0x9c ),	/* Offset= 156 (594) */
/* 440 */	NdrFcLong( 0x9 ),	/* 9 */
/* 444 */	NdrFcShort( 0xd0 ),	/* Offset= 208 (652) */
/* 446 */	NdrFcLong( 0xc ),	/* 12 */
/* 450 */	NdrFcShort( 0x104 ),	/* Offset= 260 (710) */
/* 452 */	NdrFcLong( 0x24 ),	/* 36 */
/* 456 */	NdrFcShort( 0x174 ),	/* Offset= 372 (828) */
/* 458 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 462 */	NdrFcShort( 0x190 ),	/* Offset= 400 (862) */
/* 464 */	NdrFcLong( 0x10 ),	/* 16 */
/* 468 */	NdrFcShort( 0x1b4 ),	/* Offset= 436 (904) */
/* 470 */	NdrFcLong( 0x2 ),	/* 2 */
/* 474 */	NdrFcShort( 0x1d8 ),	/* Offset= 472 (946) */
/* 476 */	NdrFcLong( 0x3 ),	/* 3 */
/* 480 */	NdrFcShort( 0x1fc ),	/* Offset= 508 (988) */
/* 482 */	NdrFcLong( 0x14 ),	/* 20 */
/* 486 */	NdrFcShort( 0x220 ),	/* Offset= 544 (1030) */
/* 488 */	NdrFcShort( 0xffff ),	/* Offset= -1 (487) */
/* 490 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 492 */	NdrFcShort( 0x4 ),	/* 4 */
/* 494 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 496 */	NdrFcShort( 0x0 ),	/* 0 */
/* 498 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 500 */	0x0 , 
			0x0,		/* 0 */
/* 502 */	NdrFcLong( 0x0 ),	/* 0 */
/* 506 */	NdrFcLong( 0x0 ),	/* 0 */
/* 510 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 512 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 514 */	NdrFcShort( 0x4 ),	/* 4 */
/* 516 */	NdrFcShort( 0x0 ),	/* 0 */
/* 518 */	NdrFcShort( 0x1 ),	/* 1 */
/* 520 */	NdrFcShort( 0x0 ),	/* 0 */
/* 522 */	NdrFcShort( 0x0 ),	/* 0 */
/* 524 */	0x13, 0x0,	/* FC_OP */
/* 526 */	NdrFcShort( 0xfe12 ),	/* Offset= -494 (32) */
/* 528 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 530 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 532 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 534 */	NdrFcShort( 0x8 ),	/* 8 */
/* 536 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 538 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 540 */	NdrFcShort( 0x4 ),	/* 4 */
/* 542 */	NdrFcShort( 0x4 ),	/* 4 */
/* 544 */	0x11, 0x0,	/* FC_RP */
/* 546 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (490) */
/* 548 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 550 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 552 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 554 */	NdrFcShort( 0x0 ),	/* 0 */
/* 556 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 558 */	NdrFcShort( 0x0 ),	/* 0 */
/* 560 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 562 */	0x0 , 
			0x0,		/* 0 */
/* 564 */	NdrFcLong( 0x0 ),	/* 0 */
/* 568 */	NdrFcLong( 0x0 ),	/* 0 */
/* 572 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 576 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 578 */	0x0 , 
			0x0,		/* 0 */
/* 580 */	NdrFcLong( 0x0 ),	/* 0 */
/* 584 */	NdrFcLong( 0x0 ),	/* 0 */
/* 588 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 590 */	NdrFcShort( 0xff2c ),	/* Offset= -212 (378) */
/* 592 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 594 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 596 */	NdrFcShort( 0x8 ),	/* 8 */
/* 598 */	NdrFcShort( 0x0 ),	/* 0 */
/* 600 */	NdrFcShort( 0x6 ),	/* Offset= 6 (606) */
/* 602 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 604 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 606 */	
			0x11, 0x0,	/* FC_RP */
/* 608 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (552) */
/* 610 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 612 */	NdrFcShort( 0x0 ),	/* 0 */
/* 614 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 616 */	NdrFcShort( 0x0 ),	/* 0 */
/* 618 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 620 */	0x0 , 
			0x0,		/* 0 */
/* 622 */	NdrFcLong( 0x0 ),	/* 0 */
/* 626 */	NdrFcLong( 0x0 ),	/* 0 */
/* 630 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 634 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 636 */	0x0 , 
			0x0,		/* 0 */
/* 638 */	NdrFcLong( 0x0 ),	/* 0 */
/* 642 */	NdrFcLong( 0x0 ),	/* 0 */
/* 646 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 648 */	NdrFcShort( 0xff04 ),	/* Offset= -252 (396) */
/* 650 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 652 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 654 */	NdrFcShort( 0x8 ),	/* 8 */
/* 656 */	NdrFcShort( 0x0 ),	/* 0 */
/* 658 */	NdrFcShort( 0x6 ),	/* Offset= 6 (664) */
/* 660 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 662 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 664 */	
			0x11, 0x0,	/* FC_RP */
/* 666 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (610) */
/* 668 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 670 */	NdrFcShort( 0x4 ),	/* 4 */
/* 672 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 674 */	NdrFcShort( 0x0 ),	/* 0 */
/* 676 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 678 */	0x0 , 
			0x0,		/* 0 */
/* 680 */	NdrFcLong( 0x0 ),	/* 0 */
/* 684 */	NdrFcLong( 0x0 ),	/* 0 */
/* 688 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 690 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 692 */	NdrFcShort( 0x4 ),	/* 4 */
/* 694 */	NdrFcShort( 0x0 ),	/* 0 */
/* 696 */	NdrFcShort( 0x1 ),	/* 1 */
/* 698 */	NdrFcShort( 0x0 ),	/* 0 */
/* 700 */	NdrFcShort( 0x0 ),	/* 0 */
/* 702 */	0x13, 0x0,	/* FC_OP */
/* 704 */	NdrFcShort( 0x1d8 ),	/* Offset= 472 (1176) */
/* 706 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 708 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 710 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 712 */	NdrFcShort( 0x8 ),	/* 8 */
/* 714 */	NdrFcShort( 0x0 ),	/* 0 */
/* 716 */	NdrFcShort( 0x6 ),	/* Offset= 6 (722) */
/* 718 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 720 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 722 */	
			0x11, 0x0,	/* FC_RP */
/* 724 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (668) */
/* 726 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 728 */	NdrFcLong( 0x2f ),	/* 47 */
/* 732 */	NdrFcShort( 0x0 ),	/* 0 */
/* 734 */	NdrFcShort( 0x0 ),	/* 0 */
/* 736 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 738 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 740 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 742 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 744 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 746 */	NdrFcShort( 0x1 ),	/* 1 */
/* 748 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 750 */	NdrFcShort( 0x4 ),	/* 4 */
/* 752 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 754 */	0x0 , 
			0x0,		/* 0 */
/* 756 */	NdrFcLong( 0x0 ),	/* 0 */
/* 760 */	NdrFcLong( 0x0 ),	/* 0 */
/* 764 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 766 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 768 */	NdrFcShort( 0x10 ),	/* 16 */
/* 770 */	NdrFcShort( 0x0 ),	/* 0 */
/* 772 */	NdrFcShort( 0xa ),	/* Offset= 10 (782) */
/* 774 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 776 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 778 */	NdrFcShort( 0xffcc ),	/* Offset= -52 (726) */
/* 780 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 782 */	
			0x13, 0x20,	/* FC_OP [maybenull_sizeis] */
/* 784 */	NdrFcShort( 0xffd8 ),	/* Offset= -40 (744) */
/* 786 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 788 */	NdrFcShort( 0x4 ),	/* 4 */
/* 790 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 792 */	NdrFcShort( 0x0 ),	/* 0 */
/* 794 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 796 */	0x0 , 
			0x0,		/* 0 */
/* 798 */	NdrFcLong( 0x0 ),	/* 0 */
/* 802 */	NdrFcLong( 0x0 ),	/* 0 */
/* 806 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 808 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 810 */	NdrFcShort( 0x4 ),	/* 4 */
/* 812 */	NdrFcShort( 0x0 ),	/* 0 */
/* 814 */	NdrFcShort( 0x1 ),	/* 1 */
/* 816 */	NdrFcShort( 0x0 ),	/* 0 */
/* 818 */	NdrFcShort( 0x0 ),	/* 0 */
/* 820 */	0x13, 0x0,	/* FC_OP */
/* 822 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (766) */
/* 824 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 826 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 828 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 830 */	NdrFcShort( 0x8 ),	/* 8 */
/* 832 */	NdrFcShort( 0x0 ),	/* 0 */
/* 834 */	NdrFcShort( 0x6 ),	/* Offset= 6 (840) */
/* 836 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 838 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 840 */	
			0x11, 0x0,	/* FC_RP */
/* 842 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (786) */
/* 844 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 846 */	NdrFcShort( 0x8 ),	/* 8 */
/* 848 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 850 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 852 */	NdrFcShort( 0x10 ),	/* 16 */
/* 854 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 856 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 858 */	0x0,		/* 0 */
			NdrFcShort( 0xfff1 ),	/* Offset= -15 (844) */
			0x5b,		/* FC_END */
/* 862 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 864 */	NdrFcShort( 0x18 ),	/* 24 */
/* 866 */	NdrFcShort( 0x0 ),	/* 0 */
/* 868 */	NdrFcShort( 0xa ),	/* Offset= 10 (878) */
/* 870 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 872 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 874 */	NdrFcShort( 0xffe8 ),	/* Offset= -24 (850) */
/* 876 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 878 */	
			0x11, 0x0,	/* FC_RP */
/* 880 */	NdrFcShort( 0xfeb8 ),	/* Offset= -328 (552) */
/* 882 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 884 */	NdrFcShort( 0x1 ),	/* 1 */
/* 886 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 888 */	NdrFcShort( 0x0 ),	/* 0 */
/* 890 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 892 */	0x0 , 
			0x0,		/* 0 */
/* 894 */	NdrFcLong( 0x0 ),	/* 0 */
/* 898 */	NdrFcLong( 0x0 ),	/* 0 */
/* 902 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 904 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 906 */	NdrFcShort( 0x8 ),	/* 8 */
/* 908 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 910 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 912 */	NdrFcShort( 0x4 ),	/* 4 */
/* 914 */	NdrFcShort( 0x4 ),	/* 4 */
/* 916 */	0x13, 0x20,	/* FC_OP [maybenull_sizeis] */
/* 918 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (882) */
/* 920 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 922 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 924 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 926 */	NdrFcShort( 0x2 ),	/* 2 */
/* 928 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 930 */	NdrFcShort( 0x0 ),	/* 0 */
/* 932 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 934 */	0x0 , 
			0x0,		/* 0 */
/* 936 */	NdrFcLong( 0x0 ),	/* 0 */
/* 940 */	NdrFcLong( 0x0 ),	/* 0 */
/* 944 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 946 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 948 */	NdrFcShort( 0x8 ),	/* 8 */
/* 950 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 952 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 954 */	NdrFcShort( 0x4 ),	/* 4 */
/* 956 */	NdrFcShort( 0x4 ),	/* 4 */
/* 958 */	0x13, 0x20,	/* FC_OP [maybenull_sizeis] */
/* 960 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (924) */
/* 962 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 964 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 966 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 968 */	NdrFcShort( 0x4 ),	/* 4 */
/* 970 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 972 */	NdrFcShort( 0x0 ),	/* 0 */
/* 974 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 976 */	0x0 , 
			0x0,		/* 0 */
/* 978 */	NdrFcLong( 0x0 ),	/* 0 */
/* 982 */	NdrFcLong( 0x0 ),	/* 0 */
/* 986 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 988 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 990 */	NdrFcShort( 0x8 ),	/* 8 */
/* 992 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 994 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 996 */	NdrFcShort( 0x4 ),	/* 4 */
/* 998 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1000 */	0x13, 0x20,	/* FC_OP [maybenull_sizeis] */
/* 1002 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (966) */
/* 1004 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 1006 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1008 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 1010 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1012 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1014 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1016 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1018 */	0x0 , 
			0x0,		/* 0 */
/* 1020 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1024 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1028 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 1030 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 1032 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1034 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 1036 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 1038 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1040 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1042 */	0x13, 0x20,	/* FC_OP [maybenull_sizeis] */
/* 1044 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (1008) */
/* 1046 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 1048 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1050 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 1052 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1054 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1056 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1058 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1060 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1062 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 1064 */	NdrFcShort( 0xffd8 ),	/* -40 */
/* 1066 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1068 */	0x0 , 
			0x0,		/* 0 */
/* 1070 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1074 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1078 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1080 */	NdrFcShort( 0xffe2 ),	/* Offset= -30 (1050) */
/* 1082 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1084 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1086 */	NdrFcShort( 0x28 ),	/* 40 */
/* 1088 */	NdrFcShort( 0xffe2 ),	/* Offset= -30 (1058) */
/* 1090 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1090) */
/* 1092 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1094 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1096 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1098 */	NdrFcShort( 0xfd5c ),	/* Offset= -676 (422) */
/* 1100 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1102 */	
			0x13, 0x0,	/* FC_OP */
/* 1104 */	NdrFcShort( 0xfeae ),	/* Offset= -338 (766) */
/* 1106 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1108 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 1110 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1112 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 1114 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1116 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 1118 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1120 */	0xb,		/* FC_HYPER */
			0x5c,		/* FC_PAD */
/* 1122 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1124 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 1126 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1128 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 1130 */	
			0x13, 0x0,	/* FC_OP */
/* 1132 */	NdrFcShort( 0xfd04 ),	/* Offset= -764 (368) */
/* 1134 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1136 */	NdrFcShort( 0xfd06 ),	/* Offset= -762 (374) */
/* 1138 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1140 */	NdrFcShort( 0xfd06 ),	/* Offset= -762 (378) */
/* 1142 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1144 */	NdrFcShort( 0xfd14 ),	/* Offset= -748 (396) */
/* 1146 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1148 */	NdrFcShort( 0xfd22 ),	/* Offset= -734 (414) */
/* 1150 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1152 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1154) */
/* 1154 */	
			0x13, 0x0,	/* FC_OP */
/* 1156 */	NdrFcShort( 0x14 ),	/* Offset= 20 (1176) */
/* 1158 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 1160 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1162 */	0x6,		/* FC_SHORT */
			0x1,		/* FC_BYTE */
/* 1164 */	0x1,		/* FC_BYTE */
			0x8,		/* FC_LONG */
/* 1166 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 1168 */	
			0x13, 0x0,	/* FC_OP */
/* 1170 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (1158) */
/* 1172 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1174 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 1176 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 1178 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1180 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1182 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1182) */
/* 1184 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1186 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1188 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1190 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1192 */	NdrFcShort( 0xfb94 ),	/* Offset= -1132 (60) */
/* 1194 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1196 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1198 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1200 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1202 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1204 */	NdrFcShort( 0xfb84 ),	/* Offset= -1148 (56) */
/* 1206 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 1208 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1210) */
/* 1210 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1212 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1214 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1216 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1218 */	NdrFcShort( 0xfcb4 ),	/* Offset= -844 (374) */
/* 1220 */	
			0x11, 0x0,	/* FC_RP */
/* 1222 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1228) */
/* 1224 */	
			0x12, 0x0,	/* FC_UP */
/* 1226 */	NdrFcShort( 0xffce ),	/* Offset= -50 (1176) */
/* 1228 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1230 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1232 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1234 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1236 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (1224) */

			0x0
        }
    };

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            },
            {
            VARIANT_UserSize
            ,VARIANT_UserMarshal
            ,VARIANT_UserUnmarshal
            ,VARIANT_UserFree
            }

        };



/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IWeChatRobot, ver. 0.0,
   GUID={0xa836e359,0x59b2,0x4e01,{0x9b,0x0c,0x0e,0x5a,0x5a,0x2e,0xfa,0x22}} */

#pragma code_seg(".orpc")
static const unsigned short IWeChatRobot_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    36,
    72,
    120,
    168,
    216,
    276,
    330,
    366,
    402,
    444,
    480,
    516,
    558,
    594,
    630,
    666,
    702,
    738,
    792,
    834,
    870,
    918
    };

static const MIDL_STUBLESS_PROXY_INFO IWeChatRobot_ProxyInfo =
    {
    &Object_StubDesc,
    WeChatRobotCOM__MIDL_ProcFormatString.Format,
    &IWeChatRobot_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IWeChatRobot_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    WeChatRobotCOM__MIDL_ProcFormatString.Format,
    &IWeChatRobot_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(30) _IWeChatRobotProxyVtbl = 
{
    &IWeChatRobot_ProxyInfo,
    &IID_IWeChatRobot,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IWeChatRobot::CStartRobotService */ ,
    (void *) (INT_PTR) -1 /* IWeChatRobot::CStopRobotService */ ,
    (void *) (INT_PTR) -1 /* IWeChatRobot::CSendText */ ,
    (void *) (INT_PTR) -1 /* IWeChatRobot::CSendImage */ ,
    (void *) (INT_PTR) -1 /* IWeChatRobot::CSendFile */ ,
    (void *) (INT_PTR) -1 /* IWeChatRobot::CSendArticle */ ,
    (void *) (INT_PTR) -1 /* IWeChatRobot::CSendCard */ ,
    (void *) (INT_PTR) -1 /* IWeChatRobot::CGetFriendList */ ,
    (void *) (INT_PTR) -1 /* IWeChatRobot::CGetFriendListString */ ,
    (void *) (INT_PTR) -1 /* IWeChatRobot::CGetWxUserInfo */ ,
    (void *) (INT_PTR) -1 /* IWeChatRobot::CGetSelfInfo */ ,
    (void *) (INT_PTR) -1 /* IWeChatRobot::CCheckFriendStatusInit */ ,
    (void *) (INT_PTR) -1 /* IWeChatRobot::CCheckFriendStatus */ ,
    (void *) (INT_PTR) -1 /* IWeChatRobot::CCheckFriendStatusFinish */ ,
    (void *) (INT_PTR) -1 /* IWeChatRobot::CGetComWorkPath */ ,
    (void *) (INT_PTR) -1 /* IWeChatRobot::CStartReceiveMessage */ ,
    (void *) (INT_PTR) -1 /* IWeChatRobot::CReceiveMessage */ ,
    (void *) (INT_PTR) -1 /* IWeChatRobot::CStopReceiveMessage */ ,
    (void *) (INT_PTR) -1 /* IWeChatRobot::CSendAtText */ ,
    (void *) (INT_PTR) -1 /* IWeChatRobot::CGetChatRoomMembers */ ,
    (void *) (INT_PTR) -1 /* IWeChatRobot::CGetDbHandles */ ,
    (void *) (INT_PTR) -1 /* IWeChatRobot::CExecuteSQL */ ,
    (void *) (INT_PTR) -1 /* IWeChatRobot::CBackupSQLiteDB */
};


static const PRPC_STUB_FUNCTION IWeChatRobot_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IWeChatRobotStubVtbl =
{
    &IID_IWeChatRobot,
    &IWeChatRobot_ServerInfo,
    30,
    &IWeChatRobot_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    WeChatRobotCOM__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x60001, /* Ndr library version */
    0,
    0x801026e, /* MIDL Version 8.1.622 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };

const CInterfaceProxyVtbl * const _WeChatRobotCOM_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IWeChatRobotProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _WeChatRobotCOM_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IWeChatRobotStubVtbl,
    0
};

PCInterfaceName const _WeChatRobotCOM_InterfaceNamesList[] = 
{
    "IWeChatRobot",
    0
};

const IID *  const _WeChatRobotCOM_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _WeChatRobotCOM_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _WeChatRobotCOM, pIID, n)

int __stdcall _WeChatRobotCOM_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_WeChatRobotCOM_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo WeChatRobotCOM_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _WeChatRobotCOM_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _WeChatRobotCOM_StubVtblList,
    (const PCInterfaceName * ) & _WeChatRobotCOM_InterfaceNamesList,
    (const IID ** ) & _WeChatRobotCOM_BaseIIDList,
    & _WeChatRobotCOM_IID_Lookup, 
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_) */

