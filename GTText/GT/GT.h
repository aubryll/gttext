// GT.h: archivo de encabezado principal para la aplicación GT
//
#pragma once

#ifndef __AFXWIN_H__
	#error "incluir 'stdafx.h' antes de incluir este archivo para PCH"
#endif

#include "resource.h"       // Símbolos principales
#include "Xmltree_i.h"


// CGTApp:
// Consulte la sección GT.cpp para obtener información sobre la implementación de esta clase
//

class CGTApp : public CWinAppEx
{
public:
	CGTApp();


// Reemplazos
public:
	virtual BOOL InitInstance();

// Implementación
	afx_msg void OnAppAbout();

	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bATLInited;
private:
	BOOL InitATL();
	BOOL m_newFromClipboard;
public:
	afx_msg void OnFileFromclipboard();
	BOOL GetClipboardFlag(){return m_newFromClipboard;};
	
	afx_msg void OnUpdateFileFromclipboard(CCmdUI *pCmdUI);
};

extern CGTApp theApp;