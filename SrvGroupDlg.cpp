// SrvGroupDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyASIO.h"
#include "SrvGroupDlg.h"
#include "afxdialogex.h"


// CSrvGroupDlg 对话框

IMPLEMENT_DYNAMIC(CSrvGroupDlg, CDialogEx)

CSrvGroupDlg::CSrvGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSrvGroupDlg::IDD, pParent)
{

}

CSrvGroupDlg::~CSrvGroupDlg()
{
}

void CSrvGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSrvGroupDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSrvGroupDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSrvGroupDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CREATE_CLIENT, &CSrvGroupDlg::OnBnClickedCreateClient)
	ON_BN_CLICKED(IDC_STOP_CLIENT, &CSrvGroupDlg::OnBnClickedStopClient)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CSrvGroupDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_START_SERVER, &CSrvGroupDlg::OnBnClickedStartServer)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSrvGroupDlg 消息处理程序


BOOL CSrvGroupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_bIsExit = false;

	SetTimer( 1000, 1000, NULL );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSrvGroupDlg::addHistroy( const char* pStr )
{
	if ( m_sHistroy.GetLength() > 65535 )
	{
		m_sHistroy.Empty();
	}
	TRACE( pStr );
	m_sHistroy += pStr;
	CEdit* p = (CEdit*)GetDlgItem( IDC_EDIT_HISTROY );
	p->SetWindowText( m_sHistroy );
	p->SendMessage( WM_VSCROLL, SB_BOTTOM, 0 );
}

void CSrvGroupDlg::startAllServer()
{
	m_ptrSrvService = boost::shared_ptr<class XAsioService>( new XAsioService );

	stServerConfig config1, config2, config3, config4;
	config1.testGateConfig();
	m_gateServer.setIoService( m_ptrSrvService );
	m_gateServer.setLogHandler( std::bind( &CSrvGroupDlg::onLog, this, std::placeholders::_1 ));
	m_gateServer.loadConfig( &config1 );
	m_gateServer.startServer();	

	config2.testWorldConfig();
	m_worldServer.setIoService( m_ptrSrvService );
	m_worldServer.setLogHandler( std::bind( &CSrvGroupDlg::onLog, this, std::placeholders::_1 ));
	m_worldServer.loadConfig( &config2 );
	m_worldServer.startServer();	

	config3.testDBConfig();
	m_dbServer.setIoService( m_ptrSrvService );
	m_dbServer.setLogHandler( std::bind( &CSrvGroupDlg::onLog, this, std::placeholders::_1 ));
	m_dbServer.loadConfig( &config3 );
	m_dbServer.startServer();

	config4.testLogConfig();
	m_logServer.setIoService( m_ptrSrvService );
	m_logServer.setLogHandler( std::bind( &CSrvGroupDlg::onLog, this, std::placeholders::_1 ));
	m_logServer.loadConfig( &config4 );
	m_logServer.startServer();	
		
/*
	m_worldServer.connectAppServer();
	m_logServer.connectAppServer();
	m_gateServer.connectAppServer();
	m_dbServer.connectAppServer();
	return;
*/

	int port = 0;
	std::string ip;
	m_gateServer.queryAddress( ip, port );
	m_clientPool.setAddress( ip.c_str(), port );
	m_clientPool.setLogHandler( std::bind( &CSrvGroupDlg::onLog, this, std::placeholders::_1 ) );
	XTestClient::setLog( std::bind( &CSrvGroupDlg::onLog, this, std::placeholders::_1 ) );
}

void CSrvGroupDlg::closeAllServer()
{
	m_bIsExit = true;

	//m_centerServer.stopServer();
	m_dbServer.stopServer();	
	m_gateServer.stopServer();
	m_worldServer.stopServer();
	m_logServer.stopServer();
	if ( m_ptrSrvService )
	{
		m_ptrSrvService->stopAllServices();
	}

	m_clientPool.clear();
}

void CSrvGroupDlg::onLog( const char* pLog )
{
	if ( m_bIsExit )
	{
		return;
	}
	m_sHistroy += pLog;
	m_sHistroy += "\r\n";
	TRACE( pLog );
	TRACE( "\r\n" );
}

void CSrvGroupDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	closeAllServer();
	CDialogEx::OnOK();
}

void CSrvGroupDlg::OnBnClickedCancel()
{
	closeAllServer();
	CDialogEx::OnCancel();
}

void CSrvGroupDlg::OnBnClickedCreateClient()
{
	m_clientPool.createClient();
}

void CSrvGroupDlg::OnBnClickedStopClient()
{
	m_clientPool.closeClient();
}

void CSrvGroupDlg::OnBnClickedButtonLogin()
{
	m_clientPool.forEachClient( std::bind( &CSrvGroupDlg::doClientLogin, this, std::placeholders::_1 ) );
}

void CSrvGroupDlg::doClientLogin( XTestClient* pClient )
{
	XAsioSendPacket packet( EN_MSG_LOGIN );
	packet << int( pClient->getClientId() );
	//m_clientPool.send( packet );
	XAsioBuffer buff;
	packet.output( buff );
	pClient->send( buff );
}

void CSrvGroupDlg::OnBnClickedStartServer()
{
	// TODO: 在此添加控件通知处理程序代码
	startAllServer();
}


void CSrvGroupDlg::OnTimer(UINT_PTR nIDEvent)
{
	if ( nIDEvent == 1000 )
	{
		CEdit* p = (CEdit*)GetDlgItem( IDC_EDIT_HIS );
		if ( p )
		{
			p->SetWindowText( m_sHistroy );
			p->SendMessage( WM_VSCROLL, SB_BOTTOM, 0 );		
		}		
	}
	CDialogEx::OnTimer(nIDEvent);
}
