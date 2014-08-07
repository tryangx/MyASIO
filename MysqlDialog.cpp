// MysqlDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "MyASIO.h"
#include "MysqlDialog.h"
#include "afxdialogex.h"


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
END_MESSAGE_MAP()


// CMysqlDialog 消息处理程序


void CMysqlDialog::OnBnClickedButtonExecute()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMysqlDialog::OnBnClickedButtonConnect()
{
	// TODO: 在此添加控件通知处理程序代码
}
