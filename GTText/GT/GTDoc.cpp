// GTDoc.cpp : implementation of the CGTDoc class
//

#include "stdafx.h"
#include "GT.h"
#include "GTDoc.h"
#include "GTView.h"
#include "GTXMLView.h"
#include "MainFrm.h"
#include "InfoDlg.h"
#include "PropViews.h"



#ifndef TESSDLL_IMPORTS
#define TESSDLL_IMPORTS
#ifndef XPCOMPATIBLE
#include "baseapi.h"
#endif
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGTDoc

IMPLEMENT_DYNCREATE(CGTDoc, CDocument)

BEGIN_MESSAGE_MAP(CGTDoc, CDocument)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_1,&CGTDoc::OnUpdateSizeHalf)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_1X,&CGTDoc::OnUpdateSizeOriginal)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_2X,&CGTDoc::OnUpdateSizeDouble)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_4X,&CGTDoc::OnUpdateSizeX4)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_8X,&CGTDoc::OnUpdateSizeX8)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_16X,&CGTDoc::OnUpdateSizeX16)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_TOOL,&CGTDoc::OnUpdateSizeZoom)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_FIT,&CGTDoc::OnUpdateSizeFill)
	ON_COMMAND_RANGE(ID_ZOOM_1,ID_ZOOM_FIXED,&CGTDoc::OnChangeSize)

	ON_COMMAND(ID_VIEW_SHADE, &CGTDoc::OnViewShade)
	ON_COMMAND(ID_VIEW_OUTLINE, &CGTDoc::OnViewOutline)
	ON_COMMAND(ID_VIEW_CORE, &CGTDoc::OnViewCore)
	ON_COMMAND(ID_EDIT_SHADE, &CGTDoc::OnEditShade)
	ON_COMMAND(ID_EDIT_OUTLINE, &CGTDoc::OnEditOutline)
	ON_COMMAND(ID_EDIT_CORE, &CGTDoc::OnEditCore)
		
	ON_UPDATE_COMMAND_UI(ID_EDIT_SHADE, &CGTDoc::OnUpdateEditShade)
	ON_UPDATE_COMMAND_UI(ID_EDIT_OUTLINE, &CGTDoc::OnUpdateEditOutline)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CORE, &CGTDoc::OnUpdateEditCore)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHADE, &CGTDoc::OnUpdateViewShade)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTLINE, &CGTDoc::OnUpdateViewOutline)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CORE, &CGTDoc::OnUpdateViewCore)
	ON_COMMAND(ID_TOOLS_PIXELSELECT, &CGTDoc::OnToolsPixelselect)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_PIXELSELECT, &CGTDoc::OnUpdateToolsPixelselect)
	ON_COMMAND(ID_TOOLS_REGIONSELECT, &CGTDoc::OnToolsRegionselect)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_REGIONSELECT, &CGTDoc::OnUpdateToolsRegionselect)
	ON_COMMAND(ID_EDIT_UNDO, &CGTDoc::OnEditUndo)
	ON_COMMAND(ID_ZOOM_TOOL, &CGTDoc::OnZoom)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CGTDoc::OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CGTDoc::OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CGTDoc::OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_COPY, &CGTDoc::OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CGTDoc::OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, &CGTDoc::OnUpdateFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CGTDoc::OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CGTDoc::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CGTDoc::OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_CUT, &CGTDoc::OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CGTDoc::OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_UNDOXML, &CGTDoc::OnContextgtUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDOXML, &CGTDoc::OnUpdateContextGtUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDOXML, &CGTDoc::OnUpdateContextgtRedo)
	ON_COMMAND(ID_EDIT_REDOXML, &CGTDoc::OnEditRedoxml)
	ON_COMMAND(ID_IMAGE_INFO, &CGTDoc::OnImageInfo)
	ON_COMMAND(ID_EDIT_INVERT, &CGTDoc::OnEditInvert)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INVERT, &CGTDoc::OnUpdateEditInvert)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_INVERTALL, &CGTDoc::OnUpdateToolsInvertall)
	ON_COMMAND(ID_TOOLS_INVERTALL, &CGTDoc::OnToolsInvertall)
	ON_COMMAND(ID_FILE_PREFERENCES, &CGTDoc::OnFilePreferences)
	ON_COMMAND(ID_VIEW_POINT, &CGTDoc::OnViewShowpoint)
	ON_UPDATE_COMMAND_UI(ID_VIEW_POINT, &CGTDoc::OnUpdateViewPoint)
	ON_COMMAND(ID_FILE_EXPORTSELECTION, &CGTDoc::OnFileExportselection)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORTSELECTION, &CGTDoc::OnUpdateFileExportselection)
	ON_UPDATE_COMMAND_UI(ID_EXPORTSELECTION_BLACK32959, &CGTDoc::OnUpdateFileExportselection)
	ON_UPDATE_COMMAND_UI(ID_EXPORTSELECTION_COLOR32960, &CGTDoc::OnUpdateFileExportselection)
	ON_COMMAND(ID_EXPORTSELECTION_COLOR32960, &CGTDoc::OnExportselectionColor)
	ON_COMMAND(ID_EXPORTSELECTION_BLACK32959, &CGTDoc::OnExportselectionBlack)
	ON_COMMAND(ID_COPYOCRTEXT_USESELECTION, &CGTDoc::OnFileSelectionOCR)
	ON_COMMAND(ID_COPYOCRTEXT_ALLIMAGE, &CGTDoc::OnExportColorselectionText)
	ON_UPDATE_COMMAND_UI(ID_COPYOCRTEXT_USESELECTION, &CGTDoc::OnUpdateFileSelectionOCR)
	ON_UPDATE_COMMAND_UI(ID_COPYOCRTEXT_ALLIMAGE, &CGTDoc::OnUpdateFileCopyTextAll)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_AREATEXTCOPIER, &CGTDoc::OnUpdateToolsAreatextcopier)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_DISABLE_POPUP, &CGTDoc::OnUpdateDisablePopUp)
	ON_COMMAND(ID_TOOLS_AREATEXTCOPIER, &CGTDoc::OnToolsAreatextcopier)
	ON_COMMAND(ID_TOOLS_DISABLE_POPUP, &CGTDoc::OnToolsDisablePopUp)
	ON_COMMAND(ID_HELP_HELP, &CGTDoc::OnHelpHelp)
END_MESSAGE_MAP()


// CGTDoc construction/destruction

CGTDoc::CGTDoc()
{
	m_strGlyphPathFile = "res/gl-000000000-20090126T000000000-0000";
	m_strPagePathFile = "res/pc-00006664-20081120T12402926-4796";
	m_strLinkPathFile = "res/gt-00006664-20081107T11432018-0000";
	m_strImagePathFile = "";
	m_pDOMLinkDoc = NULL;
	m_pDOMPageDoc = NULL;
	m_pCurrentSection = NULL;
	m_pointSelected = CPoint(0,0);
	m_isLocked = false;
	m_newLinkPathFile = _T("gt-00006664-20081107T11432018-0815.xml");
	m_edition_state = EDIT_NONE;
	m_isDirty = false;
	m_tool = OCR_TOOL;
	m_isBackuped = 0;
	m_isXMLBackuped = 0;
	m_pcDefaultFolder = "pc";
	m_glDefaultFolder = "gl";
	m_currentFolder = "";
	m_showPointBar = true;
	m_idCount = 0;
	m_onGlyphState = EDIT_NONE;
	m_languageOCR = "eng";
	m_isSelectionOCR = false;
}

CGTDoc::~CGTDoc()
{
	Release();
}

bool CGTDoc::IsPointBar()
{
	return m_showPointBar;
}

CString CGTDoc::GetDefPcFolder()
{
	return m_pcDefaultFolder;
}

CString CGTDoc::GetDefGlFolder()
{
	return m_glDefaultFolder;
}

CString CGTDoc::GetCurrentFolder()
{
	return m_currentFolder;
}

void CGTDoc::SetCurrentlFolder(CString folder)
{
	m_currentFolder = folder;
}

void CGTDoc::SetDefPcFolder(CString folder)
{
	m_pcDefaultFolder = folder;
}

void CGTDoc::SetDefGlFolder(CString folder)
{
	m_glDefaultFolder = folder;
}

void CGTDoc::Release(bool deep)
{
	SectionPtrs* data;
	
	if(deep)
	{
		if(m_pDOMLinkDoc != NULL)
		{
			m_pDOMLinkDoc.Release();
		}
		if(m_pDOMPageDoc != NULL)
		{
			m_pDOMPageDoc.Release();
		}
		if(m_pDOMGlyphDoc != NULL)
		{
			m_pDOMGlyphDoc.Release();
		}
		if(m_pDOMLinkDoc != NULL)
		{
			m_pDOMLinkDocOld.Release();
		}
		if(m_pDOMPageDocOld != NULL)
		{
			m_pDOMPageDocOld.Release();
		}
		if(m_pDOMGlyphDocOld != NULL)
		{
			m_pDOMGlyphDocOld.Release();
		}
		m_imgOriginal.Destroy();

		m_pDOMLinkDocOld = NULL;
		m_pDOMPageDocOld = NULL;
		m_pDOMGlyphDocOld = NULL;
		
		m_edition_state = EDIT_NONE;
		m_tool = OCR_TOOL;
		m_imgSelection = CImageSelection();
		m_pDOMLinkDoc = NULL;
		m_pDOMPageDoc = NULL;
		m_pDOMGlyphDoc = NULL;
		m_pointSelected = CPoint(0,0);
		m_isLocked = false;
		m_isLoad = true;
		m_imgSelectionOld = CImageSelection();
		m_isBackuped = 0;
		m_isXMLBackuped = 0;
		m_idCount = 0;
		m_onGlyphState = EDIT_NONE;
	}
	
	if (!m_sectionsMap.empty())
	{
		std::map <MSXML2::IXMLDOMNodePtr,SectionPtrs*>::iterator it;
		std::map <NodeName,MSXML2::IXMLDOMNodePtr>::iterator itPtrs;
		for(it = m_sectionsMap.begin();it != m_sectionsMap.end();it++)
		{
			data = (*it).second;
			if(data != NULL)
			{
				for(itPtrs = data->ptrsMap.begin();itPtrs != data->ptrsMap.end();itPtrs++)
					if((*itPtrs).second != NULL)
						(*itPtrs).second.Release();
				data->ptrsMap.clear();
				delete data;
			}
		}
		m_sectionsMap.clear();
	}

	m_copyVector.clear();
	m_pCurrentSection = NULL;
}

EditEnum CGTDoc::GetOnGlyphState()
{
	return m_onGlyphState;
}

void  CGTDoc::SetOnGlyphState(EditEnum state)
{
	m_onGlyphState = state;
}


void CGTDoc::OnCloseDocument()
{
	TCHAR strPath[ MAX_PATH ];

	// Get the special folder path.
	SHGetSpecialFolderPathW(
			0,       
			strPath, // String buffer.
			CSIDL_LOCAL_APPDATA, // CSLID of folder
			TRUE );

	CString	strFileName = CString(strPath) + CString(_T("\\gt-props"));
	std::wofstream outfile;
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView != NULL) {
		outfile.open(strFileName.GetString());
		outfile << GetOCRLanguage().GetString()<<L"\n";
		if(gtView->GetNoOCRPopUp())
			outfile << L"NoPopUp\n";
		else
			outfile << L"YesPopUp\n";
		if(gtView->GetLatestImport() == snapshot)
			outfile << L"snapshot\n";
		else if(gtView->GetLatestImport() == imagefile)
			outfile << L"imagefile\n";
		else if(gtView->GetLatestImport() == scanner)
			outfile << L"scanner\n";
		else if(gtView->GetLatestImport() == noimport)
			outfile << L"noimport\n";
		outfile.close();
	}
    Release();
	CDocument::OnCloseDocument();
}

void CGTDoc::OnImageInfo()
{
	CInfoDlg infoDlg(*GetImage());
	infoDlg.DoModal();
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView == NULL)
		return;
	gtView->OnToolsMakeBW();
}

void CGTDoc::SetDirty(bool isDirty)
{
	m_isDirty = isDirty;
}

bool CGTDoc::IsDirty()
{
	return m_isDirty;
}

void CGTDoc::SetLock(bool lock)
{
	m_isLocked = lock;
}

EditEnum  CGTDoc::GetEditState()
{
	return m_edition_state;
}

void  CGTDoc::SetToolState(ToolEnum tool)
{
	m_tool = tool;
}

ToolEnum  CGTDoc::GetToolState()
{
	return m_tool;
}

void  CGTDoc::SetEditState(EditEnum state)
{
	m_edition_state = state;
}

bool CGTDoc::GetLock()
{
	return m_isLocked;
}

int  CGTDoc::UpdateIdCounter(CString id)
{
	int i = 1,value = 0,factor = 1;
	char number = '0';
	
	while(i<=id.GetLength())
	{
		number = char(*id.Mid(id.GetLength()-i).GetString());
		if(number>=48 && number<58)
		{
			value = value + (number-48)*factor;
			factor = factor * 10;
		}
		else
			break;
		i++;
	}
		
	if(value>m_idCount)
		m_idCount = value;
	
	if(id.GetLength() == 0)
		m_idCount ++;
	
	return m_idCount;
}

BOOL CGTDoc::SwitchToView(CRuntimeClass* pNewViewClass)
{

  CFrameWnd* pMainWnd = (CFrameWnd*)AfxGetMainWnd();
  CView* pOldActiveView = pMainWnd->GetActiveView();

  // If we're already displaying this kind of view, no need to go further.
  if (pOldActiveView->IsKindOf(pNewViewClass))
    return TRUE;

  CSplitterWnd* pSplitter = (CSplitterWnd *)pOldActiveView->GetParent();
  int row, col,r[1],c[1];

  pSplitter->IsChildPane(pOldActiveView, r, c);
  row = *r;
  col = *c;
  CRect viewrect;
  pOldActiveView->GetWindowRect(&viewrect);

  // set flag so that document will not be deleted when view is destroyed
  m_bAutoDelete = FALSE;  
  // Delete existing view
  pOldActiveView->DestroyWindow();
  // set flag back to default
  m_bAutoDelete = TRUE;

  // Create new view           
  CCreateContext context;
  context.m_pNewViewClass = pNewViewClass;
  context.m_pCurrentDoc = this;
  context.m_pNewDocTemplate = NULL;
  context.m_pLastView = NULL;
  context.m_pCurrentFrame = NULL;
  if (!pSplitter->CreateView(row, col, pNewViewClass, viewrect.Size(),&context))
    return FALSE;

  // Set active
  CView* pNewView = (CView *)pSplitter->GetPane(row, col);
  pSplitter->GetParentFrame()->SetActiveView((CView *)pSplitter->GetPane(1, 0));
 
  pSplitter->RecalcLayout();
  pNewView->SendMessage(WM_PAINT);
  pSplitter->GetParentFrame()->SetActiveView((CView *)pSplitter->GetPane(1, 0));
  return TRUE;
}

SectionPtrs* CGTDoc::GetCurrentSection()
{
	return m_pCurrentSection;
}

BOOL CGTDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	
	CString file = lpszPathName;
	if(file.Find(_T("gt-")) != -1)
	{
		DeleteContents();
		Release();
		SetLinkPath(CString(lpszPathName));
		SetCurrentlFolder(CString(lpszPathName));
		SetLoad(true);
		return TRUE;
	}
	else
	{
		if(AfxMessageBox(_T("This file is not a valid (gt-*.xml)\n Do you want to open it anyway?"),MB_YESNO) == IDYES)
		{
			DeleteContents();
			Release();
			SetLinkPath(CString(lpszPathName));
			SetCurrentlFolder(CString(lpszPathName));
			SetLoad(true);
			return TRUE;
		}
		else
		{
			SetLoad(false);
			
			return FALSE;
		}
	}
}


BOOL CGTDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	SetModifiedFlag(FALSE);
	CString strFileName = LPCTSTR(lpszPathName);
	CString file,originalName;
	CString pathdir = strFileName.Left(strFileName.ReverseFind(wchar_t('\\'))+ 1);
	originalName = file = strFileName.Right(strFileName.GetLength()-strFileName.ReverseFind(wchar_t('\\'))- 1);

						
	if (file.Find(_T("gt-")) == -1)
		m_strPathName = pathdir + _T("gt-") + file;
	else
	{
		m_strPathName = pathdir + file;
	}
	//LPCTSTR lpszPathNameNew = m_strPathName.copy;
	//lstrcpy(LPWSTR(lpszPathName), lpszPathNameNew);
	

	//lpszPathName = m_strPathName;
	if(strFileName.Find(_T(".xml")) <= 0)
		SetLinkPath(m_strPathName + _T(".xml"));
	else
		SetLinkPath(m_strPathName);

	if(!GetLock())
	{
		LPTSTR lpsz = new TCHAR[m_strPathName.GetLength()+1];
		_tcscpy(lpsz, m_strPathName);
		lpszPathName = lpsz;
		SetPathName(lpszPathName,FALSE);
		delete[] lpsz;
		OnFileSave();
		if (originalName.Find(_T("gt-")) == -1)
			return FALSE;
		else
			return TRUE;	
	}
	else
	{
		return FALSE;
	}
}

CImageSelection* CGTDoc::GetImageSelection()
{
	return &m_imgSelection;
}

BOOL CGTDoc::OnNewDocument()
{
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView == NULL)
		return FALSE;
	CXMLView* xmlView = (CXMLView*) GetNextView(pos);
	if(xmlView == NULL)
		return FALSE;
	CString file;
	file = m_strImagePathFile;
	m_strImagePathFile = "";
	m_isLoad = true;
	SectionPtrs *section =  GetCurrentSection();
	if(IsDirty())
	{
		if(section != NULL)
			if(section->type == glyph)
				SavePoints();
		SetDirty(false);
	}
	if(theApp.GetImportType() == indetermined) {
		TCHAR strPath[ MAX_PATH ];

		// Get the special folder path.
		SHGetSpecialFolderPathW(
				0,       
				strPath, // String buffer.
				CSIDL_LOCAL_APPDATA, // CSLID of folder
				TRUE );

		CString	strFileName = CString(strPath) + CString(_T("\\gt-props"));
		std::wifstream infile(strFileName.GetString());
		if(infile.is_open()) {
			if( infile.good() ) {
				
				std::wstring line;
				infile>>line;
				SetOCRLanguage(CString(line.c_str()));
				infile>>line;
				if(line.compare(L"NoPopUp") == 0)
					gtView->SetNoOCRPopUp();
				infile>>line;
				if(line.compare(L"snapshot") == 0) {
					theApp.SetImportType(snapshot);					
				}
				else if(line.compare(L"image") == 0) {
					theApp.SetImportType(imagefile);
				}
				else if(line.compare(L"scanner") == 0) {
					theApp.SetImportType(noimport);
				}
				infile.close();
			}
			else
				theApp.SetImportType(imagefile);
		}
		else
			theApp.SetImportType(imagefile);
	}

	theApp.SetActive(true);

	if (m_isLoad)
	{
		if (!CDocument::OnNewDocument())
			return FALSE;
		DeleteContents();
		Release();
		m_strGlyphPathFile = "res/gl-000000000-20090126T000000000-0000";
		m_strPagePathFile = "res/pc-00006664-20081120T12402926-4796";
		m_strLinkPathFile = _T("gt-00006664-20081107T11432018-0815.xml");
		m_newLinkPathFile = _T("gt-00006664-20081107T11432018-0815.xml");
		m_strLinkPathFileOld = _T("gt-00006664-20081107T11432018-0815.xml");
		m_currentFolder = _T("");


		return TRUE;
	}
	else
	{
		m_strImagePathFile = file;
		gtView->SetActiveWindow();
		
		if(m_strImagePathFile.IsEmpty())
		{
			SetModifiedFlag(FALSE);
			return TRUE;
		}
		else
		{
			xmlView->OnTvnSelchangedXmltree(0,0);
			return FALSE;
		}
	}
}

void CGTDoc::SetCurrentSection(SectionPtrs* section)
{
	m_pCurrentSection = section;
}

void CGTDoc::SetLoad(bool isLoad)
{
	m_isLoad  = isLoad;
}

bool CGTDoc::IsLoad()
{
	return m_isLoad;
}

void  CGTDoc::Backup(int nView)
{
	if(nView == 0)
	{
		m_imgSelection.LoadMaskPoints(m_edition_state,CRect(0,0,m_imgOriginal.GetWidth(),m_imgOriginal.GetHeight()));
		m_imgSelectionOld = CImageSelection(m_imgSelection);
		m_isBackuped = 1;
	}
	else if(nView == 1)
	{
		if(m_pDOMPageDoc != NULL)
			m_pDOMPageDocOld = m_pDOMPageDoc->cloneNode(VARIANT_TRUE);
		if(m_pDOMGlyphDoc != NULL)
			m_pDOMGlyphDocOld = m_pDOMGlyphDoc->cloneNode(VARIANT_TRUE);
		if(m_pDOMLinkDoc != NULL)
			m_pDOMLinkDocOld = m_pDOMLinkDoc->cloneNode(VARIANT_TRUE);
		m_isXMLBackuped = 1;
	}
}

BOOL CGTDoc::UpdateSection(MSXML2::IXMLDOMNodePtr pNode,NodeName nodeName)
{
	MSXML2::IXMLDOMNodePtr pSectionNode = pNode;
	if(pNode != NULL)
	{
		std::map<MSXML2::IXMLDOMNodePtr,SectionPtrs*>::iterator it;

		for(it = m_sectionsMap.begin();it != m_sectionsMap.end();it++)
		{
			CString strFile1 = W2T(_bstr_t(pSectionNode->GetnodeName()));
			if (it->first->GetnodeName() == pSectionNode->GetnodeName())
			{
				CString strFile1,strFile2;
				if (it->first->Getattributes()->nextNode() != NULL) 
					strFile1 = W2T(_bstr_t(it->first->Getattributes()->nextNode()->GetnodeValue()));
				if(pSectionNode->Getattributes()->nextNode() != NULL)
					strFile2 = W2T(_bstr_t(pSectionNode->Getattributes()->nextNode()->GetnodeValue()));
				if(strFile1.Compare(strFile2) == 0)
				{
					m_pCurrentSection = (SectionPtrs *)(it->second);
					return S_OK;
				}
			}
		}
	}
	else
	{
		std::map<MSXML2::IXMLDOMNodePtr,SectionPtrs*>::iterator it;
		if(nodeName != none)
		{
			for(it = m_sectionsMap.begin();it != m_sectionsMap.end();it++)
			{
				if (it->second->type == nodeName)
				{
					m_pCurrentSection = (SectionPtrs *)(it->second);
					return S_OK;
				}
			}
		}
	}
	return S_FALSE;
}

void CGTDoc::SetGlyphPath(CString path)
{
	m_strGlyphPathFile = path;
}

void CGTDoc::SetPagePath(CString path)
{
	m_strPagePathFile = path;
}

void CGTDoc::SetLinkPath(CString path)
{
	m_strLinkPathFile = path;
}

void CGTDoc::SetImagePath(CString path)
{
	m_strImagePathFile = path;
}

CString CGTDoc::GetImagePath()
{
	return m_strImagePathFile;
}

CString CGTDoc::GetGlyphPath()
{
	return m_strGlyphPathFile;
}

CString CGTDoc::GetPagePath()
{
	return m_strPagePathFile;
}

CString CGTDoc::GetLinkPath()
{
	return m_strLinkPathFile;
}

CString CGTDoc::GetNewDocPath()
{
	return m_newLinkPathFile;
}

void   CGTDoc::SetLinkDom(MSXML2::IXMLDOMDocumentPtr docPtr)
{
	m_pDOMLinkDoc = docPtr;
}

void   CGTDoc::SetGlyphDom(MSXML2::IXMLDOMDocumentPtr docPtr)
{
	m_pDOMGlyphDoc = docPtr;
}

void CGTDoc::SetSelectedPoint(CPoint point)
{
	m_pointSelected = point;
}

void CGTDoc::SetZoomPoint(CPoint imagePoint,CPoint sreenPoint)
{
	m_zoomImagePoint = imagePoint;
	m_zoomSreenPoint = sreenPoint;
}

void CGTDoc::GetZoomPoint(CPoint &imagePoint,CPoint &sreenPoint)
{
	CPoint imagePointcopy = m_zoomImagePoint,sreenPointCopy = m_zoomSreenPoint;
	imagePoint = imagePointcopy;
	sreenPoint = sreenPointCopy;
}

void CGTDoc::SetPenPoint(CPoint penPoint)
{
	m_lastPenPoint = m_penPoint;
	m_penPoint = penPoint;
}

CPoint CGTDoc::GetLastPenPoint()
{
	return m_lastPenPoint;
}


CPoint CGTDoc::GetPenPoint()
{
	return m_penPoint;
}

CPoint CGTDoc::GetSelectedPoint()
{
	return m_pointSelected;
}

void   CGTDoc::SetPageDom(MSXML2::IXMLDOMDocumentPtr docPtr)
{
	m_pDOMPageDoc = docPtr;
}

CImage* CGTDoc::GetImage()
{
	return &m_imgOriginal;
}

MSXML2::IXMLDOMDocumentPtr CGTDoc::GetLinkDom()
{
	return m_pDOMLinkDoc;
}

MSXML2::IXMLDOMDocumentPtr CGTDoc::GetPageDom()
{
	return m_pDOMPageDoc;
}

MSXML2::IXMLDOMDocumentPtr CGTDoc::GetGlyphDom()
{
	return m_pDOMGlyphDoc;
}

void CGTDoc::AddNodeToCurrSection(MSXML2::IXMLDOMNodePtr pNode,NodeName nodeName)
{
	if(m_pCurrentSection && !m_isLocked)
		m_pCurrentSection->ptrsMap.insert(PtrPair(nodeName,pNode));
}

MSXML2::IXMLDOMNodePtr CGTDoc::GetNodeFromCurrSection(NodeName nodeName)
{
	MSXML2::IXMLDOMNodePtr node;
	if(m_pCurrentSection != NULL)
	{
		if (m_pCurrentSection->ptrsMap.find(nodeName) != m_pCurrentSection->ptrsMap.end())
		{
			node = (*m_pCurrentSection->ptrsMap.find(nodeName)).second;
			return node;
		}
	}
	return NULL;
}

void CGTDoc::DeleteNodeFromCurrSection(NodeName nodeName)
{
	MSXML2::IXMLDOMNodePtr node;
	if(m_pCurrentSection != NULL)
		m_pCurrentSection->ptrsMap.erase(nodeName);
}

SectionPtrs* CGTDoc::AddSection(MSXML2::IXMLDOMNodePtr pNodeKey,NodeName nodeName)
{
	if(!m_isLocked)
	{
		SectionPtrs *newSec = new SectionPtrs;
		newSec->type = nodeName;
		if(pNodeKey != NULL)
		{
			m_sectionsMap.insert(SectionPair(pNodeKey,newSec));
			m_pCurrentSection = newSec;
			return newSec;
		}
	}

	return NULL;
}

void CGTDoc::SavePoints()
{
	MSXML2::IXMLDOMNodePtr pNewNodeGlyph,pNodeGlyph,pParent,pNewNode,pGlyphParent,pGroup;
	MSXML2::IXMLDOMElementPtr pNewElem,pCoorElem,pPointEl;	
	MSXML2::IXMLDOMNodePtr pPageGlyphNode;
	MSXML2::IXMLDOMNodeListPtr pNodeList;
	MSXML2::IXMLDOMAttributePtr pNewAttr,pGlyphIdAttr,pWordIdAttr;
	MSXML2::IXMLDOMNamedNodeMapPtr pAttrList,pNewAttrList,pointAttrs;
	std::vector<MaskVector> pointsMap;
	std::vector<CPoint>::iterator it;

	int i,n,r;
	__int8 g;

	CString name;
	_bstr_t bstrName;
	BSTR nameSpace;
	CImageSelection* sel = GetImageSelection();

	SectionPtrs* sec;
	bool found = false;
	sec = GetCurrentSection();
	if(sec == NULL)
		return;
	if(sec->type != glyph)
		return;
	CGTDoc::BeginWaitCursor();
	DeleteNodeFromCurrSection(glyph_coordGroup1);
	DeleteNodeFromCurrSection(glyph_coordGroup2);
	DeleteNodeFromCurrSection(glyph_coordGroup3);
	DeleteNodeFromCurrSection(glyph_coordGroup4);
	DeleteNodeFromCurrSection(glyph_coordGroup5);
	DeleteNodeFromCurrSection(glyph_coordGroup6);
	DeleteNodeFromCurrSection(glyph_coordGroup7);

	pNodeGlyph = GetNodeFromCurrSection(glyph_glyph);


	if(pNodeGlyph != NULL)
	{
				
				pParent = pNodeGlyph->GetparentNode();
				pParent->removeChild(pNodeGlyph);
				pNewNodeGlyph = pNodeGlyph->cloneNode(false);
				pNodeList = pNewNodeGlyph->GetchildNodes();
				while(pGroup = pNodeList->nextNode())
						pNewNodeGlyph->removeChild(pGroup);
				pParent->appendChild(pNewNodeGlyph);
				DeleteNodeFromCurrSection(glyph_glyph);
				AddNodeToCurrSection(pNewNodeGlyph,glyph_glyph);
	}
	else
	{
		pPageGlyphNode = GetNodeFromCurrSection(glyph);
		
		pAttrList = pPageGlyphNode->Getattributes();
		if (NULL == pAttrList)
			return;
		pGlyphIdAttr = pAttrList->nextNode();
		pNewAttr = GetLinkDom()->createAttribute(pGlyphIdAttr->Getname());
		pNewAttr->PutnodeValue(pGlyphIdAttr->Getvalue());
		name = _T("glyph");
		bstrName = (_bstr_t)(LPCTSTR)name;
		UpdateSection(NULL,page);
		pGlyphParent = GetNodeFromCurrSection(glyph_pcGts);
	
		if(pGlyphParent != NULL)
		{
			pGlyphParent->get_namespaceURI(&nameSpace);
			pNewElem = GetPageDom()->createNode(_variant_t((short)pGlyphParent->GetnodeType()), bstrName, nameSpace);
		}
		else
		{
			m_pDOMGlyphDoc->get_namespaceURI(&nameSpace);
			pNewElem = GetPageDom()->createNode(_variant_t((short)m_pDOMGlyphDoc->GetnodeType()), bstrName, nameSpace);

		}

		if(pNewElem != NULL)
			pNewAttrList = pNewElem->Getattributes();
		else
			return;
		if (NULL == pNewAttrList)
			return;

		pNewAttrList->setNamedItem(pNewAttr);

		pParent = pPageGlyphNode->GetparentNode();

		pAttrList = pParent->Getattributes();
		if (NULL == pAttrList)
			return;
		pWordIdAttr = pAttrList->nextNode();

	
		
		name = _T("wordRefId");
		bstrName = (_bstr_t)(LPCTSTR)name;
		// Do add attr id
		
		pNewAttr = GetLinkDom()->createAttribute(bstrName);
		pNewAttr->PutnodeValue(pWordIdAttr->Getvalue());
		pNewAttrList->setNamedItem(pNewAttr);
		pGlyphParent->appendChild(pNewElem);
		UpdateSection(pPageGlyphNode);
		AddNodeToCurrSection(pNewElem,glyph_glyph);
		pNewNodeGlyph = pNewElem;
	}
	sel->LoadMaskPoints(m_edition_state);
	pointsMap = sel->GetAllPoints();
	pNewNode = pNewNodeGlyph;
	i = 0;
	for(g = 1;g<8;g++)
	{
		name = _T("coordGroup");
		bstrName = (_bstr_t)(LPCTSTR)name;
		pNewNode->get_namespaceURI(&nameSpace);
		pCoorElem = GetLinkDom()->createNode(_variant_t((short)pNewNode->GetnodeType()), bstrName, nameSpace);
		if(pCoorElem != NULL)
			pAttrList = pCoorElem->Getattributes();
		else
			return;
		if (NULL == pAttrList)
			return;
		name = _T("id");
		bstrName = (_bstr_t)(LPCTSTR)name;
		pNewAttr = GetLinkDom()->createAttribute(bstrName);
		long idval;

		idval = UpdateIdCounter();
		name = "";
		name = wchar_t(idval % 10 + 48) + name;
		idval /= 10;
		while(idval !=0)
		{
			name = wchar_t(idval % 10 + 48) + name;
			idval /= 10;
		}
		name = _T("id") + name;
		
		i = 0;
		pNewAttr->PutnodeValue(BSTR(name.GetString()));
		pAttrList->setNamedItem(pNewAttr);

		for(unsigned j = 0;j<pointsMap[g-1].size();j++)
		{
			
			name = _T("p");
			bstrName = (_bstr_t)(LPCTSTR)name;
			pPointEl = GetLinkDom()->createNode(_variant_t((short)pNewNode->GetnodeType()), bstrName, nameSpace);
			pointAttrs = pPointEl->Getattributes();
			
			name = _T("x");
			bstrName = (_bstr_t)(LPCTSTR)name;
			pNewAttr = GetLinkDom()->createAttribute(bstrName);
			n = pointsMap[g-1][j].first.x;
			name = "";
			if(n==0)
				name = "0";
			while(n != 0)
			{
				name = wchar_t(n%10+48) + name;
				n = n/10;
			}
			
			pNewAttr->PutnodeValue(BSTR(name.GetString()));
			pointAttrs->setNamedItem(pNewAttr);

			name = _T("y");
			bstrName = (_bstr_t)(LPCTSTR)name;
			pNewAttr = GetLinkDom()->createAttribute(bstrName);
			
			n = pointsMap[g-1][j].first.y;
			name = "";
			if(n==0)
				name = "0";
			while(n != 0)
			{
				name = wchar_t(n%10+48) + name;
				n = n/10;
			}
			pNewAttr->PutnodeValue(BSTR(name.GetString()));
			pointAttrs->setNamedItem(pNewAttr);

			name = _T("r");
			bstrName = (_bstr_t)(LPCTSTR)name;
			pNewAttr = GetLinkDom()->createAttribute(bstrName);

			r = n = pointsMap[g-1][j].second;
			name = "";
			if(n==0)
				name = "0";
			while(n != 0)
			{
				name = wchar_t(n%10+48) + name;
				n = n/10;
			}
			pNewAttr->PutnodeValue(BSTR(name.GetString()));
			pointAttrs->setNamedItem(pNewAttr);
			i+=r;
			pCoorElem->appendChild(pPointEl);

		
		}
		if(i>0)
		{
			name = _T("no_coords");
			bstrName = (_bstr_t)(LPCTSTR)name;
			pNewAttr = GetLinkDom()->createAttribute(bstrName);
			
			n = i;			
			name = "";
			while(n != 0)
			{
				name = wchar_t(n%10+48) + name;
				n = n/10;
			}
			pNewAttr->PutnodeValue(BSTR(name.GetString()));
			pAttrList->setNamedItem(pNewAttr);

			name = _T("core");
			bstrName = (_bstr_t)(LPCTSTR)name;
			pNewAttr = GetLinkDom()->createAttribute(bstrName);
			if(g==1 || g==3 || g==5 || g==7) 
				name = "-1";
			else
				name = "0";
			pNewAttr->PutnodeValue(BSTR(name.GetString()));
			pAttrList->setNamedItem(pNewAttr);

			name = _T("outline");
			bstrName = (_bstr_t)(LPCTSTR)name;
			pNewAttr = GetLinkDom()->createAttribute(bstrName);
			if(g==2 || g==3 || g==6 || g==7) 
				name = "-1";
			else
				name = "0";
			pNewAttr->PutnodeValue(BSTR(name.GetString()));
			pAttrList->setNamedItem(pNewAttr);
			
			name = _T("shade");
			bstrName = (_bstr_t)(LPCTSTR)name;
			pNewAttr = GetLinkDom()->createAttribute(bstrName);
			if(g==4 || g==5 || g==6 || g==7) 
				name = "-1";
			else
				name = "0";
			pNewAttr->PutnodeValue(BSTR(name.GetString()));
			pAttrList->setNamedItem(pNewAttr);

			pNewNodeGlyph->appendChild(pCoorElem);
			UpdateSection(pPageGlyphNode);
			switch(g)
			{
			case 1:
				AddNodeToCurrSection(pCoorElem,glyph_coordGroup1);
				break;
			case 2:
				AddNodeToCurrSection(pCoorElem,glyph_coordGroup2);
				break;
			case 3:
				AddNodeToCurrSection(pCoorElem,glyph_coordGroup3);
				break;
			case 4:
				AddNodeToCurrSection(pCoorElem,glyph_coordGroup4);
				break;
			case 5:
				AddNodeToCurrSection(pCoorElem,glyph_coordGroup5);
				break;
			case 6:
				AddNodeToCurrSection(pCoorElem,glyph_coordGroup6);
				break;
			case 7:
				AddNodeToCurrSection(pCoorElem,glyph_coordGroup7);
				break;
			default:break;
			}
			
		}	
	}
	CGTDoc::EndWaitCursor();
}


void CGTDoc::OnFileSave()
{
	SectionPtrs* currentSeccion = GetCurrentSection();
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView == NULL)
		return;
	CXMLView* xmlView = (CXMLView*) GetNextView(pos);
	if(xmlView == NULL)
		return;
	xmlView->OnFileSave(false);
	
	SetCurrentSection(currentSeccion);
}

void CGTDoc::LoadPointsSelected(MSXML2::IXMLDOMNodePtr pRootNode)
{
	CString strVal;
	MSXML2::IXMLDOMNodeListPtr pNodeList,pNodeListC,pNodeListXY;
	MSXML2::IXMLDOMNodePtr pChild,pNode,pNodeC,pNodeXY;
	MSXML2::IXMLDOMAttributePtr pNodeA;
	MSXML2::IXMLDOMNamedNodeMapPtr pAttrMap;
	SectionPtrs* sec;
		
	int x,y,r;
	
	if(pRootNode != NULL)
	{
		UpdateSection(pRootNode);
		sec =  CGTDoc::GetCurrentSection();
		if(sec != NULL)
			if(sec->type != glyph)
			{
				pNodeList = pRootNode->GetchildNodes();
				while(pChild = pNodeList->nextNode())
						LoadPointsSelected(pChild);
			}
			else
			{
				pNode = GetNodeFromCurrSection(glyph_coordGroup1);
				if(pNode != NULL)
				{
					pNodeListC = pNode->GetchildNodes();
					_bstr_t a;
					_variant_t b;
					while(pNodeC = pNodeListC->nextNode())
					{
						pAttrMap = pNodeC->Getattributes();
						pNodeA = pAttrMap->nextNode();
						x =((pNodeA->GetnodeValue()));
						 pNodeA = pAttrMap->nextNode();
						y =((pNodeA->GetnodeValue()));
						pNodeA = pAttrMap->nextNode();
						if(pNodeA != NULL)
							r =((pNodeA->GetnodeValue()));
						else
							r = 1;
						m_imgSelection.SetPixelMask(x,y,r,1);
					}
				}
				pNode = GetNodeFromCurrSection(glyph_coordGroup2);
				if(pNode != NULL)
				{
					pNodeListC = pNode->GetchildNodes();
					while(pNodeC = pNodeListC->nextNode())
					{
						pAttrMap = pNodeC->Getattributes();
						pNodeA = pAttrMap->nextNode();
						x =((pNodeA->GetnodeValue()));
						pNodeA = pAttrMap->nextNode();
						y =((pNodeA->GetnodeValue()));
						pNodeA = pAttrMap->nextNode();
						if(pNodeA != NULL)
							r =((pNodeA->GetnodeValue()));
						else
							r = 1;
						m_imgSelection.SetPixelMask(x,y,r,2);
					}
				}
				pNode = GetNodeFromCurrSection(glyph_coordGroup3);
				if(pNode != NULL)
				{
					pNodeListC = pNode->GetchildNodes();
					while(pNodeC = pNodeListC->nextNode())
					{
						pAttrMap = pNodeC->Getattributes();
						pNodeA = pAttrMap->nextNode();
						x =((pNodeA->GetnodeValue()));
						pNodeA = pAttrMap->nextNode();
						y =((pNodeA->GetnodeValue()));
						pNodeA = pAttrMap->nextNode();
						if(pNodeA != NULL)
							r =((pNodeA->GetnodeValue()));
						else
							r = 1;
						m_imgSelection.SetPixelMask(x,y,r,3);
					}
				}
				pNode = GetNodeFromCurrSection(glyph_coordGroup4);
				if(pNode != NULL)
				{
					pNodeListC = pNode->GetchildNodes();
					while(pNodeC = pNodeListC->nextNode())
					{
						pAttrMap = pNodeC->Getattributes();
						pNodeA = pAttrMap->nextNode();
						x =((pNodeA->GetnodeValue()));
						pNodeA = pAttrMap->nextNode();
						y =((pNodeA->GetnodeValue()));
						pNodeA = pAttrMap->nextNode();
						if(pNodeA != NULL)
							r =((pNodeA->GetnodeValue()));
						else
							r = 1;
						m_imgSelection.SetPixelMask(x,y,r,4);

					}
				}
				pNode = GetNodeFromCurrSection(glyph_coordGroup5);
				if(pNode != NULL)
				{
					pNodeListC = pNode->GetchildNodes();
					while(pNodeC = pNodeListC->nextNode())
					{
						pAttrMap = pNodeC->Getattributes();
						pNodeA = pAttrMap->nextNode();
						x =((pNodeA->GetnodeValue()));
						pNodeA = pAttrMap->nextNode();
						y =((pNodeA->GetnodeValue()));
						pNodeA = pAttrMap->nextNode();
						if(pNodeA != NULL)
							r =((pNodeA->GetnodeValue()));
						else
							r = 1;
						m_imgSelection.SetPixelMask(x,y,r,5);
					}
				}
				pNode = GetNodeFromCurrSection(glyph_coordGroup6);
				if(pNode != NULL)
				{
					pNodeListC = pNode->GetchildNodes();
					while(pNodeC = pNodeListC->nextNode())
					{
						pAttrMap = pNodeC->Getattributes();
						pNodeA = pAttrMap->nextNode();
						x =((pNodeA->GetnodeValue()));
						pNodeA = pAttrMap->nextNode();
						y =((pNodeA->GetnodeValue()));
						pNodeA = pAttrMap->nextNode();
						if(pNodeA != NULL)
							r =((pNodeA->GetnodeValue()));
						else
							r = 1;
						m_imgSelection.SetPixelMask(x,y,r,6);
					}
				}
				pNode = GetNodeFromCurrSection(glyph_coordGroup7);
				if(pNode != NULL)
				{
					pNodeListC = pNode->GetchildNodes();
					while(pNodeC = pNodeListC->nextNode())
					{
						pAttrMap = pNodeC->Getattributes();
						pNodeA = pAttrMap->nextNode();
						x =((pNodeA->GetnodeValue()));
						pNodeA = pAttrMap->nextNode();
						y =((pNodeA->GetnodeValue()));
						pNodeA = pAttrMap->nextNode();
						if(pNodeA != NULL)
							r =((pNodeA->GetnodeValue()));
						else
							r = 1;
						m_imgSelection.SetPixelMask(x,y,r,7);
					}
				}
			}
	}
}




// CGTDoc serialization

void CGTDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		if(ar.m_strFileName.Find(_T(".xml")) <= 0)
			SetLinkPath(ar.m_strFileName + _T(".xml"));
		else
			SetLinkPath(ar.m_strFileName);
		OnFileSave();
	}
	else
	{
		
	
	}
}

void CGTDoc::OnUpdateSizeHalf(CCmdUI* pCmdUI)
{
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView == NULL)
		return;
	
	pCmdUI->Enable(!m_imgOriginal.IsNull());
	gtView->OnUpdateSizeHalf(pCmdUI);
	
}

void CGTDoc::OnUpdateSizeOriginal(CCmdUI* pCmdUI)
{
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView == NULL)
		return;
	pCmdUI->Enable(!m_imgOriginal.IsNull());
	gtView->OnUpdateSizeOriginal(pCmdUI);
}


void CGTDoc::OnUpdateSizeDouble(CCmdUI* pCmdUI)
{
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView == NULL)
		return;
	pCmdUI->Enable(!m_imgOriginal.IsNull());
	gtView->OnUpdateSizeDouble(pCmdUI);
}

void CGTDoc::OnUpdateSizeX4(CCmdUI* pCmdUI)
{
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView == NULL)
		return;
	pCmdUI->Enable(!m_imgOriginal.IsNull());
	gtView->OnUpdateSizeX4(pCmdUI);
}

void CGTDoc::OnUpdateSizeX8(CCmdUI* pCmdUI)
{
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView == NULL)
		return;
	pCmdUI->Enable(!m_imgOriginal.IsNull());
	gtView->OnUpdateSizeX8(pCmdUI);
}

void CGTDoc::OnUpdateSizeX16(CCmdUI* pCmdUI)
{
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView == NULL)
		return;
	pCmdUI->Enable(!m_imgOriginal.IsNull());
	gtView->OnUpdateSizeX16(pCmdUI);
}

void CGTDoc::OnUpdateSizeFill(CCmdUI* pCmdUI)
{
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView == NULL)
		return;
	pCmdUI->Enable(!m_imgOriginal.IsNull());
	gtView->OnUpdateSizeFill(pCmdUI);
}

void CGTDoc::OnUpdateSizeZoom(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_imgOriginal.IsNull());
	pCmdUI->SetCheck((m_tool == ZOOM_TOOL));
}

void CGTDoc::OnChangeSize(UINT nID)
{
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView == NULL)
		return;
	gtView->OnChangeSize(nID);
}

void CGTDoc::OnViewShade()
{
	POSITION pos = GetFirstViewPosition();
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	CImageSelection* sel = GetImageSelection();
	if(gtView == NULL)
		return;
	
	if(m_edition_state !=  EDIT_NONE)
	{
		sel->LoadMaskPoints(m_edition_state);
		if(m_edition_state == EDIT_SHADE)
			m_edition_state = EDIT_NONE;
	}
	if(m_edition_state ==  EDIT_NONE)
	{
		if(!((GetImageSelection()->GetOutline()) || (GetImageSelection()->GetCore())))
		{
			if(!GetImageSelection()->GetShade())  
				pMainWnd->AddBrightBar(true);
			else
				pMainWnd->AddBrightBar(false);
		}
			pMainWnd->AddSensitivityBar(false);
		pMainWnd->AddPen(false);
	}
	GetImageSelection()->SetShade(!(GetImageSelection()->GetShade()));
	SetPenPoint(CPoint(-1,-1));
	gtView->SetShowPen();
	gtView->Invalidate(false);
}

void CGTDoc::OnViewOutline()
{
	POSITION pos = GetFirstViewPosition();
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	CImageSelection* sel = GetImageSelection();
	if(gtView == NULL)
		return;
	
	if(m_edition_state !=  EDIT_NONE)
	{
		sel->LoadMaskPoints(m_edition_state);
		if(m_edition_state == EDIT_OUTLINE)
			m_edition_state = EDIT_NONE;
	}
	if(m_edition_state ==  EDIT_NONE)
	{
		if(!((GetImageSelection()->GetCore()) ||  (GetImageSelection()->GetShade())))
		{
			if(!GetImageSelection()->GetOutline()) 
				pMainWnd->AddBrightBar(true);
			else
				pMainWnd->AddBrightBar(false);
		}
		pMainWnd->AddSensitivityBar(false);
		pMainWnd->AddPen(false);
	}
	GetImageSelection()->SetOutline(!(GetImageSelection()->GetOutline()));
	SetPenPoint(CPoint(-1,-1));
	gtView->SetShowPen();
	gtView->Invalidate(false);
}


void CGTDoc::OnViewCore()
{
	POSITION pos = GetFirstViewPosition();
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	CImageSelection* sel = GetImageSelection();
	if(gtView == NULL)
		return;

	
	if(m_edition_state !=  EDIT_NONE)
	{
		sel->LoadMaskPoints(m_edition_state);
		if(m_edition_state == EDIT_CORE)
			m_edition_state = EDIT_NONE;
	}
	if(m_edition_state ==  EDIT_NONE)
	{
		if(!((GetImageSelection()->GetOutline()) || (GetImageSelection()->GetShade())))
		{
			if(!GetImageSelection()->GetCore())  
				pMainWnd->AddBrightBar(true);
			else
				pMainWnd->AddBrightBar(false);
		}
		pMainWnd->AddSensitivityBar(false);
		pMainWnd->AddPen(false);
	}	
	GetImageSelection()->SetCore(!(GetImageSelection()->GetCore()));
	SetPenPoint(CPoint(-1,-1));
	gtView->SetShowPen();
	gtView->Invalidate(false);
}


void CGTDoc::OnEditShade()
{
	POSITION pos = GetFirstViewPosition();
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	CImageSelection *sel = GetImageSelection();
	if(gtView == NULL)
		return;

	ToolEnum state = GetToolState();
	gtView->BeginWaitCursor();
	switch(state)
	{
		case REGION_TOOL:
			pMainWnd->AddSensitivityBar(true);
			pMainWnd->AddPen(false);
			break;
		case PIXEL_TOOL:
			pMainWnd->AddPen(true);
			pMainWnd->AddSensitivityBar(false);
			break;
		case NO_TOOL:
			m_tool = PIXEL_TOOL;
			pMainWnd->AddPen(true);
			pMainWnd->AddSensitivityBar(false);
			break;
		default:break;
	}
	if(m_edition_state != EDIT_NONE)
		sel->LoadMaskPoints(m_edition_state);

	sel->SetShade(true);
	m_edition_state = EDIT_SHADE;
	m_isBackuped = 0;
		
	pMainWnd->AddBrightBar(true);
	SetPenPoint(CPoint(-1,-1));
	gtView->SetShowPen();
	gtView->Invalidate(false);
	gtView->EndWaitCursor();
}

void CGTDoc::OnEditOutline()
{
	POSITION pos = GetFirstViewPosition();
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	CImageSelection *sel = GetImageSelection();
	if(gtView == NULL)
		return;

	ToolEnum state = GetToolState();
	gtView->BeginWaitCursor();
	switch(state)
	{
		case REGION_TOOL:
			pMainWnd->AddSensitivityBar(true);
			pMainWnd->AddPen(false);
			break;
		case PIXEL_TOOL:
			pMainWnd->AddPen(true);
			pMainWnd->AddSensitivityBar(false);
			break;
		case NO_TOOL:
			m_tool = PIXEL_TOOL;
			pMainWnd->AddPen(true);
			pMainWnd->AddSensitivityBar(false);
			break;
		default:break;
	}
	if(m_edition_state != EDIT_NONE)
		sel->LoadMaskPoints(m_edition_state);
	
	sel->SetOutline(true);
	m_edition_state = EDIT_OUTLINE;
	m_isBackuped = 0;
	
	pMainWnd->AddBrightBar(true);
	SetPenPoint(CPoint(-1,-1));
	gtView->SetShowPen();
	gtView->Invalidate(false);
	gtView->EndWaitCursor();
}

void CGTDoc::OnEditCore()
{
	POSITION pos = GetFirstViewPosition();
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	CImageSelection *sel = GetImageSelection();
	if(gtView == NULL)
		return;

	ToolEnum state = GetToolState();
	gtView->BeginWaitCursor();
	switch(state)
	{
		case REGION_TOOL:
			pMainWnd->AddSensitivityBar(true);
			pMainWnd->AddPen(false);
			break;
		case PIXEL_TOOL:
			pMainWnd->AddPen(true);
			pMainWnd->AddSensitivityBar(false);
			break;
		case NO_TOOL:
			m_tool = PIXEL_TOOL;
			pMainWnd->AddPen(true);
			pMainWnd->AddSensitivityBar(false);
			break;
		default:break;
	}
	if(m_edition_state != EDIT_NONE)
		sel->LoadMaskPoints(m_edition_state);
	sel->SetCore(true);
	m_edition_state = EDIT_CORE;
	m_isBackuped = 0;
		
	pMainWnd->AddBrightBar(true);
	SetPenPoint(CPoint(-1,-1));
	gtView->SetShowPen();
	gtView->Invalidate(false);
	gtView->EndWaitCursor();
}

void CGTDoc::OnUpdateEditShade(CCmdUI *pCmdUI)
{
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView == NULL)
		return;

	SectionPtrs* sec;
	sec = GetCurrentSection();
	if(sec != NULL)
	{
		if(sec->type == glyph)
			pCmdUI->Enable(true);
		else
			pCmdUI->Enable(false);
	}
	else
		pCmdUI->Enable(false);
	pCmdUI->SetCheck(m_edition_state == EDIT_SHADE);
}

void CGTDoc::OnUpdateEditOutline(CCmdUI *pCmdUI)
{
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView == NULL)
		return;

	SectionPtrs* sec;
	sec = GetCurrentSection();
	if(sec != NULL)
	{
		if(sec->type == glyph)
			pCmdUI->Enable(true);
		else
			pCmdUI->Enable(false);
	}
	else
		pCmdUI->Enable(false);
	pCmdUI->SetCheck(m_edition_state == EDIT_OUTLINE);
}

void CGTDoc::OnUpdateEditCore(CCmdUI *pCmdUI)
{
	SectionPtrs* sec;
	sec = GetCurrentSection();
	if(sec != NULL)
	{
		if(sec->type == glyph)
			pCmdUI->Enable(true);
		else
			pCmdUI->Enable(false);
	}
	else
		pCmdUI->Enable(false);
	pCmdUI->SetCheck(m_edition_state == EDIT_CORE);
}

void CGTDoc::OnUpdateViewShade(CCmdUI *pCmdUI)
{
	if(m_imgOriginal.IsNull())
	{
		pCmdUI->Enable(false);
	}
	else
	{
		pCmdUI->Enable(true);
		pCmdUI->SetCheck(GetImageSelection()->GetShade()/* && (m_edition_state == EDIT_NONE)*/);
	}
}

void CGTDoc::OnUpdateViewOutline(CCmdUI *pCmdUI)
{
	if(m_imgOriginal.IsNull())
	{
		pCmdUI->Enable(false);
	}
	else
	{
		pCmdUI->Enable(true);
		pCmdUI->SetCheck(GetImageSelection()->GetOutline());
	}
}

void CGTDoc::OnUpdateViewCore(CCmdUI *pCmdUI)
{
	if(m_imgOriginal.IsNull())
	{
		pCmdUI->Enable(false);
	}
	else
	{
		pCmdUI->Enable(true);
		pCmdUI->SetCheck(GetImageSelection()->GetCore());
	}	
}

void CGTDoc::OnToolsPixelselect()
{
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	pMainWnd->AddSensitivityBar(false);
	pMainWnd->AddZoom(false);
	if(m_tool != PIXEL_TOOL)
	{
		pMainWnd->AddPen(true);
		m_tool = PIXEL_TOOL;
	}
	else
	{
		pMainWnd->AddPen(false);
		m_tool = NO_TOOL;
	}
}

void CGTDoc::OnUpdateToolsPixelselect(CCmdUI *pCmdUI)
{
	SectionPtrs* sec;
	sec = GetCurrentSection();
	if(sec != NULL)
	{
		if((sec->type == glyph) && (m_edition_state != EDIT_NONE))
			pCmdUI->Enable(true);
		else
			pCmdUI->Enable(false);
	}
	else
		pCmdUI->Enable(false);
	
	pCmdUI->SetCheck((m_tool == PIXEL_TOOL));
}

void CGTDoc::OnToolsRegionselect()
{
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();	
	pMainWnd->AddZoom(false);
	pMainWnd->AddPen(false);
	if(m_tool != REGION_TOOL)
	{
		pMainWnd->AddSensitivityBar(true);
		m_tool = REGION_TOOL;
	}
	else
	{
		pMainWnd->AddSensitivityBar(false);
		m_tool = NO_TOOL;
	}
}

void CGTDoc::OnUpdateToolsRegionselect(CCmdUI *pCmdUI)
{
	SectionPtrs* sec;
	sec = GetCurrentSection();
	if(sec != NULL)
	{
		if((sec->type == glyph) && (m_edition_state != EDIT_NONE))
			pCmdUI->Enable(true);
		else
			pCmdUI->Enable(false);
	}
	else
		pCmdUI->Enable(false);

	pCmdUI->SetCheck((m_tool == REGION_TOOL));
}


void CGTDoc::LoadMask()
{
	m_imgSelection.DeltePoints();
	m_isBackuped = 0;
}

void CGTDoc::OnEditUndo()
{
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView == NULL)
		return;
	
	CImageSelection imageSel;
	BeginWaitCursor();
	m_imgSelection.LoadMaskPoints(m_edition_state);
	imageSel = CImageSelection(m_imgSelection);
	m_imgSelection = CImageSelection(m_imgSelectionOld);
	m_imgSelectionOld = CImageSelection(imageSel);
	
	m_isBackuped = 2;	
	CImage *mask;
	m_imgSelection.ResetMask();
	mask = m_imgSelection.GetImageMask();
	if(mask->IsNull())
	{
		m_imgSelection.MergePoints(&m_imgOriginal);
	}
	SetPenPoint(CPoint(-1,-1));
	gtView->SetShowPen();
	gtView->Invalidate(false);
	EndWaitCursor();	
}

void CGTDoc::OnZoom()
{
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	
	bool isOn = (m_tool == ZOOM_TOOL);
	pMainWnd->AddPen(false);
	pMainWnd->AddSensitivityBar(false);

	if(isOn)
	{
		m_tool = NO_TOOL;
		pMainWnd->AddZoom(!isOn);
	}
	else
	{
		m_tool = ZOOM_TOOL;
		pMainWnd->AddZoom(!isOn);
	}
}

void CGTDoc::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_isBackuped==1);
}

void CGTDoc::OnEditRedo()
{
	
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView == NULL)
		return;
	CImageSelection imageSel;
	BeginWaitCursor();
	imageSel = CImageSelection(m_imgSelection);
	m_imgSelection = CImageSelection(m_imgSelectionOld);
	m_imgSelectionOld = CImageSelection(imageSel);
	
	m_isBackuped = 1;
	CImage *mask;
	m_imgSelection.ResetMask();
	mask = m_imgSelection.GetImageMask();
	if(mask->IsNull())
	{
		m_imgSelection.MergePoints(&m_imgOriginal);
	}
	SetPenPoint(CPoint(-1,-1));
	gtView->SetShowPen();
	gtView->Invalidate(false);
	EndWaitCursor();
}

void CGTDoc::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_isBackuped==2);
}

void CGTDoc::OnEditCopy()
{
	MaskVector *maskVector,fullWitdthVector;
	
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	CRect Clrect,imageRect;
	if(gtView == NULL)
		return;
	BeginWaitCursor();
	gtView->GetClientRect(&Clrect);
	Clrect.OffsetRect(gtView->GetScrollPosition());
	imageRect.left = int(Clrect.left / gtView->GetZoom());
	imageRect.top = int(Clrect.top / gtView->GetZoom());
	imageRect.bottom = int(Clrect.bottom / gtView->GetZoom())+1;
	imageRect.right = int(Clrect.right / gtView->GetZoom())+1;
		
	if(imageRect.left < 0)
		imageRect.left = 0;
	if(imageRect.Width() > m_imgSelection.GetImageMask()->GetWidth())
		imageRect.right = imageRect.left + m_imgSelection.GetImageMask()->GetWidth();
	if(imageRect.bottom < 0)
		imageRect.bottom = 0;
	if(imageRect.Height() > m_imgSelection.GetImageMask()->GetHeight())
		imageRect.bottom = imageRect.top + m_imgSelection.GetImageMask()->GetHeight();

	m_imgSelection.LoadMaskPoints(m_edition_state);
	maskVector = m_imgSelection.GetCurrentMaskVector(m_edition_state);
	if(maskVector != NULL)
	{
		fullWitdthVector = *maskVector;
		maskVector->clear();
		m_imgSelection.LoadMaskPoints(m_edition_state,imageRect);
		m_copyVector = *maskVector;
		*maskVector = fullWitdthVector;
	}
	else
		m_copyVector.clear();
	
	EndWaitCursor();
}

void CGTDoc::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(IsModified());
}

void CGTDoc::OnUpdateFileSaveAs(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_imgOriginal.IsNull());
}

void CGTDoc::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((m_imgSelection.GetCurrentMaskVector(m_edition_state) != NULL) && (m_edition_state != EDIT_NONE));
}

void CGTDoc::OnEditPaste()
{
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView == NULL)
		return;
	CPoint point;
	if(m_edition_state != EDIT_NONE)
	{
		if(m_copyVector.size() != 0)
		{	
			BeginWaitCursor();
			Backup();
			for(unsigned i = 0;i<m_copyVector.size();i++)
			{
				for(int k = 0;k<m_copyVector[i].second;k++)
				{
					point = m_copyVector[i].first;
					point.x += k; 
					m_imgSelection.ChangePoint(point,m_edition_state,true,1);
				}
			}
			m_imgSelection.LoadMaskPoints(m_edition_state);
			SetDirty(true);
			CImage *mask;
			m_imgSelection.ResetMask();
			mask = m_imgSelection.GetImageMask();
			if(mask->IsNull())
			{
				m_imgSelection.MergePoints(&m_imgOriginal);
			}
			SetPenPoint(CPoint(-1,-1));
			gtView->SetShowPen();
			gtView->Invalidate(false);
			EndWaitCursor();
		}
	}
	else
	{
		theApp.OnFileFromclipboard();
	}
}

void CGTDoc::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	bool existContent = false;
	::OpenClipboard(AfxGetMainWnd()->GetSafeHwnd());
	HANDLE hClip = ::GetClipboardData(CF_BITMAP);
	HBITMAP hbClip = (HBITMAP) hClip;
	if(hbClip != NULL)
		existContent = true;
	CloseClipboard();

	pCmdUI->Enable(((m_copyVector.size() != 0) && (m_edition_state != EDIT_NONE))||existContent);
}

void CGTDoc::OnEditCut()
{
	MaskVector *maskVector,fullWitdthVector;
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	CRect Clrect,imageRect;
	if(gtView == NULL)
		return;
	BeginWaitCursor();
		
	Backup();
	
	gtView->GetClientRect(&Clrect);
	Clrect.OffsetRect(gtView->GetScrollPosition());
	imageRect.left = int(Clrect.left / gtView->GetZoom());
	imageRect.top = int(Clrect.top / gtView->GetZoom());
	imageRect.bottom = int(Clrect.bottom / gtView->GetZoom())+1;
	imageRect.right = int(Clrect.right / gtView->GetZoom())+1;
		
	if(imageRect.left < 0)
		imageRect.left = 0;
	if(imageRect.Width() > m_imgSelection.GetImageMask()->GetWidth())
		imageRect.right = imageRect.left + m_imgSelection.GetImageMask()->GetWidth();
	if(imageRect.bottom < 0)
		imageRect.bottom = 0;
	if(imageRect.Height() > m_imgSelection.GetImageMask()->GetHeight())
		imageRect.bottom = imageRect.top + m_imgSelection.GetImageMask()->GetHeight();

	maskVector = m_imgSelection.GetCurrentMaskVector(m_edition_state);
	if(maskVector != NULL)
	{
		//COPY
		fullWitdthVector = *maskVector;
		maskVector->clear();
		m_imgSelection.LoadMaskPoints(m_edition_state,imageRect);
		m_copyVector = * maskVector;
		*maskVector = fullWitdthVector;
		OnEditCopy();
		

		//ERASE
		m_imgSelection.EraseArea(imageRect,m_edition_state);
		SetDirty(true);
		SetPenPoint(CPoint(-1,-1));
		gtView->SetShowPen();
		gtView->Invalidate(false);
	}
	else
		m_copyVector.clear();

	EndWaitCursor();
}

void CGTDoc::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((m_imgSelection.GetCurrentMaskVector(m_edition_state) != NULL) && (m_edition_state != EDIT_NONE));
}

void CGTDoc::OnContextgtUndo()
{
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	CXMLView* xmlView = (CXMLView*) GetNextView(pos);
	if(xmlView == NULL)
		return;
	MSXML2::IXMLDOMDocumentPtr pDOMbackup;
		
	pDOMbackup = m_pDOMGlyphDoc;
	m_pDOMGlyphDoc = m_pDOMGlyphDocOld->cloneNode(VARIANT_TRUE);
	m_pDOMGlyphDocOld = pDOMbackup;

	pDOMbackup = m_pDOMLinkDoc;
	m_pDOMLinkDoc = m_pDOMLinkDocOld->cloneNode(VARIANT_TRUE);
	m_pDOMLinkDocOld = pDOMbackup;

	pDOMbackup = m_pDOMPageDoc;
	m_pDOMPageDoc = m_pDOMPageDocOld->cloneNode(VARIANT_TRUE);
	m_pDOMPageDocOld = pDOMbackup;

	Release(false);
	SetLock(false);
	xmlView->ParseLinkXML(m_pDOMLinkDoc);
	xmlView->OnPageRefresh();
	xmlView->ParseGlyphXMLTree(m_pDOMGlyphDoc);
	CImage *mask;
	m_imgSelection.ResetMask();
	mask = m_imgSelection.GetImageMask();
	if(mask->IsNull())
	{
		m_imgSelection.MergePoints(&m_imgOriginal);
	}
	xmlView->Invalidate(false);

	m_isXMLBackuped = 2;
}

void CGTDoc::OnUpdateContextGtUndo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((m_isXMLBackuped == 1));
}

void CGTDoc::OnUpdateViewPoint(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_showPointBar);
}

void CGTDoc::OnUpdateContextgtRedo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((m_isXMLBackuped == 2));
}

void CGTDoc::OnEditRedoxml()
{
	OnContextgtUndo();

	m_isXMLBackuped = 1;
}

void CGTDoc::OnEditInvert()
{
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	pMainWnd->AddSensitivityBar(false);
	pMainWnd->AddZoom(false);
	pMainWnd->AddPen(true);
	m_tool = INVERT_TOOL;
}


void CGTDoc::OnUpdateEditInvert(CCmdUI *pCmdUI)
{
	SectionPtrs* sec;
	sec = GetCurrentSection();
	if(sec != NULL)
	{
		if((sec->type == glyph) && (m_edition_state != EDIT_NONE))
		{
			pCmdUI->Enable(true);
			pCmdUI->SetCheck((m_tool == INVERT_TOOL));
		}
		else
			pCmdUI->Enable(false);
	}
	else
		pCmdUI->Enable(false);
}

void CGTDoc::OnUpdateToolsInvertall(CCmdUI *pCmdUI)
{
	SectionPtrs* sec;
	sec = GetCurrentSection();
	if(sec != NULL)
	{
		if((sec->type == glyph) && (m_edition_state != EDIT_NONE))
			pCmdUI->Enable(true);
		else
			pCmdUI->Enable(false);
	}
	else
		pCmdUI->Enable(false);
}

void CGTDoc::OnToolsInvertall()
{
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	CRect Clrect,imageRect;
	if(gtView == NULL)
		return;
	BeginWaitCursor();
	gtView->GetClientRect(&Clrect);
	Clrect.OffsetRect(gtView->GetScrollPosition());
	imageRect.left = int(Clrect.left / gtView->GetZoom());
	imageRect.top = int(Clrect.top / gtView->GetZoom());
	imageRect.bottom = int(Clrect.bottom / gtView->GetZoom())+1;
	imageRect.right = int(Clrect.right / gtView->GetZoom())+1;
		
	if(imageRect.left < 0)
		imageRect.left = 0;
	if(imageRect.Width() > m_imgSelection.GetImageMask()->GetWidth())
		imageRect.right = imageRect.left + m_imgSelection.GetImageMask()->GetWidth();
	if(imageRect.bottom < 0)
		imageRect.bottom = 0;
	if(imageRect.Height() > m_imgSelection.GetImageMask()->GetHeight())
		imageRect.bottom = imageRect.top + m_imgSelection.GetImageMask()->GetHeight();

	Backup();
	m_imgSelection.InvertMask(imageRect,m_edition_state);
	m_imgSelection.LoadMaskPoints(m_edition_state);
	SetModifiedFlag(TRUE);
	SetDirty(true);
	gtView->Invalidate(false);
	EndWaitCursor();
}

void CGTDoc::OnViewShowpoint()
{
	m_showPointBar = !m_showPointBar;
}

void CGTDoc::OnFilePreferences()
{
	CString file = GetLinkPath().Right(GetLinkPath().GetLength()-GetLinkPath().ReverseFind(wchar_t('\\'))-1);
	CString folder = GetLinkPath().Left(GetLinkPath().ReverseFind(wchar_t('\\'))+ 1);
	CFolders foldersForm(m_glDefaultFolder,m_pcDefaultFolder,m_languageOCR,folder);
	
	foldersForm.DoModal();
	m_strLinkPathFile = folder + file;
}

// CGTDoc diagnostics

#ifdef _DEBUG
void CGTDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGTDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG



// CImageSelection commands

 CImageSelection::CImageSelection()
 {	
	 m_core =  false;
	 m_shade = false;
	 m_outline = false;
	 m_isChanged = true;
	 m_editRect = CRect(0,0,0,0);
	 m_coreVector.clear();
	 m_outlineVector.clear();
	 m_shadeVector.clear();
 }

  CImageSelection::~CImageSelection()
 {
	m_imgMask.Destroy();
	m_coreVector.clear();
	m_outlineVector.clear();
	m_shadeVector.clear();
 }

CImageSelection::CImageSelection(const CImageSelection &copy)
{
	this->m_shadeVector.clear();
	this->m_shadeVector = copy.m_shadeVector;
	this->m_core = copy.m_core;
	this->m_outline = copy.m_outline;
	this->m_shade = copy.m_shade;
	this->m_coreVector.clear();
	this->m_coreVector = copy.m_coreVector;
	this->m_outlineVector.clear();
	this->m_outlineVector = copy.m_outlineVector;
	this->m_editRect = copy.m_editRect;
}


CImageSelection::CImageSelection(CImage *source)
{
	 m_core =  false;
	 m_shade = false;
	 m_outline = false;
	 m_imgMask = *source;
}

CImage* CImageSelection::GetImageMask()
{
	return &m_imgMask;
}
 
 void CImageSelection::SetPixelMask(int x,int y,int r,int group)
 {
	 if ( (x>=0) && (x < m_imgMask.GetWidth()) && (y>=0) &&  (y < m_imgMask.GetHeight()) && (unsigned) group<8)
	 {
		 switch(group)
		 {
			 case 1: m_coreVector.push_back(VectorRun(CPoint(x,y),r));
				 break;
			 case 2: m_outlineVector.push_back(VectorRun(CPoint(x,y),r));
				 break;
			 case 3: m_coreVector.push_back(VectorRun(CPoint(x,y),r));
				 m_outlineVector.push_back(VectorRun(CPoint(x,y),r));
				 break;
			 case 4: m_shadeVector.push_back(VectorRun(CPoint(x,y),r));
				 break;
			 case 5: m_shadeVector.push_back(VectorRun(CPoint(x,y),r));
				 m_coreVector.push_back(VectorRun(CPoint(x,y),r));
				 break;
			 case 6: m_shadeVector.push_back(VectorRun(CPoint(x,y),r));
				 m_outlineVector.push_back(VectorRun(CPoint(x,y),r));
				 break;
			 case 7: m_shadeVector.push_back(VectorRun(CPoint(x,y),r));
				  m_coreVector.push_back(VectorRun(CPoint(x,y),r));
				 m_outlineVector.push_back(VectorRun(CPoint(x,y),r));
				 break;
			 default:break;
		 }
	 }
 }

bool  CImageSelection::GetCore(){return m_core;}
bool  CImageSelection::GetShade(){return m_shade;} 
bool  CImageSelection::GetOutline(){return m_outline;}

void CImageSelection::UpdateEditBox(CPoint point)
{
	if(point.x<0 || point.y<0)
	{
		m_editRect = CRect(0,0,0,0);
	}
	else if(m_editRect == CRect(0,0,0,0))
	{
		m_editRect.left = point.x;
		m_editRect.right = point.x + 1;
		m_editRect.top = point.y;
		m_editRect.bottom = point.y + 1;
	}
	else
	{
		if(point.x < m_editRect.left)
			m_editRect.left = point.x;
		if(point.x > m_editRect.right - 1)
			m_editRect.right = point.x + 1;
		if(point.y < m_editRect.top)
			m_editRect.top = point.y;
		if(point.y > m_editRect.bottom - 1)
			m_editRect.bottom = point.y + 1;
	}
}

//Delete CImage pointer later if changeSelection == false
CImage* CImageSelection::MergePoints(CImage* source,bool changeSelection)
{
	CPoint p;
	COLORREF color;
	CImage *newMask = new CImage();
	if(source != NULL)
	{
		newMask->Create(source->GetWidth(), source->GetHeight(), 24,0);
	}
	else
		return NULL;
	unsigned i;
	int r;

	UpdateEditBox(CPoint(-1,-1)); //Reset

	if(m_core)
	{
		for ( i = 0; i < m_coreVector.size(); i++)
		{
			p = m_coreVector[i].first;
			r = m_coreVector[i].second;
			UpdateEditBox(p);
			for(int k = 0;k<r;k++)
			{
				color = GetPixelFast(newMask,p.x+k, p.y);
				color = color | RGB(255,0, 0);
				SetPixelFast(newMask,p.x+k, p.y, color);
			}
			p.x += r-1;
			UpdateEditBox(p);
		}
	}
	if(m_outline)
	{
		for ( i = 0; i < m_outlineVector.size(); i++)
		{
			p = m_outlineVector[i].first;
			r = m_outlineVector[i].second;
			UpdateEditBox(p);
			for(int k = 0;k<r;k++)
			{
				color = GetPixelFast(newMask,p.x+k,p.y);
				color = color | RGB(0,255, 0);
				SetPixelFast(newMask,p.x+k, p.y, color);
			}
			p.x += r-1;
			UpdateEditBox(p);
			
		}
	}
	if(m_shade)
	{
		for ( i = 0; i < m_shadeVector.size(); i++)
		{
			p = m_shadeVector[i].first;
			r = m_shadeVector[i].second;
			UpdateEditBox(p);
			for(int k = 0;k<r;k++)
			{
				color = GetPixelFast(newMask,p.x+k, p.y);
				color = color | RGB(0,0, 255);
				SetPixelFast(newMask,p.x+k, p.y, color);
			}
			p.x += r-1;
			UpdateEditBox(p);
		}
	}
	if(changeSelection)
	{
		if(!m_imgMask.IsNull())
				m_imgMask.Destroy();
		m_imgMask.Attach(*newMask);
		newMask->Detach();
		delete newMask;

		return &m_imgMask;
	}
	else
	{
		return newMask;
	}
}

void CImageSelection::InvertMask(CRect size,EditEnum region)
{
	if((size.left < 0) || (size.right > m_imgMask.GetWidth()) || (size.top < 0) || (size.bottom > m_imgMask.GetHeight()))
	{
		if(size.left < 0)
			size.left = 0;
		if(size.right > m_imgMask.GetWidth())
			size.right = m_imgMask.GetWidth();
		if(size.top < 0)
			size.top = 0;
		if(size.bottom > m_imgMask.GetHeight())
			size.bottom = m_imgMask.GetHeight();
	}

	COLORREF editColor,imagePix;
	switch(region)
	{
		case EDIT_CORE:
			editColor = RGB(255,0,0);
			break;

		case EDIT_OUTLINE:
			editColor = RGB(0,255,0);
			break;
		
		case EDIT_SHADE:
			editColor = RGB(0,0,255);
			break;

		default:break;
	}
	imagePix = RGB(0,0,0);

	for(int x = size.left;x<size.right;x++)
	{
		for(int y = size.top;y<size.bottom;y++)
		{
			imagePix = GetPixelFast(&m_imgMask,x,y);

			if((imagePix & editColor) == editColor)
				SetPixelFast(&m_imgMask,x,y,(imagePix & (~editColor)));
			else
			{	
				UpdateEditBox(CPoint(x,y));
				SetPixelFast(&m_imgMask,x,y,(imagePix | editColor));
			}
		}
	}
}


void CImageSelection::EraseArea(CRect size,EditEnum region)
{
	if((size.left < 0) || (size.right > m_imgMask.GetWidth()) || (size.top < 0) || (size.bottom > m_imgMask.GetHeight()))
	{
		if(size.left < 0)
			size.left = 0;
		if(size.right > m_imgMask.GetWidth())
			size.right = m_imgMask.GetWidth();
		if(size.top < 0)
			size.top = 0;
		if(size.bottom > m_imgMask.GetHeight())
			size.bottom = m_imgMask.GetHeight();
	}

	COLORREF editColor,imagePix;
	switch(region)
	{
		case EDIT_CORE:
			editColor = RGB(255,0,0);
			break;

		case EDIT_OUTLINE:
			editColor = RGB(0,255,0);
			break;
		
		case EDIT_SHADE:
			editColor = RGB(0,0,255);
			break;

		default:break;
	}
	imagePix = RGB(0,0,0);

	for(int y = size.top;y<size.bottom;y++)
	{
		for(int x = size.left;x<size.right;x++)
		{
			imagePix = GetPixelFast(&m_imgMask,x,y);

			if((imagePix & editColor) == editColor)
				SetPixelFast(&m_imgMask,x,y,(imagePix & (~editColor)));
		}
	}
}

MaskVector* CImageSelection::GetCurrentMaskVector(EditEnum region)
{
	MaskVector *mask;

	switch(region)
	{
		case EDIT_CORE:
			mask = &m_coreVector;
			break;

		case EDIT_OUTLINE:
			mask = &m_outlineVector;
			break;
		
		case EDIT_SHADE:
			mask = &m_shadeVector;
			break;

		default:mask = NULL;
			break;
	}
	return mask;
}

void CImageSelection::LoadMaskPoints(EditEnum region,CRect loadRect)
{
	MaskVector *mask;
	COLORREF editColor,pixColor,emptyPix;
	CRect maskRect;
	int x,y,r=0;
	if(loadRect.left == 0 && loadRect.right == 0 && loadRect.top == 0 && loadRect.bottom == 0)
		maskRect = m_editRect;
	else
	{
		if(loadRect.left < m_editRect.left && loadRect.right > m_editRect.left)
			maskRect.left = m_editRect.left;
		else
			maskRect.left = loadRect.left;

		if(loadRect.left < m_editRect.right && loadRect.right > m_editRect.right)
			maskRect.right = m_editRect.right;
		else
			maskRect.right = loadRect.right;

		if(loadRect.top < m_editRect.top && loadRect.bottom > m_editRect.top)
			maskRect.top = m_editRect.top;
		else
			maskRect.top = loadRect.top;

		if(loadRect.top < m_editRect.bottom && loadRect.bottom > m_editRect.bottom)
			maskRect.bottom = m_editRect.bottom;
		else
			maskRect.bottom = loadRect.bottom;
	}

	switch(region)
	{
		case EDIT_CORE:
			editColor = RGB(255,0,0);
			mask = &m_coreVector;
			break;

		case EDIT_OUTLINE:
			editColor = RGB(0,255,0);
			mask = &m_outlineVector;
			break;
		
		case EDIT_SHADE:
			editColor = RGB(0,0,255);
			mask = &m_shadeVector;
			break;

		default:return;
			break;
	}

	emptyPix =  RGB(0,0,0);

	if(m_imgMask.IsNull() || (mask == NULL))
		return;
	mask->clear();
	
	
	for(int i = maskRect.top ; i<maskRect.bottom;i++)
	{
		for(int j = maskRect.left ; j<maskRect.right;j++)
		{
			pixColor = GetPixelFast(&m_imgMask,j,i);
			if ((pixColor & editColor) != emptyPix)
			{	
				if(r == 0)
				{
					x = j;
					y = i;
					r = 1;
				}
				else
					r++;
			}
			else
			{
				if(r > 0)
				{
					mask->push_back(VectorRun(CPoint(x,y),r));
				}
				r=0;
			}
		}
		if(r > 0)
		{
			mask->push_back(VectorRun(CPoint(x,y),r));
		}
		r=0;
	}
}
void CImageSelection::DeltePoints()
{
	m_coreVector.clear();
	m_outlineVector.clear();
	m_shadeVector.clear();
	m_isChanged = true;
}

void CImageSelection::ResetMask()
{
	if(m_isChanged)
	{
		if(!m_imgMask.IsNull())
			m_imgMask.Destroy();
	}
	m_isChanged = false;
}

BOOL CImageSelection::ChangeLine(CPoint point1,CPoint point2,EditEnum region,bool isAdded,int size)
{
	bool found = false,changedAxis = false;
	CPoint pointUp,pointDown,point;
	MaskVector::iterator it,it_backup;
	MaskVector *pointsVector;
	COLORREF pixColor,editColor;
	int i,j,halfSize,highSize,widthSize,swap;
	double xLine;
	halfSize = size>>1;
	if(point1.x >= m_imgMask.GetWidth() || point1.y >= m_imgMask.GetHeight() || point1.x<0 || point1.y<0)
		return FALSE;
	if(point2.x >= m_imgMask.GetWidth() || point2.y >= m_imgMask.GetHeight() || point2.x<0 || point2.y<0)
		return FALSE;
	switch(region)
	{
		case EDIT_CORE:
			editColor = RGB(255,0,0);
			pointsVector = &m_coreVector;
			break;

		case EDIT_OUTLINE:
			editColor = RGB(0,255,0);
			pointsVector = &m_outlineVector;
			break;
		
		case EDIT_SHADE:
			editColor = RGB(0,0,255);
			pointsVector = &m_shadeVector;
			break;

		default:break;
	}
	

	if(point1.y < point2.y)
	{
		pointUp = point1;
		pointDown = point2;
	}
	else
	{
		pointUp = point2;
		pointDown = point1;
	}

	if(((pointDown.y-pointUp.y)*(pointDown.y-pointUp.y)) < ((pointDown.x-pointUp.x)*(pointDown.x-pointUp.x)))
	{
		swap = pointDown.x;
		pointDown.x = pointDown.y;
		pointDown.y = swap;

		swap = pointUp.x;
		pointUp.x = pointUp.y;
		pointUp.y = swap;

		if(pointDown.y < pointUp.y)
		{
			point = pointDown;
			pointDown = pointUp;
			pointUp= point;
		}

		changedAxis = true;
	}

	highSize = (pointDown.y-pointUp.y);
	
	if((pointDown.y-pointUp.y) == 0)
	{
			widthSize = pointDown.x - pointUp.x;
			highSize = 1;
			if(widthSize < 0)
			{
				widthSize = - widthSize;
				xLine =  pointDown.x;
			}
			else
				xLine =  pointUp.x;
	}
	else
		widthSize = size;

	for(i=0;i<=highSize;i++)
	{
		point.y = pointUp.y-halfSize+i;
				
		if((pointDown.y-pointUp.y) != 0)
		{
			xLine = double((i)*double((pointDown.x-pointUp.x)))/double(pointDown.y-pointUp.y)+pointUp.x;
		}
						
		if(!( (point.y <0) || (((point.y+size-1) >= m_imgMask.GetHeight()) && (!changedAxis)) || ((point.y+size-1) >= m_imgMask.GetWidth() && (changedAxis))))
		{
			for(j=0;j<widthSize;j++)
			{
				for(int k=0;k<size;k++)
				{
					point.y = pointUp.y - halfSize + i + k;
				
					point.x = int(xLine) + j - halfSize;

									
					if(!( (point.x<0) || ((point.x >= m_imgMask.GetWidth()) && (!changedAxis)) || ((point.x >= m_imgMask.GetHeight()) && (changedAxis))) )
					{
						if(changedAxis)
						{
							swap = point.x;
							point.x = point.y;
							point.y = swap;

						}
						pixColor = GetPixelFast(&m_imgMask,point.x,point.y);
					
						if (((pixColor & editColor) == 0) && isAdded)
						{	
							pointsVector->push_back(VectorRun(point,1));
							SetPixelFast(&m_imgMask,point.x,point.y,(pixColor | editColor));
						}
						else if (((pixColor & editColor) == editColor) && !isAdded)
						{	
							SetPixelFast(&m_imgMask,point.x,point.y,(pixColor & (~editColor)));
						}
					}
				}
			}
		}
	}

	point.x = point1.x-halfSize;
	point.y = point1.y-halfSize;
	
	if(point.x < 0)
		point.x = 0;
	
	if(point.y < 0)
		point.y = 0;
	
	UpdateEditBox(point);

	if(size != 1)
	{

		point.x = point1.x-halfSize+size-1;
		point.y = point1.y-halfSize+size-1;
		
		if(point.x >= m_imgMask.GetWidth())
			point.x = m_imgMask.GetWidth()-1;
		
		if(point.y >= m_imgMask.GetHeight())
			point.y = m_imgMask.GetHeight()-1;
		
		UpdateEditBox(point);
	}

	point.x = point2.x-halfSize;
	point.y = point2.y-halfSize;
	
	if(point.x < 0)
		point.x = 0;
	
	if(point.y < 0)
		point.y = 0;
	
	UpdateEditBox(point);

	if(size != 1)
	{

		point.x = point2.x-halfSize+size-1;
		point.y = point2.y-halfSize+size-1;
		
		if(point.x >= m_imgMask.GetWidth())
			point.x = m_imgMask.GetWidth()-1;
		
		if(point.y >= m_imgMask.GetHeight())
			point.y = m_imgMask.GetHeight()-1;
		
		UpdateEditBox(point);
	}


	return TRUE;
}


BOOL CImageSelection::ChangePoint(CPoint point,EditEnum region,bool isAdded,int size)
{
	bool found = false;
	MaskVector::iterator it,it_backup;
	MaskVector *pointsVector;
	COLORREF pixColor,editColor;
	int i,j,halfSize;
	CPoint mainPoint=point;
	halfSize = size>>1;
	if(point.x >= m_imgMask.GetWidth() || point.y >= m_imgMask.GetHeight())
		return FALSE;
	switch(region)
	{
		case EDIT_CORE:
			editColor = RGB(255,0,0);
			pointsVector = &m_coreVector;
			break;

		case EDIT_OUTLINE:
			editColor = RGB(0,255,0);
			pointsVector = &m_outlineVector;
			break;
		
		case EDIT_SHADE:
			editColor = RGB(0,0,255);
			pointsVector = &m_shadeVector;
			break;

		default:break;
	}

	for(i=0;i<size;i++)
	{
		point.x = mainPoint.x-halfSize+i;
		if(!(point.x <0 || point.x >= m_imgMask.GetWidth()))
			for(j=0;j<size;j++)
			{
				point.y = mainPoint.y-halfSize+j;
				if(!(point.y<0 || point.y >= m_imgMask.GetHeight()))
				{
					pixColor = GetPixelFast(&m_imgMask,point.x,point.y);
				
					if (((pixColor & editColor) == 0) && isAdded)
					{	
						pointsVector->push_back(VectorRun(point,1));
						SetPixelFast(&m_imgMask,point.x,point.y,(pixColor | editColor));
					}
					else if (((pixColor & editColor) == editColor) && !isAdded)
					{	
						SetPixelFast(&m_imgMask,point.x,point.y,(pixColor & (~editColor)));
					}
				}
			}
	}
	point.x = mainPoint.x-halfSize;
	point.y = mainPoint.y-halfSize;
	
	if(point.x < 0)
		point.x = 0;
	
	if(point.y < 0)
		point.y = 0;
	
	UpdateEditBox(point);

	if(size != 1)
	{

		point.x = mainPoint.x-halfSize+size-1;
		point.y = mainPoint.y-halfSize+size-1;
		
		if(point.x >= m_imgMask.GetWidth())
			point.x = m_imgMask.GetWidth()-1;
		
		if(point.y >= m_imgMask.GetHeight())
			point.y = m_imgMask.GetHeight()-1;
		
		UpdateEditBox(point);
	}


	return TRUE;
}


std::vector<MaskVector> CImageSelection::GetAllPoints()
{
	std::vector<MaskVector> mapPoints;
	bool c = m_core,o = m_outline,s = m_shade;

	COLORREF pixColor;
	CImage *pointsMask;
	int x,y,r = 0, g = 9;

	m_core = m_outline = m_shade = true;
	for(int i = 0;i<7;i++)
	{
		mapPoints.push_back(MaskVector());
	}

	pointsMask = MergePoints(&m_imgMask,false);
	for(int i = m_editRect.top ; i<m_editRect.bottom;i++)
	{
		for(int j = m_editRect.left ; j<m_editRect.right;j++)
		{
			pixColor = GetPixelFast(pointsMask,j,i);
			switch(pixColor)
			{
			case RGB(255,0,0):	if(g != 0)
								{
									if(r != 0)
									{
										mapPoints[g].push_back(VectorRun(CPoint(x,y),r));
										r = 0;
									}
									x = j;
									y = i;
									g = 0;
								}
								r++;
								break;
			case RGB(0,255,0):	if(g != 1)
								{
									if(r != 0)
									{
										mapPoints[g].push_back(VectorRun(CPoint(x,y),r));
										r = 0;
									}
									x = j;
									y = i;
									g = 1;
								}
								r++;
								break;
			case RGB(255,255,0):if(g != 2)
								{
									if(r != 0)
									{
										mapPoints[g].push_back(VectorRun(CPoint(x,y),r));
										r = 0;
									}
									x = j;
									y = i;
									g = 2;
								}
								r++;
								break;
			case RGB(0,0,255):	if(g != 3)
								{
									if(r != 0)
									{
										mapPoints[g].push_back(VectorRun(CPoint(x,y),r));
										r = 0;
									}
									x = j;
									y = i;
									g = 3;
								}
								r++;
								break;
			case RGB(255,0,255):if(g != 4)
								{
									if(r != 0)
									{
										mapPoints[g].push_back(VectorRun(CPoint(x,y),r));
										r = 0;
									}
									x = j;
									y = i;
									g = 4;
								}
								r++;
								break;
			case RGB(0,255,255):if(g != 5)
								{
									if(r != 0)
									{
										mapPoints[g].push_back(VectorRun(CPoint(x,y),r));
										r = 0;
									}
									x = j;
									y = i;
									g = 5;
								}
								r++;
								break;
			case RGB(255,255,255):if(g != 6)
								{
									if(r != 0)
									{
										mapPoints[g].push_back(VectorRun(CPoint(x,y),r));
										r = 0;
									}
									x = j;
									y = i;
									g = 6;
								}
								r++;
								break;
			default: 
								if(r > 0)
								{
									mapPoints[g].push_back(VectorRun(CPoint(x,y),r));
									r = 0;
								}
								g = 9;
								break;
				
			}
		}
		if(r > 0)
		{
			mapPoints[g].push_back(VectorRun(CPoint(x,y),r));
			r = 0;
			g = 9;
		}
	}
	m_core = c;
	m_shade = s;
	m_outline = o;
	delete pointsMask;
	return mapPoints;
}

void CImageSelection::SetCore(bool core)
{
	m_core = core;
	m_isChanged = true;
}

void CImageSelection::SetShade(bool shade)
{
	m_shade = shade;
	m_isChanged = true;
}
void CImageSelection::SetOutline(bool outline)
{
	m_outline = outline;
	m_isChanged = true;
}

bool CImageSelection::IsEmpty()
{
	return (m_coreVector.empty() && m_outlineVector.empty() && m_shadeVector.empty());
}

CImageSelection & CImageSelection::operator= (const CImageSelection &copy)
{
	if(&copy == this)
		return *this;
	else 
	{
		this->m_shadeVector.clear();
		this->m_shadeVector = copy.m_shadeVector;
		this->m_core = copy.m_core;
		this->m_outline = copy.m_outline;
		this->m_shade = copy.m_shade;
		this->m_coreVector.clear();
		this->m_coreVector = copy.m_coreVector;
		this->m_outlineVector.clear();
		this->m_outlineVector = copy.m_outlineVector;
		this->m_isChanged = copy.m_isChanged;
		this->m_imgMask.Destroy();
		this->m_imgMask = copy.m_imgMask;
	}
	return *this;
}

COLORREF CImageSelection::GetPixelFast(CImage *pImage,int x, int y)
{
   ASSERT(pImage != NULL);
   ASSERT((x<pImage->GetWidth() && x>=0 && y>=0 && y<pImage->GetHeight()));
   int bbp = pImage->GetBPP();
   BYTE* p = NULL;
   BYTE r,g,b;
   switch(bbp)
   {
   case 32:
   case 24:
			p=(BYTE*)pImage->GetPixelAddress(x,y);
			b=*p;
			p++;
			g=*p;
			p++;
			r=*p;
			break;
   case 16:
			WORD* pW;
			pW =(WORD*)pImage->GetPixelAddress(x,y);
			b=(*pW & 0x001f);
			g=(*pW & 0x03e0);
			r=(*pW & 0x7c00);
			break;
   case 8:
			RGBQUAD colorQuad8;
			p=(BYTE*)pImage->GetPixelAddress(x,y);
			pImage->GetColorTable(UINT(*p),1,&colorQuad8);
			b = colorQuad8.rgbBlue;
			g = colorQuad8.rgbGreen;
			r = colorQuad8.rgbRed;
			break;
	case 4:
			RGBQUAD colorQuad4;
			UINT pos;
			p=(BYTE*)pImage->GetPixelAddress(x,y);
			if(((x*y) % 2) == 0)
				pos = *p & 0x0F;
			else
				pos = *p & 0xF0;

			pImage->GetColorTable(pos,1,&colorQuad4);
			b = colorQuad4.rgbBlue;
			g = colorQuad4.rgbGreen;
			r = colorQuad4.rgbRed;
			break;
	case 1:
			RGBQUAD colorQuad1;
			BYTE bitPos;
			BYTE maskPos;
			maskPos= 1;
			p=(BYTE*)pImage->GetPixelAddress(x,y);
			bitPos = 8-((x) % 8);
			for(int i = 1 ;i<bitPos;i++)
				maskPos = maskPos<<1;
			maskPos = (*p) & maskPos;
			if(maskPos > 0)
				pImage->GetColorTable(1,1,&colorQuad1);
			else
				pImage->GetColorTable(0,1,&colorQuad1);
			b = colorQuad1.rgbBlue;
			g = colorQuad1.rgbGreen;
			r = colorQuad1.rgbRed;
			break;
   default:
			return pImage->GetPixel(x,y);
			break;

   }
   
   return RGB(r,g,b);
}

void CImageSelection::SetPixelFast(CImage *pImage,int x, int y, COLORREF color)
{
   ASSERT(pImage != NULL);
   ASSERT((x<pImage->GetWidth() && x>=0 && y>=0 && y<pImage->GetHeight()));
   int bbp = pImage->GetBPP();
   BYTE r,g,b;
   BYTE* p = NULL;
  
   WORD pColor = 0;
   b = GetBValue(color);
   r = GetRValue(color);
   g = GetGValue(color);
   switch(bbp)
   {
   case 32:
   case 24:
			
			p=(BYTE*)pImage->GetPixelAddress(x,y);
			*p++=b;
			*p++=g;
			*p=r;
			break;
   case 16:
			WORD* pW;
			pW =(WORD*)pImage->GetPixelAddress(x,y);
			b=(b & 0x1f);
			g=(g & 0x1f);
			r=(r & 0x1f);
			pColor = r;
			pColor = pColor<5;
			pColor = pColor | g;
			pColor = pColor<5;
			pColor = pColor | b;
			*pW = pColor;
			break;
   default:
			pImage->SetPixel(x,y,color);
			break;

   }
 }

bool operator< (const MSXML2::IXMLDOMNodePtr &op1,const MSXML2::IXMLDOMNodePtr &op2)
{
	return (&op1 < &op2);
}

bool operator< (const CPoint op1, const CPoint op2)
{
	if(op1.x < op2.x)
		return true;
	else 
		if(op1.x == op2.x)
		{
			if(op1.y < op2.y)
				return true;
		}
	return false;
}



void CGTDoc::OnFileExportselection()
{
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView == NULL)
		return;
	gtView->OnFileSaveImage();
}


void CGTDoc::OnUpdateFileExportselection(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_imgOriginal.IsNull() && !m_imgSelection.IsEmpty());
}

void CGTDoc::OnUpdateFileCopyTextAll(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_imgOriginal.IsNull());
}


void CGTDoc::OnExportselectionColor()
{
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView == NULL)
		return;
	gtView->OnFileSaveImage(true);
}


void CGTDoc::OnExportselectionBlack()
{
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView == NULL)
		return;
	gtView->OnFileSaveImage(false,false,true);
}


void CGTDoc::OnExportBlackselectionText()
{
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView == NULL)
		return;
	gtView->OnFileSaveImage(false,true,false);
	
}

void CGTDoc::OnExportColorselectionText()
{
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView == NULL)
		return;
	gtView->OnFileSaveImage(true,true,false);
	
}

void CGTDoc::OnUpdateToolsAreatextcopier(CCmdUI *pCmdUI)
{
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView == NULL)
		return;
	pCmdUI->SetCheck(gtView->GetAreaOCRCheck());
	pCmdUI->SetCheck(GetToolState() == OCR_TOOL);
	pCmdUI->Enable(!m_imgOriginal.IsNull());
}

void CGTDoc::OnToolsAreatextcopier()
{
	if(GetToolState() != OCR_TOOL)
		SetToolState(OCR_TOOL);
	else
		SetToolState(NO_TOOL);
}

void CGTDoc::OnToolsDisablePopUp()
{
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView == NULL)
		return;
	gtView->SetNoOCRPopUp();
}

void CGTDoc::OnUpdateDisablePopUp(CCmdUI *pCmdUI)
{
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView == NULL)
		return;
	pCmdUI->Enable(true);
	pCmdUI->SetCheck(gtView->GetNoOCRPopUp());
}

void CGTDoc::OnUpdateFileSelectionOCR(CCmdUI *pCmdUI)
{
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView == NULL)
		return;
	
	if(!m_imgSelection.GetImageMask()->IsNull() && !m_imgOriginal.IsNull())
		pCmdUI->Enable(!GetImageSelection()->IsEmpty() && (GetImageSelection()->GetOutline() || GetImageSelection()->GetShade() || GetImageSelection()->GetCore()));
	else
		pCmdUI->Enable(FALSE);
	
	pCmdUI->SetCheck(gtView->GetAreaOCRCheck());
	m_isSelectionOCR = gtView->GetAreaOCRCheck();
}

void CGTDoc::OnFileSelectionOCR()
{
	POSITION pos = GetFirstViewPosition();
	CGTView* gtView = (CGTView*) GetNextView(pos);
	if(gtView == NULL)
		return;
	m_isSelectionOCR = !m_isSelectionOCR;
	gtView->OnToolsAreatextcopier(m_isSelectionOCR);
}

void CGTDoc::OnHelpHelp()
{
	ShellExecute(NULL,L"open",L"http://www.youtube.com/watch?v=G3v1uGn2_yc",NULL, NULL, SW_SHOWNORMAL);
}
