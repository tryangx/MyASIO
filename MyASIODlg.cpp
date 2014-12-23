
// MyASIODlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyASIO.h"
#include "MyASIODlg.h"
#include "afxdialogex.h"

#include "../asiowrapper/include/util/XDataTable.h"
#include "../asiowrapper/include/util/XStringUtil.h"

#include <exception>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TIME_INTERVAL		1000

enum 
{
	UPDATE_INFO_TIMER	= 1000,
	CREATE_CLIENT_TIMER = 1001,
	CLOSE_CLIENT_TIMER	= 1002,
	SERVER_SEND_TIMER	= 1003,
	CLIENT_SEND_TIMER	= 1004,
};

class CardData
{
public:
	static const char* getFileName() { return "card.csv"; }
	int			getId() { return m_id; }
	void		parse( std::vector<std::string>& v )
	{
		m_id = XGAMESTRING::parseInt( v[0] );
		char output[4096];		
		utf8ToAnsi( v[1].c_str(), output, 4096 );
		m_name = output;
		m_type = XGAMESTRING::parseInt( v[2] );
		std::vector<std::string> arr;
		XGAMESTRING::split( v[3].c_str(), arr, ":" );
		m_reward1 = XGAMESTRING::parseInt( v[3] );
		m_reward2 = XGAMESTRING::parseInt( v[4] );
	}

public:
	int			m_id;
	std::string	m_name;
	int			m_type;
	int			m_reward1;
	int			m_reward2;
	int			m_reward3;
};

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
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_TESTECHO, &CMyASIODlg::OnBnClickedButtonTestecho)
	ON_BN_CLICKED(IDC_CHECK_LOG, &CMyASIODlg::OnBnClickedCheckLog)
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

	XTableDataLoader<CardData>	cardLoader;
	cardLoader.load();

	// TODO: 在此添加额外的初始化代码
	m_iPort = 8888;//9999;
	CString sPort;
	sPort.Format( _T("%d"), m_iPort );
	GetDlgItem( IDC_EDIT_PORT )->SetWindowText( sPort );

	GetDlgItem( IDC_EDIT_CREATE_TIMES )->SetWindowText( CString( "100" ) );

/*
	char text1[16] = "first";
	char text2[16] = "first1";
	std::string str = "second";
	
	XAsioBuffer buffer( 4 );
	buffer.writeData( text1, 16 );
	buffer << text2;
	buffer << str;
	buffer << 12345;
	buffer << std::string( "third" );

	memset( text1, 1, sizeof(text1) );
	int v;
	std::string s1, s2;
	buffer.readData( text1, 16 );
	buffer >> text2;
	buffer >> s1;
	buffer >> v;
	buffer >> s2;*/

	m_bShowLog = false;	
	m_bTestEcho = false;
	m_bExit = false;

	m_idCounter = 0;
	m_iCloseClientCnt = 0;

	m_iStartServerTick = GetTickCount();

	m_iMaxClient = 1;//1000;
	CString s;
	s.Format( _T("%d"), m_iMaxClient );
	GetDlgItem( IDC_EDIT_LINK_COUNT )->SetWindowText( s );

	m_log.setLogFileName( "asio_debug", false );
	m_log.writeLog( "startup" );

	SetTimer( 1000, TIME_INTERVAL, NULL );
	
	outputString( "hello %d", 128 );
	
	XAsioPacket packet;
	packet.s = "test";
	packet.value = 12345;	
	std::ostringstream oss;
	boost::archive::binary_oarchive oa( oss );
	oa << packet;
	std::string str;
	int v = 0;	
	std::string content = oss.str();
	std::istringstream iss( content );
	XAsioPacket p1;
	boost::archive::binary_iarchive ia( iss );
	ia >> p1;

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
	//m_ptrServer->setLogHandler( std::bind( &CMyASIODlg::onServerLog, this, std::placeholders::_1 ) );
	m_ptrServer->setAddress( m_iPort );
	m_ptrServer->setAcceptThreadNum( 1 );
	m_ptrServer->startServer();

	m_serverService.startService( m_ptrServer->getService().get(), 2 );

	m_iStartServerTick = GetTickCount();

	m_consumeThread.interrupt();
	m_consumeThread = boost::thread( boost::bind( &CMyASIODlg::onConsumeThread, this ) );
}

void CMyASIODlg::onConsumeThread()
{
	try
	{
		while( true )
		{
			boost::this_thread::interruption_point();
			if ( m_ptrServer )
			{
				XAsioRecvPacket packet;
				if ( m_ptrServer->queryRecvPacket( packet ) )
				{
					std::string s;
					int v = 0;
					packet >> s;
					packet >> v;
//					TRACE( "recv" );
//					TRACE( s.c_str() );
//					TRACE( "\n" );
				}
				else
				{
					this_thread::sleep(get_system_time() + posix_time::milliseconds(10));
				}
			}
		}
	}
	catch(...)
	{
	}
}
void CMyASIODlg::onCloseClient( size_t id )
{
	closeClient( id );
}
void CMyASIODlg::onServerLog( const char* str )
{
	mutex::scoped_lock lock( m_serverMutex );
	std::string err = "Server:";
	err += str;	
	if ( m_bShowLog )
	{
		m_log.writeLog( "%s", err.c_str() );
		err += "\r\n";
		addHistroy( m_sServerHistroy, m_serverHistroy, err.c_str() );
	}
	else
	{
		err += "\r\n";
	}	
	TRACE( err.c_str() );
}
void CMyASIODlg::onClientLog( const char* str )
{
	mutex::scoped_lock lock( m_clientMutex );
	std::string err = "Client:";
	err += str;	
	if ( m_bShowLog )
	{
		m_log.writeLog( "%s", err.c_str() );
		err += "\r\n";
		addHistroy( m_sClientHistroy, m_clientHistroy, err.c_str() );
	}
	else
	{
		err += "\r\n";
	}
	TRACE( err.c_str() );
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
/*
	edit.SetWindowText( s );
	edit.SendMessage( WM_VSCROLL, SB_BOTTOM, 0 );
*/
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
	SetTimer( SERVER_SEND_TIMER, TIME_INTERVAL, NULL );
}

void CMyASIODlg::OnBnClickedButtonSendclient()
{
	SetTimer( CLIENT_SEND_TIMER, TIME_INTERVAL, NULL );
}

void CMyASIODlg::OnBnClickedButtonStartclient()
{
	CString s;
	GetDlgItem( IDC_EDIT_LINK_COUNT )->GetWindowText( s );
	m_iMaxClient = boost::lexical_cast<int>( s.GetBuffer(0) );

	SetTimer( CREATE_CLIENT_TIMER, TIME_INTERVAL, NULL );
}
void CMyASIODlg::OnBnClickedButtonStopclient()
{
	SetTimer( CLOSE_CLIENT_TIMER, TIME_INTERVAL, NULL );
}

void CMyASIODlg::OnBnClickedButtonStopserver()
{
	if ( m_ptrServer )
	{
		m_ptrServer->stopServer();
		//m_serverService.stopService( m_ptrServer->getService().get() );
		m_serverService.stopAllServices();
	}	
	m_consumeThread.interrupt();
}

void CMyASIODlg::doClose()
{
	TRACE( "!!!!!!exit\n" );
	m_bExit = true;

	KillTimer( UPDATE_INFO_TIMER );
	KillTimer( CREATE_CLIENT_TIMER );
	KillTimer( CLOSE_CLIENT_TIMER );
	KillTimer( SERVER_SEND_TIMER );
	KillTimer( CLIENT_SEND_TIMER );

	m_consumeThread.interrupt();

	TRACE( "!!!!!!thread exit\n" );

	mutex::scoped_lock lock( m_listMutex );

	auto it = std::begin( m_mapClient );
	for ( ; it != std::end( m_mapClient ); it++ )
	{
		CLIENT_PTR ptr = it->second;
		ptr->disconnect();
		m_clientService.removeService( ptr->getService() );
	}
	m_clientService.stopAllServices();
	//m_clientService.forceStopAllServices();
	m_mapClient.clear();

	TRACE( "!!!!!!thread client\n" );

	if ( m_ptrServer )
	{
		m_ptrServer->stopServer();
		m_serverService.stopAllServices();
		//m_serverService.forceStopAllServices();
	}
	TRACE( "!!!!!!thread server\n" );

/*
	while( m_clientService.isStarted() && m_clientService.isRunning()
		|| ( m_serverService.isStarted() && m_serverService.isRunning() ) )
	{
		this_thread::sleep( posix_time::milliseconds( 1000 ) );
	}*/
	m_log.writeLog( "doclose" );
}

void CMyASIODlg::doUpdateInfo()
{
	mutex::scoped_lock lock( m_listMutex );

	size_t kb = 1024 * 10;
	size_t in, out;
	CString s;

	int time = ( GetTickCount() - m_iStartServerTick ) / 1000;

	in = m_ptrServer ? m_ptrServer->getStat()->getTotalRecvSize() : 0;
	out = m_ptrServer ? m_ptrServer->getStat()->getTotalSendSize() : 0;
	int cnt = m_ptrServer ? m_ptrServer->getClientCount() : 0;	
	s = outputString( "Time:%d\nAccept:%d\nRecv:%dK\nSend:%dK(%.1f KB/s)", 
			time, cnt, in / 1024, out / 1024, (double)out / ( 1024 * time ) );	
	GetDlgItem( IDC_STATIC_SERVER )->SetWindowText( s );

	in = XAsioStatClientAgent::getMutableInstance()->getTotalRecvSize();
	out = XAsioStatClientAgent::getMutableInstance()->getTotalSendSize();
	cnt = m_mapClient.size();
	int tmp = m_mapTempClient.size();
	s = outputString( "Connect:%d\nTemp:%d\nClose:%d\nRecv:%dK\nSend:%dK(%.1f KB/s)", 
		cnt, tmp, m_iCloseClientCnt, in / 1024, out / 1024, (double)out / ( 1024 * time ) );	
	GetDlgItem( IDC_STATIC_CLIENT )->SetWindowText( s );

	m_clientHistroy.SetWindowText( m_sClientHistroy );
	m_clientHistroy.SendMessage( WM_VSCROLL, SB_BOTTOM, 0 );
	m_serverHistroy.SetWindowText( m_sServerHistroy );
	m_serverHistroy.SendMessage( WM_VSCROLL, SB_BOTTOM, 0 );
}
void CMyASIODlg::doCreateClient()
{
	mutex::scoped_lock lock( m_listMutex );

	if ( !m_ptrServer || !m_ptrServer->isStarted() )
	{
		return;
	}

	CString s;
	GetDlgItem( IDC_EDIT_CREATE_TIMES )->GetWindowText( s );
	int concurrentCnt = boost::lexical_cast<int>( s.GetBuffer(0) );//10 + rand() % 40;

	bool bAddClient = false;
	int begTick = GetTickCount();
	for ( int i = 0; i < concurrentCnt; i++ )
	{
		boost::this_thread::interruption_point();
		if ( (int)m_mapClient.size() < m_iMaxClient )
		{
			bAddClient = true;
			m_idCounter++;
			CLIENT_PTR client = CLIENT_PTR( new XClient( m_clientService ) );						
			client->setClientId( m_idCounter );
			client->setAddress( "localhost", m_iPort );
			client->connect();
			//client->setLogHandler( std::bind( &CMyASIODlg::onClientLog, this, std::placeholders::_1 ) );
			client->setCloseHandler( std::bind( &CMyASIODlg::onCloseClient, this, std::placeholders::_1 ) );
			m_clientService.startService( client->getService(), 1 );
			m_mapClient.insert( std::make_pair( client->getClientId(), client ) );
			if ( m_bTestEcho )
			{
				client->testEcho();
			}
		}
	}
	if ( bAddClient )
	{
		int endTick = GetTickCount();
		//TRACE( outputString( "connect finished %d\n", endTick - begTick ) );
	}
	lock.unlock();
}
void CMyASIODlg::closeClient( unsigned int id )
{
	mutex::scoped_lock lock( m_listMutex );
	auto it = std::begin( m_mapClient );
	for ( ; it != std::end( m_mapClient ); it++ )
	{
		CLIENT_PTR& ptr = it->second;
		if ( ptr->getClientId() == id )
		{
			m_iCloseClientCnt++;
			ptr->disconnect();
			m_clientService.removeService( ptr->getService() );
			m_mapTempClient.insert( std::make_pair( ptr->getClientId(), ptr ) );
			m_mapClient.erase( it );
			break;
		}
	}
}
void CMyASIODlg::doCloseClient()
{
	mutex::scoped_lock lock( m_listMutex );
	if ( m_mapTempClient.size() != 0 )
	{
		auto it = std::begin( m_mapTempClient );
		for ( ; it != std::end( m_mapTempClient ); )
		{
			CLIENT_PTR& ptr = it->second;
			if ( !ptr->isConnected() )
			{
				it = m_mapTempClient.erase( it );
			}
			else
			{
				it++;
			}
		}
	}
	if ( m_mapClient.size() != 0 )
	{
		int inx = 0;
		int del = rand() % 5 + 1;
		for ( int left = 0; left < del && !m_mapClient.empty(); left++ )
		{
			inx = 0;
			int targetIndex = rand() % m_mapClient.size();
			//targetIndex = 0;
			auto it = std::begin( m_mapClient );
			for ( ; it != std::end( m_mapClient ); it++, inx++ )
			{
				if ( inx == targetIndex )
				{
					CLIENT_PTR& ptr = it->second;
					if ( ptr->isConnected() )
					{
						m_iCloseClientCnt++;
						//TRACE( outputString( "tryclose %d\n", ptr->getId() ) );
						ptr->disconnect();
						m_clientService.removeService( ptr->getService() );
						m_mapTempClient.insert( std::make_pair( ptr->getClientId(), ptr ) );
						m_mapClient.erase( it );
						break;
					}						
				}
			}
		}
	}
	lock.unlock();
}
void CMyASIODlg::doServerSend()
{
	if ( m_bTestEcho )
	{
		return;
	}
	XAsioBuffer buffer;
	XAsioSendPacket packet( 1 );
	packet << std::string( "from server" );
	packet << int( 123456 );
	packet.output( buffer );
	m_ptrServer->sendToAll( buffer );
}
void CMyASIODlg::doClientSend()
{
	if ( m_bTestEcho )
	{
		return;
	}
	mutex::scoped_lock lock( m_listMutex );
	auto it = std::begin( m_mapClient );
	for ( ; it != std::end( m_mapClient ); it++ )
	{
		CLIENT_PTR ptr = it->second;
		XAsioBuffer buffer;
		XAsioSendPacket packet( 1 );
		char text[256];
		sprintf( text, "from client %d" ,ptr->getClientId() );
		std::string s = text;
		packet << s;
		packet << int( 123456 );
//		char text[256];
//		memset( text, 1, sizeof(text) - 1 );
//		text[255] = 0;
//		packet << text;
		packet.output( buffer );
		ptr->send( buffer );
	}
}
void CMyASIODlg::OnTimer(UINT_PTR nIDEvent)
{
	switch( nIDEvent )
	{
	case UPDATE_INFO_TIMER:
		doUpdateInfo();
		break;
	case CREATE_CLIENT_TIMER:
		doCreateClient();
		break;
	case CLOSE_CLIENT_TIMER:
		doCloseClient();
		break;
	case SERVER_SEND_TIMER:
		doServerSend();
		break;
	case CLIENT_SEND_TIMER:
		doClientSend();
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CMyASIODlg::OnBnClickedButtonTestecho()
{
	m_bTestEcho = true;
	auto it = std::begin( m_mapClient );
	for ( ; it != std::end( m_mapClient ); it++ )
	{
		CLIENT_PTR ptr = it->second;
		ptr->testEcho();
	}
}

void CMyASIODlg::OnBnClickedCheckLog()
{
	m_bShowLog = !m_bShowLog;
}