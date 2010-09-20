// MainFrm.h: interfaz de la clase CMainFrame
//


#pragma once
class CSideBar:public CDialogBar
{
public:
 CSideBar() {};
 virtual ~CSideBar() {};
 
 protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
 afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};




class CMainFrame : public CFrameWnd
{
	
protected: // Crear sólo a partir de serialización
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Atributos
protected:
	CSplitterWnd m_wndSplitter;
	CSplitterWnd m_wndSplitterXML;
	CDialogBar  m_wndBrightBar;
	CDialogBar  m_wndSensitivityBar;
	CSideBar  m_wndZoomBar;
	CSideBar  m_wndPenBar;
	//bit x1 enabling
	//bit 1x visible
	int isSelBrightActive;
	int isSelSensActive;
	int isZoomActive;
	int isPenActive;
	bool m_isLoaded;

// Reemplazos
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void AddBrightBar(bool isAdded = true);
	void AddSensitivityBar(bool isAdded = true);
	void AddZoom(bool isAdded);
	void AddPen(bool isAdded);
	void HideToolBars();

// Implementación
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // Miembros incrustados de la barra de control
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CToolBar    m_wndImageBar;

// Funciones de asignación de mensajes generadas
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnViewImagebar();
	afx_msg void OnUpdateViewImagebar(CCmdUI *pCmdUI);
	afx_msg void OnViewSelectionbright();
	afx_msg void OnUpdateViewSelectionbright(CCmdUI *pCmdUI);
	afx_msg void OnNMReleasedcaptureBrightSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnViewSensitivity();
	afx_msg void OnUpdateViewSensitivity(CCmdUI *pCmdUI);
	afx_msg void OnNMReleasedcaptureOnfillSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSpinsens(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinsens(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnViewZoombar();
	afx_msg void OnUpdateViewZoombar(CCmdUI *pCmdUI);
	afx_msg void OnCbnSelchangeCombozoom();
	afx_msg void OnCombozoom(UINT nID);
	afx_msg void OnUpdateViewBrushsizebar(CCmdUI *pCmdUI);
	afx_msg void OnCbnSelchangeCombopen();
	afx_msg void OnBnClickedCheckborder();
	afx_msg void OnBnClickedExt();
	afx_msg void OnBnClickedToAllImage();
	afx_msg void OnViewBrushsizebar();
};


