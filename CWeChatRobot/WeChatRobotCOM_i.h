

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


#ifndef __WeChatRobot_FWD_DEFINED__
#define __WeChatRobot_FWD_DEFINED__

#ifdef __cplusplus
typedef class WeChatRobot WeChatRobot;
#else
typedef struct WeChatRobot WeChatRobot;
#endif /* __cplusplus */

#endif 	/* __WeChatRobot_FWD_DEFINED__ */


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
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE CStartRobotService( 
            /* [in] */ BSTR workPath,
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE CStopRobotService( 
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE CSendText( 
            /* [in] */ BSTR wxid,
            /* [in] */ BSTR wxmsg,
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE CSendImage( 
            /* [in] */ BSTR wxid,
            /* [in] */ BSTR imagepath,
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE CSendFile( 
            /* [in] */ BSTR wxid,
            /* [in] */ BSTR filepath,
            /* [retval][out] */ int *__result) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE CGetFriendList( 
            /* [retval][out] */ BSTR *__result) = 0;
        
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
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *CStartRobotService )( 
            IWeChatRobot * This,
            /* [in] */ BSTR workPath,
            /* [retval][out] */ int *__result);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *CStopRobotService )( 
            IWeChatRobot * This,
            /* [retval][out] */ int *__result);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *CSendText )( 
            IWeChatRobot * This,
            /* [in] */ BSTR wxid,
            /* [in] */ BSTR wxmsg,
            /* [retval][out] */ int *__result);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *CSendImage )( 
            IWeChatRobot * This,
            /* [in] */ BSTR wxid,
            /* [in] */ BSTR imagepath,
            /* [retval][out] */ int *__result);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *CSendFile )( 
            IWeChatRobot * This,
            /* [in] */ BSTR wxid,
            /* [in] */ BSTR filepath,
            /* [retval][out] */ int *__result);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *CGetFriendList )( 
            IWeChatRobot * This,
            /* [retval][out] */ BSTR *__result);
        
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


#define IWeChatRobot_CStartRobotService(This,workPath,__result)	\
    ( (This)->lpVtbl -> CStartRobotService(This,workPath,__result) ) 

#define IWeChatRobot_CStopRobotService(This,__result)	\
    ( (This)->lpVtbl -> CStopRobotService(This,__result) ) 

#define IWeChatRobot_CSendText(This,wxid,wxmsg,__result)	\
    ( (This)->lpVtbl -> CSendText(This,wxid,wxmsg,__result) ) 

#define IWeChatRobot_CSendImage(This,wxid,imagepath,__result)	\
    ( (This)->lpVtbl -> CSendImage(This,wxid,imagepath,__result) ) 

#define IWeChatRobot_CSendFile(This,wxid,filepath,__result)	\
    ( (This)->lpVtbl -> CSendFile(This,wxid,filepath,__result) ) 

#define IWeChatRobot_CGetFriendList(This,__result)	\
    ( (This)->lpVtbl -> CGetFriendList(This,__result) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWeChatRobot_INTERFACE_DEFINED__ */



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
#endif /* __WeChatRobotCOMLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  BSTR_UserSize64(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal64(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal64(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree64(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


