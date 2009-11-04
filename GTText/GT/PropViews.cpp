// PropViews.cpp: archivo de implementación
//

#include "stdafx.h"
#include "GT.h"
#include "PropViews.h"
#include "GTDoc.h"
#include "GTXMLView.h"




// CMetaDataView

IMPLEMENT_DYNCREATE(CMetaDataView, CFormView)

CMetaDataView::CMetaDataView()
	: CFormView(CMetaDataView::IDD),
		c_comentaries(_T("")),
		c_creator(_T(""))
{
}

	
CMetaDataView::~CMetaDataView()
{
}

void CMetaDataView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, c_comentaries);
	DDX_Text(pDX, IDC_EDIT1, c_creator);
}

void CMetaDataView::OnUpdate()
{
	MSXML2::IXMLDOMNodePtr pNodeP,pNodeC;
	CGTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	NodeName sectionName;
	UpdateData(true);

	sectionName = pDoc->GetCurrentSection()->type;

	pNodeP = pDoc->GetNodeFromCurrSection(pcComments);
	if (pNodeP != NULL)
	{
		pNodeC = pNodeP->GetchildNodes()->nextNode();
		if (pNodeC != NULL){
			c_comentaries = W2T(_bstr_t(pNodeC->GetnodeValue()));
		}
	}

	pNodeP = pDoc->GetNodeFromCurrSection(pcCreator);
	if (pNodeP != NULL)
	{
		pNodeC = pNodeP->GetchildNodes()->nextNode();
		if (pNodeC != NULL)
			c_creator = W2T(_bstr_t(pNodeC->GetnodeValue()));
	}
	UpdateData(FALSE);
}


void CMetaDataView::OnInitialUpdate()
{
	
	CFormView::OnInitialUpdate();
	
	CSize sizeTotal;
	// TODO: calcular el tamaño total de esta vista
	sizeTotal.cx = 100;
	sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


BEGIN_MESSAGE_MAP(CMetaDataView, CFormView)
	ON_EN_CHANGE(IDC_EDIT1, &CMetaDataView::OnBnClickedChangeMeta)
	ON_EN_CHANGE(IDC_EDIT2, &CMetaDataView::OnBnClickedChangeMeta)
END_MESSAGE_MAP()


// Diagnósticos de CMetaDataView

#ifdef _DEBUG
void CMetaDataView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMetaDataView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// Controladores de mensajes de CMetaDataView

CGTDoc* CMetaDataView::GetDocument() const // La versión de no depuración es en línea
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGTDoc)));
	return (CGTDoc*)m_pDocument;
}


void CMetaDataView::OnBnClickedChangeMeta()
{
	MSXML2::IXMLDOMNodePtr pNodeP,pNodeC;
	CGTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	POSITION pos = pDoc->GetFirstViewPosition();
	pDoc->GetNextView(pos);
	CXMLView* xmlView = (CXMLView*) pDoc->GetNextView(pos);
	if(xmlView == NULL)
		return;

	UpdateData(true);
	pNodeP = pDoc->GetNodeFromCurrSection(pcComments);
	if (pNodeP != NULL)
	{
		pNodeC = pNodeP->GetchildNodes()->nextNode();
		if (pNodeC != NULL){
			pNodeC->put_text(BSTR(c_comentaries.GetString()));
			xmlView->PrintNodeName(pNodeC,c_comentaries);
		}
	}

	pNodeP = pDoc->GetNodeFromCurrSection(glyph_pcComments);
	if (pNodeP != NULL)
	{
		pNodeC = pNodeP->GetchildNodes()->nextNode();
		if (pNodeC != NULL){
			pNodeC->put_text(BSTR(c_comentaries.GetString()));
		}
	}


	pNodeP = pDoc->GetNodeFromCurrSection(pcCreator);
	if (pNodeP != NULL)
	{
		pNodeC = pNodeP->GetchildNodes()->nextNode();
		if (pNodeC != NULL)
		{
				pNodeC->put_text(BSTR(c_creator.GetString()));
				xmlView->PrintNodeName(pNodeC,c_creator);
		}
	}

	pNodeP = pDoc->GetNodeFromCurrSection(glyph_pcCreator);
	if (pNodeP != NULL)
	{
		pNodeC = pNodeP->GetchildNodes()->nextNode();
		if (pNodeC != NULL)
				pNodeC->put_text(BSTR(c_creator.GetString()));
	}
	pDoc->SetModifiedFlag(TRUE);
	/*pDoc->SetLoad(false);
	xmlView->OnUpdate(this,0,0);*/
}



// CTextRegionView

IMPLEMENT_DYNCREATE(CTextRegionView, CFormView)

CTextRegionView::CTextRegionView()
	: CFormView(CTextRegionView::IDD)
	, c_txtcolor(_T(""))
	, c_bgrndColor(_T(""))
	, c_fontSize(_T(""))
	, c_language(_T(""))
	, c_orientation(_T(""))
	, c_script(_T(""))
	, c_readingOrient(_T(""))
	, c_readingDirection(_T(""))
	, c_secondScript(_T(""))
	, c_secondLange(_T(""))
	, c_reVideo(FALSE)
	, c_textEqui(_T(""))
{
}

CTextRegionView::~CTextRegionView()
{
}

void CTextRegionView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_COMBO16, c_bgrndColor);
	DDX_Text(pDX, IDC_COMBO4, c_txtcolor);	
	DDX_Text(pDX, IDC_EDIT25, c_fontSize);
	DDX_Text(pDX, IDC_COMBO6, c_language);
	DDX_Text(pDX, IDC_EDIT17, c_orientation);
	DDX_Text(pDX, IDC_COMBO8, c_script);
	DDX_Text(pDX, IDC_EDIT20, c_readingOrient);
	DDX_Text(pDX, IDC_COMBO11, c_readingDirection);
	DDX_Text(pDX, IDC_COMBO13, c_secondScript);
	DDX_Text(pDX, IDC_COMBO14, c_secondLange);
	DDX_Check(pDX, IDC_CHECK2, c_reVideo);
	DDX_Text(pDX, IDC_COMBO15, c_txttype);
	DDX_Text(pDX, IDC_EDIT12, c_textEqui);
}



void CTextRegionView::OnUpdate()
{
	MSXML2::IXMLDOMNodePtr pNodeP,pNodeC;
	MSXML2::IXMLDOMAttributePtr pNodeA;
	CString video;
	CGTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	pNodeA = pDoc->GetNodeFromCurrSection(txt_bgcolor);
	if (pNodeA != NULL)
		c_bgrndColor = W2T(_bstr_t(pNodeA->GetnodeValue()));
	else
		c_bgrndColor = _T("");
		
	pNodeA = pDoc->GetNodeFromCurrSection(txt_color);
	if (pNodeA != NULL)
		c_txtcolor = W2T(_bstr_t(pNodeA->GetnodeValue()));
	else
		c_txtcolor = _T("");

	pNodeA = pDoc->GetNodeFromCurrSection(txt_font_size);
	if (pNodeA != NULL)
		c_fontSize = W2T(_bstr_t(pNodeA->GetnodeValue()));
	else
		c_fontSize = _T("");
		
	pNodeA = pDoc->GetNodeFromCurrSection(txt_primary_language);
	if (pNodeA != NULL)
		c_language = W2T(_bstr_t(pNodeA->GetnodeValue()));
	else
		c_language = _T("");

	pNodeA = pDoc->GetNodeFromCurrSection(txt_orientation);
	if (pNodeA != NULL)
		c_orientation = W2T(_bstr_t(pNodeA->GetnodeValue()));
	else
		c_orientation = _T("");

	pNodeA = pDoc->GetNodeFromCurrSection(txt_type);
	if (pNodeA != NULL)
		c_txttype = W2T(_bstr_t(pNodeA->GetnodeValue()));
	else
		c_txttype = _T("");

	pNodeA = pDoc->GetNodeFromCurrSection(txt_primary_script);
	if (pNodeA != NULL)
		c_script = W2T(_bstr_t(pNodeA->GetnodeValue()));
	else
		c_script = _T("");

	pNodeA = pDoc->GetNodeFromCurrSection(txt_reading_orientation);
	if (pNodeA != NULL)
		c_readingOrient = W2T(_bstr_t(pNodeA->GetnodeValue()));
	else
		c_readingOrient = _T("");

	pNodeA = pDoc->GetNodeFromCurrSection(txt_reading_direction);
	if (pNodeA != NULL)
		c_readingDirection = W2T(_bstr_t(pNodeA->GetnodeValue()));
	else
		c_readingDirection = _T("");
	
	pNodeA = pDoc->GetNodeFromCurrSection(txt_secondary_script);
	if (pNodeA != NULL)
		c_secondScript = W2T(_bstr_t(pNodeA->GetnodeValue()));
	else
		c_secondScript = _T("");
	
	pNodeA = pDoc->GetNodeFromCurrSection(txt_secondary_language);
	if (pNodeA != NULL)
		c_secondLange = W2T(_bstr_t(pNodeA->GetnodeValue()));
	else
		c_secondLange = _T(""); 
	
	pNodeA = pDoc->GetNodeFromCurrSection(txt_secondary_script);
	if (pNodeA != NULL)
		c_secondScript = W2T(_bstr_t(pNodeA->GetnodeValue()));
	else
		c_secondScript = _T(""); 

	pNodeP = pDoc->GetNodeFromCurrSection(plain_text);
	if (pNodeP != NULL)
	{
		pNodeC = pNodeP->GetchildNodes()->nextNode();
		if (pNodeC != NULL){
			c_textEqui = W2T(_bstr_t(pNodeC->GetnodeValue()));
		}
	}
	else
		c_textEqui = _T("");

	pNodeA = pDoc->GetNodeFromCurrSection(txt_reverse_video);
	if (pNodeA != NULL)
	{
		video = W2T(_bstr_t(pNodeA->GetnodeValue()));
		if (video.CompareNoCase(_T("Yes")) == 0)
			c_reVideo = true;
		else 
			c_reVideo = false;

	}
	else
		c_reVideo = false;

	UpdateData(FALSE);
}



void CTextRegionView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	CSize sizeTotal;
	// TODO: calcular el tamaño total de esta vista
	sizeTotal.cx = 100;
	sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


BEGIN_MESSAGE_MAP(CTextRegionView, CFormView)

	ON_BN_CLICKED(IDC_CHECK2, &CTextRegionView::OnBnClickedChRegion)
	ON_CBN_KILLFOCUS(IDC_COMBO3, &CTextRegionView::OnBnClickedChRegion)
	ON_CBN_EDITCHANGE(IDC_COMBO3, &CTextRegionView::OnBnClickedChRegion)
	ON_CBN_KILLFOCUS(IDC_COMBO4, &CTextRegionView::OnBnClickedChRegion)
	ON_CBN_EDITCHANGE(IDC_COMBO4, &CTextRegionView::OnBnClickedChRegion)
	ON_EN_CHANGE(IDC_EDIT25, &CTextRegionView::OnBnClickedChRegion)
	ON_CBN_KILLFOCUS(IDC_COMBO6, &CTextRegionView::OnBnClickedChRegion)
	ON_CBN_EDITCHANGE(IDC_COMBO6, &CTextRegionView::OnBnClickedChRegion)
	ON_EN_CHANGE(IDC_EDIT17, &CTextRegionView::OnBnClickedChRegion)
	ON_CBN_KILLFOCUS(IDC_COMBO8, &CTextRegionView::OnBnClickedChRegion)
	ON_CBN_EDITCHANGE(IDC_COMBO8, &CTextRegionView::OnBnClickedChRegion)
	ON_CBN_KILLFOCUS(IDC_COMBO9, &CTextRegionView::OnBnClickedChRegion)
	ON_CBN_EDITCHANGE(IDC_COMBO9, &CTextRegionView::OnBnClickedChRegion)
	ON_EN_CHANGE(IDC_EDIT20, &CTextRegionView::OnBnClickedChRegion)
	ON_CBN_KILLFOCUS(IDC_COMBO11, &CTextRegionView::OnBnClickedChRegion)
	ON_CBN_EDITCHANGE(IDC_COMBO11, &CTextRegionView::OnBnClickedChRegion)
	ON_EN_CHANGE(IDC_EDIT12, &CTextRegionView::OnBnClickedChRegion)
	ON_CBN_KILLFOCUS(IDC_COMBO13, &CTextRegionView::OnBnClickedChRegion)
	ON_CBN_EDITCHANGE(IDC_COMBO13, &CTextRegionView::OnBnClickedChRegion)
	ON_CBN_KILLFOCUS(IDC_COMBO14, &CTextRegionView::OnBnClickedChRegion)
	ON_CBN_EDITCHANGE(IDC_COMBO14, &CTextRegionView::OnBnClickedChRegion)
	ON_CBN_KILLFOCUS(IDC_COMBO15, &CTextRegionView::OnBnClickedChRegion)
	ON_CBN_EDITCHANGE(IDC_COMBO15, &CTextRegionView::OnBnClickedChRegion)
	ON_CBN_KILLFOCUS(IDC_COMBO16, &CTextRegionView::OnBnClickedChRegion)
	ON_CBN_EDITCHANGE(IDC_COMBO16, &CTextRegionView::OnBnClickedChRegion)
	


END_MESSAGE_MAP()


// Diagnósticos de CTextRegionView

#ifdef _DEBUG
void CTextRegionView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTextRegionView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// Controladores de mensajes de CTextRegionView

CGTDoc* CTextRegionView::GetDocument() const // La versión de no depuración es en línea
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGTDoc)));
	return (CGTDoc*)m_pDocument;
}

void CTextRegionView::OnBnClickedChRegion()
{
	MSXML2::IXMLDOMNodePtr pNodeP,pNodeC,pNewNode;
	MSXML2::IXMLDOMAttributePtr pNodeA;
	MSXML2::IXMLDOMNamedNodeMapPtr pAttrList;
	MSXML2::IXMLDOMAttributePtr pNewAttr;
	MSXML2::IXMLDOMElementPtr pNewElem;
	BSTR nameSpace;
	
	
	CString name;
	_bstr_t bstrName;

	CGTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
		
	POSITION pos = pDoc->GetFirstViewPosition();
	pDoc->GetNextView(pos);
	CXMLView* xmlView = (CXMLView*) pDoc->GetNextView(pos);
	if(xmlView == NULL)
		return;


	UpdateData(true);

	pNewNode = pDoc->GetNodeFromCurrSection(text_region);
	if (pNewNode != NULL)
	{
		pAttrList = pNewNode->Getattributes();

		pNodeA = pDoc->GetNodeFromCurrSection(txt_bgcolor);
		if (pNodeA != NULL)
		{
			pNodeA->put_text(BSTR(c_bgrndColor.GetString()));
			xmlView->PrintNodeName(pNodeA,c_bgrndColor);
		}
		else
		{	
			if(c_bgrndColor.Compare(_T("")) != 0)
			{
				name = _T("txt_bgcolor");
				bstrName = (_bstr_t)(LPCTSTR)name;
				pNewAttr = pDoc->GetPageDom()->createAttribute(bstrName);
				pNewAttr->put_text(BSTR(c_bgrndColor.GetString()));
				pAttrList->setNamedItem(pNewAttr);
				pDoc->AddNodeToCurrSection(pNewAttr,txt_bgcolor);
				xmlView->AddNodeToTree(pNewAttr,xmlView->GetSelectionItem());
				xmlView->PrintNodeName(pNewAttr,c_bgrndColor);
			}
		}
						
		pNodeA = pDoc->GetNodeFromCurrSection(txt_color);
		if (pNodeA != NULL)
		{
			pNodeA->put_text(BSTR(c_txtcolor.GetString()));
			xmlView->PrintNodeName(pNodeA,c_txtcolor);
		}
		else
		{	
			if(c_txtcolor.Compare(_T("")) != 0)
			{
				name = _T("txt_color");
				bstrName = (_bstr_t)(LPCTSTR)name;
				pNewAttr = pDoc->GetPageDom()->createAttribute(bstrName);
				pNewAttr->put_text(BSTR(c_txtcolor.GetString()));
				pAttrList->setNamedItem(pNewAttr);
				pDoc->AddNodeToCurrSection(pNewAttr,txt_color);
				xmlView->AddNodeToTree(pNewAttr,xmlView->GetSelectionItem());
				xmlView->PrintNodeName(pNewAttr,c_txtcolor);
			}
		}
				
		pNodeA = pDoc->GetNodeFromCurrSection(txt_font_size);
		if (pNodeA != NULL)
		{
			pNodeA->put_text(BSTR(c_fontSize.GetString()));
			xmlView->PrintNodeName(pNodeA,c_fontSize);
		}
		else
		{	
			if(c_fontSize.Compare(_T("")) != 0)
			{
				name = _T("txt_font_size");
				bstrName = (_bstr_t)(LPCTSTR)name;
				pNewAttr = pDoc->GetPageDom()->createAttribute(bstrName);
				pNewAttr->put_text(BSTR(c_fontSize.GetString()));
				pAttrList->setNamedItem(pNewAttr);
				pDoc->AddNodeToCurrSection(pNewAttr,txt_font_size);
				xmlView->AddNodeToTree(pNewAttr,xmlView->GetSelectionItem());
				xmlView->PrintNodeName(pNewAttr,c_fontSize);
			}
		}

		pNodeA = pDoc->GetNodeFromCurrSection(txt_primary_language);
		if (pNodeA != NULL)
		{
			pNodeA->put_text(BSTR(c_language.GetString()));
			xmlView->PrintNodeName(pNodeA,c_language);
		}
		else
		{	
			if(c_language.Compare(_T("")) != 0)
			{
				name = _T("txt_primary_language");
				bstrName = (_bstr_t)(LPCTSTR)name;
				pNewAttr = pDoc->GetPageDom()->createAttribute(bstrName);
				pNewAttr->put_text(BSTR(c_language.GetString()));
				pAttrList->setNamedItem(pNewAttr);
				pDoc->AddNodeToCurrSection(pNewAttr,txt_primary_language);
				xmlView->AddNodeToTree(pNewAttr,xmlView->GetSelectionItem());
				xmlView->PrintNodeName(pNewAttr,c_language);
			}
		}
		
		pNodeA = pDoc->GetNodeFromCurrSection(txt_orientation);
		if (pNodeA != NULL)
		{
			pNodeA->put_text(BSTR(c_orientation.GetString()));
			xmlView->PrintNodeName(pNodeA,c_orientation);
		}
		else
		{	
			if(c_orientation.Compare(_T("")) != 0)
			{
				name = _T("txt_orientation");
				bstrName = (_bstr_t)(LPCTSTR)name;
				pNewAttr = pDoc->GetPageDom()->createAttribute(bstrName);
				pNewAttr->put_text(BSTR(c_orientation.GetString()));
				pAttrList->setNamedItem(pNewAttr);
				pDoc->AddNodeToCurrSection(pNewAttr,txt_orientation);
				xmlView->AddNodeToTree(pNewAttr,xmlView->GetSelectionItem());
				xmlView->PrintNodeName(pNewAttr,c_orientation);
			}
		}
		
		pNodeA = pDoc->GetNodeFromCurrSection(txt_type);
		if (pNodeA != NULL)
		{
			pNodeA->put_text(BSTR(c_txttype.GetString()));
			xmlView->PrintNodeName(pNodeA,c_txttype);
		}
		else
		{	
			if(c_txttype.Compare(_T("")) != 0)
			{
				name = _T("txt_type");
				bstrName = (_bstr_t)(LPCTSTR)name;
				pNewAttr = pDoc->GetPageDom()->createAttribute(bstrName);
				pNewAttr->put_text(BSTR(c_txttype.GetString()));
				pAttrList->setNamedItem(pNewAttr);
				pDoc->AddNodeToCurrSection(pNewAttr,txt_type);
				xmlView->AddNodeToTree(pNewAttr,xmlView->GetSelectionItem());
				xmlView->PrintNodeName(pNewAttr,c_txttype);
			}
		}
			
		pNodeA = pDoc->GetNodeFromCurrSection(txt_primary_script);
		if (pNodeA != NULL)
		{
			pNodeA->put_text(BSTR(c_script.GetString()));
			xmlView->PrintNodeName(pNodeA,c_script);
		}
		else
		{	
			if(c_script.Compare(_T("")) != 0)
			{
				name = _T("txt_primary_script");
				bstrName = (_bstr_t)(LPCTSTR)name;
				pNewAttr = pDoc->GetPageDom()->createAttribute(bstrName);
				pNewAttr->put_text(BSTR(c_script.GetString()));
				pAttrList->setNamedItem(pNewAttr);
				pDoc->AddNodeToCurrSection(pNewAttr,txt_primary_script);
				xmlView->AddNodeToTree(pNewAttr,xmlView->GetSelectionItem());
				xmlView->PrintNodeName(pNewAttr,c_script);
			}
		}
			
		pNodeA = pDoc->GetNodeFromCurrSection(txt_reading_orientation);
		if (pNodeA != NULL)
		{
			pNodeA->put_text(BSTR(c_readingOrient.GetString()));
			xmlView->PrintNodeName(pNodeA,c_readingOrient);
		}
		else
		{	
			if(c_readingOrient.Compare(_T("")) != 0)
			{
				name = _T("txt_reading_orientation");
				bstrName = (_bstr_t)(LPCTSTR)name;
				pNewAttr = pDoc->GetPageDom()->createAttribute(bstrName);
				pNewAttr->put_text(BSTR(c_readingOrient.GetString()));
				pAttrList->setNamedItem(pNewAttr);
				pDoc->AddNodeToCurrSection(pNewAttr,txt_reading_orientation);
				xmlView->AddNodeToTree(pNewAttr,xmlView->GetSelectionItem());
				xmlView->PrintNodeName(pNewAttr,c_readingOrient);
			}
		}

		pNodeA = pDoc->GetNodeFromCurrSection(txt_reading_direction);
		if (pNodeA != NULL)
		{
			pNodeA->put_text(BSTR(c_readingDirection.GetString()));
			xmlView->PrintNodeName(pNodeA,c_readingDirection);
		}
		else
		{	
			if(c_readingDirection.Compare(_T("")) != 0)
			{
				name = _T("txt_reading_direction");
				bstrName = (_bstr_t)(LPCTSTR)name;
				pNewAttr = pDoc->GetPageDom()->createAttribute(bstrName);
				pNewAttr->put_text(BSTR(c_readingDirection.GetString()));
				pAttrList->setNamedItem(pNewAttr);
				pDoc->AddNodeToCurrSection(pNewAttr,txt_reading_direction);
				xmlView->AddNodeToTree(pNewAttr,xmlView->GetSelectionItem());
				xmlView->PrintNodeName(pNewAttr,c_readingDirection);
			}
		}
			
		pNodeA = pDoc->GetNodeFromCurrSection(txt_secondary_script);
		if (pNodeA != NULL)
		{
			pNodeA->put_text(BSTR(c_secondScript.GetString()));
			xmlView->PrintNodeName(pNodeA,c_secondScript);
		}
		else
		{	
			if(c_secondScript.Compare(_T("")) != 0)
			{
				name = _T("txt_secondary_script");
				bstrName = (_bstr_t)(LPCTSTR)name;
				pNewAttr = pDoc->GetPageDom()->createAttribute(bstrName);
				pNewAttr->put_text(BSTR(c_secondScript.GetString()));
				pAttrList->setNamedItem(pNewAttr);
				pDoc->AddNodeToCurrSection(pNewAttr,txt_secondary_script);
				xmlView->AddNodeToTree(pNewAttr,xmlView->GetSelectionItem());
				xmlView->PrintNodeName(pNewAttr,c_secondScript);
			}
		}
			
		pNodeA = pDoc->GetNodeFromCurrSection(txt_secondary_language);
		if (pNodeA != NULL)
		{
			pNodeA->put_text(BSTR(c_secondLange.GetString()));
			xmlView->PrintNodeName(pNodeA,c_secondLange);
		}
		else
		{	
			if(c_secondLange.Compare(_T("")) != 0)
			{
				name = _T("txt_secondary_language");
				bstrName = (_bstr_t)(LPCTSTR)name;
				pNewAttr = pDoc->GetPageDom()->createAttribute(bstrName);
				pNewAttr->put_text(BSTR(c_secondLange.GetString()));
				pAttrList->setNamedItem(pNewAttr);
				pDoc->AddNodeToCurrSection(pNewAttr,txt_secondary_language);
				xmlView->AddNodeToTree(pNewAttr,xmlView->GetSelectionItem());
				xmlView->PrintNodeName(pNewAttr,c_secondLange);
			}
		}
			
		pNodeA = pDoc->GetNodeFromCurrSection(txt_secondary_script);
		if (pNodeA != NULL)
		{
			pNodeA->put_text(BSTR(c_secondScript.GetString()));
			xmlView->PrintNodeName(pNodeA,c_secondScript);
		}
		else
		{	
			if(c_secondScript.Compare(_T("")) != 0)
			{
				name = _T("txt_secondary_script");
				bstrName = (_bstr_t)(LPCTSTR)name;
				pNewAttr = pDoc->GetPageDom()->createAttribute(bstrName);
				pNewAttr->put_text(BSTR(c_secondScript.GetString()));
				pAttrList->setNamedItem(pNewAttr);
				pDoc->AddNodeToCurrSection(pNewAttr,txt_secondary_script);
				xmlView->AddNodeToTree(pNewAttr,xmlView->GetSelectionItem());
				xmlView->PrintNodeName(pNewAttr,c_secondScript);
			}
		}

		pNodeP = pDoc->GetNodeFromCurrSection(plain_text);
		if (pNodeP != NULL)
		{
			pNodeC = pNodeP->GetchildNodes()->nextNode();
			if (pNodeC != NULL){
				pNodeC->put_text(BSTR(c_textEqui.GetString()));
				pNodeP = pDoc->GetNodeFromCurrSection(text_region);
				if (pNodeP != NULL)
					xmlView->PrintNodeName(pNodeP,c_textEqui);
			}
		}
		else
		{
			if(c_textEqui.Compare(_T("")) != 0)
			{
				name = _T("text_equiv");
				bstrName = (_bstr_t)(LPCTSTR)name;
				pNewNode->get_namespaceURI(&nameSpace);
				pNewElem = pDoc->GetLinkDom()->createNode(_variant_t((short)pNewNode->GetnodeType()), bstrName, nameSpace);
				pNodeP = pNewNode->appendChild(pNewElem);

				name = _T("plain_text");
				bstrName = (_bstr_t)(LPCTSTR)name;
				pNewElem = pDoc->GetLinkDom()->createNode(_variant_t((short)pNewNode->GetnodeType()), bstrName, nameSpace);
				pNodeP = pNodeP->appendChild(pNewElem);

				pNodeC = pDoc->GetPageDom()->createTextNode(BSTR(c_textEqui.GetString()));
				pNodeP->appendChild(pNodeC);

				pDoc->AddNodeToCurrSection(pNodeP,plain_text);
				xmlView->PrintNodeName(pNodeP,c_textEqui);

			}
		}

		pNodeA = pDoc->GetNodeFromCurrSection(txt_reverse_video);
		if (pNodeA != NULL)
			if(c_reVideo)
			{
				pNodeA->put_text(BSTR(_T("Yes")));
				xmlView->PrintNodeName(pNodeA,CString("Yes"));
			}
			else
			{
				pNodeA->put_text(BSTR(_T("No")));
				xmlView->PrintNodeName(pNodeA,CString("No"));
			}
		else
		{
			if(c_reVideo)
			{
				name = _T("txt_reverse_video");
				bstrName = (_bstr_t)(LPCTSTR)name;
				pNewAttr = pDoc->GetPageDom()->createAttribute(bstrName);
				pNewAttr->put_text(BSTR(_T("Yes")));
				pAttrList->setNamedItem(pNewAttr);
				pDoc->AddNodeToCurrSection(pNewAttr,txt_reverse_video);
				xmlView->AddNodeToTree(pNewAttr,xmlView->GetSelectionItem());
				xmlView->PrintNodeName(pNewAttr,CString("Yes"));
			}
		}
		pDoc->SetModifiedFlag(TRUE);
	}
}



// CTextEquivalView

IMPLEMENT_DYNCREATE(CTextEquivalView, CFormView)

CTextEquivalView::CTextEquivalView()
	: CFormView(CTextEquivalView::IDD)
	, c_text(_T(""))

{
}

	
CTextEquivalView::~CTextEquivalView()
{
}

void CTextEquivalView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT16,c_text );
	DDX_Text(pDX, IDC_EDIT1, c_title);
}

void CTextEquivalView::OnUpdate()
{
	MSXML2::IXMLDOMNodePtr pNodeP,pNodeC;
	MSXML2::IXMLDOMAttributePtr pNodeA;
	CGTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	NodeName sectionName;

	UpdateData(true);

	sectionName = pDoc->GetCurrentSection()->type;
	switch(sectionName)
	{
		case text_line: c_title = _T("Text Line");
			break;
		case word: c_title = _T("Word");
			break;
	};

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
	UpdateData(FALSE);
}



void CTextEquivalView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	CSize sizeTotal;
	// TODO: calcular el tamaño total de esta vista
	sizeTotal.cx = 100;
	sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


BEGIN_MESSAGE_MAP(CTextEquivalView, CFormView)
	ON_EN_CHANGE(IDC_EDIT16, &CTextEquivalView::OnBnClickedChangedText)
END_MESSAGE_MAP()


// Diagnósticos de CTextEquivalView

#ifdef _DEBUG
void CTextEquivalView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTextEquivalView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// Controladores de mensajes de CTextEquivalView

CGTDoc* CTextEquivalView::GetDocument() const // La versión de no depuración es en línea
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGTDoc)));
	return (CGTDoc*)m_pDocument;
}

void CTextEquivalView::OnBnClickedChangedText()
{
	MSXML2::IXMLDOMNodePtr pNodeP,pNodeC,pNewNode;
	MSXML2::IXMLDOMAttributePtr pNodeA;
	MSXML2::IXMLDOMNamedNodeMapPtr pAttrList;
	MSXML2::IXMLDOMAttributePtr pNewAttr;
	MSXML2::IXMLDOMElementPtr pNewElem;
	BSTR nameSpace;
	bool isNew = false;

	
	CString name;
	_bstr_t bstrName;

	CGTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
		
	POSITION pos = pDoc->GetFirstViewPosition();
	pDoc->GetNextView(pos);
	CXMLView* xmlView = (CXMLView*) pDoc->GetNextView(pos);
	if(xmlView == NULL)
		return;
	
	UpdateData(true);

	pNewNode = pDoc->GetNodeFromCurrSection(pDoc->GetCurrentSection()->type);
	if (pNewNode != NULL)
	{
		pNodeP = pDoc->GetNodeFromCurrSection(plain_text);
		if (pNodeP != NULL)
		{
			pNodeC = pNodeP->GetchildNodes()->nextNode();
			if (pNodeC != NULL){
				pNodeC->put_text(BSTR(c_text.GetString()));
				xmlView->PrintNodeName(pNewNode,c_text);
			}
		}

		else
		{
			if(c_text.Compare(_T("")) != 0)
			{
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

				pNodeC = pDoc->GetPageDom()->createTextNode(BSTR(c_text.GetString()));
				pNodeP->appendChild(pNodeC);
			}

			pDoc->AddNodeToCurrSection(pNodeP,plain_text);
			xmlView->PrintNodeName(pNewNode,c_text);
		}
		pDoc->SetModifiedFlag(TRUE);
		if(isNew)
		{
			pDoc->SetLoad(false);
			xmlView->OnUpdate(this,0,0);
		}
	}
}





// CGlyphView

IMPLEMENT_DYNCREATE(CGlyphView, CFormView)

CGlyphView::CGlyphView()
	: CFormView(CGlyphView::IDD)
	, c_textglyph(_T(""))
	, c_symbol(FALSE)
	, c_ligature(FALSE)
{
}

CGlyphView::~CGlyphView()
{
}

void CGlyphView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT18,c_textglyph);
	DDX_Check(pDX, IDC_CHECKSYM, c_symbol);
	DDX_Check(pDX, IDC_CHECKLIG, c_ligature);
}

void CGlyphView::OnUpdate()
{
	MSXML2::IXMLDOMNodePtr pNodeP,pNodeC;
	MSXML2::IXMLDOMAttributePtr pNodeA;
	CString booleans;
	CGTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	pNodeP = pDoc->GetNodeFromCurrSection(plain_text);
	if (pNodeP != NULL)
	{
		pNodeC = pNodeP->GetchildNodes()->nextNode();
		if (pNodeC != NULL){
			c_textglyph = W2T(_bstr_t(pNodeC->GetnodeValue()));
		}
	}
	else
		c_textglyph = _T("");


	pNodeA = pDoc->GetNodeFromCurrSection(ligature);
	if (pNodeA != NULL)
	{
		booleans = W2T(_bstr_t(pNodeA->GetnodeValue()));
		if (booleans.CompareNoCase(_T("true")) == 0)
			c_ligature = true;
		else 
			c_ligature = false;
	}
	else
		c_ligature = false;

	pNodeA = pDoc->GetNodeFromCurrSection(symbol);
	if (pNodeA != NULL)
	{
		booleans = W2T(_bstr_t(pNodeA->GetnodeValue()));
		if (booleans.CompareNoCase(_T("true")) == 0)
			c_symbol = true;
		else 
			c_symbol = false;
	}
	else
		c_symbol = false;

	UpdateData(FALSE);
}



void CGlyphView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	CSize sizeTotal;
	// TODO: calcular el tamaño total de esta vista
	sizeTotal.cx = 100;
	sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


BEGIN_MESSAGE_MAP(CGlyphView, CFormView)
	ON_BN_CLICKED(IDC_CHECKLIG, &CGlyphView::OnBnClickedChangeGlph)
	ON_BN_CLICKED(IDC_CHECKSYM, &CGlyphView::OnBnClickedChangeGlph)
	ON_EN_CHANGE(IDC_EDIT18, &CGlyphView::OnBnClickedChangeGlph)
END_MESSAGE_MAP()


// Diagnósticos de CGlyphView

#ifdef _DEBUG
void CGlyphView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CGlyphView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// Controladores de mensajes de CGlyphView

CGTDoc* CGlyphView::GetDocument() const // La versión de no depuración es en línea
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGTDoc)));
	return (CGTDoc*)m_pDocument;
}



void CGlyphView::OnBnClickedChangeGlph()
{
	MSXML2::IXMLDOMNodePtr pNodeP,pNodeC,pNewNode;
	MSXML2::IXMLDOMAttributePtr pNodeA;
	MSXML2::IXMLDOMNamedNodeMapPtr pAttrList;
	MSXML2::IXMLDOMAttributePtr pNewAttr;
	MSXML2::IXMLDOMElementPtr pNewElem;
	BSTR nameSpace;

	
	CString name;
	_bstr_t bstrName;

	CGTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
		
	POSITION pos = pDoc->GetFirstViewPosition();
	pDoc->GetNextView(pos);
	CXMLView* xmlView = (CXMLView*) pDoc->GetNextView(pos);
	if(xmlView == NULL)
		return;

	UpdateData(true);

	pNewNode = pDoc->GetNodeFromCurrSection(glyph);
	if (pNewNode != NULL)
	{
		pAttrList = pNewNode->Getattributes();
		pNodeP = pDoc->GetNodeFromCurrSection(plain_text);
		if (pNodeP != NULL)
		{
			pNodeC = pNodeP->GetchildNodes()->nextNode();
			if (pNodeC != NULL){
				pNodeC->put_text(BSTR(c_textglyph.GetString()));
				xmlView->PrintNodeName(pNewNode,c_textglyph);
			}
		}
		else
		{
			if(c_textglyph.Compare(_T("")) != 0)
			{
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

				pNodeC = pDoc->GetPageDom()->createTextNode(BSTR(c_textglyph.GetString()));
				pNodeP->appendChild(pNodeC);

				pDoc->AddNodeToCurrSection(pNodeP,plain_text);
				xmlView->PrintNodeName(pNewNode,c_textglyph);
			}
		}

		pNodeA = pDoc->GetNodeFromCurrSection(ligature);
		if (pNodeA != NULL)
			if(c_ligature)
			{
				pNodeA->put_text(BSTR(_T("true")));
				xmlView->PrintNodeName(pNodeA,CString("true"));
			}
			else
			{
				pNodeA->put_text(BSTR(_T("false")));
				xmlView->PrintNodeName(pNodeA,CString("false"));
			}
		else
		{
			if(c_ligature)
			{
				name = _T("ligature");
				bstrName = (_bstr_t)(LPCTSTR)name;
				pNewAttr = pDoc->GetPageDom()->createAttribute(bstrName);
				pNewAttr->put_text(BSTR(_T("true")));
				pAttrList->setNamedItem(pNewAttr);
				pDoc->AddNodeToCurrSection(pNewAttr,ligature);
				xmlView->AddNodeToTree(pNewAttr,xmlView->GetSelectionItem());
				xmlView->PrintNodeName(pNewAttr,CString("true"));
			}
		}

		pNodeA = pDoc->GetNodeFromCurrSection(symbol);
		if (pNodeA != NULL)
			if(c_symbol)
			{
				pNodeA->put_text(BSTR(_T("true")));
				xmlView->PrintNodeName(pNodeA,CString("true"));
			}
			else
			{
				pNodeA->put_text(BSTR(_T("false")));
				xmlView->PrintNodeName(pNodeA,CString("false"));
			}
		else
		{
			if(c_symbol)
			{
				name = _T("symbol");
				bstrName = (_bstr_t)(LPCTSTR)name;
				pNewAttr = pDoc->GetPageDom()->createAttribute(bstrName);
				pNewAttr->put_text(BSTR(_T("true")));
				pAttrList->setNamedItem(pNewAttr);
				pDoc->AddNodeToCurrSection(pNewAttr,symbol);
				xmlView->AddNodeToTree(pNewAttr,xmlView->GetSelectionItem());
				xmlView->PrintNodeName(pNewAttr,CString("true"));
			}
		}
		pDoc->SetModifiedFlag(TRUE);
	}
}


// CBlankView

IMPLEMENT_DYNCREATE(CBlankView, CFormView)

CBlankView::CBlankView()
	: CFormView(CBlankView::IDD)
	
{
}

CBlankView::~CBlankView()
{
}

void CBlankView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT1, c_title);
}

void CBlankView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	CSize sizeTotal;
	// TODO: calcular el tamaño total de esta vista
	sizeTotal.cx = 50;
	sizeTotal.cy = 50;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


BEGIN_MESSAGE_MAP(CBlankView, CFormView)
	
END_MESSAGE_MAP()


// Diagnósticos de CBlankView

#ifdef _DEBUG
void CBlankView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CBlankView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// Controladores de mensajes de CBlankView

CGTDoc* CBlankView::GetDocument() const // La versión de no depuración es en línea
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGTDoc)));
	return (CGTDoc*)m_pDocument;
}


void CBlankView::OnUpdate()
{
	
	MSXML2::IXMLDOMNodePtr pNodeP,pNodeC;
	MSXML2::IXMLDOMAttributePtr pNodeA;
	CGTDoc* pDoc = GetDocument();

	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	NodeName sectionName;

	UpdateData(true);

	if(pDoc->GetCurrentSection() != NULL)
	{
		sectionName = pDoc->GetCurrentSection()->type;
		switch(sectionName)
		{
			case page: c_title = _T("Page");
				break;
			default: c_title = _T("GT Doc");
				break;
		};
	}

	UpdateData(FALSE);
}

// CFolders


CFolders::CFolders(CString &glyphFold,CString &pageFold) : CDialog(CFolders::IDD)
{
	c_glyphFolder = &glyphFold;
	c_pageFolder = &pageFold;
}

BOOL CFolders::OnInitDialog()
{
	CDialog::OnInitDialog();

	CEdit* pGlyph;
	CEdit* pPage;
	
	pGlyph = (CEdit*) GetDlgItem(IDC_GFOLDER);
	pPage = (CEdit*) GetDlgItem(IDC_PFOLDER);
	pGlyph->SetWindowTextW(LPCTSTR(CString(*c_glyphFolder)));
	pPage->SetWindowTextW(LPCTSTR(CString(*c_pageFolder)));

	return TRUE;

}

BEGIN_MESSAGE_MAP(CFolders,CDialog)
	ON_BN_CLICKED(IDOK, &CFolders::OnBnClickedOk)
END_MESSAGE_MAP()

void CFolders::OnBnClickedOk()
{
	CEdit* pGlyph;
	CEdit* pPage;
	
	pGlyph = (CEdit*) GetDlgItem(IDC_GFOLDER);
	pPage = (CEdit*) GetDlgItem(IDC_PFOLDER);
	pGlyph->GetWindowTextW((*c_glyphFolder));
	pPage->GetWindowTextW((*c_pageFolder));
	
	OnOK();
}
