#if !defined(AFX_MFLEXHANDLER_H__B4F42467_6EB2_41C3_8D52_B6BC1D47199E__INCLUDED_)
#define AFX_MFLEXHANDLER_H__B4F42467_6EB2_41C3_8D52_B6BC1D47199E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#pragma warning(disable : 4786)
#include <shlobj.h>
#include <shlwapi.h>
#include <objbase.h>
#include <sstream>
#include <windows.h>
#include "Logger.h"
#include "FlexBridge.h"
#include "ASObject.h"
#include "BridgeException.h" 
#include "BridgeManager.h"
#include "FlexCallQueue.h"
#include "FlashIDManager.h"
#include "BridgeManager.h"
#include "FlexCtrl.h"
#endif // _MSC_VER > 1000
// MFlexHandler.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MFlexHandler window

class CFlexHandler : public CWnd
{
// Construction
public:
	CFlexHandler();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MFlexHandler)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFlexHandler();
	afx_msg LRESULT OnFlexCallback(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFlexCallbackRet(WPARAM wParam, LPARAM lParam);
	// Generated message map functions
protected:
	//{{AFX_MSG(MFlexHandler)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFLEXHANDLER_H__B4F42467_6EB2_41C3_8D52_B6BC1D47199E__INCLUDED_)
