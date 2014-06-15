// GTView.cpp: implementación de la clase CGTView
//



#include "stdafx.h"
#include "GT.h"

#include "GTDoc.h"
#include "GTView.h"
#include "GTXMLView.h"
#include "MainFrm.h"
#include <time.h>
#include "./Scanner/MainWnd.h"

#ifndef XPCOMPATIBLE
//#include "mfcpch.h"
// #define USE_VLD //Uncomment for Visual Leak Detector.
#if (defined _MSC_VER && defined USE_VLD)
#include <vld.h>
#endif

// Include automatically generated configuration file if running autoconf
#ifdef HAVE_CONFIG_H
#include "config_auto.h"
#endif
#ifdef USING_GETTEXT
#include <libintl.h>
#include <locale.h>
#define _(x) gettext(x)
#else
#define _(x) (x)
#endif
#ifdef HAVE_LIBLEPT
	#include "strngs.h"
	#include "tprintf.h"
	#include "tesseractmain.h"
#endif
#endif
#include "allheaders.h"
#include "baseapi.h"
#include "strngs.h"
#define _(x) (x)


#define NUM_LINES_OCR_SHOW 35




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CGTView

IMPLEMENT_DYNCREATE(CGTView, CScrollView)

BEGIN_MESSAGE_MAP(CGTView, CScrollView)
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	//ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


CGTView::CGTView()
{
	m_nImageSize = SIZE_NONE;
	m_zoom = 1;
	m_floodDistance = 10;
	m_bright = 128;
	m_nPenSize = PEN_1;
	m_isOutline = 0;
	m_isToAll = 0;
	m_showPen = true;
	m_isZoomed = false;
	m_isExt = 1;
	m_cursorMap.Create(1,1,24);
	m_cursorMap.SetPixelRGB(0,0,255,255,255);
	m_isHold = false;
	m_loadingFromClipboard = false;
	m_ocrRect.SetRectEmpty();
	m_ocrPoint = CPoint(-1,-1);
	m_checkOCRAreaTool = false;
	m_scaleRatio = 2;
	m_secondRound = false;
	m_nopopup = false;
	m_latestImport = imagefile;
}

CGTView::~CGTView()
{
	m_Map.Destroy();
	m_cursorMap.Destroy();
}

BOOL CGTView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: modificar aquí la clase Window o los estilos cambiando
	//  CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

BOOL CGTView::OnEraseBkgnd(CDC* pDC)
{
	
	if(!m_showPen && m_isZoomed)
		return CView::OnEraseBkgnd(pDC);
	return TRUE;
}
void CGTView::OnFileOpen()
{
	SetActiveWindow();
	CFrameWnd* pMainWnd = (CFrameWnd*)AfxGetMainWnd();
	CSplitterWnd* pParentSplitter = GetParentSplitter(this,true);
	CXMLView* xmlView = ((CXMLView*)((CSplitterWnd*)pParentSplitter->GetPane(0,1))->GetPane(0,0));
	xmlView->OnFileOpen();
}

void CGTView::OnLButtonDown(UINT nFlag,CPoint point)
{
	CGTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	SectionPtrs* sec = pDoc->GetCurrentSection();
	if(sec == NULL)
		return;
	if(pDoc->GetImage()->IsNull())
		return;

	CPoint ul;
	CPoint PxlReal;
	ul = this->GetScrollPosition();
	PxlReal = ul + point;
	PxlReal.x = int(double(PxlReal.x) / m_zoom);
	PxlReal.y = int(double(PxlReal.y) / m_zoom);
	point = PxlReal;

	if(pDoc->GetEditState() != EDIT_NONE && (sec->type == glyph))
	{
		pDoc->Backup();
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetDirty(true);
		pDoc->SetPenPoint(PxlReal);

		if(pDoc->GetToolState() == PIXEL_TOOL)
		{
			pDoc->SetPenPoint(PxlReal);
		}
		else
			pDoc->SetPenPoint(CPoint(-1,-1));

		m_showPen = true;
		m_isHold = false;
		OnLPaint(nFlag,point);
		m_isHold = false;
	}
	
	if(pDoc->GetToolState() == OCR_TOOL)
	{
		m_ocrPoint.x = point.x*m_zoom;
		m_ocrPoint.y = point.y*m_zoom;
		SetPixelFast(&m_cursorMap,0,0,RGB(0,0,63));
		if(pDoc->GetImageSelection()->GetOutline() || pDoc->GetImageSelection()->GetShade() || pDoc->GetImageSelection()->GetCore())
			m_checkOCRAreaTool = pDoc->IsOCRSelection();
		else
			m_checkOCRAreaTool = false;
		//m_ocrRect.SetRect(m_ocrPoint,m_ocrPoint);
	}
}

BOOL CGTView::OnMouseWheel(UINT nFlags,short zDelta,CPoint point)
{
	CGTDoc* pDoc = GetDocument();
	int pos,step;
	CSize barSize;
	CPoint PxlReal,ul;
	if(pDoc->GetImage()->IsNull())
		return FALSE;
	GetScrollBarSizes(barSize);
	step = int(pDoc->GetImage()->GetHeight()*m_zoom)/120;
	ul = this->GetScrollPosition();
	PxlReal = ul + point;
	PxlReal.x = int(double(PxlReal.x) / m_zoom);
	PxlReal.y = int(double(PxlReal.y) / m_zoom);
	if(zDelta < 0)
	{
		if(pDoc->GetToolState() == ZOOM_TOOL)
		{	
			
			if(m_zoom  > 0.0033333)
			{
				pDoc->SetZoomPoint(PxlReal,point);
				m_zoom = m_zoom / 5 * 4;
				//Uncomment/Comment to zoom using mouse point
				m_isZoomed = true;
				m_showPen = false;
				OnChangeSize(ID_ZOOM);
			}
			else
				return TRUE;
		}
		else if((!m_showPen && pDoc->GetToolState() != PIXEL_TOOL && pDoc->GetToolState() != REGION_TOOL || pDoc->GetEditState() == EDIT_NONE) && m_isHighSrcllBar)
		{
			pos = GetScrollPos(SB_VERT);
			if(pos !=  GetScrollLimit(1))
			{
				pos = pos + step*barSize.cy;
				if(pos < GetScrollLimit(1))
					SetScrollPos(1,(pos));
				else
					SetScrollPos(1,GetScrollLimit(1));
			}
			else
				return TRUE;
		}	
		else
			return TRUE;
	}
	else
	{
		if(pDoc->GetToolState() == ZOOM_TOOL)
		{
			if(m_zoom < 300)
			{
				pDoc->SetZoomPoint(PxlReal,point);
				m_zoom = m_zoom / 4 * 5;
				//Uncomment/Comment to zoom using mouse point
				m_isZoomed = true;
				m_showPen = false;
				OnChangeSize(ID_ZOOM);
			}
			else
				return TRUE;
			
		}
		else if((!m_showPen && pDoc->GetToolState() != PIXEL_TOOL && pDoc->GetToolState() != REGION_TOOL || pDoc->GetEditState() == EDIT_NONE) && m_isHighSrcllBar)
		{
			pos = GetScrollPos(SB_VERT);
			if(pos != 0)
			{
				pos = pos - step*barSize.cy;
				if(pos > 0)
					SetScrollPos(1,(pos));
				else 
					SetScrollPos(1,0);
			}
			else
				return TRUE;
		}
		else
			return TRUE;
	}
	m_ocrPoint = CPoint(-1,-1);
	Invalidate(false);
	return TRUE;
}

void CGTView::OnRButtonDown(UINT nFlag,CPoint point)
{
	CGTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	SectionPtrs* sec = pDoc->GetCurrentSection();
	if(sec == NULL)
		return;
	if(pDoc->GetImage()->IsNull())
		return;
	if((pDoc->GetEditState() != EDIT_NONE) && (sec->type == glyph))
	{	
		pDoc->Backup();
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetDirty(true);
		m_isHold = false;
		
		CPoint ul;
		CPoint PxlReal;
		ul = this->GetScrollPosition();
		PxlReal = ul + point;
		PxlReal.x = int(double(PxlReal.x) / m_zoom);
		PxlReal.y = int(double(PxlReal.y) / m_zoom);
		pDoc->SetPenPoint(PxlReal);
		point = PxlReal;
		
		if(pDoc->GetToolState() == PIXEL_TOOL)
		{
			pDoc->SetPenPoint(PxlReal);
		}
		else
			pDoc->SetPenPoint(CPoint(-1,-1));
		m_showPen = true;
		m_isHold = false;

		OnRPaint(nFlag,point);
		m_isHold = false;
		
	}
	m_ocrPoint = CPoint(-1,-1);
}

void CGTView::OnLButtonUp(UINT nFlag,CPoint point)
{
	CGTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CPoint ul,penPoint;
	CPoint PxlReal;
	CRect penRect;
	BYTE r,g,b;
	COLORREF imagePixel;
	int penSize; 
	int halfSize; 
	if(pDoc->GetImage()->IsNull())
		return;
	
	ul = this->GetScrollPosition();
	PxlReal = ul + point;
	PxlReal.x = int(double(PxlReal.x) / m_zoom);
	PxlReal.y = int(double(PxlReal.y) / m_zoom);
	ToolEnum state = pDoc->GetToolState();
	if(pDoc->GetEditState() == EDIT_NONE && state != ZOOM_TOOL)
	{
		state = NO_TOOL;
	}

	switch(state)
	{
		case REGION_TOOL:
			if(pDoc->GetEditState() != EDIT_NONE)
			{
				m_isHold = false;

				if((PxlReal.x >= 0) && (PxlReal.x < pDoc->GetImage()->GetWidth()) && (PxlReal.y >= 0) && (PxlReal.y < pDoc->GetImage()->GetHeight()))
				{
					imagePixel = pDoc->GetImage()->GetPixel(PxlReal.x,PxlReal.y);
					b = GetBValue(imagePixel);
					r = GetRValue(imagePixel);
					g = GetGValue(imagePixel);
					OnFloodFill(r,g,b,pDoc->GetImage(),pDoc->GetImageSelection()->GetImageMask(),PxlReal,pDoc->GetImageSelection()->GetCurrentMaskVector(pDoc->GetEditState()),true);
					pDoc->SetPenPoint(CPoint(-1,-1));
					m_showPen = true;
				}
				
			}
			break;
		case PIXEL_TOOL:
			if(pDoc->GetEditState() != EDIT_NONE)
			{
				pDoc->GetImageSelection()->ChangePoint(CPoint(PxlReal.x,PxlReal.y),pDoc->GetEditState(),true,GetPenSize());
				pDoc->SetPenPoint(CPoint(-1,-1));
				m_showPen = true;
			}
			break;
		case ZOOM_TOOL:

			if(m_zoom < 300)
			{
				pDoc->SetZoomPoint(PxlReal,point);
				m_zoom = m_zoom / 4 * 5;
				m_isZoomed = true;
				m_showPen = false;
				OnChangeSize(ID_ZOOM);
			}
			break;

		case INVERT_TOOL:
			
			penSize = GetPenSize();
			halfSize = penSize/2;
			penPoint = pDoc->GetPenPoint();

			penRect.left = penPoint.x - halfSize;
			penRect.right = penRect.left + penSize;
			penRect.top = penPoint.y - halfSize;
			penRect.bottom = penRect.top + penSize;
			
			if(penRect.left < 0)
				penRect.left = 0;
			if(penRect.top < 0)
				penRect.top = 0;
			pDoc->GetImageSelection()->InvertMask(penRect,pDoc->GetEditState());

			pDoc->GetImageSelection()->LoadMaskPoints(pDoc->GetEditState());
			break;
		default:
			if(m_ocrPoint != CPoint(-1,-1))
			{	
				CPoint	point1 = CPoint(int(double(m_ocrPoint.x)/m_zoom),int(double(m_ocrPoint.y)/m_zoom)),
						point2 = PxlReal,
						topLeft = CPoint(min(point1.x,point2.x),min(point1.y,point2.y)),
						bottomRight = CPoint(max(point1.x,point2.x),max(point1.y,point2.y));
				m_ocrRect.SetRect(topLeft,bottomRight);
				//m_cursorMap.AlphaBlend(GetDC()->GetSafeHdc(),m_ocrRect,CRect(0, 0, 1, 1), 127);
				if(!m_ocrRect.IsRectEmpty())
				{
					OnFileSaveImage(true,true,true);
				}
				m_checkOCRAreaTool = pDoc->IsOCRSelection();
				m_ocrRect.SetRectEmpty();
			}
			
			break;
	}
	m_ocrPoint = CPoint(-1,-1);
	SetPixelFast(&m_cursorMap,0,0,RGB(255,255,255));
	PrintPoint(PxlReal);
	Invalidate(false);
}

void  CGTView::SetShowPen()
{
	m_showPen = true;
}

COLORREF CGTView::GetPixelFast(CImage *pImage,int x, int y)
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

void CGTView::SetPixelFast(CImage *pImage,int x, int y, COLORREF color)
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

//Currently using the diference of speed in the color change as the stop decision

BOOL CGTView::OnExtendedFloodFill(BYTE targetRed,BYTE targetGreen,BYTE targetBlue,CImage *picture,CImage *mask,CPoint point,MaskVector *pointsVector,bool isAdded,bool isHidden)
{
	CGTDoc* pDoc = GetDocument();
	FloodFillPoints seedPoints;
	FloodFillPoints newSeedPoints;
	FloodFillPoints::iterator it,itb;
	CImageSelection *sel;
	CImage floodMask;
	EditEnum region = pDoc->GetEditState();
	COLORREF maskPixel, imagePixel,zeroPix,targetPixel,floodColor,editColor;
	BYTE r,g,b;
	int j = 0;
	BOOL morePix = TRUE;
	BOOL isSelected = FALSE;
	CPoint nextPoint;
	long double targetColor,floodDistance,pixelColor;
	CRect Clrect,imageRect;
	GetClientRect(&Clrect);
	Clrect.OffsetRect(GetScrollPosition());
	imageRect.left = int(Clrect.left / m_zoom);
	imageRect.top = int(Clrect.top / m_zoom);
	imageRect.bottom = int(Clrect.bottom / m_zoom)+1;
	imageRect.right = int(Clrect.right / m_zoom)+1;
		
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return FALSE;
	if(mask->IsNull())
		return FALSE;
	//if(m_isHold == false)
		CGTView::BeginWaitCursor();

	if(imageRect.left < 0 || m_isToAll)
		imageRect.left = 0;
	if(imageRect.right > picture->GetWidth() || m_isToAll)
		imageRect.right = picture->GetWidth();
	if(imageRect.top < 0 || m_isToAll)
		imageRect.top = 0;
	if(imageRect.bottom > picture->GetHeight() || m_isToAll)
		imageRect.bottom = picture->GetHeight();

	
	floodDistance =(long double(m_floodDistance));
	floodDistance = 3*(floodDistance*floodDistance);
	floodDistance = floodDistance*floodDistance;
	floodMask.Create(mask->GetWidth(),mask->GetHeight(),24);
	sel = pDoc->GetImageSelection();
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

		default:
			editColor = RGB(255,255,255);
			break;
	}
		
	floodColor = RGB(255,255,255);
	
	if(floodMask.GetBPP() != 24)
		return FALSE;

	SetPixelFast(&floodMask,point.x,point.y,floodColor);

	newSeedPoints.push_back(FloodFillPair(point,FloodFillPair3(RGB(targetRed,targetGreen,targetBlue),0)));
	while(!newSeedPoints.empty())
	{
		seedPoints.clear();
		seedPoints = newSeedPoints;
		newSeedPoints.clear();

		for(it=seedPoints.begin();it!=seedPoints.end();it++)
		{
			point = it->first;
			targetPixel = it->second.first;

			targetRed = GetRValue(targetPixel);
			targetGreen = GetGValue(targetPixel);
			targetBlue = GetBValue(targetPixel);
			imagePixel = GetPixelFast(picture,point.x,point.y);
			maskPixel = GetPixelFast(mask,point.x,point.y);
			

			r = GetRValue(imagePixel);
			g = GetGValue(imagePixel);
			b = GetBValue(imagePixel);
			zeroPix = RGB(0,0,0);
			targetColor = long double(targetRed)*long double(targetRed)+long double(targetBlue)*long double(targetBlue)+long double(targetGreen)*long double(targetGreen);
			pixelColor = long double(r)*long double(r)+long double(b)*long double(b)+long double(g)*long double(g);

			double dis = (pixelColor - targetColor)*(pixelColor - targetColor);
			
			if((dis <= it->second.second + floodDistance) &&
				(dis >= it->second.second - floodDistance))
				isSelected = TRUE;
			else
				isSelected = FALSE;

			if(!isSelected)
			{
				if(m_isOutline == 1)
				{
					if(((maskPixel & editColor) == zeroPix) == isAdded)
					{
						pointsVector->push_back(VectorRun(point,1));
						if(isAdded)
						{
							if(!isHidden)
								sel->UpdateEditBox(point);
							SetPixelFast(mask,point.x,point.y,(editColor | maskPixel));
						}
						else
							SetPixelFast(mask,point.x,point.y,((~editColor) & maskPixel));
					}
				}
			}

			if (isSelected || (m_isToAll))
			{
				if(isSelected && (((maskPixel & editColor) == zeroPix) == isAdded) && (m_isOutline != 1))
				{
					pointsVector->push_back(VectorRun(point,1));
					
					if(isAdded)
					{
						if(!isHidden)
							sel->UpdateEditBox(point);
						SetPixelFast(mask,point.x,point.y,(editColor | maskPixel));
					}
					else
						SetPixelFast(mask,point.x,point.y,(maskPixel & (~editColor)));
				}
				
				
				// 1 2 3
				// 4 x 5
				// 6 7 8
				
				for(int i = 0;i<8;i++)
				{
					j=i;
					switch(j)
					{
					case 1:
						//1
						nextPoint = point;
						nextPoint.x--;
						nextPoint.y--;
						break;
					case 2:
						//5
						nextPoint = point;
						nextPoint.x++;
						break;
					case 3:
						//6
						nextPoint = point;
						nextPoint.x--;
						nextPoint.y++;
						break;
					case 4:
						//2
						nextPoint = point;
						nextPoint.y--;
						break;
					case 5:
						//8
						nextPoint = point;
						nextPoint.x++;
						nextPoint.y++;
						break;
					case 6:
						//4
						nextPoint = point;
						nextPoint.x--;
						break;
					case 7:
						//3
						nextPoint = point;
						nextPoint.x++;
						nextPoint.y--;
						break;
					case 0:
						//7
						nextPoint = point;
						nextPoint.y++;
						break;
						
					}
					if(!((nextPoint.x >= imageRect.right) || (nextPoint.x < imageRect.left) || (nextPoint.y >= imageRect.bottom) || (nextPoint.y < imageRect.top)))
					{
						maskPixel = GetPixelFast(&floodMask,nextPoint.x,nextPoint.y);
						if(maskPixel != floodColor)
						{
							double dist = it->second.second-dis;
							if (dist<0)
								dist = -dist;
							if(!isSelected)
								dist = 0;

							if(m_isExt == 0)
							{
								if(dist == 0)
									newSeedPoints.push_back(FloodFillPair(nextPoint,FloodFillPair3(RGB(targetRed,targetGreen,targetBlue),dist)));
								else
									newSeedPoints.push_back(FloodFillPair(nextPoint,FloodFillPair3(RGB(targetRed,targetGreen,targetBlue), it->second.second)));
							}
							else
								newSeedPoints.push_back(FloodFillPair(nextPoint,FloodFillPair3(RGB(r,g,b),dist)));
							
							SetPixelFast(&floodMask,nextPoint.x,nextPoint.y,floodColor);

						}
					}							
				}
			}
		}
	}
	//if(m_isHold == false)
		CGTView::EndWaitCursor();
	return TRUE;
}


BOOL CGTView::OnFloodFill(BYTE targetRed,BYTE targetGreen,BYTE targetBlue,CImage *picture,CImage *mask,CPoint point,MaskVector *pointsVector,bool isAdded,bool isHidden)
{
	CGTDoc* pDoc = GetDocument();
	FloodFillPoints2 seedPoints;
	FloodFillPoints2 newSeedPoints;
	FloodFillPoints2::iterator it,itb;
	CImageSelection *sel;
	EditEnum region = pDoc->GetEditState();
	CImage floodMask;
	COLORREF maskPixel, imagePixel,zeroPix,targetPixel,floodColor,editColor;
	BYTE r,g,b;
	int j = 0;
	BOOL morePix = TRUE;
	BOOL isSelected = FALSE;
	CPoint nextPoint;
	long double targetColor,floodDistance,pixelColor;
	CRect Clrect,imageRect;
	GetClientRect(&Clrect);
	Clrect.OffsetRect(GetScrollPosition());
	imageRect.left = int(Clrect.left / m_zoom);
	imageRect.top = int(Clrect.top / m_zoom);
	imageRect.bottom = int(Clrect.bottom / m_zoom)+1;
	imageRect.right = int(Clrect.right / m_zoom)+1;
		
	if(m_isExt == 2 || (m_isExt == 0 && m_isToAll && m_isOutline))
	{
		return OnExtendedFloodFill(targetRed,targetGreen,targetBlue,picture,mask,point,pointsVector,isAdded,isHidden);
	}
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return FALSE;
	if(mask->IsNull())
		return FALSE;
	//if(m_isHold == false)
		CGTView::BeginWaitCursor();

	if(imageRect.left < 0 || m_isToAll)
		imageRect.left = 0;
	if(imageRect.right > picture->GetWidth() || m_isToAll)
		imageRect.right = picture->GetWidth();
	if(imageRect.bottom < 0 || m_isToAll)
		imageRect.top = 0;
	if(imageRect.bottom > picture->GetHeight() || m_isToAll)
		imageRect.bottom = picture->GetHeight();


	floodDistance =(long double(m_floodDistance));
	floodDistance = 3*(floodDistance*floodDistance);
	floodMask.Create(mask->GetWidth(),mask->GetHeight(),24);
	sel = pDoc->GetImageSelection();
	
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

		default:
			editColor = RGB(255,255,255);
			break;
	}

	floodColor = RGB(255,255,255);
	
	if(floodMask.GetBPP() != 24)
		return FALSE;

	SetPixelFast(&floodMask,point.x,point.y,floodColor);

	newSeedPoints.push_back(FloodFillPair2(point,RGB(targetRed,targetGreen,targetBlue)));
	while(!newSeedPoints.empty())
	{
		seedPoints.clear();
		seedPoints = newSeedPoints;
		newSeedPoints.clear();

		for(it=seedPoints.begin();it!=seedPoints.end();it++)
		{
			point = it->first;
			targetPixel = it->second;

			targetRed = GetRValue(targetPixel);
			targetGreen = GetGValue(targetPixel);
			targetBlue = GetBValue(targetPixel);
			imagePixel = GetPixelFast(picture,point.x,point.y);
			maskPixel = GetPixelFast(mask,point.x,point.y);
			
			r = GetRValue(imagePixel);
			g = GetGValue(imagePixel);
			b = GetBValue(imagePixel);
			zeroPix = RGB(0,0,0);
			targetColor = long double(targetRed)*long double(targetRed)+long double(targetBlue)*long double(targetBlue)+long double(targetGreen)*long double(targetGreen);
			pixelColor = long double(r)*long double(r)+long double(b)*long double(b)+long double(g)*long double(g);

			if((pixelColor <= targetColor + floodDistance) &&
				(pixelColor >= targetColor - floodDistance))
				isSelected = TRUE;
			else
				isSelected = FALSE;

			if(!isSelected)
			{
				if(m_isOutline == 1)
				{
					if(((maskPixel & editColor) == zeroPix) == isAdded)
					{
						pointsVector->push_back(VectorRun(point,1));
						if(isAdded)
						{
							if(!isHidden)
								sel->UpdateEditBox(point);
							SetPixelFast(mask,point.x,point.y,(editColor | maskPixel));
						}
						else
							SetPixelFast(mask,point.x,point.y,((~editColor) & maskPixel));
					}
				}
			}
			
			if(isSelected || (m_isToAll))
			{
				if(isSelected && ((((maskPixel & editColor) == zeroPix) == isAdded) && (m_isOutline != 1)))
				{
					pointsVector->push_back(VectorRun(point,1));	
					if(isAdded)
					{
						if(!isHidden)
							sel->UpdateEditBox(point);
						SetPixelFast(mask,point.x,point.y,(editColor | maskPixel));
					}
					else
						SetPixelFast(mask,point.x,point.y,(maskPixel & (~editColor)));
				}
				
				
				// 1 2 3
				// 4 x 5
				// 6 7 8
				
				for(int i = 0;i<8;i++)
				{
					j=i;
					switch(j)
					{
					case 1:
						//1
						nextPoint = point;
						nextPoint.x--;
						nextPoint.y--;
						break;
					case 2:
						//5
						nextPoint = point;
						nextPoint.x++;
						break;
					case 3:
						//6
						nextPoint = point;
						nextPoint.x--;
						nextPoint.y++;
						break;
					case 4:
						//2
						nextPoint = point;
						nextPoint.y--;
						break;
					case 5:
						//8
						nextPoint = point;
						nextPoint.x++;
						nextPoint.y++;
						break;
					case 6:
						//4
						nextPoint = point;
						nextPoint.x--;
						break;
					case 7:
						//3
						nextPoint = point;
						nextPoint.x++;
						nextPoint.y--;
						break;
					case 0:
						//7
						nextPoint = point;
						nextPoint.y++;
						break;
						
					}
					if(!((nextPoint.x >= imageRect.right) || (nextPoint.x < imageRect.left) || (nextPoint.y >= imageRect.bottom) || (nextPoint.y < imageRect.top)))
					{
						maskPixel = GetPixelFast(&floodMask,nextPoint.x,nextPoint.y);
						if(maskPixel != floodColor)
						{
							if(m_isExt == 1)
								newSeedPoints.push_back(FloodFillPair2(nextPoint,RGB(r,g,b)));
							else
								newSeedPoints.push_back(FloodFillPair2(nextPoint,RGB(targetRed,targetGreen,targetBlue)));
							SetPixelFast(&floodMask,nextPoint.x,nextPoint.y,floodColor);
						}
					}							
				}
			}
		}
	}
	//if(m_isHold == false)
		CGTView::EndWaitCursor();
	return TRUE;
}

double CGTView::GetZoom()
{
	return m_zoom;
}

void CGTView::OnRButtonUp(UINT nFlag,CPoint point)
{
	CGTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	BYTE r,g,b;
	COLORREF imagePixel;
	CPoint ul;
	CPoint PxlReal;
	MaskVector deletePoints;
	if(pDoc->GetImage()->IsNull())
		return;
	
	ul = this->GetScrollPosition();
	PxlReal = ul + point;
	PxlReal.x = int(double(PxlReal.x) / m_zoom);
	PxlReal.y = int(double(PxlReal.y) / m_zoom);
	ToolEnum state = pDoc->GetToolState();
	switch(state)
	{
		case PIXEL_TOOL:
			if(pDoc->GetEditState() != EDIT_NONE)
			{
				pDoc->GetImageSelection()->ChangePoint(CPoint(PxlReal.x,PxlReal.y),pDoc->GetEditState(),false,GetPenSize());
				BeginWaitCursor();
				pDoc->GetImageSelection()->LoadMaskPoints(pDoc->GetEditState());
				EndWaitCursor();
				pDoc->SetPenPoint(CPoint(-1,-1));
				m_showPen = true;
			}
			break;
		case REGION_TOOL:
			m_isHold = false;

			if((pDoc->GetEditState() != EDIT_NONE)&&(PxlReal.x >= 0) && (PxlReal.x < pDoc->GetImage()->GetWidth()) && (PxlReal.y >= 0) && (PxlReal.y < pDoc->GetImage()->GetHeight()))
			{
				imagePixel = GetPixelFast(pDoc->GetImage(),PxlReal.x,PxlReal.y);
				b = GetBValue(imagePixel);
				r = GetRValue(imagePixel);
				g = GetGValue(imagePixel);
				OnFloodFill(r,g,b,pDoc->GetImage(),pDoc->GetImageSelection()->GetImageMask(),PxlReal,&deletePoints,false);
				BeginWaitCursor();
				pDoc->GetImageSelection()->LoadMaskPoints(pDoc->GetEditState());
				EndWaitCursor();
				pDoc->SetPenPoint(CPoint(-1,-1));
				m_showPen = true;
			}
			break;
		case ZOOM_TOOL:
			if(m_zoom > 0.0033333)
			{
				pDoc->SetZoomPoint(PxlReal,point);
				m_zoom = m_zoom /5 * 4;
				m_isZoomed = true;
				m_showPen = false;
				OnChangeSize(ID_ZOOM);
			}
			
			break;
		default:break;
	}
	m_ocrPoint = CPoint(-1,-1);
	PrintPoint(PxlReal);
	Invalidate(false);
}

void CGTView::OnLPaint(UINT nFlag,CPoint PxlReal)
{
	CGTDoc* pDoc = GetDocument();
	BYTE r,g,b;
	COLORREF imagePixel;
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
		
	if(pDoc->GetEditState()!= EDIT_NONE)
	{
		ToolEnum toolState = pDoc->GetToolState();

		switch(toolState)
		{
		case PIXEL_TOOL:
			if(pDoc->GetImageSelection()->ChangeLine(pDoc->GetLastPenPoint(),PxlReal,pDoc->GetEditState(),true,GetPenSize()))
			{
				Invalidate(false);
			}
			break;
		case REGION_TOOL:
			if((PxlReal.x >= 0) && (PxlReal.x < pDoc->GetImage()->GetWidth()) && (PxlReal.y >= 0) && (PxlReal.y < pDoc->GetImage()->GetHeight()))
			{
				imagePixel = GetPixelFast(pDoc->GetImage(),PxlReal.x,PxlReal.y);
				b = GetBValue(imagePixel);
				r = GetRValue(imagePixel);
				g = GetGValue(imagePixel);
				if(((nFlag & MK_CONTROL) != 0) || m_isHold)
				{
					OnFloodFill(r,g,b,pDoc->GetImage(),pDoc->GetImageSelection()->GetImageMask(),PxlReal,pDoc->GetImageSelection()->GetCurrentMaskVector(pDoc->GetEditState()));
					Invalidate(false);
				}
				m_isHold = true;
			}		
			break;
		default:break;
		}
	}
}

int CGTView::GetPenSize()
{
	switch(m_nPenSize)
	{
	case PEN_1: return 1;
		break;
	case PEN_3: return 3;
		break;
	case PEN_5: return 5;
		break;
	case PEN_10: return 10;
		break;
	case PEN_20: return 20;
		break;
	case PEN_40: return 40;
		break;
	case PEN_80: return 80;
		break;
	default:break;
	}
	return 1;
}

void CGTView::OnRPaint(UINT nFlag,CPoint PxlReal)
{
	CGTDoc* pDoc = GetDocument();
	BYTE r,g,b;
	COLORREF imagePixel;
	CImageSelection *sel = pDoc->GetImageSelection();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	if(pDoc->GetEditState()!= EDIT_NONE)
	{
		ToolEnum toolState = pDoc->GetToolState();
	

		switch(toolState)
		{
		case PIXEL_TOOL:
			//if(pDoc->GetImageSelection()->ChangePoint(CPoint(PxlReal.x,PxlReal.y),pDoc->GetEditState(),false,GetPenSize()))
				if(pDoc->GetImageSelection()->ChangeLine(pDoc->GetLastPenPoint(),PxlReal,pDoc->GetEditState(),false,GetPenSize()))
				{
					Invalidate(false);
				}
				break;
		case REGION_TOOL:
			if((PxlReal.x >= 0) && (PxlReal.x < pDoc->GetImage()->GetWidth()) && (PxlReal.y >= 0) && (PxlReal.y < pDoc->GetImage()->GetHeight()))
			{
				imagePixel = GetPixelFast(pDoc->GetImage(),PxlReal.x,PxlReal.y);
				b = GetBValue(imagePixel);
				r = GetRValue(imagePixel);
				g = GetGValue(imagePixel);
				if(((nFlag & MK_CONTROL) != 0) || m_isHold)
				{
					OnFloodFill(r,g,b,pDoc->GetImage(),pDoc->GetImageSelection()->GetImageMask(),PxlReal,pDoc->GetImageSelection()->GetCurrentMaskVector(pDoc->GetEditState()),false);
					Invalidate(false);			
				}	
				m_isHold = true;
			}
			break;
		default:break;
		}
	}
}

BOOL CGTView::OnFileImageOpen()
{
	CImage cp_image;
	SetActiveWindow();
	CGTDoc* pDoc = GetDocument();
	CImage *m_imgOriginal;	
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return FALSE;
	CString file = pDoc->GetImagePath();
	CSize sizeTotal;
	CString strFilter;
	CSimpleArray<GUID> aguidFileTypes;
	HRESULT hResult;
	m_imgOriginal = pDoc->GetImage();

	

	hResult = m_imgOriginal->GetExporterFilterString(strFilter,aguidFileTypes);
	if (FAILED(hResult)) {
		CString fmt;
		fmt.Format(_T("GetExporterFilter failed:\n%x - %s"), hResult, _com_error(hResult).ErrorMessage());
		::AfxMessageBox(fmt);
		pDoc->SetLoad(false);
		return FALSE;
	}

	m_latestImport = theApp.GetImportType();

	if(theApp.GetImportType() == clipboard )
	{
		if ( OpenClipboard() )
		{
			HANDLE hClip = GetClipboardData(CF_BITMAP);
			HBITMAP hbClip = (HBITMAP) hClip;
			if(hbClip != NULL)
			{
				time_t rawtime;
				struct tm * timeinfo;
				wchar_t buffer [80];

				cp_image.Attach(hbClip);
				TCHAR lpszOldPath[MAX_PATH]; // MAX_PATH = 260,is defined in windef.h
				GetTempPath(MAX_PATH, lpszOldPath);
				file = CString(lpszOldPath);
				file.Append(CString('\\'));

				time ( &rawtime );
				timeinfo = localtime ( &rawtime );

				wcsftime (buffer,80,L"Clipboard%y%m%d%H%M%S.bmp",timeinfo);
				
				file.Append(buffer);
				cp_image.Save(file);
			}
			CloseClipboard();
		}
	}
	else if(theApp.GetImportType() == scanner )
	{
		theApp.SetImportType(imagefile);
		CImage* image = NULL;
		CMainWindow* scannerManager = theApp.GetScannerManager();
		if(scannerManager != NULL)
		{
			image = scannerManager->GetScannedImage();
		}
		
		if ( image != NULL )
		{
			time_t rawtime;
			struct tm * timeinfo;
			wchar_t buffer [80];

			cp_image =*image;
			TCHAR lpszOldPath[MAX_PATH]; // MAX_PATH = 260,is defined in windef.h
			GetTempPath(MAX_PATH, lpszOldPath);
			file = CString(lpszOldPath);
			file.Append(CString('\\'));
			time ( &rawtime );
			timeinfo = localtime ( &rawtime );

			wcsftime (buffer,80,L"Scanner%y%m%d%H%M%S.bmp",timeinfo);
				
			file.Append(buffer);

			cp_image.Save(file);
		}
	}
	else if(theApp.GetImportType() == snapshot )
	{
		theApp.SetImportType(imagefile);
		time_t rawtime;
		struct tm * timeinfo;
		wchar_t buffer [80];

		TCHAR lpszOldPath[MAX_PATH]; // MAX_PATH = 260,is defined in windef.h
		GetTempPath(MAX_PATH, lpszOldPath);
		file = CString(lpszOldPath);
		file.Append(CString('\\'));
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );

		wcsftime (buffer,80,L"Snapshot%y%m%d%H%M%S.bmp",timeinfo);
				
		file.Append(buffer);

		CaptureBMP(file);
		
	}
	else if(theApp.GetImportType() == noimport )
	{
		pDoc->SetLoad(false);
		return FALSE;
	}
		
	
	if (file.IsEmpty())
	{
		strFilter = L"IMAGE FILE |*.BMP;*.JPG;*.JPEG;*.JPE;*.JFIF;*.GIF;*.TIF;*.TIFF;*.PNG||";
		CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, strFilter);
		dlg.m_ofn.nFilterIndex = m_nFilterLoad;

		hResult = (int)dlg.DoModal();
		if(FAILED(hResult)||hResult == 2) {
			pDoc->SetLoad(false);
			m_latestImport = noimport;
				return FALSE;
		}

		m_nFilterLoad = dlg.m_ofn.nFilterIndex;
		//file = dlg.GetFolderPath();
		file = dlg.GetPathName();
		//file.Append(CString('\\'));
		//file.Append(dlg.GetFileName());
	}


	pDoc->LoadMask();
	
	MSXML2::IXMLDOMAttributePtr pNodeA;
	
	pDoc->UpdateSection(NULL,page);
	pNodeA = pDoc->GetNodeFromCurrSection(image_filename);
	if (pNodeA != NULL)
		pNodeA->put_text(BSTR(file.GetString()));
	

	
	m_imgOriginal->Destroy();
	
	if(pDoc->GetImagePath().IsEmpty())
		hResult = m_imgOriginal->Load(file);
	else 
		hResult = m_imgOriginal->Load((pDoc->GetLinkPath().Left(pDoc->GetLinkPath().ReverseFind(wchar_t('\\'))+ 1)) + file);
		
	if (FAILED(hResult)) {
		CString fmt;
		fmt = _T("Image \"")+file + _T("\" could not be opened in path \"") + (pDoc->GetLinkPath().Left(pDoc->GetLinkPath().ReverseFind(wchar_t('\\'))+ 1)+_T("\".")+_T("Try to change the image source in File->Preference") );
	
		::AfxMessageBox(fmt);
		pDoc->SetLoad(true);
		return FALSE;
	}
	
		

	pDoc->SetImagePath(file);
	
	sizeTotal.cx = m_imgOriginal->GetWidth();
	sizeTotal.cy = m_imgOriginal->GetHeight();
	SetScrollSizes(MM_TEXT, sizeTotal);

	m_nImageSize = SIZE_ORIGINAL;
	m_zoom = 1;
	Invalidate(false);
	UpdateWindow();
	pDoc->UpdateAllViews(this);
	return TRUE;
}
void CGTView::SetBright(int bright)
{
	if(bright<256 && bright>=0)
		m_bright = bright;
}

void CGTView::SetSesitivity(int sensitivity)
{
	if(sensitivity<256 && sensitivity>=0)
		m_floodDistance = sensitivity;
}

ImportSource CGTView::GetLatestImport()
{
	return m_latestImport;
}

void CGTView::SetLatestImport(ImportSource latest)
{
	m_latestImport = latest;
}

// Draw of CGTView

void CGTView::OnDraw(CDC* pDC)
{
	CGTDoc* pDoc = GetDocument();
	CImage *m_imgOriginal;
	CImageSelection* sel;
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_imgOriginal = pDoc->GetImage();
	CImage *mask;
	CRect Clrect;
	BYTE  grade = 0;
	int height,width;
	bool isOCRSelection = false;
	
	if (!m_imgOriginal->IsNull()) 
	{
		sel = pDoc->GetImageSelection();
		
		if(m_ocrPoint != CPoint(-1,-1))
		{
			if(m_checkOCRAreaTool && !sel->IsEmpty() && (sel->GetCore() || sel->GetOutline() || sel->GetShade()))
			{
				grade = 250;
				isOCRSelection = true;
			}
			else
				grade = 0;
		}
		else if(sel->GetCore() || sel->GetOutline() || sel->GetShade())
			grade = m_bright;

		sel->ResetMask();
		mask = sel->GetImageMask();
		if(mask->IsNull())
		{
			sel->MergePoints(m_imgOriginal);
		}
		
		m_Map = *mask;

		GetClientRect(&Clrect);

						
		if(!m_showPen)
		{
			switch (m_nImageSize)
			{
				case SIZE_HALF:
					m_zoom = double(0.5);
					break;
				case SIZE_ORIGINAL:
					m_zoom = 1;
					break;
				case SIZE_DOUBLE:
					m_zoom = 2;
					break;
				case SIZE_x4:
					m_zoom = 4;
					break;
				case SIZE_x8:
					m_zoom = 8;
					break;
				case SIZE_x16:
					m_zoom = 16;
					break;
				case SIZE_NONE:
				case SIZE_ZOOM:
					break;
				case SIZE_FILL:
					m_zoom = (double) Clrect.Width()/m_imgOriginal->GetWidth();
					if((m_imgOriginal->GetHeight()*m_zoom) > Clrect.Height())
						m_zoom = (double) Clrect.Height()/m_imgOriginal->GetHeight();
					break;
			};
			width = int(m_imgOriginal->GetWidth()*m_zoom);
			height = int(m_imgOriginal->GetHeight()*m_zoom);

			
			SetScrollSizes(MM_TEXT, CSize(width,height), CSize(Clrect.Width(), Clrect.Height()), CSize((int) m_zoom, (int) m_zoom));
		}
		else if(m_nImageSize == SIZE_FILL)
		{
			m_zoom = (double) Clrect.Width()/m_imgOriginal->GetWidth();
			if((m_imgOriginal->GetHeight()*m_zoom) > Clrect.Height())
				m_zoom = (double) Clrect.Height()/m_imgOriginal->GetHeight();

			width = int(m_imgOriginal->GetWidth()*m_zoom);
			height = int(m_imgOriginal->GetHeight()*m_zoom);

			SetScrollSizes(MM_TEXT, CSize(width,height), CSize(Clrect.Width(), Clrect.Height()), CSize((int) m_zoom, (int) m_zoom));
		}
		else
		{
			width = int(m_imgOriginal->GetWidth()*m_zoom);
			height = int(m_imgOriginal->GetHeight()*m_zoom);
		}

		if(!m_isZoomed)
		{
			if(height < Clrect.Height())
				m_isHighSrcllBar = false;
			else
				m_isHighSrcllBar = true;
		}


		
		if(!m_isZoomed && m_showPen)
		{
			int zoomWidth = int(Clrect.Width()/m_zoom)+2;
			int zoomHeight = int(Clrect.Height()/m_zoom)+2;
			int zoomX = int(GetScrollPos(SB_HORZ)/m_zoom);
			int zoomY = int(GetScrollPos(SB_VERT)/m_zoom);
			
			HDC memDC = CreateCompatibleDC(pDC->GetSafeHdc());
			HBITMAP hMemBmp;
			if( (zoomWidth+zoomX) > m_Map.GetWidth() || (zoomHeight+zoomY) > m_Map.GetHeight())
				hMemBmp = CreateCompatibleBitmap(pDC->GetSafeHdc(), width, height);
			else
				hMemBmp = CreateCompatibleBitmap(pDC->GetSafeHdc(), int(zoomWidth*m_zoom), int(zoomHeight*m_zoom));
			
			HBITMAP hOldBmp = (HBITMAP) SelectObject(memDC, hMemBmp);

			// Now all your drawing is done in memDC instead of in hDC

			
			
			if( (zoomWidth+zoomX) > m_Map.GetWidth() || (zoomHeight+zoomY) > m_Map.GetHeight())
			{
				if(!isOCRSelection)
					m_imgOriginal->StretchBlt(memDC,0,0,width,height,SRCCOPY);
				m_Map.AlphaBlend(memDC, 0, 0, width,height, 0, 0, m_imgOriginal->GetWidth(), m_imgOriginal->GetHeight(), grade);
			}
			else
			{
				if(!isOCRSelection)
					m_imgOriginal->StretchBlt(memDC,0,0,int(zoomWidth*m_zoom),int(zoomHeight*m_zoom),zoomX,zoomY,zoomWidth,zoomHeight,SRCCOPY);
				m_Map.AlphaBlend(memDC,0,0,int(zoomWidth*m_zoom),int(zoomHeight*m_zoom),zoomX,zoomY,zoomWidth,zoomHeight,grade);
			}

			CPoint penPoint = pDoc->GetPenPoint();

			if(penPoint.x > 0 && penPoint.y > 0 && m_showPen)
			{				
				int penSize = GetPenSize();
				int halfSize = penSize/2;
				 
				CRect penRect;

				penRect.left = penPoint.x - halfSize;
				penRect.right = penRect.left + penSize;
				penRect.top = penPoint.y - halfSize;
				penRect.bottom = penRect.top + penSize;
				
				if(penRect.left < 0)
					penRect.left = 0;
				if(penRect.top < 0)
					penRect.top = 0;

				if( (zoomWidth+zoomX) > m_Map.GetWidth() || (zoomHeight+zoomY) > m_Map.GetHeight())		
					m_cursorMap.AlphaBlend(memDC,int(penRect.left*m_zoom),int(penRect.top*m_zoom), int(penRect.Width()*m_zoom),int(penRect.Height()*m_zoom), 0, 0, 1, 1, grade);
				else
					m_cursorMap.AlphaBlend(memDC,int(penRect.left*m_zoom)-int(zoomX*m_zoom),int(penRect.top*m_zoom)-int(zoomY*m_zoom), int(penRect.Width()*m_zoom),int(penRect.Height()*m_zoom), 0, 0, 1, 1, grade);

			}
			else
				m_showPen = false;

		
			if( (zoomWidth+zoomX) > m_Map.GetWidth() || (zoomHeight+zoomY) > m_Map.GetHeight())			
				BitBlt(pDC->GetSafeHdc(), 0, 0, width, height, memDC, 0, 0, SRCCOPY);
			else
				BitBlt(pDC->GetSafeHdc(),int(zoomX*m_zoom), int(zoomY*m_zoom), int(zoomWidth*m_zoom),int(zoomHeight*m_zoom), memDC, 0, 0, SRCCOPY);
		
			// The old bitmap is selected back into the device context
			SelectObject(memDC, hOldBmp);
			DeleteObject(hMemBmp);
			DeleteDC(memDC);
			
	
		}
		else
		{
			if(!m_isZoomed)
			{
				if(!isOCRSelection)
					m_imgOriginal->StretchBlt(pDC->GetSafeHdc(),0,0,width,height,SRCCOPY);
				m_Map.AlphaBlend(pDC->GetSafeHdc(), 0, 0, width,height, 0, 0, m_imgOriginal->GetWidth(), m_imgOriginal->GetHeight(), grade);
		
			}
			else
			{

				SCROLLINFO scrollInfoH,scrollInfoW;

				GetScrollInfo(SB_HORZ,&scrollInfoW);
				GetScrollInfo(SB_VERT,&scrollInfoH);


				if(height < Clrect.Height())
				{
					int heightDiff = height*25/16+1 - height;
					m_cursorMap.StretchBlt(pDC->GetSafeHdc(),0,height,width*25/16+1,heightDiff,SRCCOPY);
				}

				if(width < Clrect.Width())
				{
					int witdthDiff = width*25/16+1 - width;
					m_cursorMap.StretchBlt(pDC->GetSafeHdc(),width,0,witdthDiff,height*25/16+1,SRCCOPY);
				}

				CPoint PxlReal,ul,point;
				pDoc->GetZoomPoint(PxlReal,point);
				PxlReal.x = int(double(PxlReal.x) * m_zoom);
				PxlReal.y = int(double(PxlReal.y) * m_zoom);
				ul = PxlReal - point;
			

				if(width>Clrect.Width())
				{
					if(ul.x<0)
						ul.x = 0;
					else if(ul.x > GetScrollLimit(SB_HORZ))
						ul.x = GetScrollLimit(SB_HORZ);
					scrollInfoW.nPos = ul.x;
					SetScrollInfo(SB_HORZ,&scrollInfoW);

				}
				
				if(height>Clrect.Height())
				{
					if(ul.y<0)
						ul.y = 0;
					else if(ul.y > GetScrollLimit(SB_VERT))
						ul.y = GetScrollLimit(SB_VERT);
					scrollInfoH.nPos = ul.y;
					SetScrollInfo(SB_VERT,&scrollInfoH);
				}
				
				m_isZoomed = false;
			
			}
		}
	}
	
	if(m_ocrPoint != CPoint(-1,-1))
	{
		//m_cursorMap.AlphaBlend(pDC->GetSafeHdc(),m_ocrRect,CRect(0, 0, 1, 1), 15);
		pDC->DrawFocusRect(&m_ocrRect);
		//pDC->DrawEdge(&m_ocrRect,EDGE_ETCHED,BF_FLAT);
	}
}
void CGTView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	sizeTotal.cx = 100;
	sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
	m_nImageSize = SIZE_ORIGINAL;
	OnChangeSize(ID_ZOOM);

}

void  CGTView::OnChangePen(PenEnum pen)
{
	m_nPenSize = pen;
}


// Print of CGTView

BOOL CGTView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CGTView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CGTView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

CGTDoc* CGTView::GetDocument() const
{
	return (CGTDoc*)m_pDocument;
}


// Debug of CGTView

#ifdef _DEBUG
void CGTView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CGTView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}


#endif //_DEBUG



//isOCR 1 isExport 1 for Area OCR
//isOCR 1 isExport 0 for all image OCR

//Performs OCR operations and its locally image related enhancements
void CGTView::OnFileSaveImage(bool isColor,bool isOCR, bool isExport) 
{

	CString strFilter;
	CSimpleArray<GUID> aguidFileTypes;
	CString strFileName,
			strScaleFileName;
	CString strExtension;
	HRESULT hResult;
	CGTDoc* pDoc = GetDocument();
	int height = 0,width = 0;
	CImage *m_imgOriginal,
		   *m_imgMask,
		   m_imgCopy,
		   m_imgMaskCopy;
	COLORREF clickColor,
			 cornerColor;
	bool secondRound = false, evenRound = false;
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	m_imgOriginal = pDoc->GetImage();
	m_imgMask = pDoc->GetImageSelection()->GetImageMask();
	height = m_imgOriginal->GetHeight();
	width = m_imgOriginal->GetWidth();
	DWORD nBufferLength = MAX_PATH;
	//wchar_t languageDirectory[MAX_PATH + 1];
	char languageDirectory[MAX_PATH + 1];
	
	if(!(pDoc->GetImageSelection()->GetImageMask()->IsNull()) &&
		pDoc->IsOCRSelection() &&
		isOCR &&
		m_checkOCRAreaTool &&
		!(pDoc->GetImageSelection()->IsEmpty()) &&
		((pDoc->GetImageSelection()->GetOutline())  || (pDoc->GetImageSelection()->GetShade())  || (pDoc->GetImageSelection()->GetCore())))
	{
		isColor = false;
	}
	if(isOCR)
	{
		
		GetCurrentDirectoryA(nBufferLength,languageDirectory);
		strcpy(languageDirectory,(CStringA(languageDirectory) + CStringA("\\")).GetString());
	}
	
	
	if(isExport && isOCR)
	{
		if(m_imgOriginal->GetBPP()<=8)
			m_imgCopy.Create(m_ocrRect.Width(),m_ocrRect.Height(),24);
		else
			m_imgCopy.Create(m_ocrRect.Width(),m_ocrRect.Height(),m_imgMask->GetBPP());

		for(int i = 0;i < width; i++)
			for(int j = 0;j < height ; j++)
				if(m_ocrRect.PtInRect(CPoint(i,j)))
				{
					if(isColor)
						SetPixelFast(&m_imgCopy,i-m_ocrRect.left,j-m_ocrRect.top,GetPixelFast(m_imgOriginal,i,j));
					else
						SetPixelFast(&m_imgCopy,i-m_ocrRect.left,j-m_ocrRect.top,(GetPixelFast(m_imgMask,i,j) != COLORREF(0))?RGB(0,0,0):RGB(255,255,255));
				}
	}
	else if(isExport || (!isColor))
	{
		if(m_imgOriginal->GetBPP()<=8)
			m_imgCopy.Create(width,height,24);
		else
			m_imgCopy.Create(width,height,m_imgMask->GetBPP());

		for(int i = 0;i < width; i++)
			for(int j = 0;j < height ; j++)
				if(GetPixelFast(m_imgMask,i,j) != COLORREF(0))
				{
					if(isColor)
						SetPixelFast(&m_imgCopy,i,j,GetPixelFast(m_imgOriginal,i,j));
					else
						SetPixelFast(&m_imgCopy,i,j,RGB(0,0,0));
				}
				else
					SetPixelFast(&m_imgCopy,i,j,RGB(255,255,255));
	}

	if(!isOCR)
	{
		if(!isExport)
			strFileName = pDoc->GetImagePath();
		else
		{
			strFilter = "Bitmap image|*.bmp|JPEG image|*.jpg|GIF image|*.gif|PNG image|*.png|TIFF image|*.tif||";

			CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
			dlg.m_ofn.nFilterIndex = m_nFilterLoad;
			dlg.m_ofn.nFileExtension = 0;

			hResult = (int)dlg.DoModal();
			if (FAILED(hResult) || HRESULT_CODE(hResult) == 2) {
				return;
			}

			// Add the appropriate extension if the user didn't type one

			strFileName = dlg.m_ofn.lpstrFile;

			// add the file extension if the user didn't supply one
			if (strFileName.ReverseFind(_T('.')) < strFileName.ReverseFind(_T('\\'))) 
			{
				switch (dlg.m_ofn.nFilterIndex)
				{
				case 1:
					strExtension = "bmp";
					break;
				case 2:
					strExtension = "jpg";
					break;
				case 3:
					strExtension = "gif";
					break;
				case 4:
					strExtension = "png";
					break;
				case 5:
					strExtension = "tif";
					break;

				default:
					break;
				}

				strFileName = strFileName + '.' + strExtension;

			}
		}

		// the extension on the file name will determine the file type that is saved
		if(isExport)
			hResult = m_imgCopy.Save(strFileName);
		else
			hResult = m_imgOriginal->Save(strFileName);
		if (FAILED(hResult)) {
			CString fmt;
			fmt.Format(_T("Save image failed:\n%x - %s"), hResult, _com_error(hResult).ErrorMessage());
			::AfxMessageBox(fmt);
			return;
		}
	}
	else
	{
		BeginWaitCursor();
			
		TCHAR strPath[ MAX_PATH ];

		// Get the special folder path.
		SHGetSpecialFolderPathW(
			0,       
			strPath, // String buffer.
			CSIDL_LOCAL_APPDATA, // CSLID of folder
			TRUE );

		char* text = "";
		
		CString translated_text = "";
		strFileName = CString(strPath) + CString(_T("\\BlackToText907.tif"));
		strScaleFileName = CString(strPath) + CString(_T("\\BlackToText908.tif"));
		std::string ss = std::string(CT2CA(strFileName)),
					sss = std::string(CT2CA(strScaleFileName));
		CStringA language = CStringA(pDoc->GetOCRLanguage());
		const char *file = ss.c_str(),
			*lang = language.GetString(),
			*scaleFile = sss.c_str();
		double signDev = 1;
		
		bool musttryagain = true;
		l_float32 scaleRatio = m_scaleRatio;
		l_float32 numPixels = (m_imgOriginal->GetHeight()*m_imgOriginal->GetWidth());
		if(isExport || (!isColor))
		{
			numPixels = (m_ocrRect.Height()*m_ocrRect.Width());
		}
		l_float32	maxRatio = (numPixels)<l_float32(250000)?l_float32(5):l_float32(3);
		if(maxRatio == 5)
		{
			if(numPixels<l_float32(24000))
			{
				maxRatio = 12;
				scaleRatio = 5;
			}
		}
		else if(maxRatio == 3)
		{
			if(numPixels>l_float32(5000000))
				maxRatio = 1.5;
			else if(numPixels>l_float32(1000000))
				maxRatio = 2;
		}

		if(!isColor || isExport)
		{
			m_imgCopy.Save(strFileName);
		}
		else
		{
			m_imgOriginal->Save(strFileName);
		}
	//	#ifndef XPCOMPATIBLE
		tesseract::PageSegMode pagesegmode = tesseract::PSM_AUTO;
		tesseract::TessBaseAPI  api;
		//api.Init(languageDirectory, lang,tesseract::OEM_TESSERACT_ONLY);
		api.Init(languageDirectory, lang);
		while(musttryagain)
		{
			/*if(evenRound) {
				api.End();
				api.SetPageSegMode(tesseract::PSM_AUTO_OSD);
				api.Init(languageDirectory, lang,tesseract::OEM_TESSERACT_CUBE_COMBINED);
			}*/
			
			PIX* pix = pixRead(file);
			//We copy to disk in case existed an error or we enhance the binary text selection (secondRound)
			if(pix == NULL || (m_imgOriginal->GetBPP()<=8 && !isExport) || secondRound)
			{
				m_imgCopy.Destroy();

				if(secondRound && pix != NULL)
				{
					int isExt = m_isExt,
						isOutline = m_isOutline,
						floodDistance = m_floodDistance,
						roundsCounter = 0;
					bool	isToAll = m_isToAll;
					MaskVector maskVect;
					double meanDistance = 0,deviation  = 0, vect_size;

										
					//PIX* doublepix = pixScaleAreaMap(pix,scaleRatio,scaleRatio);
					//PIX *doublepix = pixScale(pix,scaleRatio,scaleRatio);
					//pixDestroy(&pix);
					//pix = doublepix;
					////pixWrite("C:\\Users\\David\\AppData\\Local\\BlackToText908.tif",pix,IFF_TIFF);
					//pixWrite(scaleFile,pix,IFF_TIFF);
					m_imgCopy.Destroy();
					m_imgCopy.Load(CString(strScaleFileName).GetString());
					
					
					m_isExt = 0;
					m_isToAll = true;
					m_isOutline = 1;
					//m_floodDistance = 182;
					EditEnum region = pDoc->GetEditState();
					pDoc->SetEditState(EDIT_NONE);
					cornerColor = 1;
					m_floodDistance = 253;
					
					CPoint point;
					
					point.x = (rand()%10 > 4)?m_imgCopy.GetWidth()-1:0;
					point.y = (rand()%10 > 4)?m_imgCopy.GetHeight()-1:0;
					
					clickColor = GetPixelFast(&m_imgCopy,point.x,point.y);
						
					m_isExt = 1;
					m_isToAll = false;
					m_isOutline = 1;
					while(roundsCounter<10)
					{
						maskVect.clear();
						m_imgMaskCopy.Destroy();
						m_imgMaskCopy.Create(m_imgCopy.GetWidth(),m_imgCopy.GetHeight(),m_imgCopy.GetBPP());
						OnFloodFill(GetRValue(clickColor),GetGValue(clickColor),GetBValue(clickColor),&m_imgCopy,&m_imgMaskCopy,point,&maskVect);
						if(!maskVect.empty())
						{
							VectorRun pointRun;
							COLORREF colorText;
							MaskVector deviationVect = maskVect; 
							meanDistance = 0;
							deviation  = 0;
							vect_size = double(maskVect.size());

							while(!maskVect.empty())
							{
								pointRun = maskVect.back();
								maskVect.pop_back();
								colorText = GetPixelFast(&m_imgCopy,pointRun.first.x,pointRun.first.y);
								m_floodDistance = sqrt(pow(double(GetRValue(colorText))-double(GetRValue(clickColor)),2)+pow(double(GetGValue(colorText))-double(GetGValue(clickColor)),2)+pow(double(GetBValue(colorText))-double(GetBValue(clickColor)),2));
								meanDistance += m_floodDistance;
							}
							meanDistance = meanDistance / vect_size;
							maskVect = deviationVect;
							while(!maskVect.empty())
							{
								pointRun = maskVect.back();
								maskVect.pop_back();
								colorText = GetPixelFast(&m_imgCopy,pointRun.first.x,pointRun.first.y);
								m_floodDistance = sqrt(pow(double(GetRValue(colorText))-double(GetRValue(clickColor)),2)+pow(double(GetGValue(colorText))-double(GetGValue(clickColor)),2)+pow(double(GetBValue(colorText))-double(GetBValue(clickColor)),2));
								deviation += pow(meanDistance - m_floodDistance,2);
							}
							
							
							m_floodDistance = (meanDistance/vect_size) + 3*(sqrt(deviation / vect_size));
							roundsCounter = 10;			
						}
						else
						{
							m_floodDistance /= 2;
							if(m_floodDistance <= 1)
							{
								m_floodDistance = 255;
								point.x = ((rand()%10) > 4)?m_imgCopy.GetWidth()-1:0;
								point.y = ((rand()%10) > 4)?m_imgCopy.GetHeight()-1:0;
								clickColor = GetPixelFast(&m_imgCopy,point.x,point.y);
					
								roundsCounter++;
								continue;

							}
							
						}
						
						
						m_isExt = 0;
						m_isToAll = true;
						m_isOutline = 1;
						m_imgMaskCopy.Destroy();
						m_imgMaskCopy.Create(m_imgCopy.GetWidth(),m_imgCopy.GetHeight(),m_imgCopy.GetBPP());
						roundsCounter++;
						int totalPixels = m_imgCopy.GetWidth()*m_imgCopy.GetHeight();
						int i=0;
						do
						{
							maskVect.clear();
							//2+double((rand()%100)/100))*
							m_floodDistance = (meanDistance/vect_size) + (4+double((rand()%200)/100))*sqrt(deviation / vect_size);
							OnFloodFill(GetRValue(clickColor),GetGValue(clickColor),GetBValue(clickColor),&m_imgCopy,&m_imgMaskCopy,point,&maskVect);
							if(maskVect.size()>(totalPixels-(totalPixels/2)))
								signDev = signDev*1.2;
							if(maskVect.size()<(totalPixels/50))
								signDev = -signDev;
						}
						while((i++<10) && ((maskVect.size()==(totalPixels-(totalPixels))) || (maskVect.size()==(totalPixels))));
						
						if(i>=10)
							signDev = 1;
						
						if(!maskVect.empty())
							break;
					}	

					m_isExt = isExt;
					m_isToAll = isToAll,
					m_isOutline = isOutline;
					m_floodDistance = floodDistance;
					pDoc->SetEditState(region);
				}
				else if(m_imgOriginal->GetBPP()<=8)
				{
					m_imgCopy.Create(width,height,24);
				}
				else
				{
					m_imgCopy.Create(width,height,m_imgMask->GetBPP());
				}
				
				
				
				if(secondRound)
				{
					int areaWidth = m_imgMaskCopy.GetWidth(),
						areaHeight = m_imgMaskCopy.GetHeight();

					for(int i = 0;i < areaWidth; i++)
						for(int j = 0;j < areaHeight ; j++)
							if(GetPixelFast(&m_imgMaskCopy,i,j) != COLORREF(0))
							{
								SetPixelFast(&m_imgCopy,i,j,RGB(0,0,0));
							}
							else
								SetPixelFast(&m_imgCopy,i,j,RGB(255,255,255));
				}
				else 
				{
					for(int i = 0;i < width; i++)
						for(int j = 0;j < height ; j++)
							if(isColor || GetPixelFast(m_imgMask,i,j) != COLORREF(0) || (isExport && isOCR))
							{
								if(isColor)
									SetPixelFast(&m_imgCopy,i,j,GetPixelFast(m_imgOriginal,i,j));
								else
									SetPixelFast(&m_imgCopy,i,j,RGB(0,0,0));
							}
							else
								SetPixelFast(&m_imgCopy,i,j,RGB(255,255,255));
				}
				
				if(secondRound)
				{
					//Pix* openpix,*openPax;
					hResult = m_imgCopy.Save(strScaleFileName);
					//pix = pixRead("C:\\Users\\David\\AppData\\Local\\BlackToText908.tif");
					pix = pixRead(scaleFile);

					//if(scaleRatio > 2.0)
						//pixOpen(pix,pix,selCreate(3,3,"cell"));
					//else if((scaleRatio > 4.0))
					//pix =	pixOpen(pix,pix,selCreate(int(scaleRatio),int(scaleRatio),"cell"));
					//openpix = pixConvert1To32(openpix,/*pixOpen(NULL,*/pixConvertTo1(openpix,200)/*,selCreate(int(0),int(0),"cell"))*/,255,0);
					//pix = pixOpenGray(pix,5,5);
#ifdef _DEBUG
					pixWrite("C:\\Users\\David\\AppData\\Local\\BlackToText909.tif",pix,IFF_TIFF);
#endif
				}
				else
				{
					hResult = m_imgCopy.Save(strFileName);
					pix = pixRead(file);
				}
			}
				
			if(pix != NULL)
			{
				if(scaleRatio != 1 && !secondRound)
				{
					PIX* doublepix = pixScale(pix,scaleRatio,scaleRatio);
					pixDestroy(&pix);
					pix = doublepix;
				}
										
				api.SetImage(pix);
				int bytes_per_line =(((pix->w)*(pix->d)+7)/8);
				
				text = api.TesseractRect((const unsigned char *)pix->data,pix->d/8,bytes_per_line,0,0,pix->w,pix->h);
				EndWaitCursor();
				pixDestroy(&pix);

				
			}

		 
			int msgboxID;
			int startLine = 0,
				lineCount = 0;

			CStringW originalText = CStringW(UTF8toUTF16(CStringA(text))),
					printString;
			
			while((startLine >= 0) && (lineCount < NUM_LINES_OCR_SHOW))
			{
				startLine = originalText.Find(_T("\n"),startLine+1);
				lineCount++;
			}
		
			if(lineCount >= NUM_LINES_OCR_SHOW)
				printString = originalText.Left(startLine) + _T("...");
			else
				printString = originalText;
		/*	HWND hEdit = CreateWindowEx(WS_EX_CLIENTEDGE,
		L"RichEdit",
		printString,
		WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_AUTOVSCROLL|ES_AUTOHSCROLL,
		100,
		100,
		200,
		100,
		this->GetSafeHwnd(),
		NULL,
		GetModuleHandle(NULL),
		NULL);
			SendMessage(hEdit,WM_SETTEXT,NULL,(LPARAM)"Insert text here...");*/

			if(m_nopopup)
				msgboxID = IDCONTINUE;
			else if(CStringW(text).IsEmpty())
				msgboxID = MessageBox(_T("..."),_T("Copy text"),MB_ICONASTERISK | MB_CANCELTRYCONTINUE | MB_DEFBUTTON3);
			else if(!isColor)
				msgboxID = MessageBox(printString,_T("Copy text from Selection"),MB_ICONASTERISK | MB_CANCELTRYCONTINUE | MB_DEFBUTTON3);
			else if(isExport)
				msgboxID = MessageBox(printString,_T("Copy text from selected area"),MB_ICONASTERISK | MB_CANCELTRYCONTINUE | MB_DEFBUTTON3);
			else
				msgboxID = MessageBox(printString,_T("Copy text from full color Image"),MB_ICONASTERISK | MB_CANCELTRYCONTINUE | MB_DEFBUTTON3);

			switch (msgboxID)
			{
				case IDCANCEL:
					musttryagain = false;
					break;
				case IDTRYAGAIN:
					// Resize image and try again
					if(scaleRatio < maxRatio)
					{
						scaleRatio += 2;
					}
					else
					{
						scaleRatio = 1;
					//	api.End();
						//api.SetPageSegMode(tesseract::PSM_AUTO_OSD);
					//	api.Init(languageDirectory, lang,tesseract::OEM_TESSERACT_CUBE_COMBINED);
						//scaleRatio = 0.5;
						//secondRound = secondRound?false:true;
						scaleRatio = evenRound?1:2;				
						evenRound = evenRound?false:true;
					}
					BeginWaitCursor();	
					musttryagain = true;
					break;
				case IDCONTINUE:
					// Copy to Clipboard
					if (!OpenClipboard())
					{
						return;
					}

#ifdef _UNICODE
					EmptyClipboard();
					HGLOBAL hglbCopy;
					wchar_t *wcBuffer;
					wcBuffer = 0;
					hglbCopy = GlobalAlloc( GMEM_DDESHARE,	( originalText.GetLength() + 1 ) * sizeof(wchar_t) ); 
	
					wcBuffer = ( wchar_t* )GlobalLock( hglbCopy );
					wcscpy_s( wcBuffer,( originalText.GetLength() + 1 ), originalText );
	
					GlobalUnlock( hglbCopy );
					::SetClipboardData( CF_UNICODETEXT, hglbCopy );
					CloseClipboard();
#else
        
					//clear clipboard
					EmptyClipboard();
 
					HGLOBAL clipbuffer;
					char * buffer;
   
					//alloc enough mem for the string;
					//must be GMEM_DDESHARE to work with the clipboard
					clipbuffer = GlobalAlloc(GMEM_DDESHARE, strlen(text)+1);
					buffer = (char*)GlobalLock(clipbuffer);
					strcpy_s(buffer, strlen(text)+1,text);
					GlobalUnlock(clipbuffer);
   
					//fill the clipboard with data
					//CF_TEXT indicates that the buffer is a text-string
					::SetClipboardData(CF_TEXT,clipbuffer);
					//close clipboard as we don't need it anymore
					CloseClipboard();
					
#endif
					musttryagain = false;
					break;
				default:
					musttryagain = false;
					break;
			}
			m_scaleRatio = scaleRatio;
		}
		api.End();
		//#endif
	}
}

void CGTView::OnToolsMakeBW(void)
{

	CWaitCursor wait;
	CGTDoc* pDoc = GetDocument();
	CImage *m_imgOriginal;	
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	m_imgOriginal = pDoc->GetImage();

	if (!m_imgOriginal->IsIndexed()) {

		// the image does not use an indexed palette, so we will change each pixel to B&W (slow)
		COLORREF pixel;
		int maxY = m_imgOriginal->GetHeight(), maxX = m_imgOriginal->GetWidth();
	/*	byte r,g,b,avg;
		for (int x=0; x<maxX; x++) {
			for (int y=0; y<maxY; y++) {
				pixel = m_imgOriginal->GetPixel(x,y);
				r = GetRValue(pixel);
				g = GetGValue(pixel);
				b = GetBValue(pixel);
				avg = ((r + g + b)/3);
				m_imgOriginal->SetPixelRGB(x,y,avg,avg,avg);
			}
		}*/
		//New method using RGB 3D Space distance
		double r,g,b,avg;
		for (int x=0; x<maxX; x++) {
			for (int y=0; y<maxY; y++) {
				pixel = GetPixelFast(m_imgOriginal,x,y);
				r = double(GetRValue(pixel));
				g = double(GetGValue(pixel));
				b = double(GetBValue(pixel));
				avg = (sqrt(double(((r*r + g*g + b*b)/3))));
				SetPixelFast(m_imgOriginal,x,y,RGB(int(avg),int(avg),int(avg)));
			}
		}

	} 
	else
	{

		// the image uses an indexed palette, so we will just change the palette table entries to
		// their B&W equivalents 
		int MaxColors = m_imgOriginal->GetMaxColorTableEntries();
		RGBQUAD* ColorTable;
		ColorTable = new RGBQUAD[MaxColors];

		m_imgOriginal->GetColorTable(0,MaxColors,ColorTable);
		for (int i=0; i<MaxColors; i++)
		{
			int avg = (ColorTable[i].rgbBlue + ColorTable[i].rgbGreen + ColorTable[i].rgbRed)/3;
			ColorTable[i].rgbBlue = (BYTE)avg;
			ColorTable[i].rgbGreen = (BYTE)avg;
			ColorTable[i].rgbRed = (BYTE)avg;
		}
		m_imgOriginal->SetColorTable(0,MaxColors,ColorTable);
	
		delete[] ColorTable;
	}

	Invalidate(false);
	UpdateWindow();
}



void CGTView::OnChangeSize(UINT nID)
{
	CGTDoc* pDoc = GetDocument();
	CImage *m_imgOriginal;
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_imgOriginal = pDoc->GetImage();
	CRect Clrect;
	
	GetClientRect(&Clrect);

	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	m_nImageSize = (SizesEnum)(nID - ID_SIZE_BASE);
	pMainWnd->OnCombozoom(nID);
	if (m_imgOriginal->IsNull())
		return;

	Invalidate(false);

	UpdateWindow();
}

void CGTView::OnUpdateSizeHalf(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck((UINT)(m_nImageSize == SIZE_HALF));
}

void CGTView::OnUpdateSizeOriginal(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck((UINT)(m_nImageSize == SIZE_ORIGINAL));
}

void CGTView::OnUpdateSizeDouble(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck((UINT)(m_nImageSize == SIZE_DOUBLE));
}

void CGTView::OnUpdateSizeX4(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck((UINT)(m_nImageSize == SIZE_x4));
}

void CGTView::OnUpdateSizeX8(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck((UINT)(m_nImageSize == SIZE_x8));
}

void CGTView::OnUpdateSizeX16(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck((UINT)(m_nImageSize == SIZE_x16));
}

void CGTView::OnUpdateSizeFill(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck((UINT)(m_nImageSize == SIZE_FILL));
}

void  CGTView::PrintPoint(CPoint PxlReal)
{
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	CString x,y;
	CGTDoc* pDoc = GetDocument();
	if(!pDoc->IsPointBar())
		return;
	x="";
	x = wchar_t(PxlReal.x % 10 + 48) + x;
	PxlReal.x /= 10;
	
	while(PxlReal.x != 0)
	{
		x = wchar_t(PxlReal.x % 10 + 48) + x;
		PxlReal.x /= 10;

	}

	y="";
	y = wchar_t(PxlReal.y % 10 + 48) + y;
	PxlReal.y /= 10;
	
	while(PxlReal.y != 0)
	{
		y = wchar_t(PxlReal.y % 10 + 48) + y;
		PxlReal.y /= 10;

	}

	pMainWnd->SetMessageText(LPCTSTR( CString(CString(_T("(")) + x  + CString(_T(",")) + y + CString(_T(")")))));

}

void CGTView::OnMouseMove(UINT nFlags, CPoint point)
{
	CGTDoc* pDoc = GetDocument();
	ToolEnum state;

	if(pDoc->GetImage()->IsNull())
		return;
	if(m_ocrPoint != CPoint(-1,-1))
	{
		CPoint	point1 = m_ocrPoint,
				point2 = this->GetScrollPosition()+point, //Point in screen
				topLeft = CPoint(min(point1.x,point2.x),min(point1.y,point2.y)),
				bottomRight = CPoint(max(point1.x,point2.x),max(point1.y,point2.y));
		
		bottomRight.x = min(bottomRight.x,int(double(pDoc->GetImage()->GetWidth())*m_zoom));
		bottomRight.y = min(bottomRight.y,int(double(pDoc->GetImage()->GetHeight())*m_zoom));
		m_ocrRect.SetRect(topLeft,bottomRight);
			
		Invalidate(false);
		return;
	}

	if(((nFlags & MK_LBUTTON) != 0) || ((nFlags & MK_RBUTTON) != 0) || ((nFlags & MK_CONTROL) != 0) /* || (pDoc->GetToolState() == INVERT_TOOL)*/)
	{
		
		CPoint ul;
		CPoint PxlReal;
		ul = this->GetScrollPosition();
		
		PxlReal = ul + point;
		PxlReal.x = int(double(PxlReal.x) / m_zoom);
		PxlReal.y = int(double(PxlReal.y) / m_zoom);
		state = pDoc->GetToolState();
		point = PxlReal;

		if((state == PIXEL_TOOL) || (state == REGION_TOOL)/* || (pDoc->GetToolState() == INVERT_TOOL)*/)
		{
			if((PxlReal.x < (pDoc->GetImage()->GetWidth())) && (PxlReal.y < (pDoc->GetImage()->GetHeight())))
			{
				if(state == REGION_TOOL)
				{
					pDoc->SetPenPoint(CPoint(-1,-1));
				}
				else
					pDoc->SetPenPoint(PxlReal);
				m_showPen =  true;
				if((nFlags & MK_CONTROL) != 0)
				{
					Invalidate(false);
					m_showPen =  true;
				}	
				
			}
			else if(m_showPen ==  true)
			{
				pDoc->SetPenPoint(CPoint(-1,-1));
				m_showPen = true;
				Invalidate(false);
			}
		}
		

		PrintPoint(PxlReal);
	}
	else if(m_showPen ==  true)
	{
		pDoc->SetPenPoint(CPoint(-1,-1));
		m_showPen = true;
		Invalidate(false);
	}

	if( (nFlags & MK_LBUTTON) != 0)
	{
		OnLPaint(nFlags,point);
	}
	else if((nFlags & MK_RBUTTON) != 0)
	{
		OnRPaint(nFlags,point);
	}
}

void CGTView::SetBorder()
{
	m_isOutline++;
	m_isOutline = m_isOutline % 2;
}

void CGTView::SetExt()
{
	m_isExt++;
	m_isExt = m_isExt % 3;
}

void CGTView::SetToAll()
{
	m_isToAll = m_isToAll? false:true;
}

void CGTView::SetNoOCRPopUp()
{
	m_nopopup = m_nopopup? false:true;
}

bool CGTView::GetNoOCRPopUp()
{
	return m_nopopup;
}

CStringW CGTView::UTF8toUTF16(const CStringA& utf8)
{
   CStringW utf16;
   int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
   if (len>1)
   { 
      wchar_t *ptr = utf16.GetBuffer(len-1);
      if (ptr) MultiByteToWideChar(CP_UTF8, 0, utf8, -1, ptr, len);
      utf16.ReleaseBuffer();
   }
   return utf16;
}

void CGTView::OnToolsAreatextcopier(bool value)
{
	m_checkOCRAreaTool = value;
}

int CGTView::CaptureBMP(LPCTSTR szFile)
{
    // Source[1]
    HDC hdcScr, hdcMem;
    HBITMAP hbmScr;
    BITMAP bmp;
    int iXRes, iYRes;

    // Create a normal DC and a memory DC for the entire screen. The
    // normal DC provides a "snapshot" of the screen contents. The
    // memory DC keeps a copy of this "snapshot" in the associated
    // bitmap.
    hdcScr = CreateDC(L"DISPLAY", NULL, NULL, NULL);
    hdcMem = CreateCompatibleDC(hdcScr);

    iXRes = GetDeviceCaps(hdcScr, HORZRES);
    iYRes = GetDeviceCaps(hdcScr, VERTRES);

    // Create a compatible bitmap for hdcScreen.
    hbmScr = CreateCompatibleBitmap(hdcScr, iXRes, iYRes);
    if (hbmScr == 0) return 0;

    // Select the bitmaps into the compatible DC.
    if (!SelectObject(hdcMem, hbmScr)) return 0;
	
	//Hide the program 333ms to capture the screen
	CWnd* pWnd = AfxGetMainWnd();
    pWnd->ShowWindow(SW_HIDE);
    ::Sleep(333);

    // Copy color data for the entire display into a
    // bitmap that is selected into a compatible DC.
    if (!StretchBlt(hdcMem,
        0, 0, iXRes, iYRes,
        hdcScr,
        0, 0, iXRes, iYRes,
        SRCCOPY | CAPTUREBLT))

        return 0;
	
	//Shows the screen again
	pWnd->ShowWindow(SW_SHOW);
    
    // Source[2]
    PBITMAPINFO pbmi;
    WORD cClrBits;

    // Retrieve the bitmap's color format, width, and height.
    if (!GetObject(hbmScr, sizeof(BITMAP), (LPSTR) &bmp)) return 0;

    // Convert the color format to a count of bits.
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
    if (cClrBits == 1)
        cClrBits = 1;
    else if (cClrBits <= 4)
        cClrBits = 4;
    else if (cClrBits <= 8)
        cClrBits = 8;
    else if (cClrBits <= 16)
        cClrBits = 16;
    else if (cClrBits <= 24)
        cClrBits = 24;
    else cClrBits = 32;

    // Allocate memory for the BITMAPINFO structure. (This structure
    // contains a BITMAPINFOHEADER structure and an array of RGBQUAD
    // data structures.)
    if (cClrBits != 24)
        pbmi = (PBITMAPINFO) LocalAlloc(LPTR,
                sizeof(BITMAPINFOHEADER) +
                sizeof(RGBQUAD) * (1 << cClrBits));

    // There is no RGBQUAD array for the 24-bit-per-pixel format.
    else
        pbmi = (PBITMAPINFO) LocalAlloc(LPTR,
                sizeof(BITMAPINFOHEADER));

    // Initialize the fields in the BITMAPINFO structure.
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biWidth = bmp.bmWidth;
    pbmi->bmiHeader.biHeight = bmp.bmHeight;
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
    if (cClrBits < 24)
        pbmi->bmiHeader.biClrUsed = (1 << cClrBits);

    // If the bitmap is not compressed, set the BI_RGB flag.
    pbmi->bmiHeader.biCompression = BI_RGB;

    // Compute the number of bytes in the array of color
    // indices and store the result in biSizeImage.
    pbmi->bmiHeader.biSizeImage = (pbmi->bmiHeader.biWidth + 7) / 8
                                    * pbmi->bmiHeader.biHeight * cClrBits;

    // Set biClrImportant to 0, indicating that all of the
    // device colors are important.
    pbmi->bmiHeader.biClrImportant = 0;

    HANDLE hf;                  // file handle
    BITMAPFILEHEADER hdr;       // bitmap file-header
    PBITMAPINFOHEADER pbih;     // bitmap info-header
    LPBYTE lpBits;              // memory pointer
    DWORD dwTotal;              // total count of bytes
    DWORD cb;                   // incremental count of bytes
    BYTE *hp;                   // byte pointer
    DWORD dwTmp;

    pbih = (PBITMAPINFOHEADER) pbmi;
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

    if (!lpBits) return 0;

    // Retrieve the color table (RGBQUAD array) and the bits
    // (array of palette indices) from the DIB.
    if (!GetDIBits(hdcMem, hbmScr, 0, (WORD) pbih->biHeight, lpBits, pbmi, DIB_RGB_COLORS)) return 0;

    // Create the .BMP file.
    hf = CreateFile(szFile,
                    GENERIC_READ | GENERIC_WRITE,
                    (DWORD) 0,
                    NULL,
                    CREATE_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL,
                    (HANDLE) NULL);
    if (hf == INVALID_HANDLE_VALUE) return 0;

    hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"

    // Compute the size of the entire file.
    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) +
                 pbih->biSize + pbih->biClrUsed *
                 sizeof(RGBQUAD) + pbih->biSizeImage);
    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;

    // Compute the offset to the array of color indices.
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +
                    pbih->biSize + pbih->biClrUsed *
                    sizeof (RGBQUAD);

    // Copy the BITMAPFILEHEADER into the .BMP file.
    if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), (LPDWORD) &dwTmp, NULL)) return 0;

    // Copy the BITMAPINFOHEADER and RGBQUAD array into the file.
    if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER)
                + pbih->biClrUsed * sizeof (RGBQUAD),
                (LPDWORD) &dwTmp, NULL))
        return 0;

    // Copy the array of color indices into the .BMP file.
    dwTotal = cb = pbih->biSizeImage;
    hp = lpBits;
    if (!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp, NULL)) return 0;

    // Close the .BMP file.
    if (!CloseHandle(hf)) return 0;

    // Free memory.
    GlobalFree((HGLOBAL)lpBits);
	::DeleteDC(hdcScr);
    ::DeleteDC(hdcMem);

    return 1;
} 