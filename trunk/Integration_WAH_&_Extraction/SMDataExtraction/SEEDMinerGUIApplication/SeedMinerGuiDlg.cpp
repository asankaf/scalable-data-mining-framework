// IntelliCheckersUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include <iostream>
#include "SeedMinerGui.h"
#include "SeedMinerGuiDlg.h"



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
	m_source =  (*dsLoaded)(_data_source_name);
	delete dsLoaded;
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

void CIntelliCheckersUIDlg::Classifier()
{
	m_classifier = new C45TreeNominal();
	m_classifier->buildClassifier(m_source);
}
string CIntelliCheckersUIDlg::Text(source_type type,int noOfRows)
{
	string output = "Invalid Data Source Selected";
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
				output = m_classifier->toGraph();
			}

		}
		break;
	case CLASSIFIER_TEXT_SOURCE:
		{
			if (m_classifier != NULL)
			{
				output = m_classifier->toString();
			}

		}
		break;
	case WRAPPED_SOURCE :
		{
			if (m_source != NULL)
			{
				output = m_source->generateCSVStringofDecodedData(noOfRows);
			}


		}
		break;
	case BAYESIAN_SOURCE:
		{
			if (m_bayesian != NULL)
			{
				output = m_bayesian->toString();
			}

		}
		break;
	}
	return output;
}

void CIntelliCheckersUIDlg::OnFlexButtonClick(CFlexEvent *evt, CString controller)
{
	string procedure=evt->procedure;

	if (procedure=="csv->text")
	{
		string path=evt->procedurePara;
		string formattedOutPut="textViewer##";
		//path = "C:\\Data\\soyaTest.csv";
		
		//implement the procedure for get data from csv file(path is the location of the file) 
		//and make a string to out put data in the text viewer 
		//assign it to "formattedOutPut" here
		CSV(path,1000);		
		//SavedDataLoader("soyabeansmall_200000_metadata","soyabeansmall_200000_data","soyabeansmall_100000",10);
		//NullEliminator();
		formattedOutPut += Text(WRAPPED_SOURCE,100);
		DeleteAll();
		flash->root.Call("cplusPluseCallBackFunction", formattedOutPut);
	}
	else if (procedure=="csv->apriory->text")
	{
		string path=evt->procedurePara;
		string formattedOutPut="textViewer##";		
		CSV(path,1000);
		//SavedDataLoader("soyabeansmall_200000_metadata","soyabeansmall_200000_data","soyabeansmall_100000",100);
		Aprior(0.9,0.01,10);		
		//Bayesian(m_source->codedAttributes().size()-1);
		//Classifier();
		//formattedOutPut = Text(CLASSIFIER_SOURCE,0);
		formattedOutPut += Text(APRIORI_SOURCE,0);
		//formattedOutPut = Text(BAYESIAN_SOURCE,0);
		DeleteAll();
		flash->root.Call("cplusPluseCallBackFunction", formattedOutPut);

	}else if (procedure == "csv->classification->tree")
	{
		string path=evt->procedurePara;
		string formattedOutPut="treeViewer##";		
		CSV(path,1000);
		//SavedDataLoader("soyabeansmall_200000_metadata","soyabeansmall_200000_data","soyabeansmall_100000",100);
		Classifier();
		formattedOutPut += Text(CLASSIFIER_SOURCE,0);		
		DeleteAll();
						
		flash->root.Call("cplusPluseCallBackFunction",formattedOutPut);

	}
	else if (procedure == "csv->classification->text")
	{
		string path=evt->procedurePara;
		string formattedOutPut="textViewer##";		
		CSV(path,1000);
		Classifier();
		formattedOutPut += Text(CLASSIFIER_TEXT_SOURCE,0);		
		DeleteAll();
		flash->root.Call("cplusPluseCallBackFunction",formattedOutPut);

	}
	else if (procedure=="getMySqlDataSourceList")
	{
		string formattedOutPut="sqlDataSourcesList##";
		//get the real mysql data sources list as below
		formattedOutPut+="<mysqlsource><name>source1</name></mysqlsource><mysqlsource><name>source2</name></mysqlsource>";
		flash->root.Call("cplusPluseCallBackFunction", formattedOutPut);

	}
	else if (procedure == "mysql->text")
	{
		string dataSource=evt->procedurePara;
		string formattedOutPut="textViewer##";		
		//read mysql
		formattedOutPut += "test mysql read";		
		
		flash->root.Call("cplusPluseCallBackFunction",formattedOutPut);

	}
	else
	{
		string formattedOutPut="noView##";
		flash->root.Call("cplusPluseCallBackFunction", formattedOutPut);
	}
	
}