// IntelliCheckersUIDlg.h : header file
//
//{{AFX_INCLUDES()
#include "shockwaveflash.h"
#include "csvconnection.h"
#include "extractedCsvDTO.h"
#include "WrapDataSource.h"
#include "aprioriopt.h"
#include "C45TreeNominal.h"
#include "naivebayes.h"
#include "LoadSavedDataSources.h"
#include "DataSources.h"
#include "nullpreprocessor.h"
#include "CompressionHandler.h"
#include "BitStreamInfo.h"
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
	NaiveBayes * m_bayesian;
	enum source_type{WRAPPED_SOURCE,APRIORI_SOURCE,CLASSIFIER_SOURCE,CLASSIFIER_TEXT_SOURCE,BAYESIAN_SOURCE};

	/**
	Loads data from a CSV file
	@param _path location of the csv file
	@param _no_of_rows Number of rows needed to be loaded
	*/
	void CSV(string _path,int _no_of_rows);

	/**Loads data from a previously encoded file
	@param _meta_file_name name of the meta file containing description of data
	@param _data_file_name name of the file containing encoded data
	@param _data_source_name name of the data source that needed to be loaded.
	@param _noOfRows no of rows that needed to be loaded from this file
	*/
	void SavedDataLoader(string _meta_file_name,string _data_file_name,string _data_source_name,int _noOfRows);

	/**
	Converts the data source to a the given encoding scheme
	*/
	void Convert(BitStreamInfo::vertical_bit_type _type);

	/**Eliminates NULL values and return the modified data source*/
	void NullEliminator();

	/**Runs Apriori Algorithm
		@param _confidence confidence for the rules (0.9 is default)
		@param _min_suport minimum support of the rules to be found
		@param _rules no of rules to be found
	*/
	void Aprior(double _confidence,double _min_suport,int _rules);	

	void Classifier();	

	/**Runs the Bayesian Classifier, using the m_source set previously
	@param _class Index of the class that needed to be predicted
	*/
	void Bayesian(int _class);

	/**Gives the string representation of the data source or the algorithm results*/
	string Text(source_type type,int noOfRows);

	/**Clears all instance variables set for particular flow*/
	void DeleteAll();

	/**Initialises all instance variables set for particular flow*/
	void InitAll();

	/** Gives the space consumed by the Wrappeddatasource in Bytes*/
	string MeasureSpace();
public:
	
	CIntelliCheckersUIDlg(CWnd* pParent = NULL);	// standard constructor
	virtual void OnFlexInit(int flashIndex);
	virtual void OnFlexButtonClick(CFlexEvent *evt, CString controller);
	virtual void OnFlexEvent(CFlexEvent *evt);
	void Tokenize(const string& str,vector<string>& tokens,const string& delimiters = "->");


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
