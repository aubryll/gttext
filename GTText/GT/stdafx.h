// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__F3C8BBA9_DA60_11D2_866B_00A0CC23C8EA__INCLUDED_)
#define AFX_STDAFX_H__F3C8BBA9_DA60_11D2_866B_00A0CC23C8EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxole.h>

#include <afxcontrolbars.h>	// MFC support for ribbons and control bars


#import "msxml6.dll"






#include <afxconv.h>

	#define _ATL_APARTMENT_THREADED
#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
class CXmltreeModule : public CComModule
{
public:
	LONG Unlock();
	LONG Lock();
	LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2);
	DWORD dwThreadID;
};
extern CXmltreeModule _Module;
#include <atlcom.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__F3C8BBA9_DA60_11D2_866B_00A0CC23C8EA__INCLUDED_)
#include <afxdlgs.h>
// stdafx.h: archivo de inclusión para archivos de inclusión estándar del sistema,
// o archivos de inclusión específicos del proyecto utilizados frecuentemente,
// pero cambiados rara vez

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Excluir material rara vez utilizado de encabezados de Windows
#endif

// Modificar las siguientes secciones define si su objetivo es una plataforma distinta a las especificadas a continuación.
// Consulte la referencia MSDN para obtener la información más reciente sobre los valores correspondientes a diferentes plataformas.
#ifndef WINVER				// Permitir el uso de características específicas de Windows XP o posterior.
#define WINVER 0x0501		// Cambiar para establecer el valor apropiado para otras versiones de Windows.
#endif

#ifndef _WIN32_WINNT		// Permitir el uso de características específicas de Windows XP o posterior.                   
#define _WIN32_WINNT 0x0501	// Cambiar para establecer el valor apropiado para otras versiones de Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Permitir el uso de características específicas de Windows 98 o posterior.
#define _WIN32_WINDOWS 0x0410 // Cambiar para establecer el valor apropiado para Windows Me o posterior.
#endif

#ifndef _WIN32_IE			// Permitir el uso de características específicas de IE 6.0 o posterior.
#define _WIN32_IE 0x0600	// Cambiar para establecer el valor apropiado para otras versiones de IE.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// Algunos constructores CString serán explícitos
/*
// Desactiva la ocultación de MFC para algunos mensajes de advertencia comunes y, muchas veces, omitidos de forma consciente
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // Componentes principales y estándar de MFC
#include <afxext.h>         // Extensiones de MFC


#include <afxdisp.h>        // Clases de automatización de MFC



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// Compatibilidad MFC para controles comunes de Internet Explorer 4
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Compatibilidad MFC para controles comunes de Windows
#endif // _AFX_NO_AFXCMN_SUPPORT
*/



#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#ifndef TESSDLL_IMPORTS
#define TESSDLL_IMPORTS
#include "baseapi.h"
#endif
