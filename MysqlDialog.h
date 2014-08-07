#pragma once


// CMysqlDialog 对话框

class CMysqlDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CMysqlDialog)



public:
	CMysqlDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMysqlDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_MYSQL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonExecute();
	afx_msg void OnBnClickedButtonConnect();
};
