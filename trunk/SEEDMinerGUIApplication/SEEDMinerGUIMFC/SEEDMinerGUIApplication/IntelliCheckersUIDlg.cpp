// IntelliCheckersUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include <iostream>
#include "IntelliCheckersUI.h"
#include "IntelliCheckersUIDlg.h"

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
	m_Flash.SetMovie(CFlexCtrl::GetAbsolutePath("../../../Flex/bin-debug/SEEDMinerGUI.swf"));
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

void CIntelliCheckersUIDlg::OnFlexButtonClick(CFlexEvent *evt, CString controller)
{
	string procedure=evt->procedure;

	if (procedure=="csv->text")
	{
		string path=evt->procedurePara;
		string formattedOutPut="";

		for (int i=0;i<1000000000;i++)
		{

		}

		//implement the procedure for get data from csv file(path is the location of the file) 
		//and make a string to out put data in the text viewer 
		//assign it to "formattedOutPut" here

		flash->root.Call("cplusPluseCallBackFunction", procedure+formattedOutPut);
	}
	else if (procedure=="csv->apriory->text")
	{
		string path=evt->procedurePara;
		string formattedOutPut="";

		//implement the procedure for get data from csv file apply apriory algorithm 
		//and make a string to out put the rules in the text viewer 
		//assign it to "formattedOutPut" here

		flash->root.Call("cplusPluseCallBackFunction", procedure+formattedOutPut);

	}
	
}