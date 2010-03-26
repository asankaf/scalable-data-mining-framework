// CFlexHandler.cpp : implementation file
//

#include "stdafx.h"
//#include "apife.h"
#include "FlexHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlexHandler

CFlexHandler::CFlexHandler()
{
//	CFlexCall::Init((void*)m_hWnd);
	CFlexCallQueue::Start(5);
	Create(_T("STATIC"), "Hi", SW_HIDE|WS_OVERLAPPEDWINDOW,CRect(10,10,10,10),CWnd::GetDesktopWindow(),999990);
	CFlexCall::Init((void*) (this->m_hWnd));
}

CFlexHandler::~CFlexHandler()
{
	CBridgeManager::Cleanup();
	CFlexCallQueue::Stop();
	CLogger::Uninit();
}


BEGIN_MESSAGE_MAP(CFlexHandler, CWnd)
	//{{AFX_MSG_MAP(CFlexHandler)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_MESSAGE(WM_FLEXCALLBACK, OnFlexCallback)
	ON_MESSAGE(WM_FLEXCALLBACK_RETURN, OnFlexCallbackRet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

LRESULT CFlexHandler::OnFlexCallback(WPARAM wParam, LPARAM lParam)
{
	return CFlexCtrl::OnFlexCallback(wParam,lParam);
}

LRESULT CFlexHandler::OnFlexCallbackRet(WPARAM wParam, LPARAM lParam)
{
	return CFlexCtrl::OnFlexCallbackRet(wParam,lParam);
}

/////////////////////////////////////////////////////////////////////////////
// CFlexHandler message handlers
