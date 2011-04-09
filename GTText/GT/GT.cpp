// GT.cpp : define los comportamientos de las clases para la aplicación.
//

#include "stdafx.h"
#include "GT.h"
#include "MainFrm.h"
#include <initguid.h>
#include "Xmltree_i.c"
#include "GTDoc.h"
#include "GTView.h"
#include "XMLDOMDocumentEventsSink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CGTApp

BEGIN_MESSAGE_MAP(CGTApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CGTApp::OnAppAbout)
	// Comandos de documento estándar basados en archivo
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)

	// Comando de configuración de impresión estándar
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_FILE_FROMCLIPBOARD, &CGTApp::OnFileFromclipboard)
	ON_UPDATE_COMMAND_UI(ID_FILE_FROMCLIPBOARD, &CGTApp::OnUpdateFileFromclipboard)
END_MESSAGE_MAP()


// Construcción de CGTApp

CGTApp::CGTApp()
{
	// TODO: agregar aquí el código de construcción,
	// Colocar toda la inicialización importante en InitInstance
}


// El único objeto CGTApp

CGTApp theApp;




BOOL CGTApp::InitInstance()
{
	// Windows XP requiere InitCommonControlsEx() si un manifiesto de
	// aplicación especifica el uso de ComCtl32.dll versión 6 o posterior para habilitar
	// estilos visuales. De lo contrario, se generará un error al crear ventanas.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Establecer para incluir todas las clases de control comunes que desee utilizar
	// en la aplicación.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Inicializar bibliotecas OLE
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Inicialización estándar
	// Si no utiliza estas características y desea reducir el tamaño
	// del archivo ejecutable final, debe quitar
	// las rutinas de inicialización específicas que no necesite
	// Cambie la clave del Registro en la que se almacena la configuración
	// TODO: debe modificar esta cadena para que contenga información correcta
	// como el nombre de su compañía u organización
	SetRegistryKey(_T("Aplicaciones generadas con el Asistente para aplicaciones local"));
	LoadStdProfileSettings(4);  // Cargar opciones de archivo INI estándar (incluidas las de la lista MRU)
	// Registrar las plantillas de documento de la aplicación. Las plantillas de documento
	//  sirven como conexión entre documentos, ventanas de marco y vistas
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CGTDoc),
		RUNTIME_CLASS(CMainFrame),       // Ventana de marco SDI principal
		RUNTIME_CLASS(CGTView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Habilitar apertura de ejecución DDE
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Analizar línea de comandos para comandos Shell estándar, DDE, Archivo Abrir
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Enviar comandos especificados en la línea de comandos. Devolverá FALSE si
	// la aplicación se inició con los modificadores /RegServer, /Register, /Unregserver o /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Se ha inicializado la única ventana; mostrarla y actualizarla
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// Llamar a DragAcceptFiles sólo si existe un sufijo
	//  En una aplicación SDI, esto debe ocurrir después de ProcessShellCommand
	// Habilitar apertura de arrastrar y colocar
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}



// Cuadro de diálogo CAboutDlg utilizado para el comando Acerca de

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Datos del cuadro de diálogo
	enum { IDD = IDD_GTABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

// Implementación
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// Comando de la aplicación para ejecutar el cuadro de diálogo
void CGTApp::OnAppAbout()
{
	CAboutDlg /*CXmltreeDlg*/ aboutDlg;
	aboutDlg.DoModal();
}


// Controladores de mensaje de CGTApp

	
CXmltreeModule _Module;


BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

LONG CXmltreeModule::Unlock()
{
	AfxOleUnlockApp();
	return 0;
}

LONG CXmltreeModule::Lock()
{
	AfxOleLockApp();
	return 1;
}
LPCTSTR CXmltreeModule::FindOneOf(LPCTSTR p1, LPCTSTR p2)
{
	while (*p1 != NULL)
	{
		LPCTSTR p = p2;
		while (*p != NULL)
		{
			if (*p1 == *p)
				return CharNext(p1);
			p = CharNext(p);
		}
		p1++;
	}
	return NULL;
}




BOOL CGTApp::InitATL()
{
	m_bATLInited = TRUE;

#if _WIN32_WINNT >= 0x0400
	HRESULT hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED);
#else
	HRESULT hRes = CoInitialize(NULL);
#endif

	if (FAILED(hRes))
	{
		m_bATLInited = FALSE;
		return FALSE;
	}

	_Module.Init(ObjectMap, AfxGetInstanceHandle());
	_Module.dwThreadID = GetCurrentThreadId();

	LPTSTR lpCmdLine = GetCommandLine(); //this line necessary for _ATL_MIN_CRT
	TCHAR szTokens[] = _T("-/");

	BOOL bRun = TRUE;
	LPCTSTR lpszToken = _Module.FindOneOf(lpCmdLine, szTokens);
	while (lpszToken != NULL)
	{
		if (lstrcmpi(lpszToken, _T("UnregServer"))==0)
		{
			_Module.UpdateRegistryFromResource(IDR_XMLTREE, FALSE);
			_Module.UnregisterServer(TRUE); //TRUE means typelib is unreg'd
			bRun = FALSE;
			break;
		}
		if (lstrcmpi(lpszToken, _T("RegServer"))==0)
		{
			_Module.UpdateRegistryFromResource(IDR_XMLTREE, TRUE);
			_Module.RegisterServer(TRUE);
			bRun = FALSE;
			break;
		}
		lpszToken = _Module.FindOneOf(lpszToken, szTokens);
	}

	if (!bRun)
	{
		m_bATLInited = FALSE;
		_Module.Term();
		CoUninitialize();
		return FALSE;
	}

	hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER, 
		REGCLS_MULTIPLEUSE);
	if (FAILED(hRes))
	{
		m_bATLInited = FALSE;
		CoUninitialize();
		return FALSE;
	}	

	return TRUE;

}




void CGTApp::OnFileFromclipboard()
{
	m_newFromClipboard = true;
	OnFileNew();
	m_newFromClipboard = false;
}


void CGTApp::OnUpdateFileFromclipboard(CCmdUI *pCmdUI)
{
	::OpenClipboard(AfxGetMainWnd()->GetSafeHwnd());
	HANDLE hClip = ::GetClipboardData(CF_BITMAP);
	HBITMAP hbClip = (HBITMAP) hClip;
	if(hbClip != NULL)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
	CloseClipboard();
}
