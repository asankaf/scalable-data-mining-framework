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
			//flash->AddEventListener("btn1", "mouseDown");
			//flash->CallFunction("Grrr", "Modaya");
			//flash->root.Call("Grrr", "Modayaaaaaaaaaaaaaaaaaaaaaaaaaaa");
			//flash->root.Call()
	
			
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

void CIntelliCheckersUIDlg::Bayesian( int _class )
{
m_bayesian = new NaiveBayes();
m_bayesian->buildClassifier(m_source,_class);
}

void CIntelliCheckersUIDlg::CSV(string path,int noOfRows)
{
	CsvConnection cConcsv(path.data(),',','\n','""');
	ExtractedCsvDTO *dat = cConcsv.extractData();
	m_source = new WrapDataSource(dat,"0");	
	m_source->encodeAtrributes();
}

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

void CIntelliCheckersUIDlg::NullEliminator()
{
	NullPreProcessor eliminator(m_source);
	eliminator.elimiateNullValues();
	m_source = eliminator.NullEliminatedDatasource();
}

void CIntelliCheckersUIDlg::Convert(BitStreamInfo::vertical_bit_type _type)
{
	CompressionHandler::ConvertTo(m_source,_type);
}

//Gives the Space in bytes
string CIntelliCheckersUIDlg::MeasureSpace()
{
	return Utils::toStringVal((int)m_source->SpaceUtilsation());
}

void CIntelliCheckersUIDlg::Classifier()
{
	m_classifier = new C45TreeNominal();
	m_classifier->buildClassifier(m_source);
}
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

void CIntelliCheckersUIDlg::OnFlexButtonClick(CFlexEvent *evt, CString controller)
{
	static int flowSequenceNumber;
	string procedure=evt->procedure;

	//combine
	string const combine="->";
	//data
	string const data="data";
	string const csv="csv";
	string const xml="xml";
	string const mysql="mysql";
	string const mssql="mssql";
	
	//filter
	string const filter="filter";
	string const removeNull="removeNull";
	string const descritize="descritize";
	string const rangesplite="rangesplite";
	
	//algorithm
	string const algorithm="algorithm";
	string const apriory="apriory";
	string const classification="classification";	
	string const naiveBayes="naiveBayes";	
	
	//compression
	string const compression="compression";
	string const wah="wah";
	string const ewah="ewah";
	
	//view
	string const view="view";
	string const text="text";
	string const tree="tree";

	//genaric procedures
	string const DATA_FILTER=data+combine+filter;
	string const DATA_ALGORITHM=data+combine+algorithm;	
	string const DATA_COMPRESSION=data+combine+compression;	
	string const DATA_VIEW=data+combine+view;			

	string const DATA_FILTER_ALGORITHM=data+combine+filter+combine+algorithm;		
	string const DATA_FILTER_COMPRESSION=data+combine+filter+combine+compression;		
	string const DATA_FILTER_VIEW=data+combine+filter+combine+view;		
	string const DATA_ALGORITHM_VIEW=data+combine+algorithm+combine+view;					
	string const DATA_COMPRESSION_ALGORITHM=data+combine+compression+combine+algorithm;		
	string const DATA_COMPRESSION_VIEW=data+combine+compression+combine+view;

	string const DATA_FILTER_ALGORITHM_VIEW=data+combine+filter+combine+algorithm+combine+view;
	string const DATA_FILTER_COMPRESSION_VIEW=data+combine+filter+combine+compression+combine+view;		
	string const DATA_COMPRESSION_ALGORITHM_VIEW=data+combine+compression+combine+algorithm+combine+view;		

	string const DATA_FILTER_COMPRESSION_ALGORITHM_VIEW=data+combine+filter+combine+compression+combine+algorithm+combine+view;


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
	string const csv_ewah_text="csv->ewah->text";
	string const xml_ewah_text="xml->ewah->text";
	string const mysql_ewah_text="mysql->ewah->text";

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

	string const csv_wah_apriory_text="csv->wah->apriory->text";
	string const csv_wah_classification_text="csv->wah->classification->text";
	string const csv_wah_classification_tree="csv->wah->classification->tree";	
	string const xml_wah_apriory_text="xml->wah->apriory->text";
	string const xml_wah_classification_text="xml->wah->classification->text";	
	string const xml_wah_classification_tree="xml->wah->classification->tree";
	string const mysql_wah_apriory_text="mysql->wah->apriory->text";
	string const mysql_wah_classification_text="mysql->wah->classification->text";	
	string const mysql_wah_classification_tree="mysql->wah->classification->tree";

	string const csv_ewah_apriory_text="csv->ewah->apriory->text";
	string const csv_ewah_classification_text="csv->ewah->classification->text";
	string const csv_ewah_classification_tree="csv->ewah->classification->tree";	
	string const xml_ewah_apriory_text="xml->ewah->apriory->text";
	string const xml_ewah_classification_text="xml->ewah->classification->text";	
	string const xml_ewah_classification_tree="xml->ewah->classification->tree";
	string const mysql_ewah_apriory_text="mysql->ewah->apriory->text";
	string const mysql_ewah_classification_text="mysql->ewah->classification->text";	
	string const mysql_ewah_classification_tree="mysql->ewah->classification->tree";

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

	bool data_to_apriory_to_textview_procedures = procedure==csv_apriory_text || procedure==xml_apriory_text || procedure==mysql_apriory_text;
	bool data_to_classification_to_textview_procedures = procedure==csv_classification_text || procedure==xml_classification_text || procedure==mysql_classification_text;
	bool data_to_classification_to_treeview_procedures = procedure==csv_classification_tree || procedure==xml_classification_tree || procedure==mysql_classification_tree;
	bool data_to_algorithm_to_textview_procedures = data_to_apriory_to_textview_procedures || data_to_classification_to_textview_procedures;		

	bool data_to_compression_procedures = procedure==csv_wah || procedure==xml_wah || procedure==mysql_wah || procedure==csv_ewah || procedure==xml_ewah || procedure==mysql_ewah;
	bool data_to_comression_to_textview_procedures = procedure==csv_wah_text || procedure==xml_wah_text || procedure==mysql_wah_text || procedure==csv_ewah_text || procedure==xml_ewah_text || procedure==mysql_ewah_text;				

	bool data_to_compression_to_apriory_to_textview_procedures = procedure==csv_wah_apriory_text || procedure==csv_ewah_apriory_text || procedure==xml_wah_apriory_text || procedure==xml_ewah_apriory_text || procedure==mysql_ewah_apriory_text || procedure==mysql_wah_apriory_text;
	bool data_to_compression_to_classification_to_textview_procedures = procedure==csv_wah_classification_text || procedure==csv_ewah_classification_text || procedure==xml_wah_classification_text || procedure==xml_ewah_classification_text || procedure==mysql_ewah_classification_text || procedure==mysql_wah_classification_text;
	bool data_to_compression_to_classification_to_treeview_procedures = procedure==csv_wah_classification_tree || procedure==csv_ewah_classification_tree || procedure==csv_ewah_classification_tree || procedure==xml_ewah_classification_tree || procedure==mysql_ewah_classification_tree || procedure==mysql_wah_classification_tree;
	bool data_to_compression_to_algorithm_to_view_procedures = data_to_compression_to_apriory_to_textview_procedures || data_to_compression_to_classification_to_textview_procedures || data_to_compression_to_classification_to_treeview_procedures;

	bool data_to_compression_to_apriory_procedures = procedure==csv_wah_apriory || procedure==csv_ewah_apriory || procedure==xml_wah_apriory || procedure==xml_ewah_apriory || procedure==mysql_wah_apriory || procedure==mysql_ewah_apriory;
	bool data_to_compression_to_classification_procedures = procedure==csv_wah_classification || procedure==csv_ewah_classification || procedure==xml_wah_classification || procedure==xml_ewah_classification || procedure==mysql_wah_classification || procedure==mysql_ewah_classification;
	bool data_to_compression_to_algorithm_procedures = data_to_compression_to_apriory_procedures || data_to_compression_to_classification_procedures;


	if (procedure=="getMySqlDataSourceList")
	{
		DBConnectionInfo::DBConnection nrw_con("");
		vector<string> source_names = nrw_con.getDataSourceNames(DSNInfo::DSNDriverInfo::DATASOURCE_TYPE::MySQL);
		string formattedOutPut="mysqlDataSourcesList##";
	    //get the real mysql data sources list as below
		
		for (int i = 0 ; i < source_names.size() ; i++)
		{
			formattedOutPut+="<mysqlsource>";
			formattedOutPut+="<name>" + source_names[i] + "</name>";
			formattedOutPut+="</mysqlsource>";
		}
		
		//formattedOutPut+="<mysqlsource><name>source1</name></mysqlsource><mysqlsource><name>source2</name></mysqlsource>";
		flash->root.Call("cplusPluseCallBackFunction", formattedOutPut);

	}
	else if (procedure=="getMsSqlDataSourceList")
	{
		DBConnectionInfo::DBConnection nrw_con("");
		vector<string> source_names = nrw_con.getDataSourceNames(DSNInfo::DSNDriverInfo::DATASOURCE_TYPE::SQL_SERVER);
		string formattedOutPut="mssqlDataSourcesList##";
	    //get the real mysql data sources list as below
		
		for (int i = 0 ; i < source_names.size() ; i++)
		{
			formattedOutPut+="<mssqlsource>";
			formattedOutPut+="<name>" + source_names[i] + "</name>";
			formattedOutPut+="</mssqlsource>";
		}
		//string formattedOutPut="mssqlDataSourcesList##<mssqlsource><name>Mssource1</name></mssqlsource><mssqlsource><name>Mssource2</name></mssqlsource>";
		flash->root.Call("cplusPluseCallBackFunction", formattedOutPut);

	}
	else
	{	
		vector<string> procedureTokens;
		Tokenize(procedure, procedureTokens, "->");
		string genericProcedure="";

		if(false)
		{
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
				else if(procedureTokens[m]==text || procedureTokens[m]==tree)
				{
					genericProcedure+=view;				
				}

				if(m+1!=procedureTokens.size())
				{
					genericProcedure+=combine;
				}
			}
		}

		bool validProcedure=genericProcedure==DATA_FILTER||genericProcedure==DATA_ALGORITHM||genericProcedure==DATA_COMPRESSION||genericProcedure==DATA_VIEW||genericProcedure==DATA_FILTER_ALGORITHM
						||genericProcedure==DATA_FILTER_COMPRESSION||genericProcedure==DATA_FILTER_VIEW||genericProcedure==DATA_ALGORITHM_VIEW||genericProcedure==DATA_COMPRESSION_ALGORITHM
						||genericProcedure==DATA_COMPRESSION_VIEW||genericProcedure==DATA_FILTER_ALGORITHM_VIEW||genericProcedure==DATA_FILTER_COMPRESSION_VIEW||genericProcedure==DATA_COMPRESSION_ALGORITHM_VIEW
						||genericProcedure==DATA_FILTER_COMPRESSION_ALGORITHM_VIEW;

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
			
			
			time_t start,end;
			time_t startGraphTime,endGraphTime;
			string timeUnit=" s";

			string paths=evt->procedurePara;
			vector<string> pathsTokens;
			Tokenize(paths, pathsTokens, "@@");

			string csv_data_location;
			int csv_data_size;
			string xml_metadata_location;
			string xml_data_location;
			string xml_data_source;
			int xml_data_size;
			string mysql_source_name;
			string mysql_query;
			string mysql_data_size;

			string formattedOutPut="";
			string graphData="";
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

				time (&startGraphTime);

				for(int j=0;j<procedureTokens.size();j++)
				{
					if(procedureTokens[j]!=text && procedureTokens[j]!=tree)
					{
						time (&start);

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
						else if (procedureTokens[j]==apriory)
						{
							Aprior(0.9,0.01,10);
						}
						else if (procedureTokens[j]==classification)
						{
							Classifier();
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
						else if (procedureTokens[j]==classification)
						{
							
						}

						time (&end);

						stringstream timeStream;
						timeStream << difftime (end,start);	
						timeStamps+="@@";
						timeStamps+=timeStream.str()+timeUnit;

					}				
				}
				
				time (&endGraphTime);

				stringstream timeStreamGraph;
				timeStreamGraph << difftime (endGraphTime,startGraphTime);
				graphData+=timeStreamGraph.str()+"\"/>";

				if(runInALoop=="false")
				{
					if(data_to_textview_procedures)
					{
						formattedOutPut += Text(WRAPPED_SOURCE,100);
					}
					else if(data_to_apriory_to_textview_procedures)
					{
						formattedOutPut += Text(APRIORI_SOURCE,0);
					}
					else if(data_to_classification_to_textview_procedures)
					{
						formattedOutPut += Text(CLASSIFIER_TEXT_SOURCE,0);
					}
					else if(data_to_classification_to_treeview_procedures)
					{
						formattedOutPut += Text(CLASSIFIER_SOURCE,0);
					}
					else if(data_to_compression_procedures)
					{
						//keep empty
					}
					else if(data_to_comression_to_textview_procedures)
					{
						//formattedOutPut += Text(WRAPPED_SOURCE,100);
					}
					else if(data_to_algorithm_procedures)
					{
						//keep empty
					}
					else if(data_to_compression_to_apriory_to_textview_procedures)
					{
						formattedOutPut += Text(APRIORI_SOURCE,0);
					}
					else if(data_to_compression_to_classification_to_textview_procedures)
					{
						formattedOutPut += Text(CLASSIFIER_TEXT_SOURCE,0);
					}
					else if(data_to_compression_to_classification_to_treeview_procedures)
					{
						formattedOutPut += Text(CLASSIFIER_SOURCE,0);
					}
					else if(data_to_compression_to_apriory_procedures)
					{
						//keep empty
					}
					else if(data_to_compression_to_classification_procedures)
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
			string formattedOutPut="noView##";
			flash->root.Call("cplusPluseCallBackFunction", formattedOutPut);
		}
	}
	/*else
	{
		
		string measureTime=evt->measureTime;
		string loopInformation=evt->runInALoop;
		int loopCount = 1;
		int increment = 0;
		string runInALoop = "";		

		vector<string> procedureTokens;
		string graph_procedure="";

		Tokenize(procedure, procedureTokens, "->");
		int k=0;
		for (;k<procedureTokens.size()-1;k++)
		{
			graph_procedure+=procedureTokens[k]+"_";
		}
		stringstream flowSequenceSS;
		flowSequenceSS<<flowSequenceNumber;
		graph_procedure+=procedureTokens[k]+"_"+flowSequenceSS.str();
		flowSequenceNumber++;

		vector<string> loopTokens;
		
		Tokenize(loopInformation, loopTokens, "@@");
		runInALoop=loopTokens[0];
		loopCount=atoi( loopTokens[1].c_str());
		increment=atoi( loopTokens[2].c_str());
		
		
		time_t start,end;
		time_t startGraphTime,endGraphTime;
		string timeUnit=" s";

		if (procedure=="csv->text")
		{
			string path=evt->procedurePara;

			vector<string> pathsTokens;		
			Tokenize(path, pathsTokens, "@@");
			string csv_data_location=pathsTokens[0];
			int csv_data_size=atoi( pathsTokens[1].c_str());

			string formattedOutPut="";
			string graphData="";
			if(runInALoop=="false")
			{
				formattedOutPut="textViewer##";
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

				time (&startGraphTime);

				time (&start);
				if(loopCount==1)
				{
					CSV(csv_data_location,csv_data_size);
				}
				else
				{
					CSV(csv_data_location,increment*(i+1));
				}	
				time (&end);

				stringstream timeStream;
				timeStream << difftime (end,start);	
				timeStamps+="@@";
				timeStamps+=timeStream.str()+timeUnit;

				time (&endGraphTime);

				stringstream timeStreamGraph;
				timeStreamGraph << difftime (endGraphTime,startGraphTime);
				graphData+=timeStreamGraph.str()+"\"/>";

				if(runInALoop=="false")
				{
					formattedOutPut += Text(WRAPPED_SOURCE,100);
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
		else if (procedure=="csv->apriory->text")
		{

			string path=evt->procedurePara;

			vector<string> pathsTokens;		
			Tokenize(path, pathsTokens, "@@");
			string csv_data_location=pathsTokens[0];
			int csv_data_size=atoi( pathsTokens[1].c_str());

			string formattedOutPut="";
			string graphData="";
			if(runInALoop=="false")
			{
				formattedOutPut="textViewer##";
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

				time (&startGraphTime);

				time (&start);
				if(loopCount==1)
				{
					CSV(csv_data_location,csv_data_size);
				}
				else
				{
					CSV(csv_data_location,increment*(i+1));
				}	
				time (&end);
	
				stringstream timeStream;
				timeStream << difftime (end,start);	
				timeStamps+="@@";
				timeStamps+=timeStream.str()+timeUnit;
					
				time (&start);
				Aprior(0.9,0.01,10);
				time (&end);
	
				stringstream timeStream_2;
				timeStream_2 << difftime (end,start);	
				timeStamps+="@@";
				timeStamps+=timeStream_2.str()+timeUnit;

				time (&endGraphTime);

				stringstream timeStreamGraph;
				timeStreamGraph << difftime (endGraphTime,startGraphTime);
				graphData+=timeStreamGraph.str()+"\"/>";
	
				if(runInALoop=="false")
				{
					formattedOutPut += Text(APRIORI_SOURCE,0);
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
		else if (procedure == "csv->classification->tree")
		{
			string path=evt->procedurePara;

			vector<string> pathsTokens;		
			Tokenize(path, pathsTokens, "@@");
			string csv_data_location=pathsTokens[0];
			int csv_data_size=atoi( pathsTokens[1].c_str());

			string formattedOutPut="";
			string graphData="";
			if(runInALoop=="false")
			{
				formattedOutPut="treeViewer##";
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

				time (&startGraphTime);

				time (&start);
				if(loopCount==1)
				{
					CSV(csv_data_location,csv_data_size);
				}
				else
				{
					CSV(csv_data_location,increment*(i+1));
				}	
				time (&end);
	
				stringstream timeStream;
				timeStream << difftime (end,start);	
				timeStamps+="@@";
				timeStamps+=timeStream.str()+timeUnit;
					
				time (&start);
				Classifier();
				time (&end);
	
				stringstream timeStream_2;
				timeStream_2 << difftime (end,start);	
				timeStamps+="@@";
				timeStamps+=timeStream_2.str()+timeUnit;

				time (&endGraphTime);

				stringstream timeStreamGraph;
				timeStreamGraph << difftime (endGraphTime,startGraphTime);
				graphData+=timeStreamGraph.str()+"\"/>";
	
				if(runInALoop=="false")
				{
					formattedOutPut += Text(CLASSIFIER_SOURCE,0);
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
		else if (procedure=="xml->text")
		{
			string paths=evt->procedurePara;

			vector<string> pathsTokens;		
			Tokenize(paths, pathsTokens, "@@");
			string xml_metadata_location=pathsTokens[0];
			string xml_data_location=pathsTokens[1];
			int xml_data_size=atoi( pathsTokens[2].c_str());

			string formattedOutPut="";
			string graphData="";
			if(runInALoop=="false")
			{
				formattedOutPut="textViewer##";
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

				time (&startGraphTime);

				time (&start);
				if(loopCount==1)
				{
					SavedDataLoader(xml_metadata_location,xml_data_location,"poker_hand",xml_data_size);
				}
				else
				{
					SavedDataLoader(xml_metadata_location,xml_data_location,"poker_hand",increment*(i+1));
				}				
				time (&end);

				stringstream timeStream;
				timeStream << difftime (end,start);	
				timeStamps+="@@";
				timeStamps+=timeStream.str()+timeUnit;

				time (&endGraphTime);

				stringstream timeStreamGraph;
				timeStreamGraph << difftime (endGraphTime,startGraphTime);
				graphData+=timeStreamGraph.str()+"\"/>";

				if(runInALoop=="false")
				{
					formattedOutPut += Text(WRAPPED_SOURCE,100);
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
		else if (procedure=="xml->apriory->text")
		{
			string paths=evt->procedurePara;

			vector<string> pathsTokens;		
			Tokenize(paths, pathsTokens, "@@");
			string xml_metadata_location=pathsTokens[0];
			string xml_data_location=pathsTokens[1];
			int xml_data_size=atoi( pathsTokens[2].c_str());

			string formattedOutPut="";
			string graphData="";
			if(runInALoop=="false")
			{
				formattedOutPut="textViewer##";
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

				time (&startGraphTime);

				time (&start);
				if(loopCount==1)
				{
					SavedDataLoader(xml_metadata_location,xml_data_location,"poker_hand",xml_data_size);
				}
				else
				{
					SavedDataLoader(xml_metadata_location,xml_data_location,"poker_hand",increment*(i+1));
				}				
				time (&end);

				stringstream timeStream;
				timeStream << difftime (end,start);	
				timeStamps+="@@";
				timeStamps+=timeStream.str()+timeUnit;

				time (&start);
				Aprior(0.9,0.01,10);
				time (&end);
	
				stringstream timeStream_2;
				timeStream_2 << difftime (end,start);	
				timeStamps+="@@";
				timeStamps+=timeStream_2.str()+timeUnit;

				time (&endGraphTime);

				stringstream timeStreamGraph;
				timeStreamGraph << difftime (endGraphTime,startGraphTime);
				graphData+=timeStreamGraph.str()+"\"/>";

				if(runInALoop=="false")
				{
					formattedOutPut += Text(APRIORI_SOURCE,0);
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
		else if (procedure=="xml->classification->tree")
		{
			string paths=evt->procedurePara;

			vector<string> pathsTokens;		
			Tokenize(paths, pathsTokens, "@@");
			string xml_metadata_location=pathsTokens[0];
			string xml_data_location=pathsTokens[1];
			int xml_data_size=atoi( pathsTokens[2].c_str());

			string formattedOutPut="";
			string graphData="";
			if(runInALoop=="false")
			{
				formattedOutPut="treeViewer##";
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

				time (&startGraphTime);

				time (&start);
				if(loopCount==1)
				{
					SavedDataLoader(xml_metadata_location,xml_data_location,"soya_5col_",xml_data_size);
				}
				else
				{
					SavedDataLoader(xml_metadata_location,xml_data_location,"soya_5col_",increment*(i+1));
				}				
				time (&end);

				stringstream timeStream;
				timeStream << difftime (end,start);	
				timeStamps+="@@";
				timeStamps+=timeStream.str()+timeUnit;

				time (&start);
				Classifier();
				time (&end);
	
				stringstream timeStream_2;
				timeStream_2 << difftime (end,start);	
				timeStamps+="@@";
				timeStamps+=timeStream_2.str()+timeUnit;

				time (&endGraphTime);

				stringstream timeStreamGraph;
				timeStreamGraph << difftime (endGraphTime,startGraphTime);
				graphData+=timeStreamGraph.str()+"\"/>";

				if(runInALoop=="false")
				{
					formattedOutPut += Text(CLASSIFIER_SOURCE,0);
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
		else if (procedure == "csv->classification->text")
		{
			
			string path=evt->procedurePara;
			string formattedOutPut="textViewer##";
			string timeStamps="$$"+procedure;

			time (&start);
			CSV(path,1000);
			time (&end);

			stringstream timeStream;
			timeStream << difftime (end,start);	
			timeStamps+="@@";
			timeStamps+=timeStream.str()+timeUnit;

			time (&start);
			Classifier();
			time (&end);

			stringstream timeStream_2;
			timeStream_2 << difftime (end,start);	
			timeStamps+="@@";
			timeStamps+=timeStream_2.str()+timeUnit;

			formattedOutPut += Text(CLASSIFIER_TEXT_SOURCE,0);

			if(measureTime=="true")
			{
				formattedOutPut+=timeStamps;
			}

			DeleteAll();
			flash->root.Call("cplusPluseCallBackFunction",formattedOutPut);			
		}
		else if (procedure == "mysql->text")
		{

			string formattedOutPut="textViewer##";
			// Suppose that the string is csv->SpaceMeasure->EWAH->SpaceMeasure
			//Here SpaceMeasure indicates the symbol used to measure space
			CSV("C:\\Data\\soyaTestsort.csv",1000);
			formattedOutPut += "Space Before Compression : " +MeasureSpace()+"Bytes\n";
			Convert(BitStreamInfo::WAH_COMPRESSION);
			formattedOutPut += "Space After Compression : " +MeasureSpace()+"Bytes";
			DeleteAll();
			flash->root.Call("cplusPluseCallBackFunction",formattedOutPut);
		}
		else
		{
			string formattedOutPut="noView##";
			flash->root.Call("cplusPluseCallBackFunction", formattedOutPut);
		}

	}	*/

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
//void CIntelliCheckersUIDlg::OnSize(UINT nType, int cx, int cy)
//{
//	__super::OnSize(nType, cx, cy);
//
//	// TODO: Add your message handler code here
//	CAboutDlg dlgAbout;
//	dlgAbout.DoModal();
//}

//void CIntelliCheckersUIDlg::OnSize(UINT nType, int cx, int cy)
//{
//	__super::OnSize(nType, cx, cy);
//
//	// TODO: Add your message handler code here
//	//m_Flash=null;
//	CShockwaveFlash k(m_Flash);
//}
