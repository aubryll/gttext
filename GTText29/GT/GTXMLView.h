#pragma once
#include <afxstr.h>
#include "GTDoc.h"



class CXMLView : public CFormView
{
protected: // create from serialization only
	CXMLView();
	DECLARE_DYNCREATE(CXMLView)

public:
	enum{ IDD = IDD_FORMVIEW };
	CStatic	m_wndErrors;
	CTreeCtrl	m_wndXMLTree;
	BOOL	m_bAsync;
	CString	m_strURL;
	CString	m_newstrURL;
	CString	m_strName;
	CString	m_strValue;
	CString m_lrFolder;
	int		m_no_text_regions;



public:
	CGTDoc* GetDocument() const;
	void DeleteTreeItems(HTREEITEM hItem);
	void ReleaseCurrentDocument();


// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // Reemplazado para dibujar esta vista
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
		
// Implementation
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual ~CXMLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;

#endif

protected:
	HICON m_hIcon;							// Icon for dialog

	MSXML2::IXMLDOMDocumentPtr m_pDOMDoc;	// Stored reference to current XML document
	HTREEITEM m_hCurItem;					// reference to current selected item
	MSXML2::IXMLDOMNodePtr m_pCurNode;		// reference to current node pointer during clicks
	COleDataSource* m_pDataSource;			// holder for clipboard, drag&drop operations
	CImageList m_ImageList;					// image list for icons in treeview
	HACCEL m_hAccelTable;

	// async load
	bool m_bLoading;						// Are we in the middle of an async load?
	DWORD m_dwAdviseCookie;					// Advise cookie used during async load for events

private:
	bool m_bPopup;

// Helper Functions
protected:
	HRESULT FillPageXMLTree(MSXML2::IXMLDOMNodePtr pNode, HTREEITEM hParentItem, bool bRecurse = true);
	void CXMLView::DeleteGlyphBranch(MSXML2::IXMLDOMNodePtr pNode);

	MSXML2::IXMLDOMNodePtr GetParentNodeFromTree(HTREEITEM hItem = NULL);
	MSXML2::IXMLDOMNodePtr GetNodeFromTree(HTREEITEM hItem = NULL);
	CString NodeToTextImage(MSXML2::IXMLDOMNodePtr pNode, int& nImageToAdd, int& nSelectedImageToAdd);
	bool SetSelectedNode();
	HTREEITEM FindItemOnTree(const HTREEITEM hItem,MSXML2::IXMLDOMNodePtr pNode);

	void ReportError(_com_error e, CString strLocation);
	

// Generated message map functions
protected:

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnTreepopupDelete();
	afx_msg void OnTreepopupCopy();
	afx_msg void OnTreepopupCut();
	afx_msg void OnTreepopupPaste();
	afx_msg void OnBrowseforfile();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnData(WPARAM, LPARAM); 
	DECLARE_MESSAGE_MAP()

public:
	HTREEITEM CXMLView::AddNodeToTree(MSXML2::IXMLDOMNodePtr pNode, HTREEITEM hParentItem, bool bSelect = true);
	HRESULT ParseGlyphXMLTree(MSXML2::IXMLDOMNodePtr pNode, bool bRecurse = true);
	HRESULT ParseLinkXML(MSXML2::IXMLDOMNodePtr pNode, bool bRecurse = true);
	BOOL PrintNodeName(MSXML2::IXMLDOMNodePtr pNode,CString name);
	HTREEITEM GetSelectionItem();
	void SetSelectedPoint();
	CString GetDate();
	afx_msg HRESULT OnOpenLinkUrl(bool browse = true);
	afx_msg void OnPageRefresh();
	afx_msg HRESULT OnOpenPageUrl();
	afx_msg HRESULT OnOpenGlyphUrl();
	afx_msg void OnTvnSelchangedXmltree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave(bool browse = true);
	afx_msg void OnSaveXMLToFile();
	afx_msg void OnContextgtNewtextregion();
	afx_msg void OnContextgtNewtextline();
	afx_msg void OnContextgtNewword();
	afx_msg void OnContextgtNewglyph();
	afx_msg void OnContextgtDelete();
	afx_msg void OnContextgtNewglyphsfromword();
	afx_msg void OnContextgtCreatewordglyph();

};

#ifndef _DEBUG 
#endif

