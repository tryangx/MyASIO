#pragma once


// CMysqlDialog �Ի���

class CMysqlDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CMysqlDialog)



public:
	CMysqlDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMysqlDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_MYSQL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonExecute();
	afx_msg void OnBnClickedButtonConnect();
};
