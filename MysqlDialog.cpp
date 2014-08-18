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

	sql::ResultSet* pSet = m_mysql.query( (LPSTR)(LPCTSTR)query.GetBuffer(0) );
	if ( pSet )
	{
		addHistroy( "query suc" );

		while( pSet->next() )
		{
			int id = pSet->getInt( "id" );
			std::string s = pSet->getString( "name" );
			addHistroy( outputString( "id:%d  name:%d", id, s.c_str() ) );
		}
	}
}

inline char* UnicodeToAnsi( const wchar_t* szStr )  
{  
	int nLen = WideCharToMultiByte( CP_ACP, 0, szStr, -1, NULL, 0, NULL, NULL );  
	if (nLen == 0)  
	{  
		return NULL;  
	}  
	char* pResult = new char[nLen];  
	WideCharToMultiByte( CP_ACP, 0, szStr, -1, pResult, nLen, NULL, NULL );  
	return pResult;  
}  

void CMysqlDialog::OnBnClickedButtonConnect()
{
	CString user, pw;
	GetDlgItem( IDC_EDIT_USER )->GetWindowText( user );
	GetDlgItem( IDC_EDIT_PASSWORD )->GetWindowText( pw );
	m_mysql.selectSchema( "test" );
	bool ret = m_mysql.connect( "tcp://127.0.0.1:3306", UnicodeToAnsi(user.GetBuffer(0)), UnicodeToAnsi(pw.GetBuffer(0)));
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