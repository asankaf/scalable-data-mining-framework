// IntelliCheckersUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include <iostream>
#include "SeedMinerGui.h"
#include "SeedMinerGuiDlg.h"
#include <sstream>
#include <string>
#include <vector>
#include "Utils.h"
#include <wtypes.h>
#include "DBConnection.h"
#include "DiscretizeData.h"
#include "RangeSplitter.h"
#include "Init.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
using namespace std;
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIntelliCheckersUIDlg dialog

CIntelliCheckersUIDlg::CIntelliCheckersUIDlg(CWnd* pParent /*=NULL*/)
: CDialog(CIntelliCheckersUIDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIntelliCheckersUIDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	InitAll();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIntelliCheckersUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIntelliCheckersUIDlg)
	DDX_Control(pDX, IDC_SHOCKWAVEFLASH1, m_Flash);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIntelliCheckersUIDlg, CDialog)
	//{{AFX_MSG_MAP(CIntelliCheckersUIDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	//	ON_WM_SIZE()
	//ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIntelliCheckersUIDlg message handlers

BOOL CIntelliCheckersUIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	id_flash1 = CFlexCtrl::RegisterDefaultCallbacks (&m_Flash,this);
	m_Flash.SetMovie(CFlexCtrl::GetAbsolutePath("SEEDMinerGUI.swf"));
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIntelliCheckersUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIntelliCheckersUIDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIntelliCheckersUIDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BEGIN_EVENTSINK_MAP(CIntelliCheckersUIDlg, CDialog)
	//{{AFX_EVENTSINK_MAP(CIntelliCheckersUIDlg)
	ON_EVENT(CIntelliCheckersUIDlg, IDC_SHOCKWAVEFLASH1, 197 /* FlashCall */, OnFlashCallShockwaveflash1, VTS_BSTR)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CIntelliCheckersUIDlg::OnFlashCallShockwaveflash1(LPCTSTR request) 
{
	// TODO: Add your control notification handler code here
	CFlexCtrl::OnFlashCallShockwaveflash(request,&m_Flash);	
}

void CIntelliCheckersUIDlg::OnOK()
{
	if(m_Flash.GetFocus())
		m_Flash.SendMessageA(WM_KEYDOWN, VK_RETURN, 0);

}



//_______________________________Other Methods________________________________________

void CIntelliCheckersUIDlg::OnFlexInit(int flashIndex)
{
	//flash->root.Call("gettxt1").Call("settext", "Grrrrrr");
	//cout<<"hari"<<endl;

	if(flashIndex == id_flash1)
	{
		//Your initializations here.
		flash = new CFlexObject(this, id_flash1);
	}

	//Create one for each player

}

void CIntelliCheckersUIDlg::OnFlexEvent(CFlexEvent *evt)
{
	if(evt->itemId == "btn1" && evt->eventType == "mouseDown")
	{
		flash->Alert("Ok", 0);
	}
}

//////////////////////////////////// implement methods below //////////////////////////////////////////

void CIntelliCheckersUIDlg::InitAll()
{
	m_source = NULL;
	m_apriori = NULL;
	m_classifier = NULL;
	m_bayesian = NULL;
}

void CIntelliCheckersUIDlg::DeleteAll()
{
	if (m_source != NULL)
	{
		delete m_source;
		m_source = NULL;
	}
	if (m_apriori != NULL)
	{
		delete m_apriori;
		m_apriori = NULL;
	}
	if (m_classifier != NULL)
	{
		delete m_classifier;
		m_classifier = NULL;
	}
	if (m_bayesian != NULL)
	{
		delete m_bayesian;
		m_bayesian = NULL;
	}
}

void CIntelliCheckersUIDlg::SavedDataLoader(string _meta_file_name,string _data_file_name,string _data_source_name,int _noOfRows)
{
	LoadSavedDataSources lsd(_meta_file_name,_data_file_name,_noOfRows);	
	DataSources *dsLoaded = lsd.loadSavedEncodedData(true);
	/*
	LoadSavedDataSources lsd(_meta_file_name,_data_file_name);	
	DataSources *dsLoaded = lsd.loadSavedEncodedData();*/

	m_source =  (*dsLoaded)(_data_source_name);
	//delete dsLoaded;
}

/*
*Bayesian algorithm
*/

void CIntelliCheckersUIDlg::Bayesian( int _class )
{
	m_bayesian = new NaiveBayes();
	m_bayesian->buildClassifier(m_source,_class);
}

/*
*CSV data source
*/

void CIntelliCheckersUIDlg::CSV(string path,int noOfRows)
{
	CsvConnection cConcsv(path.data(),',','\n','""');
	ExtractedCsvDTO *dat = cConcsv.extractData();
	m_source = new WrapDataSource(dat,"0");	
	m_source->encodeAtrributes();
}

/*
*MYSQL data source
*/
void CIntelliCheckersUIDlg::MYSQL(string source_name,string mysql_query)
{
	char * writable = new char[source_name.size() + 1];
	std::copy(source_name.begin(), source_name.end(), writable);
	writable[source_name.size()] = '\0';
	DBConnectionInfo::DBConnection cCon(writable,"","");
	cCon.initiateConnectionToDB();
	DBQueryExecutionInfo::DBQueryExecution *cExec = new DBQueryExecutionInfo::DBQueryExecution(mysql_query.c_str());
	cExec->ExecuteQueryAndBindData(cCon.DBConnectionPtr());
	WrapDataSource *ds = new WrapDataSource(cExec,source_name);
	ds->encodeAtrributes();
	m_source = ds;
}

/*
*MSSQL data source
*/

void CIntelliCheckersUIDlg::MSSQL(string source_name,string mssql_query)
{
	char * writable = new char[source_name.size() + 1];
	std::copy(source_name.begin(), source_name.end(), writable);
	writable[source_name.size()] = '\0';
	DBConnectionInfo::DBConnection cCon(writable,"","");
	cCon.initiateConnectionToDB();
	DBQueryExecutionInfo::DBQueryExecution *cExec = new DBQueryExecutionInfo::DBQueryExecution(mssql_query.c_str());
	cExec->ExecuteQueryAndBindData(cCon.DBConnectionPtr());
	WrapDataSource *ds = new WrapDataSource(cExec,source_name);
	ds->encodeAtrributes();
	m_source = ds;
}

/*
*Aprior algorithm
*/

void CIntelliCheckersUIDlg::Aprior( double _confidence,double _min_suport,int _rules )
{
	//Setting confidence, minimum support  
	m_apriori = new AprioriOpt();
	m_apriori->Confidence(_confidence);
	m_apriori->MinSupport(_min_suport);
	m_apriori->NumRules(_rules);

	//Starting to run the algorithm
	m_apriori->BuildAssociations(m_source);
}

/*
*NullEliminator filter
*/

void CIntelliCheckersUIDlg::NullEliminator()
{
	NullPreProcessor eliminator(m_source);
	eliminator.elimiateNullValues();
	m_source = eliminator.NullEliminatedDatasource();
}

/*
*DiscretizeDataSource filter
*/

void CIntelliCheckersUIDlg::DiscretizeDataSource()
{
	DiscretizeData *discreter = new DiscretizeData(m_source);
	discreter->DiscretizeAllCtsAttributes();
}

/*
*SplitteRanges filter
*/

void CIntelliCheckersUIDlg::SplitteRanges(int no_of_ranges)
{
	RangeSplitter *splitter = new RangeSplitter(m_source);
	splitter->SplitAllNumericalAttsIntoEualRanges(no_of_ranges);
}

/*
*Convert
*/

void CIntelliCheckersUIDlg::Convert(BitStreamInfo::vertical_bit_type _type)
{
	CompressionHandler::ConvertTo(m_source,_type);
}

//Gives the Space in bytes
string CIntelliCheckersUIDlg::MeasureSpace()
{
	return Utils::toStringVal((int)m_source->SpaceUtilsation());
}

/*
*Classifier algorithm
*/

void CIntelliCheckersUIDlg::Classifier()
{
	m_classifier = new C45TreeNominal();
	m_classifier->buildClassifier(m_source);
}

/*
*Text out
*/

string CIntelliCheckersUIDlg::Text(source_type type,int noOfRows)
{
	string output = "";
	switch(type)
	{
	case APRIORI_SOURCE:
		{
			if (m_apriori != NULL)
			{
				vector<AssociateRule *> rules = m_apriori->Rules();

				for (size_t i = 0 ; i < rules.size() ; i++)
				{
					string rule = rules[i]->Rule();
					output += rule +"\n";
				}
			} 		}
		break;
	case CLASSIFIER_SOURCE:
		{
			if (m_classifier != NULL)
			{
				output += m_classifier->toGraph();
			}

		}
		break;
	case CLASSIFIER_TEXT_SOURCE:
		{
			if (m_classifier != NULL)
			{
				output += m_classifier->toString();
			}

		}
		break;
	case WRAPPED_SOURCE :
		{
			if (m_source != NULL)
			{
				output += m_source->generateCSVStringofDecodedData(noOfRows);
			}


		}
		break;
	case BAYESIAN_SOURCE:
		{
			if (m_bayesian != NULL)
			{
				output += m_bayesian->toString();
			}

		}
		break;
	default:
		{
			output += "Invalid Data Source Selected";
			break;
		}
	}


	return output;
}

/*
*data_to_filter_to_apriory_to_textview_procedures
*/

bool CIntelliCheckersUIDlg::data_to_filter_to_apriory_to_textview_procedures(vector<string>& procedureTokens)
{
	bool result=false;
	int m;
	for (m=0;m<procedureTokens.size();m++)
	{
		if(m==0&&(procedureTokens[m]==csv || procedureTokens[m]==xml || procedureTokens[m]==mysql || procedureTokens[m]==mssql))
		{
			result=true;				
		}
		else if(m==1&&(procedureTokens[m]==removeNull || procedureTokens[m]==descritize || procedureTokens[m]==rangesplite))
		{
			result&=true;				
		}
		else if(m==2&&(procedureTokens[m]==apriory))
		{
			result&=true;				
		}
		else if(m==3&&(procedureTokens[m]==text))
		{
			result&=true;			
		}
		else
		{
			result=false;
		}
	}
	result = m==4&result;
	return result;
}

/*
*data_to_filter_to_classification_to_textview_procedures
*/

bool CIntelliCheckersUIDlg::data_to_filter_to_classification_to_textview_procedures(vector<string>& procedureTokens)
{
	bool result=false;
	int m;
	for (m=0;m<procedureTokens.size();m++)
	{
		if(m==0&&(procedureTokens[m]==csv || procedureTokens[m]==xml || procedureTokens[m]==mysql || procedureTokens[m]==mssql))
		{
			result=true;				
		}
		else if(m==1&&(procedureTokens[m]==removeNull || procedureTokens[m]==descritize || procedureTokens[m]==rangesplite))
		{
			result&=true;				
		}
		else if(m==2&&(procedureTokens[m]==classification))
		{
			result&=true;				
		}
		else if(m==3&&(procedureTokens[m]==text))
		{
			result&=true;		
		}
		else
		{
			result=false;
		}
	}
	result = m==4&&result;
	return result;
}

bool CIntelliCheckersUIDlg::data_to_filter_to_naiveBayes_to_textview_procedures(vector<string>& procedureTokens)
{
	bool result=false;
	int m;
	for (m=0;m<procedureTokens.size();m++)
	{
		if(m==0&&(procedureTokens[m]==csv || procedureTokens[m]==xml || procedureTokens[m]==mysql || procedureTokens[m]==mssql))
		{
			result=true;				
		}
		else if(m==1&&(procedureTokens[m]==removeNull || procedureTokens[m]==descritize || procedureTokens[m]==rangesplite))
		{
			result&=true;	
		}
		else if(m==2&&(procedureTokens[m]==naiveBayes))
		{
			result&=true;
		}
		else if(m==3&&(procedureTokens[m]==text))
		{
			result&=true;
		}
		else
		{
			result=false;
		}
	}
	result = m==4&&result;
	return result;
}

bool CIntelliCheckersUIDlg::data_to_filter_to_classification_to_treeview_procedures(vector<string>& procedureTokens)
{
	bool result=false;
	int m;
	for (m=0;m<procedureTokens.size();m++)
	{
		if(m==0&&(procedureTokens[m]==csv || procedureTokens[m]==xml || procedureTokens[m]==mysql || procedureTokens[m]==mssql))
		{
			result=true;				
		}
		else if(m==1&&(procedureTokens[m]==removeNull || procedureTokens[m]==descritize || procedureTokens[m]==rangesplite))
		{
			result&=true;	
		}
		else if(m==2&&(procedureTokens[m]==classification))
		{
			result&=true;
		}
		else if(m==3&&(procedureTokens[m]==tree))
		{
			result&=true;
		}
		else
		{
			result=false;
		}
	}
	result = m==4&&result;
	return result;
}

bool CIntelliCheckersUIDlg::data_to_filter_to_compression_to_apriory_to_textview_procedures(vector<string>& procedureTokens)
{
	bool result=false;
	int m;
	for (m=0;m<procedureTokens.size();m++)
	{
		if(m==0&&(procedureTokens[m]==csv || procedureTokens[m]==xml || procedureTokens[m]==mysql || procedureTokens[m]==mssql))
		{
			result=true;				
		}
		else if(m==1&&(procedureTokens[m]==removeNull || procedureTokens[m]==descritize || procedureTokens[m]==rangesplite))
		{
			result&=true;
		}
		else if(m==2&&(procedureTokens[m]==wah || procedureTokens[m]==ewah))
		{
			result&=true;	
		}
		else if(m==3&&(procedureTokens[m]==apriory))
		{
			result&=true;	
		}
		else if(m==4&&(procedureTokens[m]==text))
		{
			result&=true;
		}
		else
		{
			result=false;
		}
	}
	result = m==5&&result;
	return result;
}

bool CIntelliCheckersUIDlg::data_to_filter_to_compression_to_classification_to_textview_procedures(vector<string>& procedureTokens)
{
	bool result=false;
	int m;
	for (m=0;m<procedureTokens.size();m++)
	{
		if(m==0&&(procedureTokens[m]==csv || procedureTokens[m]==xml || procedureTokens[m]==mysql || procedureTokens[m]==mssql))
		{
			result=true;				
		}
		else if(m==1&&(procedureTokens[m]==removeNull || procedureTokens[m]==descritize || procedureTokens[m]==rangesplite))
		{
			result&=true;
		}
		else if(m==2&&(procedureTokens[m]==wah || procedureTokens[m]==ewah))
		{
			result&=true;	
		}
		else if(m==3&&(procedureTokens[m]==classification))
		{
			result&=true;
		}
		else if(m==4&&(procedureTokens[m]==text))
		{
			result&=true;
		}
		else
		{
			result=false;
		}
	}
	result = m==5&&result;
	return result;
}

bool CIntelliCheckersUIDlg::data_to_filter_to_compression_to_naiveBayes_to_textview_procedures(vector<string>& procedureTokens)
{
	bool result=false;
	int m;
	for (m=0;m<procedureTokens.size();m++)
	{
		if(m==0&&(procedureTokens[m]==csv || procedureTokens[m]==xml || procedureTokens[m]==mysql || procedureTokens[m]==mssql))
		{
			result=true;				
		}
		else if(m==1&&(procedureTokens[m]==removeNull || procedureTokens[m]==descritize || procedureTokens[m]==rangesplite))
		{
			result&=true;
		}
		else if(m==2&&(procedureTokens[m]==wah || procedureTokens[m]==ewah))
		{
			result&=true;
		}
		else if(m==3&&(procedureTokens[m]==naiveBayes))
		{
			result&=true;	
		}
		else if(m==4&&(procedureTokens[m]==text))
		{
			result&=true;
		}
		else
		{
			result=false;
		}
	}
	result = m==5&&result;
	return result;
}

bool CIntelliCheckersUIDlg::data_to_filter_to_compression_to_classification_to_treeview_procedures(vector<string>& procedureTokens)
{
	bool result=false;
	int m;
	for (m=0;m<procedureTokens.size();m++)
	{
		if(m==0&&(procedureTokens[m]==csv || procedureTokens[m]==xml || procedureTokens[m]==mysql || procedureTokens[m]==mssql))
		{
			result=true;				
		}
		else if(m==1&&(procedureTokens[m]==removeNull || procedureTokens[m]==descritize || procedureTokens[m]==rangesplite))
		{
			result&=true;
		}
		else if(m==2&&(procedureTokens[m]==wah || procedureTokens[m]==ewah))
		{
			result&=true;
		}
		else if(m==3&&(procedureTokens[m]==classification))
		{
			result&=true;
		}
		else if(m==4&&(procedureTokens[m]==tree))
		{
			result&=true;
		}
		else
		{
			result=false;
		}
	}
	result = m==5&&result;
	return result;
}

/*
*message handler- GUI events hadling is done here
*/

void CIntelliCheckersUIDlg::OnFlexButtonClick(CFlexEvent *evt, CString controller)
{
	try
	{
		static int flowSequenceNumber;
		string procedure=evt->procedure;

		/*
		*genaric procedures
		*/
		string const DATA_FILTER=data+combine+filter;
		string const DATA_ALGORITHM=data+combine+algorithm;	
		string const DATA_COMPRESSION=data+combine+compression;		
		string const DATA_TEXT=data+combine+text;

		string const DATA_FILTER_ALGORITHM=data+combine+filter+combine+algorithm;		
		string const DATA_FILTER_COMPRESSION=data+combine+filter+combine+compression;		
		string const DATA_FILTER_TEXT=data+combine+filter+combine+text;			
		string const DATA_APRIORY_TEXT=data+combine+apriory+combine+text;					
		string const DATA_CLASSIFICATION_TEXT=data+combine+classification+combine+text;
		string const DATA_BAYESIAN_TEXT=data+combine+naiveBayes+combine+text;
		string const DATA_CLASSIFICATION_TREE=data+combine+classification+combine+tree;
		string const DATA_COMPRESSION_ALGORITHM=data+combine+compression+combine+algorithm;		
		string const DATA_COMPRESSION_TEXT=data+combine+compression+combine+text;

		string const DATA_FILTER_APRIORY_TEXT=data+combine+filter+combine+apriory+combine+text;
		string const DATA_FILTER_CLASSIFICATION_TEXT=data+combine+filter+combine+classification+combine+text;
		string const DATA_FILTER_BAYESIAN_TEXT=data+combine+filter+combine+naiveBayes+combine+text;
		string const DATA_FILTER_CLASSIFICATION_TREE=data+combine+filter+combine+classification+combine+tree;
		string const DATA_FILTER_COMPRESSION_TEXT=data+combine+filter+combine+compression+combine+text;		
		string const DATA_COMPRESSION_APRIORY_TEXT=data+combine+compression+combine+apriory+combine+text;		
		string const DATA_COMPRESSION_CLASSIFICATION_TEXT=data+combine+compression+combine+classification+combine+text;
		string const DATA_COMPRESSION_BAYESIAN_TEXT=data+combine+compression+combine+naiveBayes+combine+text;
		string const DATA_COMPRESSION_CLASSIFICATION_TREE=data+combine+compression+combine+classification+combine+tree;

		string const DATA_FILTER_COMPRESSION_APRIORY_TEXT=data+combine+filter+combine+compression+combine+apriory+combine+text;	
		string const DATA_FILTER_COMPRESSION_CLASSIFICATION_TEXT=data+combine+filter+combine+compression+combine+classification+combine+text;	
		string const DATA_FILTER_COMPRESSION_CLASSIFICATION_TREE=data+combine+filter+combine+compression+combine+classification+combine+tree;

		string const DATA_FILTER_COMPRESSION_BAYESIAN_TEXT=data+combine+filter+combine+compression+combine+naiveBayes+combine+text;


		string const csv_text="csv->text";
		string const xml_text="xml->text";
		string const mysql_text="mysql->text";

		string const csv_wah="csv->wah";
		string const xml_wah="xml->wah";
		string const mysql_wah="mysql->wah";
		string const csv_ewah="csv->ewah";
		string const xml_ewah="xml->ewah";
		string const mysql_ewah="mysql->ewah";

		string const csv_wah_text="csv->wah->text";
		string const xml_wah_text="xml->wah->text";
		string const mysql_wah_text="mysql->wah->text";
		string const mssql_wah_text="mssql->wah->text";
		string const csv_ewah_text="csv->ewah->text";
		string const xml_ewah_text="xml->ewah->text";
		string const mysql_ewah_text="mysql->ewah->text";
		string const mssql_ewah_text="mssql->ewah->text";

		string const csv_apriory="csv->apriory";
		string const csv_classification="csv->classification";
		string const xml_apriory="xml->apriory";
		string const xml_classification="xml->classification";	
		string const mysql_apriory="mysql->apriory";
		string const mysql_classification="mysql->classification";

		string const csv_apriory_text="csv->apriory->text";
		string const csv_classification_text="csv->classification->text";
		string const csv_classification_tree="csv->classification->tree";	
		string const xml_apriory_text="xml->apriory->text";
		string const xml_classification_text="xml->classification->text";	
		string const xml_classification_tree="xml->classification->tree";
		string const mysql_apriory_text="mysql->apriory->text";
		string const mysql_classification_text="mysql->classification->text";	
		string const mysql_classification_tree="mysql->classification->tree";
		string const mssql_apriory_text="mssql->apriory->text";
		string const mssql_classification_text="mssql->classification->text";	
		string const mssql_classification_tree="mssql->classification->tree";

		string const csv_wah_apriory_text="csv->wah->apriory->text";
		string const csv_wah_classification_text="csv->wah->classification->text";
		string const csv_wah_classification_tree="csv->wah->classification->tree";	
		string const xml_wah_apriory_text="xml->wah->apriory->text";
		string const xml_wah_classification_text="xml->wah->classification->text";	
		string const xml_wah_classification_tree="xml->wah->classification->tree";
		string const mysql_wah_apriory_text="mysql->wah->apriory->text";
		string const mysql_wah_classification_text="mysql->wah->classification->text";	
		string const mysql_wah_classification_tree="mysql->wah->classification->tree";
		string const mssql_wah_apriory_text="mssql->wah->apriory->text";
		string const mssql_wah_classification_text="mssql->wah->classification->text";	
		string const mssql_wah_classification_tree="mssql->wah->classification->tree";

		string const csv_wah_naiveBayes_text="csv->wah->naiveBayes->text";
		string const csv_ewah_naiveBayes_text="csv->ewah->naiveBayes->text";
		string const xml_wah_naiveBayes_text="xml->wah->naiveBayes->text";
		string const xml_ewah_naiveBayes_text="xml->ewah->naiveBayes->text";
		string const mysql_ewah_naiveBayes_text="mysql->ewah->naiveBayes->text";
		string const mysql_wah_naiveBayes_text="mysql->wah->naiveBayes->text";
		string const mssql_ewah_naiveBayes_text="mssql->ewah->naiveBayes->text";
		string const mssql_wah_naiveBayes_text="mssql->wah->naiveBayes->text";

		string const csv_naiveBayes_text="csv->naiveBayes->text";
		string const xml_naiveBayes_text="xml->naiveBayes->text";
		string const mysql_naiveBayes_text="mysql->naiveBayes->text";
		string const mssql_naiveBayes_text="mysql->naiveBayes->text";

		string const csv_ewah_apriory_text="csv->ewah->apriory->text";
		string const csv_ewah_classification_text="csv->ewah->classification->text";
		string const csv_ewah_classification_tree="csv->ewah->classification->tree";	
		string const xml_ewah_apriory_text="xml->ewah->apriory->text";
		string const xml_ewah_classification_text="xml->ewah->classification->text";	
		string const xml_ewah_classification_tree="xml->ewah->classification->tree";
		string const mysql_ewah_apriory_text="mysql->ewah->apriory->text";
		string const mysql_ewah_classification_text="mysql->ewah->classification->text";	
		string const mysql_ewah_classification_tree="mysql->ewah->classification->tree";
		string const mssql_ewah_apriory_text="mssql->ewah->apriory->text";
		string const mssql_ewah_classification_text="mssql->ewah->classification->text";	
		string const mssql_ewah_classification_tree="mssql->ewah->classification->tree";

		string const csv_wah_apriory="csv->wah->apriory";
		string const csv_wah_classification="csv->wah->classification";
		string const xml_wah_apriory="xml->wah->apriory";
		string const xml_wah_classification="xml->wah->classification";
		string const mysql_wah_apriory="mysql->wah->apriory";
		string const mysql_wah_classification="mysql->wah->classification";

		string const csv_ewah_apriory="csv->ewah->apriory";
		string const csv_ewah_classification="csv->ewah->classification";
		string const xml_ewah_apriory="xml->ewah->apriory";
		string const xml_ewah_classification="xml->ewah->classification";	
		string const mysql_ewah_apriory="mysql->ewah->apriory";
		string const mysql_ewah_classification="mysql->ewah->classification";	

		bool data_to_textview_procedures = procedure==csv_text || procedure==xml_text || procedure==mysql_text;

		bool data_to_algorithm_procedures = procedure==csv_apriory || procedure==csv_classification || procedure==xml_apriory || procedure==xml_classification || procedure==mysql_apriory || procedure==mysql_classification;

		bool data_to_apriory_to_textview_procedures = procedure==csv_apriory_text || procedure==xml_apriory_text || procedure==mysql_apriory_text || procedure==mssql_apriory_text;
		bool data_to_classification_to_textview_procedures = procedure==csv_classification_text || procedure==xml_classification_text || procedure==mysql_classification_text|| procedure==mssql_classification_text;
		bool data_to_naiveBayes_to_textview_procedures = procedure==csv_naiveBayes_text || procedure==xml_naiveBayes_text || procedure==mysql_naiveBayes_text|| procedure==mssql_naiveBayes_text;
		bool data_to_classification_to_treeview_procedures = procedure==csv_classification_tree || procedure==xml_classification_tree || procedure==mysql_classification_tree|| procedure==mssql_classification_tree;
		bool data_to_algorithm_to_textview_procedures = data_to_apriory_to_textview_procedures || data_to_classification_to_textview_procedures;		

		bool data_to_compression_procedures = procedure==csv_wah || procedure==xml_wah || procedure==mysql_wah || procedure==csv_ewah || procedure==xml_ewah || procedure==mysql_ewah;
		bool data_to_comression_to_textview_procedures = procedure==csv_wah_text || procedure==xml_wah_text || procedure==mysql_wah_text || procedure==mssql_wah_text || procedure==csv_ewah_text || procedure==xml_ewah_text || procedure==mysql_ewah_text || procedure==mssql_ewah_text;				

		bool data_to_compression_to_apriory_to_textview_procedures = procedure==csv_wah_apriory_text || procedure==csv_ewah_apriory_text || procedure==xml_wah_apriory_text || procedure==xml_ewah_apriory_text || procedure==mysql_ewah_apriory_text || procedure==mysql_wah_apriory_text || procedure==mssql_ewah_apriory_text || procedure==mssql_wah_apriory_text;
		bool data_to_compression_to_classification_to_textview_procedures = procedure==csv_wah_classification_text || procedure==csv_ewah_classification_text || procedure==xml_wah_classification_text || procedure==xml_ewah_classification_text || procedure==mysql_ewah_classification_text || procedure==mysql_wah_classification_text || procedure==mssql_ewah_classification_text || procedure==mssql_wah_classification_text;
		bool data_to_compression_to_naiveBayes_to_textview_procedures = procedure==csv_wah_naiveBayes_text || procedure==csv_ewah_naiveBayes_text || procedure==xml_wah_naiveBayes_text || procedure==xml_ewah_naiveBayes_text || procedure==mysql_ewah_naiveBayes_text || procedure==mysql_wah_naiveBayes_text || procedure==mssql_ewah_naiveBayes_text || procedure==mssql_wah_naiveBayes_text;
		bool data_to_compression_to_classification_to_treeview_procedures = procedure==csv_wah_classification_tree || procedure==csv_ewah_classification_tree || procedure==csv_ewah_classification_tree || procedure==xml_ewah_classification_tree || procedure==mysql_ewah_classification_tree || procedure==mysql_wah_classification_tree || procedure==mssql_ewah_classification_tree || procedure==mssql_wah_classification_tree;
		bool data_to_compression_to_algorithm_to_view_procedures = data_to_compression_to_apriory_to_textview_procedures || data_to_compression_to_classification_to_textview_procedures || data_to_compression_to_classification_to_treeview_procedures;

		/*
		*data source lists returned
		*/

		if (procedure=="getMySqlDataSourceList")
		{
			DBConnectionInfo::DBConnection nrw_con("");
			vector<string> source_names = nrw_con.getDataSourceNames(DSNInfo::DSNDriverInfo::DATASOURCE_TYPE::MySQL);
			string formattedOutPut="mysqlDataSourcesList##";
	
			for (int i = 0 ; i < source_names.size() ; i++)
			{
				formattedOutPut+="<mysqlsource>";
				formattedOutPut+="<name>" + source_names[i] + "</name>";
				formattedOutPut+="</mysqlsource>";
			}
		flash->root.Call("cplusPluseCallBackFunction", formattedOutPut);
		}
		else if (procedure=="getMsSqlDataSourceList")
		{
			DBConnectionInfo::DBConnection nrw_con("");
			vector<string> source_names = nrw_con.getDataSourceNames(DSNInfo::DSNDriverInfo::DATASOURCE_TYPE::SQL_SERVER);
			string formattedOutPut="mssqlDataSourcesList##";

			for (int i = 0 ; i < source_names.size() ; i++)
			{
				formattedOutPut+="<mssqlsource>";
				formattedOutPut+="<name>" + source_names[i] + "</name>";
				formattedOutPut+="</mssqlsource>";
			}
			flash->root.Call("cplusPluseCallBackFunction", formattedOutPut);
		}
		else
		{	
			/*
			*get procedureTokens and identify the procedure
			*/
			vector<string> procedureTokens;
			Tokenize(procedure, procedureTokens, "->");
			string genericProcedure="";

			if(data_to_apriory_to_textview_procedures)
			{
				genericProcedure=DATA_APRIORY_TEXT;
			}
			else if(data_to_classification_to_textview_procedures)
			{
				genericProcedure=DATA_CLASSIFICATION_TEXT;
			}
			else if(data_to_naiveBayes_to_textview_procedures)
			{
				genericProcedure=DATA_BAYESIAN_TEXT;
			}
			else if(data_to_classification_to_treeview_procedures)
			{
				genericProcedure=DATA_CLASSIFICATION_TREE;
			}
			else if(data_to_comression_to_textview_procedures)
			{
				genericProcedure=DATA_COMPRESSION_TEXT;
			}
			else if(data_to_compression_to_apriory_to_textview_procedures)
			{
				genericProcedure=DATA_COMPRESSION_APRIORY_TEXT;
			}
			else if(data_to_compression_to_classification_to_textview_procedures)
			{
				genericProcedure=DATA_COMPRESSION_CLASSIFICATION_TEXT;
			}
			else if(data_to_compression_to_naiveBayes_to_textview_procedures)
			{
				genericProcedure=DATA_COMPRESSION_BAYESIAN_TEXT;
			}
			else if(data_to_compression_to_classification_to_treeview_procedures)
			{
				genericProcedure=DATA_COMPRESSION_CLASSIFICATION_TREE;
			}
			else if(data_to_filter_to_apriory_to_textview_procedures(procedureTokens))
			{
				genericProcedure=DATA_FILTER_APRIORY_TEXT;
			}
			else if(data_to_filter_to_classification_to_textview_procedures(procedureTokens))
			{
				genericProcedure=DATA_FILTER_CLASSIFICATION_TEXT;
			}
			else if(data_to_filter_to_naiveBayes_to_textview_procedures(procedureTokens))
			{
				genericProcedure=DATA_FILTER_BAYESIAN_TEXT;
			}
			else if(data_to_filter_to_classification_to_treeview_procedures(procedureTokens))
			{
				genericProcedure=DATA_FILTER_CLASSIFICATION_TREE;
			}
			else if(data_to_filter_to_compression_to_apriory_to_textview_procedures(procedureTokens))
			{
				genericProcedure=DATA_FILTER_COMPRESSION_APRIORY_TEXT;
			}
			else if(data_to_filter_to_compression_to_classification_to_textview_procedures(procedureTokens))
			{
				genericProcedure=DATA_FILTER_COMPRESSION_CLASSIFICATION_TEXT;
			}
			else if(data_to_filter_to_compression_to_naiveBayes_to_textview_procedures(procedureTokens))
			{
				genericProcedure=DATA_FILTER_COMPRESSION_BAYESIAN_TEXT;
			}
			else if(data_to_filter_to_compression_to_classification_to_treeview_procedures(procedureTokens))
			{
				genericProcedure=DATA_FILTER_COMPRESSION_CLASSIFICATION_TREE;
			}
			else
			{
				for (int m=0;m<procedureTokens.size();m++)
				{
					if(procedureTokens[m]==csv || procedureTokens[m]==xml || procedureTokens[m]==mysql || procedureTokens[m]==mssql)
					{
						genericProcedure+=data;				
					}
					else if(procedureTokens[m]==removeNull || procedureTokens[m]==descritize || procedureTokens[m]==rangesplite)
					{
						genericProcedure+=filter;				
					}
					else if(procedureTokens[m]==apriory || procedureTokens[m]==classification || procedureTokens[m]==naiveBayes)
					{
						genericProcedure+=algorithm;				
					}
					else if(procedureTokens[m]==wah || procedureTokens[m]==ewah)
					{
						genericProcedure+=compression;				
					}
					else if(procedureTokens[m]==text)
					{
						genericProcedure+=text;				
					}
					else if(procedureTokens[m]==tree)
					{
						genericProcedure+=tree;				
					}

					if(m+1!=procedureTokens.size())
					{
						genericProcedure+=combine;
					}
				}
			}
			/*
			*valide procedure filter
			*/
			bool validProcedure=genericProcedure==DATA_FILTER||genericProcedure==DATA_ALGORITHM||genericProcedure==DATA_COMPRESSION||genericProcedure==DATA_TEXT||genericProcedure==DATA_FILTER_ALGORITHM
				||genericProcedure==DATA_FILTER_COMPRESSION||genericProcedure==DATA_FILTER_TEXT||genericProcedure==DATA_APRIORY_TEXT||genericProcedure==DATA_CLASSIFICATION_TEXT||genericProcedure==DATA_CLASSIFICATION_TREE||genericProcedure==DATA_COMPRESSION_ALGORITHM
				||genericProcedure==DATA_BAYESIAN_TEXT||genericProcedure==DATA_COMPRESSION_BAYESIAN_TEXT||genericProcedure==DATA_FILTER_BAYESIAN_TEXT||genericProcedure==DATA_FILTER_COMPRESSION_BAYESIAN_TEXT
				||genericProcedure==DATA_COMPRESSION_TEXT||genericProcedure==DATA_FILTER_APRIORY_TEXT||genericProcedure==DATA_FILTER_CLASSIFICATION_TEXT||genericProcedure==DATA_FILTER_CLASSIFICATION_TREE||genericProcedure==DATA_FILTER_COMPRESSION_TEXT||genericProcedure==DATA_COMPRESSION_APRIORY_TEXT||genericProcedure==DATA_COMPRESSION_CLASSIFICATION_TEXT||genericProcedure==DATA_COMPRESSION_CLASSIFICATION_TREE
				||genericProcedure==DATA_FILTER_COMPRESSION_APRIORY_TEXT||genericProcedure==DATA_FILTER_COMPRESSION_CLASSIFICATION_TEXT||genericProcedure==DATA_FILTER_COMPRESSION_CLASSIFICATION_TREE;

			if(validProcedure)
			{
				string measureTime=evt->measureTime;
				string loopInformation=evt->runInALoop;
				int loopCount = 1;
				int increment = 0;
				string runInALoop = "";		


				string graph_procedure="";

				int k=0;
				for (;k<procedureTokens.size()-1;k++)
				{
					graph_procedure+=procedureTokens[k]+"_";
				}
				stringstream flowSequenceSS;
				flowSequenceSS<<flowSequenceNumber;
				graph_procedure+=procedureTokens[k]+"_"+flowSequenceSS.str();
				flowSequenceNumber++;

				string viewer=procedureTokens[procedureTokens.size()-1];

				vector<string> loopTokens;

				Tokenize(loopInformation, loopTokens, "@@");
				runInALoop=loopTokens[0];
				loopCount=atoi( loopTokens[1].c_str());
				increment=atoi( loopTokens[2].c_str());


				//time_t start,end;
				//time_t startGraphTime,endGraphTime;
				/*
				*valide procedure iteration
				*/
				clock_t start,end;
				clock_t startGraphTime,endGraphTime;
				string timeUnit=" s";

				string paths=evt->procedurePara;
				vector<string> pathsTokens;
				Tokenize(paths, pathsTokens, "@@");

				string algoPara=evt->algoPara;
				vector<string> algoParaTokens;
				Tokenize(algoPara, algoParaTokens, "@@");

				/*
				*declare the auxiliary parameters
				*/
				string csv_data_location;
				int csv_data_size;
				string xml_metadata_location;
				string xml_data_location;
				string xml_data_source;
				int xml_data_size;
				string mysql_source_name;
				string mysql_query;
				string mysql_data_size;
				string mssql_source_name;
				string mssql_query;
				string mssql_data_size;
				string support;
				string confidence;
				string number_of_rules;

				string formattedOutPut="";
				string graphData="";

				/*
				*output identification is done here
				*/

				if(runInALoop=="false")
				{
					if(viewer=="text")
					{
						formattedOutPut="textViewer##";
					}
					else if(viewer=="tree")
					{
						formattedOutPut="treeViewer##";
					}
					else
					{
						formattedOutPut="nullView##";
					}
				}
				else {
					formattedOutPut="graph##<items>";
				}
				string timeStamps="$$"+procedure;

				for(int i=0;i<loopCount;i++)
				{
					stringstream tempstream;
					graphData+="<item datasize=\"";
					tempstream<<increment*(i+1);
					graphData+=tempstream.str()+"\" "+graph_procedure+"=\"";

					

					for(int j=0;j<procedureTokens.size();j++)
					{
						if(procedureTokens[j]!=text && procedureTokens[j]!=tree)
						{
							//time (&start);
							start=clock();

							if (procedureTokens[j]==csv)
							{
								csv_data_location=pathsTokens[0];
								csv_data_size=atoi( pathsTokens[1].c_str());
								if(loopCount==1)
								{
									CSV(csv_data_location,csv_data_size);
								}
								else
								{
									CSV(csv_data_location,increment*(i+1));
								}
							}
							else if (procedureTokens[j]==mysql)
							{
								mysql_source_name=pathsTokens[0];
								mysql_query=pathsTokens[1];
								mysql_data_size=pathsTokens[2];
								if(loopCount==1)
								{
									MYSQL(mysql_source_name,mysql_query+" LIMIT "+mysql_data_size);
								}
								else
								{
									int rowcount=atoi( mysql_data_size.c_str())*(i+1);
									stringstream rowcountstream;							
									rowcountstream<<rowcount;
									MYSQL(mysql_source_name,mysql_query+" LIMIT "+rowcountstream.str());
								}
							}
							else if (procedureTokens[j]==xml)
							{
								xml_metadata_location=pathsTokens[0];
								xml_data_location=pathsTokens[1];
								xml_data_source=pathsTokens[2];
								xml_data_size=atoi( pathsTokens[3].c_str());
								if(loopCount==1)
								{
									SavedDataLoader(xml_metadata_location,xml_data_location,xml_data_source,xml_data_size);
								}
								else
								{
									SavedDataLoader(xml_metadata_location,xml_data_location,xml_data_source,increment*(i+1));
								}
							}						
							else if (procedureTokens[j]==mssql)
							{
								mssql_source_name=pathsTokens[0];
								mssql_query=pathsTokens[1];
								mssql_data_size=pathsTokens[2];
								if(loopCount==1)
								{
									MSSQL(mssql_source_name,mssql_query/*+" LIMIT "+mssql_data_size*/);
								}
								else
								{
									int rowcount=atoi( mssql_data_size.c_str())*(i+1);
									stringstream rowcountstream;							
									rowcountstream<<rowcount;
									MSSQL(mssql_source_name,mssql_query/*+" LIMIT "+rowcountstream.str()*/);
								}
							}
							else if (procedureTokens[j]==removeNull)
							{
								NullEliminator();
							}
							else if (procedureTokens[j]==descritize)
							{
								DiscretizeDataSource();
							}
							else if (procedureTokens[j]==rangesplite)
							{
								SplitteRanges(10);//TODO get from UI
							}
							else if (procedureTokens[j]==wah)
							{
								if(runInALoop=="false")
								{
									formattedOutPut += "^&&"+MeasureSpace()+"^^";
								}					
								Convert(BitStreamInfo::WAH_COMPRESSION);
								if(runInALoop=="false")
								{
									formattedOutPut += MeasureSpace()+"^&&";
								}						
							}
							else if (procedureTokens[j]==ewah && runInALoop=="false")
							{
								if(runInALoop=="false")
								{
									formattedOutPut += "^&&"+MeasureSpace()+"^^";
								}						
								Convert(BitStreamInfo::EWAH_COMPRESSION);
								if(runInALoop=="false")
								{
									formattedOutPut += MeasureSpace()+"^&&";
								}						
							}
							//time (&startGraphTime);
							startGraphTime=clock();
							if (procedureTokens[j]==apriory)
							{
								support=algoParaTokens[0];
								confidence=algoParaTokens[1];
								number_of_rules=algoParaTokens[2];
								Aprior(atof(confidence.c_str()),atof(support.c_str()),atof(number_of_rules.c_str()));
							}
							else if (procedureTokens[j]==classification)
							{
								Classifier();
							}
							else if (procedureTokens[j]==naiveBayes)
							{
								Bayesian(m_source->codedAttributes().size()-1);//TODO get this value from gui???
							}
							//time (&endGraphTime);
							endGraphTime=clock();

							//time (&end);
							end=clock();

							//stringstream timeStream;
							//timeStream << difftime (end,start);	
							string tempstring=boost::lexical_cast<string>(end - start);
							timeStamps+="@@";
							//timeStamps+=timeStream.str()+timeUnit;
							timeStamps+=tempstring+"ms";

						}				
					}

					

					//stringstream timeStreamGraph;
					//timeStreamGraph << difftime (endGraphTime,startGraphTime);
					string tempstring1=boost::lexical_cast<string>(endGraphTime - startGraphTime);
					//graphData+=timeStreamGraph.str()+"\"/>";
					graphData+=tempstring1+"\"/>";

					if(runInALoop=="false")
					{
						if(genericProcedure==DATA_TEXT || data_to_comression_to_textview_procedures || genericProcedure==DATA_FILTER_COMPRESSION_TEXT || genericProcedure==DATA_FILTER_TEXT)//
						{
							formattedOutPut += Text(WRAPPED_SOURCE,100);
						}
						else if(genericProcedure==DATA_FILTER_COMPRESSION_APRIORY_TEXT || data_to_apriory_to_textview_procedures || data_to_compression_to_apriory_to_textview_procedures || genericProcedure==DATA_FILTER_APRIORY_TEXT)//
						{
							formattedOutPut += Text(APRIORI_SOURCE,0);
						}
						else if(genericProcedure==DATA_FILTER_COMPRESSION_CLASSIFICATION_TEXT || data_to_classification_to_textview_procedures || data_to_compression_to_classification_to_textview_procedures || genericProcedure==DATA_FILTER_CLASSIFICATION_TEXT)//
						{
							formattedOutPut += Text(CLASSIFIER_TEXT_SOURCE,0);
						}
						else if(genericProcedure==DATA_FILTER_COMPRESSION_BAYESIAN_TEXT || data_to_naiveBayes_to_textview_procedures || data_to_compression_to_naiveBayes_to_textview_procedures || genericProcedure==DATA_FILTER_BAYESIAN_TEXT)//
						{
							formattedOutPut += Text(BAYESIAN_SOURCE,0);
						}
						else if(genericProcedure==DATA_FILTER_COMPRESSION_CLASSIFICATION_TREE || data_to_classification_to_treeview_procedures || data_to_compression_to_classification_to_treeview_procedures || genericProcedure==DATA_FILTER_CLASSIFICATION_TREE)//
						{
							formattedOutPut += Text(CLASSIFIER_SOURCE,0);
						}
						else if(genericProcedure==DATA_COMPRESSION || genericProcedure==DATA_ALGORITHM || genericProcedure==DATA_COMPRESSION_ALGORITHM || genericProcedure==DATA_FILTER || genericProcedure==DATA_FILTER_ALGORITHM || genericProcedure==DATA_FILTER_COMPRESSION)//
						{
							//keep empty
						}
					}
					DeleteAll();
				}
				if(runInALoop=="true")
				{
					formattedOutPut+=graphData+"</items>";
				}
				if(measureTime=="true")
				{
					formattedOutPut+=timeStamps;
				}		
				flash->root.Call("cplusPluseCallBackFunction", formattedOutPut);
			}
			else
			{
				string formattedOutPut="errorProcedure##";
				flash->root.Call("cplusPluseCallBackFunction", formattedOutPut);
			}
		}
	}
	catch(compression_exception & e)
	{
		string s = e.toString();	
		string formattedOutPut="exceptionThrown##"+s;
		flash->root.Call("cplusPluseCallBackFunction", formattedOutPut);
	}
	catch(algorithm_exception & e)
	{
		string s = e.toString();	
		string formattedOutPut="exceptionThrown##"+s;
		flash->root.Call("cplusPluseCallBackFunction", formattedOutPut);
	}
	catch(base_exception &e)
	{
		string s = boost::diagnostic_information(e);	
		string formattedOutPut="exceptionThrown##"+s;
		flash->root.Call("cplusPluseCallBackFunction", formattedOutPut);
	}
	catch(boost::exception &e)
	{
		string s = boost::diagnostic_information(e);	
		string formattedOutPut="exceptionThrown##"+s;
		flash->root.Call("cplusPluseCallBackFunction", formattedOutPut);
	}
	catch(...)
	{
		string s = "Unknown error!!";	
		string formattedOutPut="exceptionThrown##"+s;
		flash->root.Call("cplusPluseCallBackFunction", formattedOutPut);
	}
}

void CIntelliCheckersUIDlg::Tokenize(const string& str,vector<string>& tokens,const string& delimiters)
{
	// Skip delimiters at beginning.
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	string::size_type pos     = str.find_first_of(delimiters, lastPos);

	while (string::npos != pos || string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}
