
// MyASIODlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyASIO.h"
#include "MyASIODlg.h"
#include "afxdialogex.h"

#include <exception>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyASIODlg 对话框




CMyASIODlg::CMyASIODlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyASIODlg::IDD, pParent)
	, m_sClientHistroy(_T("")), m_sServerHistroy(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyASIODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CLIENT, m_clientHistroy);
	DDX_Control(pDX, IDC_EDIT_SERVER, m_serverHistroy);
}

BEGIN_MESSAGE_MAP(CMyASIODlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_STARTSERVER, &CMyASIODlg::OnBnClickedButtonStartserver)
	ON_BN_CLICKED(IDC_BUTTON_STARTCLIENT, &CMyASIODlg::OnBnClickedButtonStartclient)
	ON_BN_CLICKED(IDOK, &CMyASIODlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMyASIODlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SENDSERVER, &CMyASIODlg::OnBnClickedButtonSendserver)
	ON_BN_CLICKED(IDC_BUTTON_SENDCLIENT, &CMyASIODlg::OnBnClickedButtonSendclient)
	ON_BN_CLICKED(IDC_BUTTON_STOPCLIENT, &CMyASIODlg::OnBnClickedButtonStopclient)
	ON_BN_CLICKED(IDC_BUTTON_STOPSERVER, &CMyASIODlg::OnBnClickedButtonStopserver)
END_MESSAGE_MAP()


// CMyASIODlg 消息处理程序

BOOL CMyASIODlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_iPort = 8888;//9999;
	m_idCounter = 0;

	m_iMaxClient = 2500;

	m_bExit = false;
	m_log.setLogFileName( "asio_debug", false );
	m_log.writeLog( "startup" );

	outputString( "hello %d", 128 );

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMyASIODlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyASIODlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMyASIODlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMyASIODlg::OnBnClickedButtonStartserver()
{
	if ( m_ptrServer == nullptr )
	{
		m_ptrServer = boost::shared_ptr<class XServer>( new XServer( m_serverService ) );		
	}
	m_ptrServer->setLogHandler( &CMyASIODlg::onServerLog, this );
	m_ptrServer->setAddress( m_iPort );
	m_ptrServer->setAcceptThreadNum( 6 );
	m_ptrServer->startServer();		

	m_serverService.startService( m_ptrServer->getService().get() );
}

void CMyASIODlg::onServerLog( std::string str )
{
	mutex::scoped_lock lock( m_serverMutex );
	std::string err = "Server:";
	err += str;
	err += "\r\n";
	TRACE( err.c_str() );
	//OutputDebugString( (LPCWSTR)err.c_str() );
	addHistroy( m_sServerHistroy, m_serverHistroy, err.c_str() );
	lock.unlock();
}
void CMyASIODlg::onClientLog( std::string str )
{	
	mutex::scoped_lock lock( m_clientMutex );
	std::string err = "Client:";
	err += str;
	err += "\r\n";
	TRACE( err.c_str() );
	//OutputDebugString( (LPCWSTR)err.c_str() );
	addHistroy( m_sClientHistroy, m_clientHistroy, err.c_str() );
}
void CMyASIODlg::addHistroy( CString& s, CEdit& edit, LPCSTR pStr )
{
	if ( !edit || m_bExit )
	{
		return;
	}
	//return;
	if ( s.GetLength() > 65535 )
	{
		s.Empty();
	}
	s += pStr;
	edit.SetWindowText( s );
	edit.SendMessage( WM_VSCROLL, SB_BOTTOM, 0 );
}

void CMyASIODlg::OnBnClickedOk()
{
	doClose();
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

void CMyASIODlg::OnBnClickedCancel()
{
	doClose();
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

void CMyASIODlg::OnBnClickedButtonSendserver()
{
	if ( !m_ptrServer )
	{
		return;
	}
	m_ptrServer->testSend();
	return;
	XAsioPackage p;
	p.i = 10;
	sprintf_s( p.info, sizeof(p.info), "from server" );

	XAsioPackageHeader header;
	header.m_dwSize = sizeof(p);

	XAsioBuffer buff;
	buff.copyFrom( &header, sizeof(header) );

	m_ptrServer->sendToAll( buff );
	buff.copyFrom( &p, sizeof(p) );
	m_ptrServer->sendToAll( buff );
}

void CMyASIODlg::OnBnClickedButtonSendclient()
{
	auto it = std::begin( m_mapClient );
	for ( ; it != std::end( m_mapClient ); it++ )
	{
		CLIENT_PTR ptr = it->second;
		ptr->testSend();
	}
}

void CMyASIODlg::OnBnClickedButtonStartclient()
{
	m_createClientThread = boost::thread( boost::bind( &CMyASIODlg::createClient, this ) );
}
void CMyASIODlg::OnBnClickedButtonStopclient()
{
	m_closeClientThread = boost::thread( boost::bind( &CMyASIODlg::closeClient, this ) );
}

void CMyASIODlg::createClient()
{
	try
	{
		int concurrentCnt = 500;
		//while( 1 )
		{
			for ( int i = 0; i < concurrentCnt; i++ )
			{
				boost::this_thread::interruption_point();
				if ( (int)m_mapClient.size() < m_iMaxClient )
				{
					m_idCounter++;
					CLIENT_PTR client = CLIENT_PTR( new XClient( m_clientService ) );	
					client->setLog( std::bind( &CMyASIODlg::onClientLog, this, std::placeholders::_1 ) );
					client->setId( m_idCounter );
					client->setAddress( "localhost", m_iPort );
					client->connect();
					m_clientService.startService( client->getService().get() );
					m_mapClient.insert( std::make_pair( client->getId(), client ) );
				}
			}
			int millseconds = rand() % 2000 + 2000;
			this_thread::sleep( get_system_time() + posix_time::milliseconds( millseconds ) );
		}
	}
	catch(boost::thread_interrupted &)
	{
	}
}

void CMyASIODlg::closeClient()
{
	try
	{
		while( 1 )
		{
			boost::this_thread::interruption_point();
			if ( m_mapClient.size() != 0 )
			{
				int inx = 0;
				int targetIndex = rand() % m_mapClient.size();
				auto it = std::begin( m_mapClient );
				for ( ; it != std::end( m_mapClient ); it++, inx++ )
				{
					if ( inx == targetIndex )
					{
						CLIENT_PTR& ptr = it->second;
						ptr->disconnect();
						m_clientService.removeService( ptr->getService().get() );
						m_mapClient.erase( it );
						break;
					}
				}
			}
			int millseconds = rand() % 2000 + 5000;
			this_thread::sleep( get_system_time() + posix_time::milliseconds( millseconds ) );
		}
	}
	catch(boost::thread_interrupted &)
	{
	}
}

void CMyASIODlg::OnBnClickedButtonStopserver()
{
	m_ptrServer->stopServer();
	m_serverService.stopService( m_ptrServer->getService().get() );
}

void CMyASIODlg::doClose()
{
	TRACE( "!!!!!!exit\n" );
	m_bExit = true;
	
	m_createClientThread.interrupt();
	m_closeClientThread.interrupt();

	auto it = std::begin( m_mapClient );
	for ( ; it != std::end( m_mapClient ); it++ )
	{
		CLIENT_PTR ptr = it->second;
		ptr->disconnect();
		m_clientService.removeService( ptr->getService().get() );
	}
	m_mapClient.clear();

	if ( m_ptrServer )
	{
		m_ptrServer->stopServer();
		m_serverService.stopService( m_ptrServer->getService().get() );
	}

	while( m_clientService.isStarted() && m_clientService.isRunning()
		|| ( m_serverService.isStarted() && m_serverService.isRunning() ) )
	{
		this_thread::sleep( posix_time::milliseconds( 1000 ) );
	}
	m_log.writeLog( "doclose" );
}