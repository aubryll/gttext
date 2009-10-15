// This is a part of the Microsoft Foundation Classes C++ library. 
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


#include "stdafx.h"
#include "InfoDlg.h"
#include "GT.h"

//#include <windows.h>
//#include <stdlib.h>
//#include <stdio.h>

CInfoDlg::CInfoDlg(CImage &image) : CDialog(CInfoDlg::IDD)
{
	m_pImage = &image;
}

// override the CDialog::OnInitDialog method
BOOL CInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CEdit* pInfo;
	char buffer[100];

	// display original size
	int width, height; 
	pInfo = (CEdit*) GetDlgItem(IDC_IMAGESIZE);
	width = m_pImage->GetWidth();
	height = m_pImage->GetHeight();
	sprintf_s(buffer, 100, "%i x %i",m_pImage->GetWidth(),m_pImage->GetHeight());
	pInfo->SetWindowTextW(LPCTSTR(CString(buffer)));

	// display bit depth
	pInfo = (CEdit*) GetDlgItem(IDC_BITDEPTH);
	sprintf_s(buffer, 100, "%i-bit",m_pImage->GetBPP());
	pInfo->SetWindowTextW(LPCTSTR(CString(buffer)));
	// display DIB section info
	pInfo = (CEdit*) GetDlgItem(IDC_DIBSECTION);
	if (m_pImage->IsDIBSection()) {
		sprintf_s(buffer, 100, "Yes");
	} else {
		sprintf_s(buffer, 100, "No");
	}
	pInfo->SetWindowTextW(LPCTSTR(CString(buffer)));

	// display indexed info
	pInfo = (CEdit*) GetDlgItem(IDC_INDEXED);
	if (m_pImage->IsIndexed()) {
		sprintf_s(buffer, 100, "Yes");
	} else {
		sprintf_s(buffer, 100, "No");
	}
	pInfo->SetWindowTextW(LPCTSTR(CString(buffer)));

	// display pitch
	pInfo = (CEdit*) GetDlgItem(IDC_PITCH);
	sprintf_s(buffer, 100, "%i bytes",m_pImage->GetPitch());
	pInfo->SetWindowTextW(LPCTSTR(CString(buffer)));

	return TRUE;

}


BEGIN_MESSAGE_MAP(CInfoDlg,CDialog)
END_MESSAGE_MAP()
