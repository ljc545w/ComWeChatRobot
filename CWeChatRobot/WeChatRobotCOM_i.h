

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __WeChatRobotCOM_i_h__
#define __WeChatRobotCOM_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IWeChatRobot_FWD_DEFINED__
#define __IWeChatRobot_FWD_DEFINED__
typedef interface IWeChatRobot IWeChatRobot;

#endif 	/* __IWeChatRobot_FWD_DEFINED__ */


#ifndef __IRobotEvent_FWD_DEFINED__
#define __IRobotEvent_FWD_DEFINED__
typedef interface IRobotEvent IRobotEvent;

#endif 	/* __IRobotEvent_FWD_DEFINED__ */


#ifndef __WeChatRobot_FWD_DEFINED__
#define __WeChatRobot_FWD_DEFINED__

#ifdef __cplusplus
typedef class WeChatRobot WeChatRobot;
#else
typedef struct WeChatRobot WeChatRobot;
#endif /* __cplusplus */

#endif 	/* __WeChatRobot_FWD_DEFINED__ */


#ifndef ___IRobotEventEvents_FWD_DEFINED__
#define ___IRobotEventEvents_FWD_DEFINED__
typedef interface _IRobotEventEvents _IRobotEventEvents;

#endif 	/* ___IRobotEventEvents_FWD_DEFINED__ */


#ifndef __RobotEvent_FWD_DEFINED__
#define __RobotEvent_FWD_DEFINED__

#ifdef __cplusplus
typedef class RobotEvent RobotEvent;
#else
typedef struct RobotEvent RobotEvent;
#endif /* __cplusplus */

#endif 	/* __RobotEvent_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "shobjidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IWeChatRobot_INTERFACE_DEFINED__
#define __IWeChatRobot_INTERFACE_DEFINED__

/* interface IWeChatRobot */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IWeChatRobot;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("a836e359-59b2-4e01-9b0c-0e5a5a2efa22")
    IWeChatRobot : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CStartRobotService( 
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CStopRobotService( 
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CSendText( 
            /* [in] */ BSTR wxid,
            /* [in] */ BSTR wxmsg,
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CSendImage( 
            /* [in] */ BSTR wxid,
            /* [in] */ BSTR imagepath,
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CSendFile( 
            /* [in] */ BSTR wxid,
            /* [in] */ BSTR filepath,
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CSendArticle( 
            /* [in] */ BSTR wxid,
            /* [in] */ BSTR title,
            /* [in] */ BSTR abstract,
            /* [in] */ BSTR url,
            /* [in] */ BSTR imgpath,
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CSendCard( 
            /* [in] */ BSTR receiver,
            /* [in] */ BSTR sharedwxid,
            /* [in] */ BSTR nickname,
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CGetFriendList( 
            /* [retval][out] */ VARIANT *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CGetFriendListString( 
            /* [retval][out] */ BSTR *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CGetWxUserInfo( 
            /* [in] */ BSTR wxid,
            /* [retval][out] */ BSTR *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CGetSelfInfo( 
            /* [retval][out] */ BSTR *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CCheckFriendStatus( 
            /* [in] */ BSTR wxid,
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CGetComWorkPath( 
            /* [retval][out] */ BSTR *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CStartReceiveMessage( 
            /* [in] */ int port,
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CStopReceiveMessage( 
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CSendAtText( 
            /* [in] */ BSTR chatroomid,
            /* [in] */ VARIANT *wxid,
            /* [in] */ BSTR wxmsg,
            /* [in] */ BOOL AutoNickName,
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CGetChatRoomMembers( 
            /* [in] */ BSTR chatroomid,
            /* [retval][out] */ VARIANT *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CGetDbHandles( 
            /* [retval][out] */ VARIANT *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CExecuteSQL( 
            /* [in] */ DWORD DbHandle,
            /* [in] */ BSTR sql,
            /* [retval][out] */ VARIANT *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CBackupSQLiteDB( 
            /* [in] */ DWORD DbHandle,
            /* [in] */ BSTR savepath,
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CVerifyFriendApply( 
            /* [in] */ BSTR v3,
            /* [in] */ BSTR v4,
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CAddFriendByWxid( 
            /* [in] */ BSTR wxid,
            /* [in] */ BSTR message,
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CAddFriendByV3( 
            /* [in] */ BSTR v3,
            /* [in] */ BSTR message,
            /* [in] */ int AddType,
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CGetWeChatVer( 
            /* [retval][out] */ BSTR *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CStartWeChat( 
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CSearchContactByNet( 
            /* [in] */ BSTR keyword,
            /* [retval][out] */ VARIANT *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CAddBrandContact( 
            /* [in] */ BSTR PublicId,
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CHookVoiceMsg( 
            /* [in] */ BSTR savepath,
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CUnHookVoiceMsg( 
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CHookImageMsg( 
            /* [in] */ BSTR savepath,
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CUnHookImageMsg( 
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CChangeWeChatVer( 
            /* [in] */ BSTR verStr,
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CSendAppMsg( 
            /* [in] */ BSTR wxid,
            /* [in] */ BSTR appid,
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CDeleteUser( 
            /* [in] */ BSTR wxid,
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CIsWxLogin( 
            /* [retval][out] */ int *__result) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IWeChatRobotVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWeChatRobot * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWeChatRobot * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWeChatRobot * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWeChatRobot * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWeChatRobot * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWeChatRobot * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWeChatRobot * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CStartRobotService )( 
            IWeChatRobot * This,
            /* [retval][out] */ int *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CStopRobotService )( 
            IWeChatRobot * This,
            /* [retval][out] */ int *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CSendText )( 
            IWeChatRobot * This,
            /* [in] */ BSTR wxid,
            /* [in] */ BSTR wxmsg,
            /* [retval][out] */ int *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CSendImage )( 
            IWeChatRobot * This,
            /* [in] */ BSTR wxid,
            /* [in] */ BSTR imagepath,
            /* [retval][out] */ int *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CSendFile )( 
            IWeChatRobot * This,
            /* [in] */ BSTR wxid,
            /* [in] */ BSTR filepath,
            /* [retval][out] */ int *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CSendArticle )( 
            IWeChatRobot * This,
            /* [in] */ BSTR wxid,
            /* [in] */ BSTR title,
            /* [in] */ BSTR abstract,
            /* [in] */ BSTR url,
            /* [in] */ BSTR imgpath,
            /* [retval][out] */ int *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CSendCard )( 
            IWeChatRobot * This,
            /* [in] */ BSTR receiver,
            /* [in] */ BSTR sharedwxid,
            /* [in] */ BSTR nickname,
            /* [retval][out] */ int *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CGetFriendList )( 
            IWeChatRobot * This,
            /* [retval][out] */ VARIANT *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CGetFriendListString )( 
            IWeChatRobot * This,
            /* [retval][out] */ BSTR *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CGetWxUserInfo )( 
            IWeChatRobot * This,
            /* [in] */ BSTR wxid,
            /* [retval][out] */ BSTR *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CGetSelfInfo )( 
            IWeChatRobot * This,
            /* [retval][out] */ BSTR *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CCheckFriendStatus )( 
            IWeChatRobot * This,
            /* [in] */ BSTR wxid,
            /* [retval][out] */ int *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CGetComWorkPath )( 
            IWeChatRobot * This,
            /* [retval][out] */ BSTR *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CStartReceiveMessage )( 
            IWeChatRobot * This,
            /* [in] */ int port,
            /* [retval][out] */ int *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CStopReceiveMessage )( 
            IWeChatRobot * This,
            /* [retval][out] */ int *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CSendAtText )( 
            IWeChatRobot * This,
            /* [in] */ BSTR chatroomid,
            /* [in] */ VARIANT *wxid,
            /* [in] */ BSTR wxmsg,
            /* [in] */ BOOL AutoNickName,
            /* [retval][out] */ int *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CGetChatRoomMembers )( 
            IWeChatRobot * This,
            /* [in] */ BSTR chatroomid,
            /* [retval][out] */ VARIANT *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CGetDbHandles )( 
            IWeChatRobot * This,
            /* [retval][out] */ VARIANT *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CExecuteSQL )( 
            IWeChatRobot * This,
            /* [in] */ DWORD DbHandle,
            /* [in] */ BSTR sql,
            /* [retval][out] */ VARIANT *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CBackupSQLiteDB )( 
            IWeChatRobot * This,
            /* [in] */ DWORD DbHandle,
            /* [in] */ BSTR savepath,
            /* [retval][out] */ int *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CVerifyFriendApply )( 
            IWeChatRobot * This,
            /* [in] */ BSTR v3,
            /* [in] */ BSTR v4,
            /* [retval][out] */ int *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CAddFriendByWxid )( 
            IWeChatRobot * This,
            /* [in] */ BSTR wxid,
            /* [in] */ BSTR message,
            /* [retval][out] */ int *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CAddFriendByV3 )( 
            IWeChatRobot * This,
            /* [in] */ BSTR v3,
            /* [in] */ BSTR message,
            /* [in] */ int AddType,
            /* [retval][out] */ int *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CGetWeChatVer )( 
            IWeChatRobot * This,
            /* [retval][out] */ BSTR *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CStartWeChat )( 
            IWeChatRobot * This,
            /* [retval][out] */ int *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CSearchContactByNet )( 
            IWeChatRobot * This,
            /* [in] */ BSTR keyword,
            /* [retval][out] */ VARIANT *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CAddBrandContact )( 
            IWeChatRobot * This,
            /* [in] */ BSTR PublicId,
            /* [retval][out] */ int *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CHookVoiceMsg )( 
            IWeChatRobot * This,
            /* [in] */ BSTR savepath,
            /* [retval][out] */ int *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CUnHookVoiceMsg )( 
            IWeChatRobot * This,
            /* [retval][out] */ int *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CHookImageMsg )( 
            IWeChatRobot * This,
            /* [in] */ BSTR savepath,
            /* [retval][out] */ int *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CUnHookImageMsg )( 
            IWeChatRobot * This,
            /* [retval][out] */ int *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CChangeWeChatVer )( 
            IWeChatRobot * This,
            /* [in] */ BSTR verStr,
            /* [retval][out] */ int *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CSendAppMsg )( 
            IWeChatRobot * This,
            /* [in] */ BSTR wxid,
            /* [in] */ BSTR appid,
            /* [retval][out] */ int *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CDeleteUser )( 
            IWeChatRobot * This,
            /* [in] */ BSTR wxid,
            /* [retval][out] */ int *__result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CIsWxLogin )( 
            IWeChatRobot * This,
            /* [retval][out] */ int *__result);
        
        END_INTERFACE
    } IWeChatRobotVtbl;

    interface IWeChatRobot
    {
        CONST_VTBL struct IWeChatRobotVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWeChatRobot_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWeChatRobot_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWeChatRobot_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWeChatRobot_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IWeChatRobot_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IWeChatRobot_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IWeChatRobot_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IWeChatRobot_CStartRobotService(This,__result)	\
    ( (This)->lpVtbl -> CStartRobotService(This,__result) ) 

#define IWeChatRobot_CStopRobotService(This,__result)	\
    ( (This)->lpVtbl -> CStopRobotService(This,__result) ) 

#define IWeChatRobot_CSendText(This,wxid,wxmsg,__result)	\
    ( (This)->lpVtbl -> CSendText(This,wxid,wxmsg,__result) ) 

#define IWeChatRobot_CSendImage(This,wxid,imagepath,__result)	\
    ( (This)->lpVtbl -> CSendImage(This,wxid,imagepath,__result) ) 

#define IWeChatRobot_CSendFile(This,wxid,filepath,__result)	\
    ( (This)->lpVtbl -> CSendFile(This,wxid,filepath,__result) ) 

#define IWeChatRobot_CSendArticle(This,wxid,title,abstract,url,imgpath,__result)	\
    ( (This)->lpVtbl -> CSendArticle(This,wxid,title,abstract,url,imgpath,__result) ) 

#define IWeChatRobot_CSendCard(This,receiver,sharedwxid,nickname,__result)	\
    ( (This)->lpVtbl -> CSendCard(This,receiver,sharedwxid,nickname,__result) ) 

#define IWeChatRobot_CGetFriendList(This,__result)	\
    ( (This)->lpVtbl -> CGetFriendList(This,__result) ) 

#define IWeChatRobot_CGetFriendListString(This,__result)	\
    ( (This)->lpVtbl -> CGetFriendListString(This,__result) ) 

#define IWeChatRobot_CGetWxUserInfo(This,wxid,__result)	\
    ( (This)->lpVtbl -> CGetWxUserInfo(This,wxid,__result) ) 

#define IWeChatRobot_CGetSelfInfo(This,__result)	\
    ( (This)->lpVtbl -> CGetSelfInfo(This,__result) ) 

#define IWeChatRobot_CCheckFriendStatus(This,wxid,__result)	\
    ( (This)->lpVtbl -> CCheckFriendStatus(This,wxid,__result) ) 

#define IWeChatRobot_CGetComWorkPath(This,__result)	\
    ( (This)->lpVtbl -> CGetComWorkPath(This,__result) ) 

#define IWeChatRobot_CStartReceiveMessage(This,port,__result)	\
    ( (This)->lpVtbl -> CStartReceiveMessage(This,port,__result) ) 

#define IWeChatRobot_CStopReceiveMessage(This,__result)	\
    ( (This)->lpVtbl -> CStopReceiveMessage(This,__result) ) 

#define IWeChatRobot_CSendAtText(This,chatroomid,wxid,wxmsg,AutoNickName,__result)	\
    ( (This)->lpVtbl -> CSendAtText(This,chatroomid,wxid,wxmsg,AutoNickName,__result) ) 

#define IWeChatRobot_CGetChatRoomMembers(This,chatroomid,__result)	\
    ( (This)->lpVtbl -> CGetChatRoomMembers(This,chatroomid,__result) ) 

#define IWeChatRobot_CGetDbHandles(This,__result)	\
    ( (This)->lpVtbl -> CGetDbHandles(This,__result) ) 

#define IWeChatRobot_CExecuteSQL(This,DbHandle,sql,__result)	\
    ( (This)->lpVtbl -> CExecuteSQL(This,DbHandle,sql,__result) ) 

#define IWeChatRobot_CBackupSQLiteDB(This,DbHandle,savepath,__result)	\
    ( (This)->lpVtbl -> CBackupSQLiteDB(This,DbHandle,savepath,__result) ) 

#define IWeChatRobot_CVerifyFriendApply(This,v3,v4,__result)	\
    ( (This)->lpVtbl -> CVerifyFriendApply(This,v3,v4,__result) ) 

#define IWeChatRobot_CAddFriendByWxid(This,wxid,message,__result)	\
    ( (This)->lpVtbl -> CAddFriendByWxid(This,wxid,message,__result) ) 

#define IWeChatRobot_CAddFriendByV3(This,v3,message,AddType,__result)	\
    ( (This)->lpVtbl -> CAddFriendByV3(This,v3,message,AddType,__result) ) 

#define IWeChatRobot_CGetWeChatVer(This,__result)	\
    ( (This)->lpVtbl -> CGetWeChatVer(This,__result) ) 

#define IWeChatRobot_CStartWeChat(This,__result)	\
    ( (This)->lpVtbl -> CStartWeChat(This,__result) ) 

#define IWeChatRobot_CSearchContactByNet(This,keyword,__result)	\
    ( (This)->lpVtbl -> CSearchContactByNet(This,keyword,__result) ) 

#define IWeChatRobot_CAddBrandContact(This,PublicId,__result)	\
    ( (This)->lpVtbl -> CAddBrandContact(This,PublicId,__result) ) 

#define IWeChatRobot_CHookVoiceMsg(This,savepath,__result)	\
    ( (This)->lpVtbl -> CHookVoiceMsg(This,savepath,__result) ) 

#define IWeChatRobot_CUnHookVoiceMsg(This,__result)	\
    ( (This)->lpVtbl -> CUnHookVoiceMsg(This,__result) ) 

#define IWeChatRobot_CHookImageMsg(This,savepath,__result)	\
    ( (This)->lpVtbl -> CHookImageMsg(This,savepath,__result) ) 

#define IWeChatRobot_CUnHookImageMsg(This,__result)	\
    ( (This)->lpVtbl -> CUnHookImageMsg(This,__result) ) 

#define IWeChatRobot_CChangeWeChatVer(This,verStr,__result)	\
    ( (This)->lpVtbl -> CChangeWeChatVer(This,verStr,__result) ) 

#define IWeChatRobot_CSendAppMsg(This,wxid,appid,__result)	\
    ( (This)->lpVtbl -> CSendAppMsg(This,wxid,appid,__result) ) 

#define IWeChatRobot_CDeleteUser(This,wxid,__result)	\
    ( (This)->lpVtbl -> CDeleteUser(This,wxid,__result) ) 

#define IWeChatRobot_CIsWxLogin(This,__result)	\
    ( (This)->lpVtbl -> CIsWxLogin(This,__result) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWeChatRobot_INTERFACE_DEFINED__ */


#ifndef __IRobotEvent_INTERFACE_DEFINED__
#define __IRobotEvent_INTERFACE_DEFINED__

/* interface IRobotEvent */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IRobotEvent;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1a9d8d1b-d40e-4f3a-8ed4-b783e6a59830")
    IRobotEvent : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CPostMessage( 
            /* [in] */ VARIANT *msg,
            /* [retval][out] */ int *__result) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IRobotEventVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRobotEvent * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRobotEvent * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRobotEvent * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IRobotEvent * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IRobotEvent * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IRobotEvent * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IRobotEvent * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CPostMessage )( 
            IRobotEvent * This,
            /* [in] */ VARIANT *msg,
            /* [retval][out] */ int *__result);
        
        END_INTERFACE
    } IRobotEventVtbl;

    interface IRobotEvent
    {
        CONST_VTBL struct IRobotEventVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRobotEvent_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IRobotEvent_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IRobotEvent_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IRobotEvent_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IRobotEvent_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IRobotEvent_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IRobotEvent_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IRobotEvent_CPostMessage(This,msg,__result)	\
    ( (This)->lpVtbl -> CPostMessage(This,msg,__result) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IRobotEvent_INTERFACE_DEFINED__ */



#ifndef __WeChatRobotCOMLib_LIBRARY_DEFINED__
#define __WeChatRobotCOMLib_LIBRARY_DEFINED__

/* library WeChatRobotCOMLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_WeChatRobotCOMLib;

EXTERN_C const CLSID CLSID_WeChatRobot;

#ifdef __cplusplus

class DECLSPEC_UUID("4ce7f5e1-2c93-4bfe-86ac-886452e47475")
WeChatRobot;
#endif

#ifndef ___IRobotEventEvents_DISPINTERFACE_DEFINED__
#define ___IRobotEventEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IRobotEventEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__IRobotEventEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("8fd26fd1-9169-4e4c-8a1d-d05093bacce5")
    _IRobotEventEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IRobotEventEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IRobotEventEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IRobotEventEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IRobotEventEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IRobotEventEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IRobotEventEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IRobotEventEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IRobotEventEvents * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } _IRobotEventEventsVtbl;

    interface _IRobotEventEvents
    {
        CONST_VTBL struct _IRobotEventEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IRobotEventEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IRobotEventEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IRobotEventEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IRobotEventEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IRobotEventEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IRobotEventEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IRobotEventEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IRobotEventEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_RobotEvent;

#ifdef __cplusplus

class DECLSPEC_UUID("178001e8-5e64-4cec-8032-1de28801df55")
RobotEvent;
#endif
#endif /* __WeChatRobotCOMLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

unsigned long             __RPC_USER  BSTR_UserSize64(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal64(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal64(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree64(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize64(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal64(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal64(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree64(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


