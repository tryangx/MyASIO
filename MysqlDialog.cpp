// MysqlDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyASIO.h"
#include "MysqlDialog.h"
#include "afxdialogex.h"


// CMysqlDialog �Ի���

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


// CMysqlDialog ��Ϣ�������


void CMysqlDialog::OnBnClickedButtonExecute()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CMysqlDialog::OnBnClickedButtonConnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
