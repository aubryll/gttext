// XMLDOMDocumentEventsSink.h: Definition of the XMLDOMDocumentEventsSink class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLDOMDOCUMENTEVENTSSINK_H__D6059A97_D1CC_47CA_8B56_ED3869C94C11__INCLUDED_)
#define AFX_XMLDOMDOCUMENTEVENTSSINK_H__D6059A97_D1CC_47CA_8B56_ED3869C94C11__INCLUDED_

#include "resource.h"       // main symbols

#define WM_DATA_AVAILABLE (WM_USER + 5)
#define WM_READYSTATE_CHANGE (WM_USER + 6)

/////////////////////////////////////////////////////////////////////////////
// XMLDOMDocumentEventsSink

extern _ATL_FUNC_INFO OnEventInfo;

class CXMLDOMDocumentEventsSink : 
	public IDispEventSimpleImpl</*nID =*/ 1, CXMLDOMDocumentEventsSink, &__uuidof(MSXML2::XMLDOMDocumentEvents)>,
	public CComObjectRoot,
	public CComCoClass<XMLDOMDocumentEventsSink,&CLSID_XMLDOMDocumentEventsSink>
{
public:
	CXMLDOMDocumentEventsSink() {}
BEGIN_COM_MAP(CXMLDOMDocumentEventsSink)
	//COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

BEGIN_SINK_MAP(CXMLDOMDocumentEventsSink)
   SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(MSXML2::XMLDOMDocumentEvents), /*dispid =*/ 0xc6, ondataavailable, &OnEventInfo)
   SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(MSXML2::XMLDOMDocumentEvents), /*dispid =*/ 0xfffffd9f, onreadystatechange, &OnEventInfo)
END_SINK_MAP()

// XMLDOMDocumentEvents
public:
    void __stdcall ondataavailable ( );
    void __stdcall onreadystatechange ( );

	HWND m_hwndPostWindow;
};

#endif // !defined(AFX_XMLDOMDOCUMENTEVENTSSINK_H__D6059A97_D1CC_47CA_8B56_ED3869C94C11__INCLUDED_)
