// GTDoc.h: interfaz de la clase CGTDoc
//

#pragma once
#include <atlimage.h>
#include <map>
#include <vector>

typedef enum
{
	page,
	pcGts,
    pcCreator,
	pcComments,
	pcMetadata,
	txt_bgcolor,
	txt_color,
	txt_font_size,
	txt_orientation,
	txt_primary_language,
	txt_primary_script,
	txt_reading_direction,
	txt_reading_orientation,
	txt_reverse_video,
	txt_secondary_language,
	txt_secondary_script,
	txt_type,
	text_region,
	text_line,
	word,
	glyph,
	plain_text,
	ligature,
	symbol,
	image_filename,
	glyph_pcGts,
	glyph_pcCreator,
	glyph_pcComments,
	glyph_glyph,
	glyph_coordGroup1,
	glyph_coordGroup2,
	glyph_coordGroup3,
	glyph_coordGroup4,
	glyph_coordGroup5,
	glyph_coordGroup6,
	glyph_coordGroup7,
	gt,
	gl,
	pc,
	gtID,
	glID,
	pcID,
	glDep,
	pcDep,
	rootDoc,
	pcCreated,
	pcLastChange,
	glyph_pcCreated,
	glyph_pcLastChange,
	no_text_regions,
	gl_lrFolder,
	pc_lrFolder,
	none
	
} NodeName;

enum EditEnum
{
	EDIT_CORE,
	EDIT_OUTLINE,
	EDIT_SHADE,
	EDIT_NONE
};

enum ToolEnum
{
	PIXEL_TOOL,
	REGION_TOOL,
	ZOOM_TOOL,
	INVERT_TOOL,
	NO_TOOL
};


typedef std::pair<NodeName, MSXML2::IXMLDOMNodePtr> PtrPair;
typedef std::pair<COLORREF,double> FloodFillPair3;
typedef std::pair<CPoint,FloodFillPair3> FloodFillPair;
typedef std::pair<CPoint,COLORREF> FloodFillPair2;
typedef std::pair<CPoint,int> VectorRun;
//typedef std::vector<std::vector<__int8>> Matrix;


typedef std::vector<VectorRun> MaskVector;
typedef std::vector<FloodFillPair> FloodFillPoints;
typedef std::vector<FloodFillPair2> FloodFillPoints2;



bool operator< (const MSXML2::IXMLDOMNodePtr &op1, const MSXML2::IXMLDOMNodePtr &op2);
bool operator< (const CPoint op1, const CPoint op2);



typedef	struct
{
	std::map <NodeName, MSXML2::IXMLDOMNodePtr> ptrsMap;
	NodeName type;
} SectionPtrs;


typedef std::pair<MSXML2::IXMLDOMNodePtr,SectionPtrs*> SectionPair;

class CImageSelection  
{
private:
	bool m_core;
	bool m_shade;
	bool m_outline;
	bool m_isChanged;
	CImage m_imgMask;
	CRect m_editRect;
	


public:
	CImageSelection();
	CImageSelection(const CImageSelection &copy);
	CImageSelection(CImage* source);
	~CImageSelection();
	COLORREF GetPixelFast(CImage *pImage,int x, int y);
	void SetPixelFast(CImage *pImage,int x, int y, COLORREF color);
	bool GetCore();
	bool GetShade();
	bool GetOutline();
	CImage* GetImageMask();
	void SetCore(bool core);
	void SetShade(bool shade);
	void SetOutline(bool outline);
	void SetPixelMask(int x,int y,int r,int group);
	CImage* MergePoints(CImage* source,bool changeSelection = true);
	void ResetMask();
	void InvertMask(CRect size,EditEnum region);
	void DeltePoints();
	void UpdateEditBox(CPoint point);
	void LoadMaskPoints(EditEnum region,CRect loadRect = CRect(0,0,0,0));
	BOOL ChangePoint(CPoint point,EditEnum region,bool isAdded,int size=1);
	std::vector<MaskVector>  GetAllPoints();
	MaskVector* GetCurrentMaskVector(EditEnum region);
	CImageSelection & operator= (const CImageSelection &copy);
	void EraseArea(CRect size,EditEnum region);



protected:
	
	MaskVector m_coreVector;
	MaskVector m_outlineVector;
	MaskVector m_shadeVector;


};



class CGTDoc : public CDocument
{
private:
	
	CImage m_imgOriginal;
	CString m_strGlyphPathFile;
	CString m_strPagePathFile;
	CString m_strLinkPathFile;
	CString m_strLinkPathFileOld;
	CString m_strImagePathFile;
	CString m_newLinkPathFile;
	CString m_currentFolder;
	CString m_pcDefaultFolder;
	CString m_glDefaultFolder;
	CPoint m_pointSelected;  //x y for the position of xmlview scrollbars 
	CPoint m_penPoint;  //Used by pen tool
	CPoint m_zoomImagePoint;
	CPoint m_zoomSreenPoint;
	std::map <MSXML2::IXMLDOMNodePtr,SectionPtrs*> m_sectionsMap;
	MSXML2::IXMLDOMDocumentPtr m_pDOMLinkDoc;
	MSXML2::IXMLDOMDocumentPtr m_pDOMPageDoc;
	MSXML2::IXMLDOMDocumentPtr m_pDOMGlyphDoc;
	MSXML2::IXMLDOMDocumentPtr m_pDOMPageDocOld;
	MSXML2::IXMLDOMDocumentPtr m_pDOMGlyphDocOld;
	MSXML2::IXMLDOMDocumentPtr m_pDOMLinkDocOld;
	SectionPtrs* m_pCurrentSection;
	bool m_isLocked;
	CImageSelection m_imgSelection;
	CImageSelection m_imgSelectionOld;
	bool m_isLoad;
	bool m_isDirty;
	bool m_showPointBar;
	int  m_isBackuped;
	int	 m_isXMLBackuped;
	int m_idCount;
	EditEnum m_edition_state;
	EditEnum m_onGlyphState;
	ToolEnum m_tool;
	MaskVector m_copyVector;

protected: // Crear sólo a partir de serialización
	CGTDoc();
	DECLARE_DYNCREATE(CGTDoc)


// Operaciones
public:
	CImage* GetImage();
	EditEnum GetEditState();
	void  SetEditState(EditEnum state);
	EditEnum GetOnGlyphState();
	void  SetOnGlyphState(EditEnum state);
	ToolEnum GetToolState();
	void  SetToolState(ToolEnum tool);
	void OnFileSave();
	void SetLoad(bool isLoad);
	void SetDirty(bool isDirty);
	bool IsDirty();
	bool IsLoad();
	bool IsPointBar();
	void OnImageInfo(void);
	MSXML2::IXMLDOMDocumentPtr GetLinkDom();
	MSXML2::IXMLDOMDocumentPtr GetPageDom();
	MSXML2::IXMLDOMDocumentPtr GetGlyphDom();
	void SetLinkDom(MSXML2::IXMLDOMDocumentPtr docPtr);
	void SetPageDom(MSXML2::IXMLDOMDocumentPtr docPtr);
	void SetGlyphDom(MSXML2::IXMLDOMDocumentPtr docPtr);
	void SetSelectedPoint(CPoint point);
	CPoint GetSelectedPoint();
	void AddNodeToCurrSection(MSXML2::IXMLDOMNodePtr pNode,NodeName nodeName);
	SectionPtrs* AddSection(MSXML2::IXMLDOMNodePtr pNodeKey,NodeName nodeName = none);
	SectionPtrs* GetCurrentSection();
	void SetCurrentSection(SectionPtrs* section);
	MSXML2::IXMLDOMNodePtr GetNodeFromCurrSection(NodeName nodeName);
	void DeleteNodeFromCurrSection(NodeName nodeName);
	BOOL SwitchToView(CRuntimeClass* pNewViewClass);
	void LoadPointsSelected(MSXML2::IXMLDOMNodePtr pRootNode);
	BOOL UpdateSection(MSXML2::IXMLDOMNodePtr pNode,NodeName nodeName = none);
	void SetGlyphPath(CString path);
	void SetPagePath(CString path);
	void SetLinkPath(CString path);
	void SetImagePath(CString path);
	void SetDefPcFolder(CString folder);
	void SetDefGlFolder(CString folder);
	void SetCurrentlFolder(CString folder);
	CString GetCurrentFolder();
	CString GetDefPcFolder();
	CString GetDefGlFolder();
	void LoadMask();
	void Release(bool deep =  true);
	void Backup(int nView = 0);
	CImageSelection *GetImageSelection();
	CString GetImagePath();
	CString GetGlyphPath();
	CString GetPagePath();
	CString GetLinkPath();
	CString GetNewDocPath();
	void	SetZoomPoint(CPoint imagePoint,CPoint sreenPoint);
	void	GetZoomPoint(CPoint &imagePoint,CPoint &sreenPoint);
	void	SetPenPoint(CPoint penPoint);
	CPoint	GetPenPoint();
	void SetLock(bool lock);
	bool GetLock();
	void SavePoints();
	int  UpdateIdCounter(CString id="");


// Reemplazos
public:
	
	
// Implementación
public:
	virtual ~CGTDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Funciones de asignación de mensajes generadas
protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();

	
public:
	afx_msg void OnUpdateZoom1x(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSizeDouble(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSizeX4(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSizeX8(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSizeX16(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSizeZoom(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSizeFill(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSizeOriginal(CCmdUI* pCmdUI);
	afx_msg void OnChangeSize(UINT nID);
	afx_msg void OnUpdateSizeHalf(CCmdUI* pCmdUI);
	afx_msg void OnViewShade();
	afx_msg void OnViewOutline();
	afx_msg void OnViewCore();
	afx_msg void OnEditShade();
	afx_msg void OnEditOutline();
	afx_msg void OnEditCore();
	afx_msg void OnUpdateEditShade(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditOutline(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditCore(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewShade(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewOutline(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewCore(CCmdUI *pCmdUI);
	afx_msg void OnToolsPixelselect();
	afx_msg void OnUpdateToolsPixelselect(CCmdUI *pCmdUI);
	afx_msg void OnToolsRegionselect();
	afx_msg void OnUpdateToolsRegionselect(CCmdUI *pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnZoom();
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnViewBrushsizebar();
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnContextgtUndo();
	afx_msg void OnUpdateContextGtUndo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateContextgtRedo(CCmdUI *pCmdUI);
	afx_msg void OnEditRedoxml();
	afx_msg void OnEditInvert();
	afx_msg void OnUpdateEditInvert(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolsInvertall(CCmdUI *pCmdUI);
	afx_msg void OnToolsInvertall();
	afx_msg void OnFilePreferences();
	afx_msg void OnViewShowpoint();
	afx_msg void OnUpdateViewPoint(CCmdUI *pCmdUI);

};


