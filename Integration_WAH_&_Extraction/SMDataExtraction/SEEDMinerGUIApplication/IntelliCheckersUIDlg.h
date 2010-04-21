// IntelliCheckersUIDlg.h : header file
//
//{{AFX_INCLUDES()
#include "shockwaveflash.h"
#include "csvconnection.h"
#include "extractedCsvDTO.h"
#include "WrapDataSource.h"
#include "aprioriopt.h"
#include "C45TreeNominal.h"
//}}AFX_INCLUDES

#if !defined(AFX_IntelliCheckersUIDLG_H__14D37E6D_A305_4612_A543_6DDA6BCCC9F0__INCLUDED_)
#define AFX_IntelliCheckersUIDLG_H__14D37E6D_A305_4612_A543_6DDA6BCCC9F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

using namespace CsvDataExtraction;
using namespace CSVConnectionInfo;
/////////////////////////////////////////////////////////////////////////////
// CIntelliCheckersUIDlg dialog

class CIntelliCheckersUIDlg : public CDialog, public CFlexCallBack
{
private:
// Construction
	WrapDataSource * m_source;
	AprioriOpt * m_apriori;
	C45TreeNominal * m_classifier;
	enum source_type{WRAPPED_SOURCE,APRIORI_SOURCE,CLASSIFIER_SOURCE};
	void CSV(string path,int noOfRows);
	void Aprior(double confidence,double minsuport,int rules);
	string Text(source_type type,int noOfRows);
	void DeleteAll();
	void InitAll();
public:
	
	CIntelliCheckersUIDlg(CWnd* pParent = NULL);	// standard constructor
	virtual void OnFlexInit(int flashIndex);
	virtual void OnFlexButtonClick(CFlexEvent *evt, CString controller);
	virtual void OnFlexEvent(CFlexEvent *evt);


	void OnOK(); 
	int id_flash1;
	CFlexObject	*flash;
// Dialog Data
	//{{AFX_DATA(CIntelliCheckersUIDlg)
	enum { IDD = IDD_IntelliCheckersUI_DIALOG };
	CShockwaveFlash	m_Flash;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIntelliCheckersUIDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CIntelliCheckersUIDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnFlashCallShockwaveflash1(LPCTSTR request);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IntelliCheckersUIDLG_H__14D37E6D_A305_4612_A543_6DDA6BCCC9F0__INCLUDED_)
