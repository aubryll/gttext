/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Oct 28 18:03:55 1999
 */
/* Compiler settings for D:\Dev\xmltree\Xmltree.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __Xmltree_i_h__
#define __Xmltree_i_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IXMLDOMDocumentEventsSink_FWD_DEFINED__
#define __IXMLDOMDocumentEventsSink_FWD_DEFINED__
typedef interface IXMLDOMDocumentEventsSink IXMLDOMDocumentEventsSink;
#endif 	/* __IXMLDOMDocumentEventsSink_FWD_DEFINED__ */


#ifndef __XMLDOMDocumentEventsSink_FWD_DEFINED__
#define __XMLDOMDocumentEventsSink_FWD_DEFINED__

#ifdef __cplusplus
typedef class XMLDOMDocumentEventsSink XMLDOMDocumentEventsSink;
#else
typedef struct XMLDOMDocumentEventsSink XMLDOMDocumentEventsSink;
#endif /* __cplusplus */

#endif 	/* __XMLDOMDocumentEventsSink_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IXMLDOMDocumentEventsSink_INTERFACE_DEFINED__
#define __IXMLDOMDocumentEventsSink_INTERFACE_DEFINED__

/* interface IXMLDOMDocumentEventsSink */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IXMLDOMDocumentEventsSink;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B30149CF-22AD-43EA-A412-3AEA62BF25FC")
    IXMLDOMDocumentEventsSink : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IXMLDOMDocumentEventsSinkVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IXMLDOMDocumentEventsSink __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IXMLDOMDocumentEventsSink __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IXMLDOMDocumentEventsSink __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IXMLDOMDocumentEventsSink __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IXMLDOMDocumentEventsSink __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IXMLDOMDocumentEventsSink __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IXMLDOMDocumentEventsSink __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } IXMLDOMDocumentEventsSinkVtbl;

    interface IXMLDOMDocumentEventsSink
    {
        CONST_VTBL struct IXMLDOMDocumentEventsSinkVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXMLDOMDocumentEventsSink_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IXMLDOMDocumentEventsSink_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IXMLDOMDocumentEventsSink_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IXMLDOMDocumentEventsSink_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IXMLDOMDocumentEventsSink_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IXMLDOMDocumentEventsSink_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IXMLDOMDocumentEventsSink_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXMLDOMDocumentEventsSink_INTERFACE_DEFINED__ */



#ifndef __XmltreeLib_LIBRARY_DEFINED__
#define __XmltreeLib_LIBRARY_DEFINED__

/* library XmltreeLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_XmltreeLib;

EXTERN_C const CLSID CLSID_XMLDOMDocumentEventsSink;

#ifdef __cplusplus

class DECLSPEC_UUID("F3F146CE-AE61-44CC-9E02-4B06DA10ABE4")
XMLDOMDocumentEventsSink;
#endif
#endif /* __XmltreeLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
