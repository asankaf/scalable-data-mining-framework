// IntelliCheckersUI.h : main header file for the IntelliCheckersUI application
//

#if !defined(AFX_IntelliCheckersUI_H__47F8577B_C6E8_484F_88BC_BA4252C3B545__INCLUDED_)
#define AFX_IntelliCheckersUI_H__47F8577B_C6E8_484F_88BC_BA4252C3B545__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIntelliCheckersUIApp:
// See IntelliCheckersUI.cpp for the implementation of this class
//

class CIntelliCheckersUIApp : public CWinApp
{
public:
	CIntelliCheckersUIApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIntelliCheckersUIApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CIntelliCheckersUIApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IntelliCheckersUI_H__47F8577B_C6E8_484F_88BC_BA4252C3B545__INCLUDED_)
