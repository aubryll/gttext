// MainFrm.cpp : implementación de la clase CMainFrame
//

#include "stdafx.h"
#include "GT.h"
#include "GTXMLView.h"
#include "MainFrm.h"
#include "PropViews.h"
#include "GTView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_IMAGEBAR, &CMainFrame::OnViewImagebar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_IMAGEBAR, &CMainFrame::OnUpdateViewImagebar)
	ON_COMMAND(ID_VIEW_SELECTIONBRIGHT, &CMainFrame::OnViewSelectionbright)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SELECTIONBRIGHT, &CMainFrame::OnUpdateViewSelectionbright)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_BRIGHT_SLIDER, &CMainFrame::OnNMReleasedcaptureBrightSlider)
	ON_COMMAND(ID_VIEW_SENSITIVITY, &CMainFrame::OnViewSensitivity)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SENSITIVITY, &CMainFrame::OnUpdateViewSensitivity)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_ONFILL_SLIDER, &CMainFrame::OnNMReleasedcaptureOnfillSlider)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINSENS, &CMainFrame::OnDeltaposSpinsens)
	ON_COMMAND(ID_VIEW_ZOOMBAR, &CMainFrame::OnViewZoombar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMBAR, &CMainFrame::OnUpdateViewZoombar)
	ON_COMMAND_RANGE(ID_ZOOM_1,ID_ZOOM_FIXED, OnCombozoom)
	ON_CBN_SELCHANGE(IDC_COMBOZOOM, &CMainFrame::OnCbnSelchangeCombozoom)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BRUSHSIZEBAR, &CMainFrame::OnUpdateViewBrushsizebar)
	ON_CBN_SELCHANGE(IDC_COMBOPEN, &CMainFrame::OnCbnSelchangeCombopen)
	ON_BN_CLICKED(IDC_CHECKBORDER, &CMainFrame::OnBnClickedCheckborder)
	ON_BN_CLICKED(IDC_EXT, &CMainFrame::OnBnClickedExt)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // indicador de la línea de estado
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// Construcción y destrucción de CMainFrame

CMainFrame::CMainFrame()
{
	isSelBrightActive = 2;
	isSelSensActive = 2;
	isZoomActive = 2;
	isPenActive = 2;
	m_isLoaded = false;
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::HideToolBars()
{
	if(m_isLoaded)
	{
		AddPen(false);
		AddBrightBar(false);
		AddZoom(false);
		AddSensitivityBar(false);
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("No se pudo crear la barra de herramientas\n");
		return -1;      // no se pudo crear
	}

	if (! m_wndBrightBar.Create(this, IDD_DIALOGBRIGHT,
		CBRS_ALIGN_LEFT|CBRS_TOOLTIPS|CBRS_FLYBY| CBRS_SIZE_DYNAMIC, IDD_DIALOGBRIGHT))
	{
		TRACE0("Failed to create m_wndBrightBar\n");
		return -1;      // fail to create
	}

	CSliderCtrl* pCSlider = (CSliderCtrl*) m_wndBrightBar.GetDlgItem(IDC_BRIGHT_SLIDER);
	pCSlider->SetRange(0,255,true);
	pCSlider->SetPos(128);

	if (! m_wndSensitivityBar.Create(this, IDD_DIALOGONFILL,
		CBRS_ALIGN_LEFT|CBRS_TOOLTIPS|CBRS_FLYBY| CBRS_SIZE_DYNAMIC, IDD_DIALOGONFILL))
	{
		TRACE0("Failed to create m_wndSensitivityBar\n");
		return -1;      // fail to create
	}
	CSpinButtonCtrl* pCSpin = (CSpinButtonCtrl*) m_wndSensitivityBar.GetDlgItem(IDC_SPINSENS);
	pCSpin->SetRange(0,255);
	pCSpin->SetPos(10);
	pCSlider = (CSliderCtrl*) m_wndSensitivityBar.GetDlgItem(IDC_ONFILL_SLIDER);
	pCSlider->SetRange(0,255,true);
	pCSlider->SetPos(245);
	CEdit* pCEdit = (CEdit*) m_wndSensitivityBar.GetDlgItem(IDC_EDITSENS);
	pCEdit->SetWindowTextW(LPCTSTR(_T("10")));
		
	
	if (!m_wndImageBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndImageBar.LoadToolBar(IDR_TOOLBARIM))
	{
		TRACE0("No se pudo crear la barra de herramientas\n");
		return -1;      // no se pudo crear
	}

	if (! m_wndZoomBar.Create(this, IDD_DIALOGZOOM,
		CBRS_ALIGN_TOP|CBRS_TOOLTIPS|CBRS_FLYBY| CBRS_SIZE_DYNAMIC, IDD_DIALOGZOOM))
	{
		TRACE0("Failed to create m_wndZoomBar\n");
		return -1;      // fail to create
	}

	CComboBox	*m_zoomCombo = (CComboBox*) m_wndZoomBar.GetDlgItem(IDC_COMBOZOOM);

	CRect rect;
	m_wndToolBar.GetItemRect(0,&rect);
	rect.top = 3;
	rect.bottom = rect.top + 5;

	//  Fill the combo box
	CString szStyle;
	
	szStyle = "Zoom 1/2";
	m_zoomCombo->AddString((LPCTSTR)szStyle);
	szStyle = "Zoom x 1";
	m_zoomCombo->AddString((LPCTSTR)szStyle);
	szStyle = "Zoom x 2";
	m_zoomCombo->AddString((LPCTSTR)szStyle);
	szStyle = "Zoom x 4";
	m_zoomCombo->AddString((LPCTSTR)szStyle);
	szStyle = "Zoom x 8";
	m_zoomCombo->AddString((LPCTSTR)szStyle);
	szStyle = "Zoom x 16";
	m_zoomCombo->AddString((LPCTSTR)szStyle);
	szStyle = "Dynamic Fit";
	m_zoomCombo->AddString((LPCTSTR)szStyle);
	szStyle = "Custom Zoom";
	m_zoomCombo->AddString((LPCTSTR)szStyle);
	m_zoomCombo->SetCurSel((int)((ID_ZOOM - ID_SIZE_BASE)));

	if (! m_wndPenBar.Create(this, IDD_DIALOGBRUSH,
		CBRS_ALIGN_TOP|CBRS_TOOLTIPS|CBRS_FLYBY| CBRS_SIZE_DYNAMIC, IDD_DIALOGBRUSH))
	{
		TRACE0("Failed to create m_wndPenBar\n");
		return -1;      // fail to create
	}
	
	CComboBox *m_penCombo = (CComboBox*) m_wndPenBar.GetDlgItem(IDC_COMBOPEN);

	szStyle = "1 POINTS";
	m_penCombo->AddString((LPCTSTR)szStyle);
	szStyle = "3 POINTS";
	m_penCombo->AddString((LPCTSTR)szStyle);
	szStyle = "5 POINTS";
	m_penCombo->AddString((LPCTSTR)szStyle);
	szStyle = "10 POINTS";
	m_penCombo->AddString((LPCTSTR)szStyle);
	szStyle = "20 POINTS";
	m_penCombo->AddString((LPCTSTR)szStyle);
	szStyle = "40 POINTS";
	m_penCombo->AddString((LPCTSTR)szStyle);
	szStyle = "80 POINTS";
	m_penCombo->AddString((LPCTSTR)szStyle);
	m_penCombo->SetCurSel((int)((PEN_1)));

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("No se pudo crear la barra de estado\n");
		return -1;      // No se pudo crear
	}

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	m_wndImageBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndImageBar);

	m_wndBrightBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(& m_wndBrightBar);

	m_wndSensitivityBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(& m_wndSensitivityBar);

	m_wndZoomBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndZoomBar);

	m_wndPenBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndPenBar);

	m_isLoaded = true;

	//Initially we hide some bars

	ShowControlBar(& m_wndBrightBar, false, FALSE);
	ShowControlBar(& m_wndSensitivityBar, false, FALSE);
	ShowControlBar(& m_wndZoomBar, false, FALSE);
	ShowControlBar(& m_wndPenBar, false, FALSE);

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// create a splitter with 1 row, 2 columns
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}

	// add the first splitter pane (CTView)
	if (!m_wndSplitter.CreateView(0, 0,
		pContext->m_pNewViewClass, CSize(1000, 50), pContext))
	{
		TRACE0("Failed to create View pane\n");
		return FALSE;
	}

	// add the second splitter pane - which is a nested splitter with 2 rows
	if (!m_wndSplitterXML.CreateStatic(
		&m_wndSplitter,     // our parent window is the first splitter
		2, 1,               // the new splitter is 2 rows, 1 column
		WS_CHILD | WS_VISIBLE | WS_BORDER,  // style, WS_BORDER is needed
		m_wndSplitter.IdFromRowCol(0, 1)
			// new splitter is in the first row, 1st column of first splitter
	   ))
	{
		TRACE0("Failed to create nested splitter\n");
		return FALSE;
	}

	// add the first splitter pane - the CXMLView view in column 0  row 0- which is in the nested splitter with 2 rows
	if (!m_wndSplitterXML.CreateView(0, 0,
		RUNTIME_CLASS(CXMLView), CSize(0, 500), pContext))
	{
		TRACE0("Failed to create xml pane\n");
		return FALSE;
	}
		// add the second splitter pane - the default view in column 0 row 1
	if (!m_wndSplitterXML.CreateView(1, 0,
		RUNTIME_CLASS(CMetaDataView), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create proprt pane\n");
		return FALSE;
	}

	// activate the image view
	SetActiveView((CView*)m_wndSplitterXML.GetPane(0,0));

	return TRUE;
}

void CMainFrame::AddBrightBar(bool isAdded)
{
	BOOL bVisible;
	//bit xxxx1 for enabling state
	//bit xxx1x for visibility state
	//Check enabling change
	
	if(isAdded)
	{
		if (isSelBrightActive == 1 || isSelBrightActive == 3)
			return;
		isSelBrightActive = isSelBrightActive + 1;
		bVisible = (isSelBrightActive == 3);
		ShowControlBar(& m_wndBrightBar, bVisible, FALSE);
	}
	else
	{
		if (isSelBrightActive == 0 || isSelBrightActive == 2)
			return;
		isSelBrightActive = isSelBrightActive - 1;
		bVisible = (( m_wndBrightBar.GetStyle() & WS_VISIBLE) != 0);
		if(bVisible)
			isSelBrightActive = 2;
		else
			isSelBrightActive = 0;
		ShowControlBar(& m_wndBrightBar, isAdded, FALSE);
	}

	ShowControlBar(& m_wndBrightBar, bVisible && isAdded, FALSE);
	RecalcLayout();
	
	int wwith, wmin;
	m_wndSplitter.GetColumnInfo(0,wwith, wmin);
	if(isAdded)
		m_wndSplitter.SetColumnInfo(0,wwith-49,wmin);
	else
		m_wndSplitter.SetColumnInfo(0,wwith+49,wmin);
	m_wndSplitter.RecalcLayout();

}

void CMainFrame::AddSensitivityBar(bool isAdded)
{
	BOOL bVisible;
	//bit xxxx1 for enabling state
	//bit xxx1x for visibility state
	//Check enabling change
	
	if(isAdded)
	{
		if (isSelSensActive == 1 || isSelSensActive == 3)
			return;
		isSelSensActive = isSelSensActive + 1;
		bVisible = (isSelSensActive == 3);
		ShowControlBar(& m_wndSensitivityBar, bVisible, FALSE);
	}
	else
	{
		if (isSelSensActive == 0 || isSelSensActive == 2)
			return;
		isSelSensActive = isSelSensActive - 1;
		bVisible = (( m_wndSensitivityBar.GetStyle() & WS_VISIBLE) != 0);
		if(bVisible)
			isSelSensActive = 2;
		else
			isSelSensActive = 0;
		ShowControlBar(& m_wndSensitivityBar, isAdded, FALSE);
	}

	ShowControlBar(& m_wndSensitivityBar, bVisible && isAdded, FALSE);
	RecalcLayout();
	int wwith, wmin;
	m_wndSplitter.GetColumnInfo(0,wwith, wmin);
	if(isAdded)
		m_wndSplitter.SetColumnInfo(0,wwith-49,wmin);
	else
		m_wndSplitter.SetColumnInfo(0,wwith+49,wmin);
	m_wndSplitter.RecalcLayout();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: modificar aquí la clase Window o los estilos cambiando
	//  CREATESTRUCT cs

	return TRUE;
}


// Diagnósticos de CMainFrame

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// Controladores de mensajes de CMainFrame

void CMainFrame::OnViewImagebar()
{
	BOOL bVisible = ((m_wndImageBar.GetStyle() & WS_VISIBLE) != 0);

	ShowControlBar(&m_wndImageBar, !bVisible, FALSE);
	RecalcLayout();
}

void CMainFrame::OnUpdateViewImagebar(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_wndImageBar.GetStyle() & WS_VISIBLE) != 0);
}

void CMainFrame::OnViewSelectionbright()
{
	BOOL bVisible = (( m_wndBrightBar.GetStyle() & WS_VISIBLE) != 0);

	ShowControlBar(& m_wndBrightBar, !bVisible, FALSE);
	RecalcLayout();
}

void CMainFrame::OnUpdateViewSelectionbright(CCmdUI *pCmdUI)
{
	if((isSelBrightActive == 1) || (isSelBrightActive == 3) )
	{
		pCmdUI->Enable(true);
		pCmdUI->SetCheck(( m_wndBrightBar.GetStyle() & WS_VISIBLE) != 0);
		
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CMainFrame::OnNMReleasedcaptureBrightSlider(NMHDR *pNMHDR, LRESULT *pResult)
{	
	*pResult = 0;

	CSliderCtrl* pCSlider = (CSliderCtrl*) m_wndBrightBar.GetDlgItem(IDC_BRIGHT_SLIDER);
	CGTView* gtView = (CGTView*) m_wndSplitter.GetPane(0,0);
	if(gtView != NULL)
	{
		gtView->SetBright(255-pCSlider->GetPos());
		gtView->Invalidate(false);
	}
}

void CMainFrame::OnViewSensitivity()
{
	BOOL bVisible = (( m_wndSensitivityBar.GetStyle() & WS_VISIBLE) != 0);

	ShowControlBar(& m_wndSensitivityBar, !bVisible, FALSE);
	RecalcLayout();
}

void CMainFrame::OnUpdateViewSensitivity(CCmdUI *pCmdUI)
{
	if((isSelSensActive == 1) || (isSelSensActive == 3) )
	{
		pCmdUI->Enable(true);
		pCmdUI->SetCheck(( m_wndSensitivityBar.GetStyle() & WS_VISIBLE) != 0);
		
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CMainFrame::OnNMReleasedcaptureOnfillSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	short pos;
	CSliderCtrl* pCSlider = (CSliderCtrl*) m_wndSensitivityBar.GetDlgItem(IDC_ONFILL_SLIDER);
	CSpinButtonCtrl* pCSpin = (CSpinButtonCtrl*) m_wndSensitivityBar.GetDlgItem(IDC_SPINSENS);
	CGTView* gtView = (CGTView*) m_wndSplitter.GetPane(0,0);
	CEdit* pCEdit = (CEdit*) m_wndSensitivityBar.GetDlgItem(IDC_EDITSENS);
	CString posS;

	if(gtView != NULL)
	{
		pos = 255-pCSlider->GetPos();
		pCSpin->SetPos(pos);
		gtView->SetSesitivity(pos);
		posS = "";
		if(pos==0)
			posS = "0";
		while(pos != 0)
		{
			posS = (wchar_t((pos%10)+48))+ posS;
			pos = short(pos/10);
		}
		pCEdit->SetWindowTextW(LPCTSTR(posS));
		pCEdit->UpdateData(false);
	}
}

void CMainFrame::OnDeltaposSpinsens(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
		*pResult = 0;
	short pos;
	CSpinButtonCtrl* pCSpin = (CSpinButtonCtrl*) m_wndSensitivityBar.GetDlgItem(IDC_SPINSENS);
	CSliderCtrl* pCSlider = (CSliderCtrl*) m_wndSensitivityBar.GetDlgItem(IDC_ONFILL_SLIDER);
	CEdit* pCEdit = (CEdit*) m_wndSensitivityBar.GetDlgItem(IDC_EDITSENS);
	CGTView* gtView = (CGTView*) m_wndSplitter.GetPane(0,0);
	CString posS;
	if(gtView != NULL)
	{
		pos = pCSpin->GetPos();
		gtView->SetSesitivity(pos);
		pCSlider->SetPos(255-pos);
		posS = "";
		if(pos==0)
			posS = "0";
		while(pos != 0)
		{
			posS = (wchar_t((pos%10)+48))+ posS;
			pos = short(pos/10);
		}
		pCEdit->SetWindowTextW(LPCTSTR(posS));
		pCEdit->UpdateData(false);
	}
}

void CMainFrame::AddZoom(bool isEnable)
{
	BOOL bVisible = (( m_wndZoomBar.GetStyle() & WS_VISIBLE) != 0);
	if(isEnable)
	{
		if((isZoomActive == 0)|| (isZoomActive == 2)) 
			isZoomActive++;
		ShowControlBar(& m_wndZoomBar, (isZoomActive == 3), FALSE);
	}
	else
	{
		m_wndZoomBar.ActivateTopParent();
		ShowControlBar(& m_wndZoomBar, isEnable, FALSE);
		if((isZoomActive == 1)|| (isZoomActive == 3)) 
		{
			if(bVisible)
				isZoomActive = 2;
			else
				isZoomActive = 0;
		}
	}
	RecalcLayout();
}

void CMainFrame::AddPen(bool isEnable)
{
	BOOL bVisible = (( m_wndPenBar.GetStyle() & WS_VISIBLE) != 0);
	if(isEnable)
	{
		if((isPenActive == 0)|| (isPenActive == 2)) 
			isPenActive++;
		ShowControlBar(& m_wndPenBar, (isPenActive == 3), FALSE);
	}
	else
	{
		ShowControlBar(& m_wndPenBar, isEnable, FALSE);
		if((isPenActive == 1)|| (isPenActive == 3)) 
		{
			if(bVisible)
				isPenActive = 2;
			else
				isPenActive = 0;
		}
	}

	RecalcLayout();
	m_wndSplitter.RecalcLayout();
}

void CMainFrame::OnViewZoombar()
{
	BOOL bVisible = (( m_wndZoomBar.GetStyle() & WS_VISIBLE) != 0);

	ShowControlBar(& m_wndZoomBar, !bVisible, FALSE);
	RecalcLayout();
}

void CMainFrame::OnUpdateViewZoombar(CCmdUI *pCmdUI)
{
	if((isZoomActive == 1) || (isZoomActive == 3) )
	{
		pCmdUI->Enable(true);
		pCmdUI->SetCheck(( m_wndZoomBar.GetStyle() & WS_VISIBLE) != 0);
		
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CMainFrame::OnCbnSelchangeCombozoom()
{
	CGTView* gtView = (CGTView*) m_wndSplitter.GetPane(0,0);
	CComboBox* pCBox = (CComboBox*)m_wndZoomBar.GetDlgItem(IDC_COMBOZOOM);
	int nIndex = pCBox->GetCurSel();
	if(gtView == NULL)
		return;
	gtView->OnChangeSize(UINT(nIndex+ID_SIZE_BASE));
}


void CMainFrame::OnCombozoom(UINT nID)
{
	CComboBox* pCBox = (CComboBox*)m_wndZoomBar.GetDlgItem(IDC_COMBOZOOM);
	pCBox->SetCurSel((int)((nID - ID_SIZE_BASE)));
}

void CMainFrame::OnUpdateViewBrushsizebar(CCmdUI *pCmdUI)
{
	if((isPenActive == 1) || (isPenActive == 3) )
	{
		pCmdUI->Enable(true);
		pCmdUI->SetCheck(( m_wndPenBar.GetStyle() & WS_VISIBLE) != 0);
		
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CMainFrame::OnCbnSelchangeCombopen()
{
	CGTView* gtView = (CGTView*) m_wndSplitter.GetPane(0,0);
	CComboBox* pCBox = (CComboBox*)m_wndPenBar.GetDlgItem(IDC_COMBOPEN);
	int nIndex = pCBox->GetCurSel();
	if(gtView == NULL)
		return;
	PenEnum pen;
	switch(nIndex)
	{
	case 0:pen=PEN_1;
			break;
	case 1:pen=PEN_3;
			break;
	case 2:pen=PEN_5;
			break;
	case 3:pen=PEN_10;
			break;
	case 4:pen=PEN_20;
			break;
	case 5:pen=PEN_40;
			break;
	case 6:pen=PEN_80;
			break;
	}
	
	gtView->OnChangePen(pen);
}

void CMainFrame::OnBnClickedCheckborder()
{
	CGTView* gtView = (CGTView*) m_wndSplitter.GetPane(0,0);
	CEdit* pCCheck = (CEdit*)m_wndSensitivityBar.GetDlgItem(IDC_CHECKBORDER);
	gtView->SetBorder(true);
}

void CMainFrame::OnBnClickedExt()
{
	CGTView* gtView = (CGTView*) m_wndSplitter.GetPane(0,0);
	CEdit* pCCheck = (CEdit*)m_wndSensitivityBar.GetDlgItem(IDC_EXT);
	gtView->SetExt();
}
