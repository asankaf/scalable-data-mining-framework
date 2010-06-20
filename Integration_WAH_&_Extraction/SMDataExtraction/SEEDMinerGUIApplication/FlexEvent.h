// FlexEvent.h: interface for the CFlexEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLEXEVENT_H__BD329A07_D79B_4C28_BC31_CC43275795E1__INCLUDED_)
#define AFX_FLEXEVENT_H__BD329A07_D79B_4C28_BC31_CC43275795E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#pragma warning(disable : 4786)
#endif // _MSC_VER > 1000

class CFlexEvent  
{
public:
	CFlexEvent();
	virtual ~CFlexEvent();
	CString itemId;
	CString itemType;
	CString eventType;
	CString procedure;
	CString procedurePara;
	CString algoPara;
	CString measureTime;
	CString runInALoop;

	int flashIndex;

};

#endif // !defined(AFX_FLEXEVENT_H__BD329A07_D79B_4C28_BC31_CC43275795E1__INCLUDED_)
