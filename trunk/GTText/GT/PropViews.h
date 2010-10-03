#pragma once

#include "GTDoc.h"
#include "afxwin.h"

// Vista de formulario de CMetaDataView

class CMetaDataView : public CFormView
{
	DECLARE_DYNCREATE(CMetaDataView)

protected:
	CMetaDataView();           // Constructor protegido utilizado por la creación dinámica
	virtual ~CMetaDataView();
	


public:
	enum { IDD = IDD_FORMMETA };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV
	

	DECLARE_MESSAGE_MAP()
public:
	CGTDoc* GetDocument() const;
	CString c_comentaries;
	CString c_creator;
	afx_msg void OnBnClickedChangeMeta();
	virtual void OnInitialUpdate(); // Se llama la primera vez después de la construcción
	virtual void OnUpdate();
};




class CTextRegionView : public CFormView
{
	DECLARE_DYNCREATE(CTextRegionView)

protected:
	CTextRegionView();           // Constructor protegido utilizado por la creación dinámica
	virtual ~CTextRegionView();
	virtual void OnInitialUpdate(); // Se llama la primera vez después de la construcción


public:
	enum { IDD = IDD_FORMREGION };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV
	

	DECLARE_MESSAGE_MAP()
public:
	CGTDoc* GetDocument() const;
	CString c_bgrndColor;
	CString c_txtcolor;
	CString c_fontSize;
	CString c_orientation;
	CString c_language;
	CString c_script;
	CString c_readingOrient;
	CString c_readingDirection;
	CString c_secondLange;
	CString c_secondScript;
	CString c_txttype;
	BOOL c_reVideo;
	afx_msg void OnBnClickedChRegion();
	virtual void OnUpdate();
	CString c_textEqui;
	afx_msg void OnCbnSelchangeCombo();
};	



class CTextEquivalView : public CFormView
{
	DECLARE_DYNCREATE(CTextEquivalView)

protected:
	CTextEquivalView();           // Constructor protegido utilizado por la creación dinámica
	virtual ~CTextEquivalView();
	virtual void OnInitialUpdate(); // Se llama la primera vez después de la construcción


public:
	enum { IDD = IDD_FORMTEXT };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV
	

	DECLARE_MESSAGE_MAP()
public:
	CGTDoc* GetDocument() const;
	CString c_text;
	virtual void OnUpdate();
	afx_msg void OnBnClickedChangedText();
	CString c_title;
};


class CGlyphView : public CFormView
{
	DECLARE_DYNCREATE(CGlyphView)

protected:
	CGlyphView();           // Constructor protegido utilizado por la creación dinámica
	virtual ~CGlyphView();
	

public:
	enum { IDD = IDD_FORMGLYPH };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV
	

	DECLARE_MESSAGE_MAP()
public:
	CGTDoc* GetDocument() const;
	CString c_textglyph;
	BOOL c_symbol;
	BOOL c_ligature;
	virtual void OnInitialUpdate(); // Se llama la primera vez después de la construcción
	virtual void OnUpdate();
	afx_msg void OnBnClickedChangeGlph();
};



class CBlankView : public CFormView
{
	DECLARE_DYNCREATE(CBlankView)

protected:
	CBlankView();           // Constructor protegido utilizado por la creación dinámica
	virtual ~CBlankView();
	

public:
	enum { IDD = IDD_FORMBLANK};
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV
	

	DECLARE_MESSAGE_MAP()
public:
	CGTDoc* GetDocument() const;
	virtual void OnInitialUpdate(); // Se llama la primera vez después de la construcción
	virtual void OnUpdate();
	CString c_title;
};
#pragma once


class CFolders : public CDialog
{
// Implementation
public:
	CFolders(CString &glyphFold,CString &pageFold,CString &language);
	// Dialog data
	enum { IDD = IDD_PREFERENCES};
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CImage* m_pImage;
	CString* c_glyphFolder;
	CString* c_pageFolder;
	CString* c_language;
public:
	afx_msg void OnBnClickedOk();
};