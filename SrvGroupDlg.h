#pragma once

#include "../asiowrapper/include/extend/XServerGroup.h"

using namespace XGAME;
// CSrvGroupDlg 对话框

class CSrvGroupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSrvGroupDlg)
	
	boost::shared_ptr<class XAsioService>	m_ptrSrvService;
	XDBServer		m_dbServer;
	XLogServer		m_logServer;
	XWorldServer	m_worldServer;
	XCenterServer	m_centerServer;
	XGateServer		m_gateServer;

	XTestClientPool m_clientPool;

	bool			m_bIsExit;

	CString			m_sHistroy;
	void			addHistroy( const char* pStr );
	void			onLog( const char* pLog );

protected:
	void	startAllServer();
	void	closeAllServer();

	void	doClientLogin( XTestClient* pClient );

public:
	CSrvGroupDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSrvGroupDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SRVGROUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCreateClient();
	afx_msg void OnBnClickedStopClient();
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedStartServer();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
