// MysqlDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "MyASIO.h"
#include "MysqlDialog.h"
#include "../asiowrapper/include/asio/XAsioHelper.h"
#include "../asiowrapper/include/util/XStringUtil.h"

using namespace XGAME;
// CMysqlDialog 对话框

IMPLEMENT_DYNAMIC(CMysqlDialog, CDialogEx)

CMysqlDialog::CMysqlDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMysqlDialog::IDD, pParent)
{

}

CMysqlDialog::~CMysqlDialog()
{
}

void CMysqlDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMysqlDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_EXECUTE, &CMysqlDialog::OnBnClickedButtonExecute)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CMysqlDialog::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDOK, &CMysqlDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CMysqlDialog 消息处理程序

BOOL CMysqlDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem( IDC_EDIT_USER )->SetWindowText( CString( "root" ) );
	GetDlgItem( IDC_EDIT_PASSWORD )->SetWindowText( CString( "test" ) );
	GetDlgItem( IDC_EDIT_SYNTAX )->SetWindowText( CString( "select * from account" ) );

	m_mysql.setLogHandler( &CMysqlDialog::onLog, this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMysqlDialog::OnBnClickedButtonExecute()
{
	CString query;
	GetDlgItem( IDC_EDIT_SYNTAX )->GetWindowText( query );

	char output[4096];
	unicodeToAnsi( query.GetBuffer(0),output,4096 );
	RESULTSET_PTR ptrSet = m_mysql.query( output );
	if ( ptrSet )
	{
		addHistroy( "query suc" );

		while( ptrSet->next() )
		{
			int id = ptrSet->getInt( "id" );
			std::string s = ptrSet->getString( "name" );
			addHistroy( outputString( "id:%d  name:%d", id, s.c_str() ) );
		}
	}
}

void CMysqlDialog::OnBnClickedButtonConnect()
{
	CString user, pw;
	GetDlgItem( IDC_EDIT_USER )->GetWindowText( user );
	GetDlgItem( IDC_EDIT_PASSWORD )->GetWindowText( pw );
	m_mysql.selectSchema( "test" );
	char output1[4096], output2[4096];
	unicodeToAnsi(user.GetBuffer(0),output1,4096);
	unicodeToAnsi(pw.GetBuffer(0),output2,4096);
	bool ret = m_mysql.connect( "tcp://127.0.0.1:3306", output1, output2);
	if ( !ret )
	{
		addHistroy( "connect failed" );
	}
	else
	{
		addHistroy( "connect suc" );
	}
}


void CMysqlDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

void CMysqlDialog::addHistroy( LPCSTR pStr )
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

void CMysqlDialog::onLog( const char* pLog )
{
	addHistroy( pLog );
}