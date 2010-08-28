// GTXMLView.cpp: implementación de la clase CXMLView
//
#include "stdafx.h"
#include "GT.h"
#include "GTDoc.h"
#include "GTXMLView.h"
#include "GTView.h"
#include "PropViews.h"
#include "MainFrm.h"
#include <direct.h>

#include "XMLDOMDocumentEventsSink.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// cut & paste globals:
const TCHAR CLIPFORMAT_XML_NODE_PTR[] = _T("XMLNodePtr");
UINT g_cfNodePtr;
MSXML2::IXMLDOMNodePtr g_pClipboardNode;

// Define type info structure
_ATL_FUNC_INFO OnEventInfo = {CC_STDCALL, VT_EMPTY, 0};


void __stdcall CXMLDOMDocumentEventsSink::ondataavailable ( )
{
	::PostMessage(m_hwndPostWindow, WM_DATA_AVAILABLE, 0, 0);
}

void __stdcall CXMLDOMDocumentEventsSink::onreadystatechange ( )
{
	::PostMessage(m_hwndPostWindow, WM_READYSTATE_CHANGE, 0, 0);
}



// CXMLView

IMPLEMENT_DYNCREATE(CXMLView, CFormView)

BEGIN_MESSAGE_MAP(CXMLView, CFormView)
	//{{AFX_MSG_MAP(CXMLView)
	ON_WM_QUERYDRAGICON()

	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY(TVN_SELCHANGED, IDC_XMLTREE, &CXMLView::OnTvnSelchangedXmltree)
	ON_COMMAND(ID_CONTEXTGT_NEWTEXTREGION, &CXMLView::OnContextgtNewtextregion)
	ON_COMMAND(ID_CONTEXTGT_NEWTEXTLINE, &CXMLView::OnContextgtNewtextline)
	ON_COMMAND(ID_CONTEXTGT_DELETETEXTREGION, &CXMLView::OnContextgtDelete)
	ON_COMMAND(ID_CONTEXTGT_NEWWORD, &CXMLView::OnContextgtNewword)
	ON_COMMAND(ID_CONTEXTGT_DELETETEXTLINE, &CXMLView::OnContextgtDelete)
	ON_COMMAND(ID_CONTEXTGT_NEWGLYPH, &CXMLView::OnContextgtNewglyph)
	ON_COMMAND(ID_CONTEXTGT_DELETEGLYPH, &CXMLView::OnContextgtDelete)
	ON_COMMAND(ID_CONTEXTGT_DELETEWORD, &CXMLView::OnContextgtDelete)
	ON_COMMAND(ID_CONTEXTGT_CREATEWORDGLYPH, &CXMLView::OnContextgtCreatewordglyph)
END_MESSAGE_MAP()

// CXMLView construction/destruction

CXMLView::CXMLView()
	: CFormView(CXMLView::IDD)
{
	// TODO: add construction code here
	m_bAsync = FALSE;
	m_strURL = _T("");
	m_newstrURL = _T("gt-00006664-20081107T11432018-0815.xml");
	m_strName = _T("");
	m_strValue = _T("");

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bLoading = false;
	m_bPopup = false;
	m_hCurItem = NULL;
	m_pDataSource = NULL;
	m_no_text_regions = 0;
}

CXMLView::~CXMLView()
{
}

void CXMLView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_XMLTREE, m_wndXMLTree);

}

BOOL CXMLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CXMLView::OnInitialUpdate()
{
	CGTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CFrameWnd* pMainWnd = (CFrameWnd*)AfxGetMainWnd();
	CSplitterWnd* pParentSplitter = GetParentSplitter(this,true);
	CMainFrame* pMainFrameWnd = (CMainFrame*) pMainWnd;

	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	m_ImageList.DeleteImageList();
	m_ImageList.Create(IDR_XMLIMAGELIST, 16, 16, 0x00A0A0A0);
	m_wndXMLTree.SetImageList(&m_ImageList, TVSIL_NORMAL);
	m_wndXMLTree.SetIndent(10);

	POSITION pos = pDoc->GetFirstViewPosition();
	CGTView* gtView = (CGTView*) pDoc->GetNextView(pos);
	if(gtView == NULL)
	return ;
	gtView->SetActiveWindow();
	pMainWnd->SetActiveView((CView*)pParentSplitter->GetPane(1,0));
	pDoc->SwitchToView(RUNTIME_CLASS(CBlankView));
	((CBlankView*)pParentSplitter->GetPane(1,0))->OnUpdate();

	g_cfNodePtr = ::RegisterClipboardFormat(CLIPFORMAT_XML_NODE_PTR);

	// Load Accelerators
	HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDR_ACCELERATOR1), RT_ACCELERATOR);
	m_hAccelTable = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	pDoc->SetLock(false);
	if (pDoc->IsLoad())
	{
		if(OnOpenLinkUrl(false) == S_OK)
		{
			if(gtView->OnFileImageOpen() != TRUE)
			{
				DeleteTreeItems(NULL);
				m_wndXMLTree.DeleteAllItems();
				if(m_pDOMDoc != NULL)
					m_pDOMDoc.Release();
				pDoc->SetLock(true);
				pDoc->Release();
			}
		}
		else
		{
			DeleteTreeItems(NULL);
			m_wndXMLTree.DeleteAllItems();
			if(m_pDOMDoc != NULL)
				m_pDOMDoc.Release();
			pDoc->SetLock(true);
			pDoc->Release();
		}
		pMainFrameWnd->HideToolBars();

	}
	else
		pDoc->SetLoad(true);
}

void CXMLView::OnDraw(CDC* pDC)
{
	CGTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (IsIconic())
	{
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) (*pDC).GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		(*pDC).DrawIcon(x, y, m_hIcon);
	}

}

HCURSOR CXMLView::OnQueryDragIcon()	
{
	return (HCURSOR) m_hIcon;
}



LRESULT CXMLView::OnData(WPARAM, LPARAM)
{
	return 0;
}

HRESULT CXMLView::OnOpenPageUrl()
{
USES_CONVERSION;

	ReleaseCurrentDocument();

	
	CGTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return S_FALSE;

	try
	{

		MSXML2::IXMLDOMDocumentPtr pPageDOMDoc(__uuidof(MSXML2::DOMDocument));
				
		pPageDOMDoc->put_async(VARIANT_FALSE);
		_bstr_t bstrURL = pDoc->GetPagePath();
		VARIANT_BOOL varResult = pPageDOMDoc->load(_variant_t(bstrURL));
		
		
		if (VARIANT_FALSE == varResult)
		{
			// Load failed
			MSXML2::IXMLDOMParseErrorPtr pParseError = pPageDOMDoc->GetparseError();
			long dwError = pParseError->GeterrorCode(); 
			_bstr_t bstrReason = pParseError->Getreason();
			CString strError;
			strError.Format(_T("Page XML not found"));
			AfxMessageBox(strError);
			return S_FALSE;
		}

		if (NULL == pPageDOMDoc)
			return S_FALSE;
		else
			pDoc->SetPageDom(pPageDOMDoc);

		MSXML2::IXMLDOMNodeListPtr pChildNodes = pPageDOMDoc->GetchildNodes();
		if (pChildNodes == NULL)
			return S_FALSE;
		pDoc->SetLock(false);
		OnPageRefresh();
		return S_OK;

	}
	catch(_com_error e)
	{
		TRACE(_T("Caught Exception: OnOpenURL"));
		AfxMessageBox(_T("Load Failure"));
		ReportError(e, _T("open of URL"));
		return S_FALSE;
	}
	catch(...)
	{
		TRACE(_T("Caught Exception: OnOpenURL"));
		AfxMessageBox(_T("Load Failure"));
		return S_FALSE;
	}
	return S_FALSE;
}

HRESULT CXMLView::OnOpenGlyphUrl()
{
USES_CONVERSION;

	CGTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return S_FALSE;

	try
	{
		MSXML2::IXMLDOMDocumentPtr pGlyphDOMDoc(__uuidof(MSXML2::DOMDocument));
				
		pGlyphDOMDoc->put_async(VARIANT_FALSE);
		_bstr_t bstrURL = pDoc->GetGlyphPath();
		VARIANT_BOOL varResult = pGlyphDOMDoc->load(_variant_t(bstrURL));
		
		
		if (VARIANT_FALSE == varResult)
		{
			// Load failed
			MSXML2::IXMLDOMParseErrorPtr pParseError = pGlyphDOMDoc->GetparseError();
			long dwError = pParseError->GeterrorCode(); 
			_bstr_t bstrReason = pParseError->Getreason();
			CString strError;
			strError.Format(_T("Glyph XML not found"));
			AfxMessageBox(strError);
			return S_FALSE;
		}

		if (NULL == pGlyphDOMDoc)
			return S_FALSE;
		else
			pDoc->SetGlyphDom(pGlyphDOMDoc);

		MSXML2::IXMLDOMNodeListPtr pChildNodes = pGlyphDOMDoc->GetchildNodes();
		if (pChildNodes == NULL)
			return S_FALSE;
		pDoc->SetLock(false);
		ParseGlyphXMLTree(pGlyphDOMDoc);
		return S_OK;
		
	}
	catch(_com_error e)
	{
		TRACE(_T("Caught Exception: OnOpenURL"));
		AfxMessageBox(_T("Load Failure"));
		ReportError(e, _T("open of URL"));
		return S_FALSE;

	}
	catch(...)
	{
		TRACE(_T("Caught Exception: OnOpenURL"));
		AfxMessageBox(_T("Load Failure"));
		return S_FALSE;
	}
	return S_OK;
}

CGTDoc* CXMLView::GetDocument() const // La versión de no depuración es en línea
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGTDoc)));
	return (CGTDoc*)m_pDocument;
}

HRESULT CXMLView::OnOpenLinkUrl(bool browse) 
{
	
	CGTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return S_FALSE;
	
	pDoc->Release();
	if (browse)
	{
		 OnBrowseforfile();
		 pDoc->SetLinkPath(m_strURL);
	}
	else
	{
		m_strURL = pDoc->GetLinkPath();
	}
	
USES_CONVERSION;

	
	try
	{
		MSXML2::IXMLDOMDocumentPtr pLinkDOMDoc(__uuidof(MSXML2::DOMDocument));
				
		pLinkDOMDoc->put_async(VARIANT_FALSE);
		_bstr_t bstrURL = m_strURL;
		VARIANT_BOOL varResult = pLinkDOMDoc->load(_variant_t(bstrURL));
		
		
		if (VARIANT_FALSE == varResult && !m_strURL.IsEmpty())
		{
		
			// Load failed
			MSXML2::IXMLDOMParseErrorPtr pParseError = pLinkDOMDoc->GetparseError();
			long dwError = pParseError->GeterrorCode(); 
			_bstr_t bstrReason = pParseError->Getreason();
			CString strError;
			strError.Format(_T("XML Parse Error: %s"), W2T(bstrReason));

			AfxMessageBox(strError);
			return S_FALSE;
		}	

		if (NULL == pLinkDOMDoc || m_strURL.IsEmpty())
			return S_FALSE;
		else
			pDoc->SetLinkDom(pLinkDOMDoc);

		MSXML2::IXMLDOMNodeListPtr pChildNodes = pLinkDOMDoc->GetchildNodes();
		if (pChildNodes == NULL)
			return S_FALSE;
		
		ParseLinkXML(pLinkDOMDoc);
		
		if(pDoc->UpdateSection(NULL,gt) != S_OK)
		{
			AfxMessageBox(_T("Invalid gt file"));
			pDoc->Release();
			pDoc->UpdateAllViews(this);
			return S_FALSE;
		}

		HRESULT RES = OnOpenPageUrl();
		
		if(RES != S_OK)
		{
			pDoc->Release();
			pDoc->UpdateAllViews(this);
		}
		else
			RES = OnOpenGlyphUrl();

		MSXML2::IXMLDOMNodePtr pNodeP,pNodeC;
		
		
		pDoc->UpdateSection(NULL,pcMetadata);
		pNodeP = pDoc->GetNodeFromCurrSection(pcCreated);

		if (pNodeP != NULL)
		{
			pNodeC = pNodeP->GetchildNodes()->nextNode();
			if (pNodeC != NULL){
				if(CString("time").Compare(W2T(_bstr_t(pNodeC->GetnodeValue()))) == 0)
					pNodeC->put_text(BSTR(GetDate().GetString()));
			}
		}

		pNodeP = pDoc->GetNodeFromCurrSection(glyph_pcCreated);

		if (pNodeP != NULL)
		{
			pNodeC = pNodeP->GetchildNodes()->nextNode();
			if (pNodeC != NULL){
				if(CString("time").Compare(W2T(_bstr_t(pNodeC->GetnodeValue()))) == 0)
					pNodeC->put_text(BSTR(GetDate().GetString()));
			}
		}
		
		return RES;
		
	}		
	
	catch(_com_error e)
	{
		TRACE(_T("Caught Exception: OnOpenURL"));
		AfxMessageBox(_T("Load Failure"));
		ReportError(e, _T("open of URL"));
	}
	catch(...)
	{
		TRACE(_T("Caught Exception: OnOpenURL"));
		AfxMessageBox(_T("Load Failure"));
	}
	return S_FALSE;
}

CString CXMLView::GetDate()
{
	CString value = "";
	CString strTime = "";
	SYSTEMTIME time;
	GetSystemTime(&time);
	WORD data = time.wYear;
	while(data != 0) 
	{
		value =  wchar_t(data%10+48)+ value;
		data = data / 10;
	}
	strTime = value + "-";
	value = "";
	data = time.wMonth;
	while(data != 0) 
	{
		value =  wchar_t(data%10+48)+ value;
		data = data / 10;
	}
	strTime = strTime + value + "-";
	value = "";
	data = time.wDay;
	while(data != 0) 
	{
		value =  wchar_t(data%10+48)+ value;
		data = data / 10;
	}
	strTime = strTime + value + "T";
	value = "";
	data = time.wHour;
	
	while(data != 0) 
	{
		value =  wchar_t(data%10+48)+ value;
		data = data / 10;
	}
	if(value.GetLength() == 0)
		value = "00";
	else if(value.GetLength() == 1)
		value = _T("0") + value;
	strTime = strTime + value + ":";
	
	value = "";
	data = time.wMinute;
	
	while(data != 0) 
	{
		value =  wchar_t(data%10+48)+ value;
		data = data / 10;
	}
	if(value.GetLength() == 0)
		value = "00";
	else if(value.GetLength() == 1)
		value = _T("0") + value;

	strTime = strTime + value + ":";
	
	value = "";
	data = time.wSecond;
	while(data != 0) 
	{
		value =  wchar_t(data%10+48)+ value;
		data = data / 10;
	}
	if(value.GetLength() == 0)
		value = "00";
	else if(value.GetLength() == 1)
		value = _T("0") + value;
	
		strTime = strTime + value;

	return strTime;
}

void CXMLView::OnUpdate(CView*, LPARAM, CObject*)
{
	CGTDoc* pDoc = GetDocument();
	SectionPtrs* sec;
	MSXML2::IXMLDOMNodePtr pNode;
	HTREEITEM hItem;
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	sec = pDoc->GetCurrentSection();
	SetSelectedPoint();
	//The lock prevents the recreation all the sections (made yet on initupdate)
	pDoc->SetLock(true);
	OnPageRefresh();
	pDoc->SetLock(false);
	
	if(!pDoc->IsLoad())
	{
		pDoc->SetCurrentSection(sec);
		if(sec!=NULL)
		{
			pNode = pDoc->GetNodeFromCurrSection(sec->type);
			SetSelectedNode();
			hItem = m_wndXMLTree.GetRootItem();
			hItem = FindItemOnTree(hItem,pNode);
			if (hItem != NULL)
			{
				CPoint pointInTree;
				m_wndXMLTree.SelectItem(hItem);
				pointInTree = pDoc->GetSelectedPoint();
				if(pointInTree.x != 0 || pointInTree.y != 0)
				{
					m_wndXMLTree.SetScrollPos(1,pointInTree.y);
					m_wndXMLTree.SetScrollPos(0,pointInTree.x);
				}
			}
		}	
	}
}

HTREEITEM CXMLView::FindItemOnTree(const HTREEITEM hItem,MSXML2::IXMLDOMNodePtr pNode)
{
	if(hItem == NULL)
		return NULL;
	if(pNode == NULL)
		return NULL;
	LPARAM itemdataAttr,itemdata = m_wndXMLTree.GetItemData(hItem);
	MSXML2::IXMLDOMNodePtr pNodeToComp = (MSXML2::IXMLDOMNodePtr)(LPUNKNOWN)itemdata;
	HTREEITEM hItemFound = NULL;
	if(pNode->GetnodeType() == MSXML2::NODE_TEXT)
	{
		CString nodeName = "pcGts",meta = "pcMetadata";

		hItemFound = hItem;

		if(nodeName.Compare(W2T(_bstr_t(pNodeToComp->GetnodeName()))) == 0)
		{
			pNodeToComp = pNodeToComp->GetchildNodes()->nextNode();
			hItemFound = m_wndXMLTree.GetNextVisibleItem(hItem);
		}
		while(meta.Compare(W2T(_bstr_t(pNodeToComp->GetnodeName()))) != 0)
		{
			hItemFound = m_wndXMLTree.GetParentItem(hItemFound);
			itemdata = m_wndXMLTree.GetItemData(hItemFound);
			pNodeToComp = (MSXML2::IXMLDOMNodePtr)(LPUNKNOWN)itemdata;
		}

		MSXML2::IXMLDOMNodeListPtr childNodes = pNodeToComp->GetchildNodes();

		pNodeToComp = childNodes->nextNode();
		pNode = pNode->GetparentNode();
		hItemFound = m_wndXMLTree.GetNextVisibleItem(hItemFound);
		if(pNodeToComp->GetnodeName() != pNode->GetnodeName())
		{
			hItemFound = m_wndXMLTree.GetNextVisibleItem(hItemFound);
			hItemFound = m_wndXMLTree.GetNextVisibleItem(hItemFound);
			pNodeToComp = childNodes->nextNode();
		}
		hItemFound = m_wndXMLTree.GetNextVisibleItem(hItemFound);
		

		
		return hItemFound;
		
	}
	else if(pNode->GetnodeType() != MSXML2::NODE_ATTRIBUTE)
	{
		if(pNodeToComp->GetnodeName() == pNode->GetnodeName())
		{
			CString strFile1,strFile2;
			if (pNodeToComp->Getattributes()->nextNode() != NULL) 
				strFile1 = W2T(_bstr_t(pNodeToComp->Getattributes()->nextNode()->GetnodeValue()));
			if(pNode->Getattributes()->nextNode() != NULL)
				strFile2 = W2T(_bstr_t(pNode->Getattributes()->nextNode()->GetnodeValue()));
			if(strFile1.Compare(strFile2) == 0)
			{
				hItemFound = hItem;
				return hItemFound;
			}
		}
	}
	else
	{
		MSXML2::IXMLDOMAttributePtr pNodeA;
		hItemFound = hItem;
		if(pNodeToComp == pNode)
			return hItemFound;
		if(hItemFound != NULL)
		{
			while((hItemFound = m_wndXMLTree.GetNextVisibleItem(hItemFound)) != NULL)
			{		
				itemdataAttr = m_wndXMLTree.GetItemData(hItemFound);
				pNodeA = (MSXML2::IXMLDOMNodePtr)(LPUNKNOWN)itemdataAttr;
				if(pNodeA == pNode)
					return hItemFound;
				
			}
		}

	}
	if ( m_wndXMLTree.ItemHasChildren(hItem))
	{
	   HTREEITEM hNextItem;
	   HTREEITEM hChildItem = m_wndXMLTree.GetChildItem(hItem);

	   while (hChildItem != NULL)
	   {
		  hNextItem = m_wndXMLTree.GetNextItem(hChildItem, TVGN_NEXT);
		  hItemFound = FindItemOnTree(hChildItem,pNode);
		  if(hItemFound != NULL)
			  break;
		  hChildItem = hNextItem;
	   }
	}
	else
		return NULL;
	   
	return hItemFound;
}

void CXMLView::OnBrowseforfile() 
{
	CString strExt((LPCSTR)IDS_DEFAULTXMLEXTENSION);
	CString strTitle((LPCSTR)IDS_OPENDIALOGTITLE);
	CString strFilter((LPCSTR)IDS_DEFAULTXMLFILTERS);

	UpdateData();

	CFileDialog fileDialog(TRUE, strExt, NULL, OFN_ENABLESIZING | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
							strFilter, this);
	fileDialog.m_ofn.lpstrTitle = strTitle;

	INT_PTR ret = fileDialog.DoModal();
	if (IDOK == ret)
	{
		CString strPathName = fileDialog.GetPathName();
		if (!strPathName.IsEmpty())
		{
			m_strURL = strPathName;
			UpdateData(FALSE);
		}
	}
	else if(ret == 2)
		m_strURL = _T("");
}

void CXMLView::OnPageRefresh() 
{
	CGTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CPoint pointInTree;
		
	m_pDOMDoc = pDoc->GetPageDom();

	if (NULL == m_pDOMDoc)
		return;

	DeleteTreeItems(NULL);
	// Need to populate with children of DOMDoc to get PIs
	MSXML2::IXMLDOMNodeListPtr pChildNodes = m_pDOMDoc->GetchildNodes();
	if (pChildNodes == NULL)
		return;

	// Need PIs to be children of main doc, right?
	m_no_text_regions = 0;
	FillPageXMLTree(m_pDOMDoc, TVI_ROOT);
}

void CXMLView::OnSaveXMLToFile() 
{
	if (NULL == m_pDOMDoc)
		return;

	CString strExt((LPCSTR)IDS_DEFAULTXMLEXTENSION);
	CString strTitle((LPCSTR)IDS_SAVEDIALOGTITLE);
	CString strFilter((LPCSTR)IDS_DEFAULTXMLFILTERS);

	CFileDialog fileDialog(FALSE, strExt, NULL, OFN_ENABLESIZING | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
							strFilter, this);
	fileDialog.m_ofn.lpstrTitle = strTitle;

	INT_PTR ret = fileDialog.DoModal();
	if (IDOK == ret)
	{
		CString strPathName = fileDialog.GetPathName();
		if (!strPathName.IsEmpty())
		{
			_bstr_t bstrPathName = strPathName;
			HRESULT hr = m_pDOMDoc->save((_variant_t)bstrPathName);
			if (FAILED(hr))
			{
				// Get Reason
				TRACE(_T("Save failed for: %s\n"), strPathName);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Context menu & accelerator handlers

void CXMLView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CGTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pWnd == &m_wndXMLTree)
	{
		CPoint pointInTree = point;
		pWnd->ScreenToClient(&pointInTree);
		// Get and store current tree item
		m_hCurItem = m_wndXMLTree.HitTest(pointInTree, NULL);
		TRACE(_T("Context Menu on item: %s\n"), m_wndXMLTree.GetItemText(m_hCurItem));
		// Set selection to item that was right-clicked
		m_wndXMLTree.SelectItem(m_hCurItem);

				// Display context menu
		CMenu menuTreePopupMaster;
		if(pDoc->GetCurrentSection() != NULL)
		{
			NodeName sectionName = pDoc->GetCurrentSection()->type;

			switch(sectionName)
			{
				case page: menuTreePopupMaster.LoadMenu(IDR_MENUTR);
					break;
				case pcMetadata: return;
					break;
				case text_region: menuTreePopupMaster.LoadMenu(IDR_MENUTL);
					break;
				case text_line: menuTreePopupMaster.LoadMenu(IDR_MENUW);
					break;
				case word: menuTreePopupMaster.LoadMenu(IDR_MENUG);
					break;
				case glyph:  menuTreePopupMaster.LoadMenu(IDR_MENUGD);
					break;

				default: return;
					break;
			};
		
			CMenu* pMenuTreePopup = menuTreePopupMaster.GetSubMenu(0);
			pMenuTreePopup->TrackPopupMenu(TPM_RIGHTBUTTON | TPM_CENTERALIGN, point.x, point.y, this, NULL);
		}
	}
}

void CXMLView::SetSelectedPoint()
{
	CGTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CPoint point;
	point.y = m_wndXMLTree.GetScrollPos(1);
	point.x = m_wndXMLTree.GetScrollPos(0);
	pDoc->SetSelectedPoint(point);

}

void CXMLView::OnTreepopupDelete() 
{
	_bstr_t bstrName;
	_bstr_t bstrValue;
	MSXML2::IXMLDOMNodePtr pParentNode;
	MSXML2::IXMLDOMNamedNodeMapPtr pAttrs;
	_bstr_t bstrNameRemove;
	CGTDoc* pDoc = GetDocument();
	HTREEITEM parentItem;
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (!SetSelectedNode())
		return;


	try
	{
		MSXML2::DOMNodeType nodeType;
		m_pCurNode->get_nodeType(&nodeType);

		UpdateData();

		if(m_hCurItem != NULL)
			parentItem = m_wndXMLTree.GetParentItem(m_hCurItem);

		if (MSXML2::NODE_DOCUMENT == nodeType ||
			MSXML2::NODE_DOCUMENT_FRAGMENT == nodeType)
			return; // IGNORE: Can't delete these items
		else if (MSXML2::NODE_ATTRIBUTE == nodeType)
		{
			pParentNode = GetParentNodeFromTree();
			pAttrs = pParentNode->Getattributes();

			bstrNameRemove = m_pCurNode->GetnodeName();
			pAttrs->removeNamedItem(bstrNameRemove);
		}
		else
		{
			pParentNode = m_pCurNode->GetparentNode();
			pParentNode->removeChild(m_pCurNode);
			m_wndXMLTree.DeleteItem(m_hCurItem);
		}

		m_hCurItem = parentItem;
		m_pCurNode = NULL;

		m_wndXMLTree.SelectItem(m_hCurItem);


	}
	catch(_com_error e)
	{
		ReportError(e, _T("delete"));
	}
	catch(...)
	{
	
	}
}

//Seldom used
void CXMLView::OnTreepopupCopy() 
{
USES_CONVERSION;

	if (!SetSelectedNode())
		return;

	m_pDataSource = new COleDataSource();

	// Add XML Text as UNICODE
	_bstr_t bstrXMLText = m_pCurNode->Getxml();
	long nBytes = (bstrXMLText.length() + 1 ) * sizeof(WCHAR);
	HGLOBAL hGlobalXMLText = GlobalAlloc(0, nBytes);
	LPVOID pDataBuff = GlobalLock(hGlobalXMLText);
	memcpy(pDataBuff, (WCHAR*)bstrXMLText, nBytes);
	GlobalUnlock(hGlobalXMLText);
	STGMEDIUM* pstgmedXMLText = new STGMEDIUM;
	memset(pstgmedXMLText, 0, sizeof(STGMEDIUM));
	pstgmedXMLText->tymed = TYMED_HGLOBAL;
	pstgmedXMLText->hGlobal = hGlobalXMLText;
	m_pDataSource->CacheData(CF_UNICODETEXT, pstgmedXMLText);

	// Add ANSI text
	nBytes = bstrXMLText.length() + 1;
	CHAR* strXMLText = W2A(bstrXMLText);
	hGlobalXMLText = GlobalAlloc(0, nBytes);
	pDataBuff = GlobalLock(hGlobalXMLText);
	memcpy(pDataBuff, strXMLText, nBytes);
	GlobalUnlock(hGlobalXMLText);
	STGMEDIUM* pstgmedXMLTextAnsi = new STGMEDIUM;
	memset(pstgmedXMLTextAnsi, 0, sizeof(STGMEDIUM));
	pstgmedXMLTextAnsi->tymed = TYMED_HGLOBAL;
	pstgmedXMLTextAnsi->hGlobal = hGlobalXMLText;
	m_pDataSource->CacheData(CF_TEXT, pstgmedXMLTextAnsi);

	// Add XML Node pointer
	// We put a dummy clipformat on the clipboard that is only used by our app
	// We don't want to pass XML pointers cross-process so we just keep a global var
	// for our application and use this dummy clipformat to give true clipboard behavior
	// (ex: user switches to word and does a CTRL-C, this should bump out the XML node)
	g_pClipboardNode = m_pCurNode->cloneNode(VARIANT_TRUE);
	STGMEDIUM* pstgmedXMLNodePtr = new STGMEDIUM; 
	memset(pstgmedXMLNodePtr, 0, sizeof(STGMEDIUM));
	pstgmedXMLNodePtr->tymed = TYMED_HGLOBAL;
	pstgmedXMLNodePtr->hGlobal = NULL;
	m_pDataSource->CacheData(g_cfNodePtr, pstgmedXMLNodePtr);

	m_pDataSource->SetClipboard();
}

//Seldom used
void CXMLView::OnTreepopupCut() 
{
	if (!SetSelectedNode())
		return;

	OnTreepopupCopy();
	OnTreepopupDelete();
}

//Seldom used
void CXMLView::OnTreepopupPaste() 
{
	if (!SetSelectedNode())
		return;

	COleDataObject clipdata;
	if (!clipdata.AttachClipboard())
		return;

	if(clipdata.IsDataAvailable(g_cfNodePtr))
	{
		MSXML2::IXMLDOMNodePtr pNode = g_pClipboardNode->cloneNode(VARIANT_TRUE);

		try
		{
			MSXML2::DOMNodeType nodeType = pNode->GetnodeType();
			if (MSXML2::NODE_ATTRIBUTE == nodeType)
			{
				MSXML2::IXMLDOMNamedNodeMapPtr pAttrList = m_pCurNode->Getattributes();
				if (NULL == pAttrList)
					return;

				pAttrList->setNamedItem(pNode);
			}
			else
			{
				m_pCurNode->appendChild(pNode);
			}			
			// Remember, this node may have children!
			HTREEITEM hParentItem = m_wndXMLTree.GetParentItem(m_hCurItem);
			DeleteTreeItems(m_hCurItem);
			FillPageXMLTree(m_pCurNode, hParentItem);
		}
		catch(_com_error e)
		{
			ReportError(e, _T("paste"));
		}
		catch(...)
		{
		
		}
		// REVIEW: Release pNode manually to counteract addref before storage?
	}

	// TODO:
	// Interestingly enough, we could probably easily support pasting XML text
	// into the tree by creating a new xmldom doc, parsing the nodes and then copying
	// the nodes over -- we'll leave this as an exercise for the future
}


/////////////////////////////////////////////////////////////////////////////
// CXMLView helper functions for tree and XML

// Special CASE: NULL == delete all items
void CXMLView::DeleteTreeItems(HTREEITEM hItem)
{
	HTREEITEM hChild;
	MSXML2::IXMLDOMNodePtr pNode;

	if (NULL != hItem)
	{

		// Recursively delete all of this item's children
		while(NULL != (hChild = m_wndXMLTree.GetChildItem(hItem)))
		{
			DeleteTreeItems(hChild);
		}
		
		// *** Here is where actually free the pointer
		LPARAM itemdata = m_wndXMLTree.GetItemData(hItem);
		if (NULL != itemdata)
		{
			LPUNKNOWN pUnknown = (LPUNKNOWN)itemdata;
			pUnknown->Release();
		}

		m_wndXMLTree.DeleteItem(hItem);

	}
	else // delete all items
	{
		// Recursively delete all of this item's children
		if((HWND) m_wndXMLTree != NULL) 
		while(NULL != (hChild = m_wndXMLTree.GetChildItem(TVI_ROOT)))
		{
			DeleteTreeItems(hChild);
		}
	}
	
	m_hCurItem = NULL;
}

HRESULT CXMLView::ParseLinkXML(MSXML2::IXMLDOMNodePtr pNode, bool bRecurse)
{
USES_CONVERSION;

	CString strTextToComp,strFile,pathdir,strid;
	MSXML2::IXMLDOMNamedNodeMapPtr pAttrMap;
	MSXML2::IXMLDOMAttributePtr pAttribute;
	MSXML2::IXMLDOMNodePtr pChild;
	MSXML2::IXMLDOMNodeListPtr pNodeList;

	CGTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return S_FALSE;

	if (NULL == pNode)
		return S_FALSE;

	try
	{
		if(pNode->GetnodeType() == MSXML2::NODE_ELEMENT)
		{
			strTextToComp = W2T(pNode->GetnodeName());
			if(strTextToComp.Compare(_T("gt")) == 0)
			{
				pDoc->AddSection(pNode,gt);
				pAttrMap = pNode->Getattributes();
				if (pAttrMap != NULL)
				{
					while(pAttribute = pAttrMap->nextNode())
					{
						strid =  W2T(_bstr_t(pAttribute->GetnodeValue()));
						if(strid.Find(_T("gt-")) != -1)

							pDoc->AddNodeToCurrSection(pAttribute,gtID);
					}
				}
			}
			else if(strTextToComp.Compare(_T("gts")) == 0)
			{
				pAttrMap = pNode->Getattributes();
				if (pAttrMap != NULL)
				{
					if(pAttribute = pAttrMap->nextNode())
					{
						strid =  W2T(_bstr_t(pAttribute->GetnodeValue()));
						if(strid.Find(_T("pc")) != -1)
						{
							pDoc->AddNodeToCurrSection(pAttribute,pcID);
							 m_lrFolder = "pcFolder";
						}
						else if(strid.Find(_T("gl")) != -1)
						{
							pDoc->AddNodeToCurrSection(pAttribute,glID);
							 m_lrFolder = "glFolder";
						}
					}
				}
			}
			else if(strTextToComp.Compare(_T("dependency")) == 0)
			{
				pAttrMap = pNode->Getattributes();
				if (pAttrMap != NULL)
				{
					if(pAttribute = pAttrMap->nextNode())
					{
						strid =  W2T(_bstr_t(pAttribute->GetnodeValue()));
						if(strid.Find(_T("gt")) != -1)
							pDoc->AddNodeToCurrSection(pAttribute,pcDep);
						if(strid.Find(_T("pc")) != -1)
							pDoc->AddNodeToCurrSection(pAttribute,glDep);
					}
				}
			}
			else if(strTextToComp.Compare(_T("lrFolder")) == 0)
			{
			 // m_lrFolder = "";
			  pNodeList = pNode->GetchildNodes();
				if (pNodeList != NULL)
				{
					if((pChild = pNodeList->nextNode()) == NULL)
					{
						pChild = pDoc->GetLinkDom()->createTextNode(_bstr_t());
						pNode->appendChild(pChild);
					}

					strFile = m_lrFolder;
					m_lrFolder = W2T(_bstr_t(pChild->GetnodeValue()));
					
					if(strFile.Find(_T("pcFolder")) != -1)
					{
						pathdir = pDoc->GetPagePath();
						strFile = pathdir.Right(pathdir.GetLength()-pathdir.ReverseFind(wchar_t('\\'))+ 1);
						pathdir = pathdir.Left(pathdir.ReverseFind(wchar_t('\\'))+ 1);
						pDoc->SetPagePath(pathdir+m_lrFolder+strFile);
						pDoc->AddNodeToCurrSection(pNode,pc_lrFolder);
					}
					else if(strFile.Find(_T("glFolder")) != -1)
					{
						pathdir = pDoc->GetGlyphPath();
						strFile = pathdir.Right(pathdir.GetLength()-pathdir.ReverseFind(wchar_t('\\'))+ 1);
						pathdir = pathdir.Left(pathdir.ReverseFind(wchar_t('\\'))+ 1);
						pDoc->SetGlyphPath(pathdir+m_lrFolder+strFile);
						pDoc->AddNodeToCurrSection(pNode,gl_lrFolder);
					}
				
				}
			}
			else if(strTextToComp.Compare(_T("lrFileName")) == 0)
			{
			  pNodeList = pNode->GetchildNodes();
				if (pNodeList != NULL)
				{
					if(pChild = pNodeList->nextNode())
					{
						strFile = W2T(_bstr_t(pChild->GetnodeValue()));
						pathdir = pDoc->GetLinkPath();
						pathdir = pathdir.Left(pathdir.ReverseFind(wchar_t('\\'))+ 1);

						if(strFile.Find(_T("pc")) != -1)
						{
							if(m_lrFolder.Find(_T("pcFolder")) != -1)
								pDoc->SetPagePath(pathdir+strFile);
							else
								pDoc->SetPagePath(pathdir+m_lrFolder+strFile);
							pDoc->AddNodeToCurrSection(pNode,pc);
							m_lrFolder = "pcFolder";
						}
						if(strFile.Find(_T("gl")) != -1)
						{
							if(m_lrFolder.Find(_T("glFolder")) != -1)
								pDoc->SetGlyphPath(pathdir+strFile);
							else
								pDoc->SetGlyphPath(pathdir+m_lrFolder+strFile);

							pDoc->AddNodeToCurrSection(pNode,gl);
							m_lrFolder = "glFolder";
						}
					}
				}
			}
		}	

		// Recurse Children
		if (bRecurse)
		{
			pNodeList = pNode->GetchildNodes();
			if (pNodeList != NULL)
			{
				while(pChild = pNodeList->nextNode())
				{
					ParseLinkXML(pChild);
				}
			}
		}
	}
	catch(_com_error e)
	{
		TRACE(_T("Caught Exception: ParseLinkXML"));
		ReportError(e, _T("tree scan"));
	}
	catch(...)
	{
		TRACE(_T("Caught Exception: ParseLinkXML"));
		AfxMessageBox(_T("Error parsing tree!"));
	}

	return S_OK;
}

HRESULT CXMLView::ParseGlyphXMLTree(MSXML2::IXMLDOMNodePtr pNode, bool bRecurse)
{	
	CGTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return S_FALSE;

	if (NULL == pNode)
		return S_FALSE;

	try
	{
		bool process_att = false;
		CString strTextToComp;
		MSXML2::DOMNodeType nodeType;
		pNode->get_nodeType(&nodeType);

		switch (nodeType)
		{
		case MSXML2::NODE_PROCESSING_INSTRUCTION: return S_FALSE;
			break;
		case MSXML2::NODE_ELEMENT:
			
			strTextToComp = W2T(pNode->GetnodeName());
			if(strTextToComp.Compare(_T("pcGts")) == 0)
			{
				pDoc->UpdateSection(NULL,page);
				pDoc->AddNodeToCurrSection(pNode,glyph_pcGts);
				process_att = false;
			}
			else if(strTextToComp.Compare(_T("pcMetadata")) == 0)
			{
				process_att = false;
				pDoc->UpdateSection(pNode,pcMetadata);
			}
			else if(strTextToComp.Compare(_T("pcCreated")) == 0)
			{
				process_att = false;
				pDoc->AddNodeToCurrSection(pNode,glyph_pcCreated);
			}
			else if(strTextToComp.Compare(_T("pcLastChange")) == 0)
			{
				process_att = false;
				pDoc->AddNodeToCurrSection(pNode,glyph_pcLastChange);
			}
			else if(strTextToComp.Compare(_T("pcCreator")) == 0)
			{
				process_att = false;
				pDoc->AddNodeToCurrSection(pNode,glyph_pcCreator);
			}
			else if(strTextToComp.Compare(_T("pcComments")) == 0)
			{
				process_att = false;
				pDoc->AddNodeToCurrSection(pNode,glyph_pcComments);
			}
	
			else if(strTextToComp.Compare(_T("glyph")) == 0)
			{
				process_att = false;
				pDoc->UpdateSection(pNode);
				pDoc->AddNodeToCurrSection(pNode,glyph_glyph);
			}
			else if(strTextToComp.Compare(_T("coordGroup")) == 0)
			{
				process_att = true;
			}
			else
				return S_FALSE;

			break;
		};
		
		if (process_att)
		{
			MSXML2::IXMLDOMNamedNodeMapPtr pAttrMap = pNode->Getattributes();
			if (pAttrMap != NULL)
			{
				__int8 group = 0;
				MSXML2::IXMLDOMAttributePtr pAttribute;
				while(pAttribute = pAttrMap->nextNode())
				{
					
					strTextToComp.Empty();
					strTextToComp = W2T(pAttribute->GetnodeName());
					if(strTextToComp.Compare(_T("core")) == 0)
					{
						strTextToComp = W2T(_bstr_t(pAttribute->Getvalue()));
						if(strTextToComp.Compare(_T("0")) != 0)
							group =  group | 0x01;
					}
					else if(strTextToComp.Compare(_T("outline")) == 0)
					{
						strTextToComp = W2T(_bstr_t(pAttribute->GetnodeValue()));
						if(strTextToComp.Compare(_T("0")) != 0)
							group = group | 0x02;
					}
					else if(strTextToComp.Compare(_T("shade")) == 0)
					{
						strTextToComp = W2T(_bstr_t(pAttribute->GetnodeValue()));
						if(strTextToComp.Compare(_T("0")) != 0)
							group = group | 0x04;
					}
					switch(group)
					{
						case 1:pDoc->AddNodeToCurrSection(pNode,glyph_coordGroup1);
						break;
						case 2:pDoc->AddNodeToCurrSection(pNode,glyph_coordGroup2);
						break;
						case 3:pDoc->AddNodeToCurrSection(pNode,glyph_coordGroup3);
						break;
						case 4:pDoc->AddNodeToCurrSection(pNode,glyph_coordGroup4);
						break;
						case 5:pDoc->AddNodeToCurrSection(pNode,glyph_coordGroup5);
						break;
						case 6:pDoc->AddNodeToCurrSection(pNode,glyph_coordGroup6);
						break;
						case 7:pDoc->AddNodeToCurrSection(pNode,glyph_coordGroup7);
						break;
						default:break;
					}
				}
			}
		}
		// Recurse Children
		if (bRecurse)
		{
			MSXML2::IXMLDOMNodeListPtr pNodeList = pNode->GetchildNodes();
			if (pNodeList != NULL)
			{
				SectionPtrs *nodeSection = pDoc->GetCurrentSection();
				MSXML2::IXMLDOMNodePtr pChild;

				while(pChild = pNodeList->nextNode())
				{
					nodeSection = pDoc->GetCurrentSection();
					ParseGlyphXMLTree(pChild);
					pDoc->SetCurrentSection(nodeSection);					
				}
			}
		}
	
	}
	catch(_com_error e)
	{
		TRACE(_T("Caught Exception: FillXMLTree"));
		ReportError(e, _T("tree scan"));
	}
	catch(...)
	{
		TRACE(_T("Caught Exception: FillXMLTree"));
		AfxMessageBox(_T("Error filling tree!"));
	}

	return S_OK;

}

HRESULT CXMLView::FillPageXMLTree(MSXML2::IXMLDOMNodePtr pNode, HTREEITEM hParentItem, bool bRecurse)
{
USES_CONVERSION;
	
	CGTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return S_FALSE;

	if (NULL == pNode)
		return S_FALSE;

	try
	{
		bool show_att = false;
		bool show_node = true;
		bool process_att = false;
		CString strTextToComp,strFile,pathdir,strTextToAdd;
		MSXML2::IXMLDOMNodePtr pNodeParent,pNextNode;
		MSXML2::DOMNodeType nodeType;
		pNode->get_nodeType(&nodeType);

		switch (nodeType)
		{
		case MSXML2::NODE_DOCUMENT:
			show_att = false;
			break;
		case MSXML2::NODE_PROCESSING_INSTRUCTION: return S_FALSE;
			break;
		case MSXML2::NODE_ELEMENT:
			
			strTextToComp = W2T(pNode->GetnodeName());
			if(strTextToComp.Compare(_T("pcGts")) == 0)
			{
				show_att = false;
				pDoc->AddSection(pNode,pcGts);
			}
			else if(strTextToComp.Compare(_T("pcMetadata")) == 0)
			{
				show_att = true;
				pDoc->AddSection(pNode,pcMetadata);
				pDoc->AddNodeToCurrSection(pNode,pcMetadata);
			}
			else if(strTextToComp.Compare(_T("page_summary")) == 0)
			{
				show_node = false;
				show_att = false;
				bRecurse = false;
			}
			else if(strTextToComp.Compare(_T("pcCreator")) == 0)
			{
				show_att = true;
				pDoc->AddNodeToCurrSection(pNode,pcCreator);
			}
			else if(strTextToComp.Compare(_T("pcCreated")) == 0)
			{
				show_node = false;
				show_att = false;
				bRecurse = false;
				pDoc->AddNodeToCurrSection(pNode,pcCreated);
			}
			else if(strTextToComp.Compare(_T("pcLastChange")) == 0)
			{
				show_node = false;
				show_att = false;
				bRecurse = false;
				pDoc->AddNodeToCurrSection(pNode,pcLastChange);
			}
			else if(strTextToComp.Compare(_T("pcComments")) == 0)
			{
				show_att = true;
				pDoc->AddNodeToCurrSection(pNode,pcComments);
			}
			else if(strTextToComp.Compare(_T("page")) == 0)
			{
				pDoc->AddSection(pNode,page);
				pDoc->AddNodeToCurrSection(pNode,page);
				show_att = false;
			}
			else if(strTextToComp.Compare(_T("text_equiv")) == 0)
			{
				show_node = false;
				show_att = false;
			}
			else if(strTextToComp.Compare(_T("plain_text")) == 0)
			{
				show_node = false;
				show_att = false;
				bRecurse = false;
				
				pDoc->AddNodeToCurrSection(pNode,plain_text);

				LPARAM itemdata = m_wndXMLTree.GetItemData(hParentItem);
				pNodeParent = (MSXML2::IXMLDOMNodePtr)(LPUNKNOWN)itemdata;
				strTextToAdd = W2T(pNodeParent->GetnodeName());
				pNextNode = pNode->GetchildNodes()->nextNode();
				if(pNextNode == NULL)
				{
					pNextNode = pDoc->GetPageDom()->createTextNode(_bstr_t(""));
					pNode->appendChild(pNextNode);
				}
			
				strTextToAdd = strTextToAdd + _T(" (") + W2T(_bstr_t(pNextNode->GetnodeValue())) + _T(")");
				m_wndXMLTree.SetItemText(hParentItem,strTextToAdd);
			}
			else if(strTextToComp.Compare(_T("text_region")) == 0)
			{
				show_att = true;
				pDoc->AddSection(pNode,text_region);
				pDoc->AddNodeToCurrSection(pNode,text_region);
				m_no_text_regions++;
			}
			else if(strTextToComp.Compare(_T("text_line")) == 0)
			{
				show_att = false;
				pDoc->AddSection(pNode,text_line);
				pDoc->AddNodeToCurrSection(pNode,text_line);
			}
			else if(strTextToComp.Compare(_T("word")) == 0)
			{
				show_att = false;
				pDoc->AddSection(pNode,word);
				pDoc->AddNodeToCurrSection(pNode,word);
			}
			else if(strTextToComp.Compare(_T("glyph")) == 0)
			{
				show_att = true;
				pDoc->AddSection(pNode,glyph);
				pDoc->AddNodeToCurrSection(pNode,glyph);
			}
			else
				return S_FALSE;

			break;
		};
		HTREEITEM hItem;
		if(show_node)
			hItem = AddNodeToTree(pNode, hParentItem, false);
		else
			hItem = hParentItem;

		if (show_att)
		{
			// Add item's attributes

			MSXML2::IXMLDOMNamedNodeMapPtr pAttrMap = pNode->Getattributes();
			if (pAttrMap != NULL)
			{
				MSXML2::IXMLDOMAttributePtr pAttribute;
				while(pAttribute = pAttrMap->nextNode())
				{
					show_att = true;
					strTextToComp.Empty();
					strTextToComp = W2T(pAttribute->GetnodeName());
					if(strTextToComp.Compare(_T("id")) == 0)
					{
							pDoc->UpdateIdCounter(W2T(_bstr_t(pAttribute->GetnodeValue())));
							show_att = false;
					}
					else if(strTextToComp.Compare(_T("txt_indented")) == 0)
							show_att = false;
					else if(strTextToComp.Compare(_T("txt_kerning")) == 0)
							show_att = false;
					else if(strTextToComp.Compare(_T("txt_leading")) == 0)
							show_att = false;

					if(show_att)
					{
						 if(strTextToComp.Compare(_T("txt_color")) == 0)
							pDoc->AddNodeToCurrSection(pAttribute,txt_color);

						 else if(strTextToComp.Compare(_T("txt_bgcolor")) == 0)
							pDoc->AddNodeToCurrSection(pAttribute,txt_bgcolor);

						 else if(strTextToComp.Compare(_T("txt_font_size")) == 0)
							pDoc->AddNodeToCurrSection(pAttribute,txt_font_size);

						 else if(strTextToComp.Compare(_T("txt_primary_language")) == 0)
							pDoc->AddNodeToCurrSection(pAttribute,txt_primary_language);

						 else if(strTextToComp.Compare(_T("txt_primary_script")) == 0)
							pDoc->AddNodeToCurrSection(pAttribute,txt_primary_script);

						 else if(strTextToComp.Compare(_T("txt_reading_direction")) == 0)
							pDoc->AddNodeToCurrSection(pAttribute,txt_reading_direction);

						 else if(strTextToComp.Compare(_T("txt_reading_orientation")) == 0)
							pDoc->AddNodeToCurrSection(pAttribute,txt_reading_orientation);

						 else if(strTextToComp.Compare(_T("txt_orientation")) == 0)
							pDoc->AddNodeToCurrSection(pAttribute,txt_orientation);

						 else if(strTextToComp.Compare(_T("txt_reverse_video")) == 0)
							pDoc->AddNodeToCurrSection(pAttribute,txt_reverse_video);

						 else if(strTextToComp.Compare(_T("txt_secondary_script")) == 0)
							pDoc->AddNodeToCurrSection(pAttribute,txt_secondary_script);

						 else if(strTextToComp.Compare(_T("txt_type")) == 0)
							pDoc->AddNodeToCurrSection(pAttribute,txt_type);

						 else if(strTextToComp.Compare(_T("ligature")) == 0)
							pDoc->AddNodeToCurrSection(pAttribute,ligature);
						 
						 else if(strTextToComp.Compare(_T("symbol")) == 0)
							pDoc->AddNodeToCurrSection(pAttribute,symbol);

						 AddNodeToTree(pAttribute, hItem, false);
					}
				}
			}
		}
		else
		{
			// Automatic item's attributes to process but not show

			MSXML2::IXMLDOMNamedNodeMapPtr pAttrMap = pNode->Getattributes();
			if (pAttrMap != NULL)
			{
				MSXML2::IXMLDOMAttributePtr pAttribute;
				while(pAttribute = pAttrMap->nextNode())
				{
					strTextToComp.Empty();
					strTextToComp = W2T(pAttribute->GetnodeName());
					if(strTextToComp.Compare(_T("image_filename")) == 0)
					{
						pDoc->AddNodeToCurrSection(pAttribute,image_filename);
						strFile = W2T(_bstr_t(pAttribute->GetnodeValue()));
						pathdir = m_strURL.Left(m_strURL.ReverseFind(wchar_t('\\'))+ 1);
						if(!strFile.IsEmpty())
							pDoc->SetImagePath(strFile);
						
					}
					else if(strTextToComp.Compare(_T("no_text_regions")) == 0)
					{
						pDoc->AddNodeToCurrSection(pAttribute,no_text_regions);
						//m_no_text_regions = pAttribute->GetnodeValue();
					}
				}
			}
		}

		// Recurse Children
		if (bRecurse)
		{
			MSXML2::IXMLDOMNodeListPtr pNodeList = pNode->GetchildNodes();
			if (pNodeList != NULL)
			{
				SectionPtrs *nodeSection = pDoc->GetCurrentSection();
				MSXML2::IXMLDOMNodePtr pChild;

				while(pChild = pNodeList->nextNode())
				{
					nodeSection = pDoc->GetCurrentSection();
					FillPageXMLTree(pChild, hItem);
					pDoc->SetCurrentSection(nodeSection);					
				}
			}
		}
	}
	catch(_com_error e)
	{
		TRACE(_T("Caught Exception: FillXMLTree"));
		ReportError(e, _T("tree scan"));
	}
	catch(...)
	{
		TRACE(_T("Caught Exception: FillXMLTree"));
		AfxMessageBox(_T("Error filling tree!"));
	}

	return S_OK;
}

MSXML2::IXMLDOMNodePtr CXMLView::GetNodeFromTree(HTREEITEM hItem)
{
	if (NULL == hItem)
		hItem = m_hCurItem;
	if (NULL == hItem)
		return NULL;

	LPARAM itemdata = m_wndXMLTree.GetItemData(hItem);
	if (NULL == itemdata)
	{
		return NULL;
	}
	MSXML2::IXMLDOMNodePtr pNode = (MSXML2::IXMLDOMNodePtr)(LPUNKNOWN)itemdata;
	if (NULL == pNode)
	{
		return NULL;
	}

	return pNode;
}

// Attributes do not have a "parent" node. Whenever we get the parent node for an attribute,
// we "cheat": we use the Treeview instead of the DOM
MSXML2::IXMLDOMNodePtr CXMLView::GetParentNodeFromTree(HTREEITEM hItem)
{
	if (NULL == hItem)
		hItem = m_hCurItem;
	if (NULL == hItem)
		return NULL;

	// replace the old node with the new one
	HTREEITEM hParentItem = m_wndXMLTree.GetParentItem(hItem);
	if (NULL == hParentItem)
	{
		// Ignore it, no parent
		return NULL;
	}
	
	return GetNodeFromTree(hParentItem);
}

CString CXMLView::NodeToTextImage( MSXML2::IXMLDOMNodePtr pNode, int& nImageToAdd, int& nSelectedImageToAdd)
{
USES_CONVERSION;

	MSXML2::IXMLDOMNodePtr pNodeP;
	MSXML2::IXMLDOMAttributePtr pNodeA;
	CGTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return CString();
	SectionPtrs* sec = pDoc->GetCurrentSection();
	NodeName nodeName;
	
	MSXML2::DOMNodeType nodeType;
	pNode->get_nodeType(&nodeType);

	CString strTextToAdd;

	HTREEITEM hItem = m_hCurItem;
	HRESULT res = S_FALSE;

	pNodeP = pNode;
	
	if(pNodeP->GetnodeType() == NODE_ATTRIBUTE)
	{
		pNodeP = GetNodeFromTree(m_hCurItem);
	}

	while(pNodeP != NULL)
	{
		if((res = pDoc->UpdateSection(pNodeP)) == S_FALSE)
			pNodeP = pNodeP->GetparentNode();
		else
			pNodeP = NULL;
	}

	if(res == S_OK)
		nodeName = pDoc->GetCurrentSection()->type;
	else	
		nodeName = none;

	//Static numers to mach the toolbar image icons 

	switch(nodeName)
	{
			case pcMetadata: 
				nImageToAdd = 33;
				break;
			case text_region: 
				nImageToAdd = 37;
				break;
			case text_line: 
				nImageToAdd = 41;
				break;
			case word: 
				nImageToAdd = 45;
				break;
			case glyph:	
				nImageToAdd = 49;
				break;
			default:
				nImageToAdd = 29;
				break;
	}
	pDoc->SetCurrentSection(sec);
	
	try
	{
		switch(nodeType)
		{
		case MSXML2::NODE_INVALID:
			strTextToAdd = _T("---");
			break;

		case MSXML2::NODE_ELEMENT:
			strTextToAdd = W2T(pNode->GetnodeName());
			break;

		case MSXML2::NODE_ATTRIBUTE:
			strTextToAdd.Format(_T("%s = %s"), W2T(pNode->GetnodeName()), W2T(_bstr_t(pNode->GetnodeValue())));
			nImageToAdd = nImageToAdd + 2;
			break;

		case MSXML2::NODE_TEXT:
			strTextToAdd = W2T(_bstr_t(pNode->GetnodeValue()));
			nImageToAdd = nImageToAdd + 2;
			break;

		case MSXML2::NODE_CDATA_SECTION:
			strTextToAdd = W2T(_bstr_t(pNode->GetnodeValue()));
			break;

		case MSXML2::NODE_ENTITY_REFERENCE:
			strTextToAdd = W2T(pNode->GetnodeName());
			break;

		case MSXML2::NODE_ENTITY:
			strTextToAdd = W2T(pNode->GetnodeName());
			break;

		case MSXML2::NODE_PROCESSING_INSTRUCTION:
			strTextToAdd.Format(_T("%s %s"), W2T(pNode->GetnodeName()), W2T(_bstr_t(pNode->GetnodeValue())));
			break;

		case MSXML2::NODE_COMMENT:
			strTextToAdd = W2T(_bstr_t(pNode->GetnodeValue()));
			break;

		case MSXML2::NODE_DOCUMENT:
			strTextToAdd = _T("GT Document");
			break;
		
		case MSXML2::NODE_DOCUMENT_TYPE:
			strTextToAdd = W2T(_bstr_t(pNode->GetnodeName()));
			break;

		case MSXML2::NODE_DOCUMENT_FRAGMENT:
			strTextToAdd = _T("Root Node of Document Fragment");
			break;

		case MSXML2::NODE_NOTATION:
			strTextToAdd = W2T(_bstr_t(pNode->GetnodeName()));
			break;
		
		default:
			ASSERT(_T("Invalid NODE TYPE!\n"));
		}
		nSelectedImageToAdd = nImageToAdd - 1;
	}
	catch(...)
	{
		strTextToAdd = _T("<ERROR!!>");
	}

	return strTextToAdd;
}

HTREEITEM CXMLView::AddNodeToTree(MSXML2::IXMLDOMNodePtr pNode, HTREEITEM hParentItem, bool bSelect)
{
	int nImageToAdd;
	int nSelectedImageToAdd;
	HTREEITEM hcurrentItem = m_hCurItem;
	m_hCurItem = hParentItem;
	
	CString strTextToAdd = NodeToTextImage(pNode, nImageToAdd, nSelectedImageToAdd);

	m_hCurItem = hcurrentItem;

	// Add the item
	pNode->AddRef();
	HTREEITEM hItem = m_wndXMLTree.InsertItem(TVIF_PARAM | TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_STATE,
				strTextToAdd, nImageToAdd, nSelectedImageToAdd, TVIS_EXPANDED, TVIS_EXPANDED, (LPARAM)(void*)pNode, hParentItem, TVI_LAST);

	if (bSelect)
		m_wndXMLTree.SelectItem(hItem);

	return hItem;
}

void CXMLView::ReleaseCurrentDocument()
{
	if (m_pDataSource)
		delete m_pDataSource;

	if (g_pClipboardNode)
		g_pClipboardNode = NULL;

	if (m_pDOMDoc)
	{
		AtlUnadvise(m_pDOMDoc, __uuidof(MSXML2::XMLDOMDocumentEvents), m_dwAdviseCookie);
		m_dwAdviseCookie = NULL;
		DeleteTreeItems(NULL);
		m_pCurNode = NULL;
		m_pDataSource = NULL;
		m_pDOMDoc = NULL;
		m_bLoading = false;
	}
}

void CXMLView::ReportError(_com_error e, CString strLocation)
{
USES_CONVERSION;
	CString strError;
	strError.Format(_T("ERROR on %s: %s"), strLocation, OLE2T(e.Description()));
	m_wndErrors.SetWindowText(strError);
}


bool CXMLView::SetSelectedNode()
{
	m_hCurItem = m_wndXMLTree.GetSelectedItem();
	m_pCurNode = GetNodeFromTree();
	
	
	if (NULL == m_pCurNode || NULL == m_hCurItem)
		return false;
	else
		return true;
}


// CXMLView diagnostics

#ifdef _DEBUG
void CXMLView::AssertValid() const
{
	CFormView::AssertValid();
}

void CXMLView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}


#endif //_DEBUG


// CXMLView message handlers



void CXMLView::OnTvnSelchangedXmltree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	SectionPtrs* section;
	//*pResult = 0;
	
	CGTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	POSITION pos = pDoc->GetFirstViewPosition();
	CGTView* gtView = (CGTView*) pDoc->GetNextView(pos);
	if(gtView == NULL)
		return;
	if(pDoc->GetLock())
		return;
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	CSplitterWnd* pParentSplitter = GetParentSplitter(this,true);
	HTREEITEM hItem;

	SetSelectedNode();

	hItem = m_hCurItem;
	pMainWnd->SetActiveView((CView*)pParentSplitter->GetPane(1,0));
	BeginWaitCursor();

	section =  pDoc->GetCurrentSection();
	if(pDoc->IsDirty())
	{
		if(section != NULL)
			if(section->type == glyph)
				pDoc->SavePoints();
		pDoc->SetDirty(false);
	}
	pDoc->LoadMask();
	NodeName sectionName;

	while ((NULL != hItem) && (pDoc->UpdateSection(GetNodeFromTree(hItem)) != S_OK))
			hItem = m_wndXMLTree.GetParentItem(hItem);
		
	if(NULL != hItem)
	{
		section = pDoc->GetCurrentSection();
		pDoc->LoadPointsSelected(GetNodeFromTree(hItem));
		pDoc->SetCurrentSection(section);
		sectionName = pDoc->GetCurrentSection()->type;
		CImageSelection *sel = pDoc->GetImageSelection();
		EditEnum editState = pDoc->GetEditState(),lastEditState;
		if(sectionName != glyph)
		{
			if(editState != EDIT_NONE)
			{
				pDoc->SetOnGlyphState(editState);
				pDoc->SetEditState(EDIT_NONE);
			}

			pMainWnd->AddSensitivityBar(false);
			pMainWnd->AddPen(false);
		}
		else
		{
			if(editState != EDIT_NONE)
			{
				pMainWnd->AddSensitivityBar(pDoc->GetToolState() == REGION_TOOL);
				pMainWnd->AddPen(pDoc->GetToolState() == PIXEL_TOOL);
			}
			else
			{
				lastEditState = pDoc->GetOnGlyphState();

				switch(lastEditState)
				{
				case EDIT_CORE:
					if(sel->GetCore())
						pDoc->SetEditState(lastEditState);
					break;
				case EDIT_OUTLINE:
					if(sel->GetOutline())
						pDoc->SetEditState(lastEditState);
					break;
				case EDIT_SHADE:
					if(sel->GetShade())
						pDoc->SetEditState(lastEditState);
					break;
				default:break;
				}
				
				if(pDoc->GetEditState() != EDIT_NONE)
				{
					pMainWnd->AddSensitivityBar(pDoc->GetToolState() == REGION_TOOL);
					pMainWnd->AddPen(pDoc->GetToolState() == PIXEL_TOOL);
				}
			}
		 }
		

		switch(sectionName)
		{
			case pcMetadata: pDoc->SwitchToView(RUNTIME_CLASS(CMetaDataView));
				 ((CMetaDataView*)pParentSplitter->GetPane(1,0))->OnUpdate();
				break;
			case text_region: pDoc->SwitchToView(RUNTIME_CLASS(CTextRegionView));
				 ((CTextRegionView*)pParentSplitter->GetPane(1,0))->OnUpdate();
				break;
			case text_line: pDoc->SwitchToView(RUNTIME_CLASS(CTextEquivalView));
				((CTextEquivalView*)pParentSplitter->GetPane(1,0))->OnUpdate();
				break;
			case word: pDoc->SwitchToView(RUNTIME_CLASS(CTextEquivalView));
				((CTextEquivalView*)pParentSplitter->GetPane(1,0))->OnUpdate();
				break;
			case glyph:	pDoc->SwitchToView(RUNTIME_CLASS(CGlyphView));
				((CGlyphView*)pParentSplitter->GetPane(1,0))->OnUpdate();
				break;
			default: 
				pDoc->SwitchToView(RUNTIME_CLASS(CBlankView));
				((CBlankView*)pParentSplitter->GetPane(1,0))->OnUpdate();
		};
	}
	else
	{
		pDoc->SetCurrentSection(NULL);
		pDoc->SwitchToView(RUNTIME_CLASS(CBlankView));
		((CBlankView*)pParentSplitter->GetPane(1,0))->OnUpdate();
	}
	if(pDoc->IsLoad())
	{
		pDoc->SetPenPoint(CPoint(-1,-1));
		gtView->SetShowPen();
		gtView->Invalidate(false);
	}
	pDoc->SetLoad(true);
	EndWaitCursor();
}

HTREEITEM CXMLView::GetSelectionItem()
{
	if(m_hCurItem == NULL)
		return NULL;
	SetSelectedNode();
	LPARAM itemdata = m_wndXMLTree.GetItemData(m_hCurItem);
	MSXML2::IXMLDOMNodePtr pNode = (MSXML2::IXMLDOMNodePtr)(LPUNKNOWN)itemdata;
	if(pNode == NULL)
		return NULL;
	MSXML2::DOMNodeType nodeType = pNode->GetnodeType();
	
	if(nodeType == MSXML2::NODE_ATTRIBUTE)
		return m_wndXMLTree.GetParentItem(m_hCurItem);
	else
		return m_hCurItem;
}

void CXMLView::OnFileOpen()
{
	CFrameWnd* pMainWnd = (CFrameWnd*)AfxGetMainWnd();
	CSplitterWnd* pParentSplitter = GetParentSplitter(this,true);
	CGTView* gtView = (CGTView*)((CSplitterWnd*)pParentSplitter->GetParent())->GetPane(0,0);
		
	OnOpenLinkUrl();
	gtView->SetActiveWindow();
	gtView->OnFileImageOpen();
}

void CXMLView::OnContextgtNewtextregion()
{
	CString name;
	_bstr_t bstrName;
	BSTR nameSpace;

	MSXML2::IXMLDOMNamedNodeMapPtr pAttrList;
	CFrameWnd* pMainWnd = (CFrameWnd*)AfxGetMainWnd();
	CSplitterWnd* pParentSplitter = GetParentSplitter(this,true);
	pMainWnd->SetActiveView((CView*)pParentSplitter->GetPane(1,0));


	CGTDoc* pDoc = GetDocument();
	if (!pDoc)
		return;
	pDoc->GetImageSelection()->DeltePoints();
	
	name = _T("text_region");
	bstrName = (_bstr_t)(LPCTSTR)name;

	MSXML2::IXMLDOMElementPtr pNewElem;
	MSXML2::IXMLDOMNodePtr pNewNode = pDoc->GetNodeFromCurrSection(page);
	pNewNode->get_namespaceURI(&nameSpace);

	pNewElem = m_pDOMDoc->createNode(_variant_t((short)pNewNode->GetnodeType()), bstrName, nameSpace);
		
	if(pNewNode != NULL)
		pAttrList = pNewElem->Getattributes();
	else
		return;
	if (NULL == pAttrList)
			return;
	name = _T("id");
	bstrName = (_bstr_t)(LPCTSTR)name;
	// Do add attr id
	MSXML2::IXMLDOMAttributePtr pNewAttr = m_pDOMDoc->createAttribute(bstrName);
	
	long idval;

	idval = pDoc->UpdateIdCounter();
	name = "";
	name = wchar_t(idval % 10 + 48) + name;
	idval /= 10;
	while(idval !=0)
	{
		name = wchar_t(idval % 10 + 48) + name;
		idval /= 10;
	}
	name = _T("id") + name;

	pNewAttr->PutnodeValue(BSTR(name.GetString()));
	pAttrList->setNamedItem(pNewAttr);
	pNewAttr = pAttrList->nextNode();
	pNewNode->appendChild(pNewElem);

	pDoc->AddSection(pNewElem,text_region);
	pDoc->AddNodeToCurrSection(pNewElem,text_region);
	AddNodeToTree(pNewElem, m_hCurItem,true);
	
	pDoc->SwitchToView(RUNTIME_CLASS(CTextRegionView));
	pDoc->SetModifiedFlag(TRUE);
	m_no_text_regions++;
}

void CXMLView::OnContextgtNewtextline()
{
	CString name;
	_bstr_t bstrName;
	BSTR nameSpace;

	MSXML2::IXMLDOMNamedNodeMapPtr pAttrList;
	CFrameWnd* pMainWnd = (CFrameWnd*)AfxGetMainWnd();
	CSplitterWnd* pParentSplitter = GetParentSplitter(this,true);
	pMainWnd->SetActiveView((CView*)pParentSplitter->GetPane(1,0));


	CGTDoc* pDoc = GetDocument();
	if (!pDoc)
		return;
	pDoc->GetImageSelection()->DeltePoints();

	name = _T("text_line");
	bstrName = (_bstr_t)(LPCTSTR)name;

	MSXML2::IXMLDOMElementPtr pNewElem = m_pDOMDoc->createElement(bstrName);
	MSXML2::IXMLDOMNodePtr pNewNode = pDoc->GetNodeFromCurrSection(text_region);
	pNewNode->get_namespaceURI(&nameSpace);

	pNewElem = m_pDOMDoc->createNode(_variant_t((short)pNewNode->GetnodeType()), bstrName, nameSpace);

	if(pNewNode != NULL)
		pAttrList = pNewElem->Getattributes();
	else
		return;
	if (NULL == pAttrList)
			return;

	name = _T("id");
	bstrName = (_bstr_t)(LPCTSTR)name;
	// Do add attr id
	MSXML2::IXMLDOMAttributePtr pNewAttr = m_pDOMDoc->createAttribute(bstrName);
	long idval;

	idval = pDoc->UpdateIdCounter();
	name = "";
	name = wchar_t(idval % 10 + 48) + name;
	idval /= 10;
	while(idval !=0)
	{
		name = wchar_t(idval % 10 + 48) + name;
		idval /= 10;
	}
	name = _T("id") + name;

	pNewAttr->PutnodeValue(BSTR(name.GetString()));
	pAttrList->setNamedItem(pNewAttr);
	pNewNode->appendChild(pNewElem);
	
	pDoc->AddSection(pNewElem,text_line);
	pDoc->AddNodeToCurrSection(pNewElem,text_line);
	AddNodeToTree(pNewElem, m_hCurItem,true);

	pDoc->SwitchToView(RUNTIME_CLASS(CTextEquivalView));
	pDoc->SetModifiedFlag(TRUE);
}


void CXMLView::OnContextgtNewword()
{
	CString name;
	_bstr_t bstrName;
	BSTR nameSpace;

	MSXML2::IXMLDOMNamedNodeMapPtr pAttrList;
	CFrameWnd* pMainWnd = (CFrameWnd*)AfxGetMainWnd();
	CSplitterWnd* pParentSplitter = GetParentSplitter(this,true);
	pMainWnd->SetActiveView((CView*)pParentSplitter->GetPane(1,0));


	CGTDoc* pDoc = GetDocument();
	if (!pDoc)
		return;

	name = _T("word");
	bstrName = (_bstr_t)(LPCTSTR)name;

	MSXML2::IXMLDOMElementPtr pNewElem = m_pDOMDoc->createElement(bstrName);
	MSXML2::IXMLDOMNodePtr pNewNode = pDoc->GetNodeFromCurrSection(text_line);
	pNewNode->get_namespaceURI(&nameSpace);

	pNewElem = m_pDOMDoc->createNode(_variant_t((short)pNewNode->GetnodeType()), bstrName, nameSpace);
	if(pNewNode != NULL)
		pAttrList = pNewElem->Getattributes();
	else
		return;
	if (NULL == pAttrList)
			return;

	name = _T("id");
	bstrName = (_bstr_t)(LPCTSTR)name;
	// Do add attr id
	MSXML2::IXMLDOMAttributePtr pNewAttr = m_pDOMDoc->createAttribute(bstrName);
	
	long idval;

	idval = pDoc->UpdateIdCounter();
	name = "";
	name = wchar_t(idval % 10 + 48) + name;
	idval /= 10;
	while(idval !=0)
	{
		name = wchar_t(idval % 10 + 48) + name;
		idval /= 10;
	}
	name = _T("id") + name;

	
	pNewAttr->PutnodeValue(BSTR(name.GetString()));
	pAttrList->setNamedItem(pNewAttr);
	pNewNode->appendChild(pNewElem);
	
	pDoc->AddSection(pNewElem,word);
	pDoc->AddNodeToCurrSection(pNewElem,word);
	AddNodeToTree(pNewElem, m_hCurItem,true);
	pDoc->SwitchToView(RUNTIME_CLASS(CTextEquivalView));
	pDoc->SetModifiedFlag(TRUE);
}


void CXMLView::OnContextgtNewglyph()
{
	CString name;
	_bstr_t bstrName;
	BSTR nameSpace;

	MSXML2::IXMLDOMNamedNodeMapPtr pAttrList;
	CFrameWnd* pMainWnd = (CFrameWnd*)AfxGetMainWnd();
	CSplitterWnd* pParentSplitter = GetParentSplitter(this,true);
	

	CGTDoc* pDoc = GetDocument();
	if (!pDoc)
		return;

	pMainWnd->SetActiveView((CView*)pParentSplitter->GetPane(1,0));

	pDoc->GetImageSelection()->DeltePoints();


	name = _T("glyph");
	bstrName = (_bstr_t)(LPCTSTR)name;

	MSXML2::IXMLDOMElementPtr pNewElem = m_pDOMDoc->createElement(bstrName);
	MSXML2::IXMLDOMNodePtr pNewNode = pDoc->GetNodeFromCurrSection(word);
	pNewNode->get_namespaceURI(&nameSpace);

	pNewElem = m_pDOMDoc->createNode(_variant_t((short)pNewNode->GetnodeType()), bstrName, nameSpace);
	if(pNewNode != NULL)
		pAttrList = pNewElem->Getattributes();
	else
		return;
	if (NULL == pAttrList)
			return;

	name = _T("id");
	bstrName = (_bstr_t)(LPCTSTR)name;
	// Do add attr id
	MSXML2::IXMLDOMAttributePtr pNewAttr = m_pDOMDoc->createAttribute(bstrName);
	long idval;

	idval = pDoc->UpdateIdCounter();
	name = "";
	name = wchar_t(idval % 10 + 48) + name;
	idval /= 10;
	while(idval !=0)
	{
		name = wchar_t(idval % 10 + 48) + name;
		idval /= 10;
	}
	name = _T("id") + name;

	pNewAttr->PutnodeValue(BSTR(name.GetString()));
	pAttrList->setNamedItem(pNewAttr);
	pNewNode->appendChild(pNewElem);
	pDoc->AddSection(pNewElem,glyph);
	pDoc->AddNodeToCurrSection(pNewElem,glyph);
	AddNodeToTree(pNewElem, m_hCurItem,true);
	pDoc->SwitchToView(RUNTIME_CLASS(CGlyphView));
	pDoc->SetModifiedFlag(TRUE);
}

void CXMLView::DeleteGlyphBranch(MSXML2::IXMLDOMNodePtr pNode)
{
	CString strTextToComp;
	MSXML2::IXMLDOMNodePtr pNodeRoot,pChild,pGlyph;
	MSXML2::IXMLDOMNodeListPtr pChildList;
	CGTDoc* pDoc = GetDocument();
	if (!pDoc)
		return;
	
	if(pNode!= NULL)
	{	
		strTextToComp = W2T(pNode->GetnodeName());
		pChildList = pNode->GetchildNodes();
		pChild = pChildList->nextNode();

		while(pChild != NULL)
		{
			DeleteGlyphBranch(pChild);
			pChild = pChildList->nextNode();
		}
		
		if(strTextToComp.Compare(_T("glyph")) == 0)
		{	
			pDoc->UpdateSection(pNode);
			pGlyph = pDoc->GetNodeFromCurrSection(glyph_glyph);
			if(pGlyph != NULL)
			{
				pNodeRoot =  pGlyph->GetparentNode();
				pNodeRoot->removeChild(pGlyph);
			}
		}
	}
}
void CXMLView::OnContextgtDelete()
{
	MSXML2::IXMLDOMNodePtr pNode,pParentNode;
	CGTDoc* pDoc = GetDocument();
	if (!pDoc)
		return;

	SectionPtrs* section = pDoc->GetCurrentSection();

	HTREEITEM hParentItem = m_wndXMLTree.GetParentItem(m_hCurItem);
	pDoc->GetImageSelection()->LoadMaskPoints(pDoc->GetEditState());
	
	if(pDoc->IsDirty())
	{
		if(section != NULL)
			if(section->type == glyph)
				pDoc->SavePoints();
		pDoc->SetDirty(false);
	}
	pDoc->LoadMask();

	pDoc->Backup(1);
	pDoc->SetModifiedFlag(TRUE);
	pNode = GetNodeFromTree();
	pParentNode = GetParentNodeFromTree();
	if(pNode != NULL)
		if(pNode->GetnodeType() == MSXML2::NODE_ATTRIBUTE)
		{
			m_hCurItem = hParentItem;
			hParentItem = m_wndXMLTree.GetParentItem(m_hCurItem);
			m_wndXMLTree.SelectItem(m_hCurItem);
			pNode = GetNodeFromTree();
			pParentNode = GetParentNodeFromTree();

		}

		
	if(section != NULL)
	{
		if(section->type == text_region)
				m_no_text_regions--;
	}
	else
		return;

	DeleteGlyphBranch(pNode);
	OnTreepopupDelete();
}

void CXMLView::OnFileSave(bool browse)
{
	CFrameWnd* pMainWnd = (CFrameWnd*)AfxGetMainWnd();
	CSplitterWnd* pParentSplitter = GetParentSplitter(this,true);

	CGTDoc* pDoc = GetDocument();
	if (!pDoc)
		return;
	if (NULL == m_pDOMDoc)
		return;

	CString strExt((LPCSTR)IDS_DEFAULTXMLEXTENSION);
	CString strTitle((LPCSTR)IDS_SAVEDIALOGTITLE);
	CString strFilter((LPCSTR)IDS_DEFAULTXMLFILTERS);
	_bstr_t bstrPathName,bstrFolderName;
	CString strPathName;
	INT_PTR ret;
	SectionPtrs* section =  pDoc->GetCurrentSection();
	
	if(pDoc->IsDirty())
	{
		if(section != NULL)
			if(section->type == glyph)
				pDoc->SavePoints();
		pDoc->SetDirty(false);
	}
	
	if (browse)
	{
		CFileDialog fileDialog(FALSE, strExt, NULL, OFN_ENABLESIZING | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
								strFilter, this);
		fileDialog.m_ofn.lpstrTitle = strTitle;
		
		ret = fileDialog.DoModal();
		strPathName = fileDialog.GetPathName();
	}
	else
	{
		strPathName = pDoc->GetLinkPath();
		ret = IDOK;
	}

	if (IDOK == ret)
	{
		MSXML2::IXMLDOMNodePtr pNodeP,pNodeC;
		MSXML2::IXMLDOMAttributePtr pNodeA;
		HRESULT hr;
		CString pathdir = strPathName.Left(strPathName.ReverseFind(wchar_t('\\'))+ 1);
		CString file,name;
		
		if (!strPathName.IsEmpty())
		{
			pDoc->UpdateSection(NULL,pcMetadata);
			pNodeP = pDoc->GetNodeFromCurrSection(pcLastChange);

			if (pNodeP != NULL)
			{
				pNodeC = pNodeP->GetchildNodes()->nextNode();
				if (pNodeC != NULL){
					pNodeC->put_text(BSTR(GetDate().GetString()));
				}
			}

			pNodeP = pDoc->GetNodeFromCurrSection(glyph_pcLastChange);

			if (pNodeP != NULL)
			{
				pNodeC = pNodeP->GetchildNodes()->nextNode();
				if (pNodeC != NULL){
					pNodeC->put_text(BSTR(GetDate().GetString()));
				}
			}
			
			pDoc->UpdateSection(NULL,page);
			pNodeA = pDoc->GetNodeFromCurrSection(image_filename);
			file = pDoc->GetImagePath();
			file = file.Right(file.GetLength()-file.ReverseFind(wchar_t('\\'))-1);
			bstrPathName = file;
			if (pNodeA != NULL)
				pNodeA->put_text(bstrPathName);

			pNodeA = pDoc->GetNodeFromCurrSection(no_text_regions);
			if (pNodeA != NULL)
					pNodeA->put_text(BSTR(CString(wchar_t(m_no_text_regions+48)).GetString()));

			pDoc->UpdateSection(NULL,gt);

			file = strPathName.Right(strPathName.GetLength()-strPathName.ReverseFind(wchar_t('\\'))-1);
			if(file.Find(_T("gt-")) != -1)
				name = file.Right(file.GetLength()-3);
			else
				name = file;

			file = CString("pc-")+name;
			bstrPathName = file;

			pNodeP = pDoc->GetNodeFromCurrSection(pc);
			if (pNodeP != NULL)
			{
				pNodeC = pNodeP->GetchildNodes()->nextNode();
				if (pNodeC != NULL)
						pNodeC->put_text(bstrPathName);
			}
			file = file.Left(file.GetLength()-4);
			bstrPathName = file;
			pNodeA = pDoc->GetNodeFromCurrSection(pcID);
			if (pNodeA != NULL)
				pNodeA->put_text(bstrPathName);
			pNodeA = pDoc->GetNodeFromCurrSection(glDep);
			if (pNodeA != NULL)
				pNodeA->put_text(bstrPathName);

			strPathName = pDoc->GetLinkPath();
			if(pDoc->GetCurrentFolder().Compare(strPathName) == 0)
			{
				strPathName = pDoc->GetPagePath();
				pathdir = strPathName.Left(strPathName.ReverseFind(wchar_t('\\'))+ 1);
				bstrPathName = pathdir + CString("pc-") + name;
			}
			else
			{
				pathdir = strPathName.Left(strPathName.ReverseFind(wchar_t('\\'))+ 1);
				if(pDoc->GetDefPcFolder().Find(_T("\\")) == -1)
				{
					bstrPathName = pathdir + pDoc->GetDefPcFolder() + _T("\\");
					_wmkdir(bstrPathName);
				}
				else
				{
					pDoc->SetDefPcFolder(_T("pc"));
					bstrPathName = pathdir + _T("pc\\");
					_wmkdir(bstrPathName);
				}
				bstrPathName = bstrPathName + CString("pc-") + name;
				pDoc->SetPagePath((char *)bstrPathName);

				pNodeP = pDoc->GetNodeFromCurrSection(pc_lrFolder);
				bstrFolderName = pDoc->GetDefPcFolder() + _T("\\");
				if (pNodeP != NULL)
				{
					pNodeC = pNodeP->GetchildNodes()->nextNode();
					if (pNodeC != NULL)
							pNodeC->put_text(bstrFolderName);
				}
			}
			

			

			hr = pDoc->GetPageDom()->save((_variant_t)bstrPathName);
			if (FAILED(hr))
			{
				// Get Reason
				TRACE(_T("Save failed for: %s\n"), pDoc->GetPagePath());
			}

		
			file =  _T("gl-") + name;		
			bstrPathName = file;
			pNodeP = pDoc->GetNodeFromCurrSection(gl);
			if (pNodeP != NULL)
			{
				pNodeC = pNodeP->GetchildNodes()->nextNode();
				if (pNodeC != NULL)
						pNodeC->put_text(bstrPathName);
			}
			file = file.Left(file.GetLength()-4);
			bstrPathName = file;
			pNodeA = pDoc->GetNodeFromCurrSection(glID);
			if (pNodeA != NULL)
				pNodeA->put_text(bstrPathName);

			strPathName = pDoc->GetLinkPath();
			if(pDoc->GetCurrentFolder().Compare(strPathName) == 0)
			{
				strPathName = pDoc->GetGlyphPath();
				pathdir = strPathName.Left(strPathName.ReverseFind(wchar_t('\\'))+ 1);
				bstrPathName = pathdir + CString("gl-") + name;
			}
			else
			{
				pathdir = strPathName.Left(strPathName.ReverseFind(wchar_t('\\'))+ 1);
				if(pDoc->GetDefGlFolder().Find(_T("\\")) == -1)
				{
					bstrPathName = pathdir + pDoc->GetDefGlFolder() + _T("\\");
					_wmkdir(bstrPathName);
				}
				else
				{
					bstrPathName = pathdir + _T("gl\\");
					pDoc->SetDefGlFolder(_T("gl"));
					_wmkdir(bstrPathName);
				}
				bstrPathName = bstrPathName + CString("gl-") + name;
				pDoc->SetGlyphPath((char *)bstrPathName);

				pNodeP = pDoc->GetNodeFromCurrSection(gl_lrFolder);
				bstrFolderName = pDoc->GetDefGlFolder() + _T("\\");
				if (pNodeP != NULL)
				{
					pNodeC = pNodeP->GetchildNodes()->nextNode();
					if (pNodeC != NULL)
							pNodeC->put_text(bstrFolderName);
				}
			}

			

			hr = pDoc->GetGlyphDom()->save((_variant_t)bstrPathName);
			if (FAILED(hr))
			{
				// Get Reason
				TRACE(_T("Save failed for: %s\n"), pDoc->GetGlyphPath());
			}

			strPathName = pDoc->GetLinkPath();
			pathdir = strPathName.Left(strPathName.ReverseFind(wchar_t('\\'))+ 1);
	
			file =  _T("gt-") + name;		
			bstrPathName = file;

			pNodeP = pDoc->GetNodeFromCurrSection(gt);
			if (pNodeP != NULL)
			{
				pNodeC = pNodeP->GetchildNodes()->nextNode();
				if (pNodeC != NULL)
						pNodeC->put_text(bstrPathName);
			}

			file = file.Left(file.GetLength()-4);
			bstrPathName = file;
			pNodeA = pDoc->GetNodeFromCurrSection(gtID);
			if (pNodeA != NULL)
				pNodeA->put_text(bstrPathName);
			pNodeA = pDoc->GetNodeFromCurrSection(pcDep);
			if (pNodeA != NULL)
				pNodeA->put_text(bstrPathName);
			
		
			LPCTSTR name;
		
			bstrPathName = file +_T(".xml");
			name = LPCTSTR(bstrPathName);
			pDoc->SetTitle(bstrPathName);
			pathdir = strPathName.Left(strPathName.ReverseFind(wchar_t('\\'))+ 1);
			bstrPathName = pathdir + file +_T(".xml");
			
			pDoc->SetPathName(name,TRUE);


			hr = pDoc->GetLinkDom()->save((_variant_t)bstrPathName);
			if (FAILED(hr))
			{
				// Get Reason
				TRACE(_T("Save failed for: %s\n"), strPathName);
			}
		}
	}
}


void CXMLView::OnContextgtCreatewordglyph()
{
	MSXML2::IXMLDOMNodePtr pNodeP,pNodeC,pNewNode;
	MSXML2::IXMLDOMAttributePtr pNodeA;
	MSXML2::IXMLDOMNamedNodeMapPtr pAttrList;
	MSXML2::IXMLDOMAttributePtr pNewAttr;
	MSXML2::IXMLDOMElementPtr pNewElem;
	BSTR nameSpace;
	CFrameWnd* pMainWnd = (CFrameWnd*)AfxGetMainWnd();
	CSplitterWnd* pParentSplitter = GetParentSplitter(this,true);
	CGTView* gtView = (CGTView*)((CSplitterWnd*)pParentSplitter->GetParent())->GetPane(0,0);

	
	CString name;
	_bstr_t bstrName;

	HTREEITEM hCurItem;					
	MSXML2::IXMLDOMNodePtr pCurNode;	

	CGTDoc* pDoc = GetDocument();
	
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;	
	CString c_text;

	SectionPtrs* sec = pDoc->GetCurrentSection();

	SetSelectedNode();
	hCurItem = m_hCurItem;
	pCurNode = m_pCurNode;

	pNodeP = pDoc->GetNodeFromCurrSection(plain_text);
	if (pNodeP != NULL)
	{
		pNodeC = pNodeP->GetchildNodes()->nextNode();
		if (pNodeC != NULL){
			c_text = W2T(_bstr_t(pNodeC->GetnodeValue()));
		}
	}
	else
		c_text = _T("");

	
	for(int i=0;i<c_text.GetLength();i++)
	{
		pDoc->SetLock(false);
		OnContextgtNewglyph();
		pNewNode = pDoc->GetNodeFromCurrSection(glyph);
		if (pNewNode != NULL)
		{
			PrintNodeName(pNewNode,c_text.Mid(i,1));
			pAttrList = pNewNode->Getattributes();
			pNodeP = pDoc->GetNodeFromCurrSection(plain_text);
	
				
			name = _T("text_equiv");
			bstrName = (_bstr_t)(LPCTSTR)name;
			pNewNode->get_namespaceURI(&nameSpace);
			pNewElem = pDoc->GetLinkDom()->createNode(_variant_t((short)pNewNode->GetnodeType()), bstrName, nameSpace);

			pNodeP = pNewNode->appendChild(pNewElem);

			name = _T("plain_text");
			bstrName = (_bstr_t)(LPCTSTR)name;
			pNewNode->get_namespaceURI(&nameSpace);
			pNewElem = pDoc->GetLinkDom()->createNode(_variant_t((short)pNewNode->GetnodeType()), bstrName, nameSpace);

			pNodeP = pNodeP->appendChild(pNewElem);

			pNodeC = pDoc->GetPageDom()->createTextNode(BSTR(c_text.Mid(i,1).GetString()));
			pNodeP->appendChild(pNodeC);

			pDoc->AddNodeToCurrSection(pNodeP,plain_text);
			
		}
		else
		{	
			SetSelectedNode();
			OnContextgtDelete();
			i--;
		}

		pDoc->SetLoad(false);
		
		pDoc->SetCurrentSection(sec);
		m_hCurItem = hCurItem;
		m_pCurNode = pCurNode;
	}
	pDoc->SetModifiedFlag(TRUE);
		
}


BOOL CXMLView::PrintNodeName(MSXML2::IXMLDOMNodePtr pNode,CString name)
{
	HTREEITEM hItem = FindItemOnTree(m_wndXMLTree.GetParentItem(m_hCurItem),pNode);
	if(hItem == NULL)
		return FALSE;
	
	LPARAM itemdata = m_wndXMLTree.GetItemData(hItem);
	MSXML2::IXMLDOMNodePtr pNodeParent = (MSXML2::IXMLDOMNodePtr)(LPUNKNOWN)itemdata;
	if(pNodeParent == NULL)
		return FALSE;
	CString strTextToAdd = W2T(pNodeParent->GetnodeName());
	MSXML2::DOMNodeType nodeType = pNodeParent->GetnodeType();
	
	if(nodeType == MSXML2::NODE_ELEMENT)
	{
		if(name.Compare(_T("")) != 0)
			strTextToAdd = strTextToAdd + _T(" (")+ name +_T(")");
	}
	else if(nodeType == MSXML2::NODE_ATTRIBUTE)
		strTextToAdd.Format(_T("%s = %s"), W2T(pNode->GetnodeName()), W2T(_bstr_t(pNode->GetnodeValue())));
	else if(nodeType == MSXML2::NODE_TEXT)
		strTextToAdd = name;

	m_wndXMLTree.SetItemText(hItem,strTextToAdd);
	
	
	return TRUE;
}