#pragma once

#include "../asiowrapper/include/db/XDBMysql.h"

using namespace XMYSQL;
// CMysqlDialog 对话框

class CMysqlDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CMysqlDialog)

	CString			m_sHistroy;

	XDBMysql		m_mysql;

	void			addHistroy( LPCSTR pStr );

	void			onLog( const char* pLog );

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
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
