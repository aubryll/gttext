// GTView.h: interfaz de la clase CGTView

#pragma once
#include <afxstr.h>
#include <atlimage.h>
#include <comdef.h>
#include "GTDoc.h"




enum SizesEnum
{
	SIZE_HALF,
	SIZE_ORIGINAL,
	SIZE_DOUBLE,
	SIZE_x4,
	SIZE_x8,
	SIZE_x16,
	SIZE_FILL,
	SIZE_ZOOM,	
	SIZE_NONE
};

enum PenEnum
{
	PEN_1=0,
	PEN_3=1,
	PEN_5=2,
	PEN_10=3,
	PEN_20=4,
	PEN_40=5,
	PEN_80=6,
};





class CGTView : public CScrollView
{
protected: 
	CGTView();
	DECLARE_DYNCREATE(CGTView)


public:
	CGTDoc* GetDocument() const;


public:

public:
	virtual void OnDraw(CDC* pDC);  
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); 
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);


public:
	virtual ~CGTView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	
	int	m_nFilterLoad;
	SizesEnum m_nImageSize;
	PenEnum m_nPenSize;
	double m_zoom;
	bool m_isZoomed;
	int m_floodDistance;
	int m_bright;
	int m_isOutline;
	int m_isExt;
	bool m_isToAll;
	bool m_showPen;
	CImage m_Map;
	CImage m_cursorMap;
	bool  m_isHold;
	bool m_isHighSrcllBar;
	HDC m_memDC;
	HBITMAP m_hMemBmp;
	HBITMAP m_hOldBmp;
	bool m_loadingFromClipboard;
	

protected:
	

protected:
	DECLARE_MESSAGE_MAP()
public:
	CStringW UTF8toUTF16(const CStringA& utf8);
	COLORREF GetPixelFast(CImage *pImage,int x, int y);
	void SetPixelFast(CImage *pImage,int x, int y, COLORREF color);
	double GetZoom();
	void SetExt();
	void SetToAll();
	void SetShowPen();
	void SetBright(int bright);
	void SetSesitivity(int sensitivity);
	void OnFileSaveImage(bool isColor = true, bool isOCR = false, bool isExport =  true);
	BOOL OnFileImageOpen();
	void OnToolsMakeBW(void);
	void OnUpdateSizeHalf(CCmdUI* pCmdUI);
	void OnUpdateSizeOriginal(CCmdUI* pCmdUI);
	void OnUpdateSizeDouble(CCmdUI* pCmdUI);
	void OnUpdateSizeX4(CCmdUI* pCmdUI);
	void OnUpdateSizeX8(CCmdUI* pCmdUI);
	void OnUpdateSizeX16(CCmdUI* pCmdUI);
	void OnUpdateSizeFill(CCmdUI* pCmdUI);
	void OnChangeSize(UINT nID);
	BOOL OnExtendedFloodFill(BYTE targetRed,BYTE targetGreen,BYTE targetBlue,CImage *picture,CImage *mask,CPoint point,MaskVector *pointsVector,bool isAdded=true);
	BOOL OnFloodFill(BYTE targetRed,BYTE targetGreen,BYTE targetBlue,CImage *picture,CImage *mask,CPoint point,MaskVector *pointsVector,bool isAdded=true);
	void OnLPaint(UINT nFlag,CPoint point);
	void OnRPaint(UINT nFlag,CPoint point);
	void OnChangePen(PenEnum pen);
	int GetPenSize();
	void SetBorder();
	void PrintPoint(CPoint PxlReal);
public:
	afx_msg void OnFileOpen();
	afx_msg void OnFileNew();
	afx_msg BOOL OnMouseWheel( UINT, short, CPoint );
	afx_msg void OnLButtonUp(UINT nFlag,CPoint point);
	afx_msg void OnLButtonDown(UINT nFlag,CPoint point);
	afx_msg void OnRButtonDown(UINT nFlag,CPoint point);
	afx_msg void OnRButtonUp(UINT nFlag,CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
