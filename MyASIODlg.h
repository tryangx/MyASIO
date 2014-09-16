
// MyASIODlg.h : 头文件
//

#pragma once

#include <boost/asio.hpp>

//#include "TestXAsio.h"
#include "../asiowrapper/include/asio/XAsioServer.h"
#include "../asiowrapper/include/asio/XAsioClient.h"
#include "../asiowrapper/include/asio/XAsioPacket.h"
#include "../asiowrapper/include/util/XStringUtil.h"
#include "../asiowrapper/include/util/XSerializtion.h"
#include "../asiowrapper/include/util/XLog.h"
#include "afxwin.h"

using namespace XGAME;

// CMyASIODlg 对话框
class CMyASIODlg : public CDialogEx
{
// 构造
	int							m_iPort;
	bool						m_bExit;
public:
	XAsioService						m_serverService;
	XAsioService						m_clientService;
	boost::shared_ptr<class XServer>	m_ptrServer;
	int		m_iMaxClient;

	bool	m_bShowLog;

	mutex	m_clientMutex;
	mutex	m_serverMutex;
	mutex	m_listMutex;
	
	int			m_iStartServerTick;
	int			m_iCloseClientCnt;

	boost::thread	m_consumeThread;
	
	bool			m_bTestEcho;

	XLogUtil	m_log;

	int			m_idCounter;
	typedef boost::shared_ptr<class XClient>	CLIENT_PTR;
	std::tr1::unordered_map<UINT, CLIENT_PTR>	m_mapClient;
	std::tr1::unordered_map<UINT, CLIENT_PTR>	m_mapTempClient;

//	CClient m_client;
//	CServer m_server;

	void		addHistroy( CString& s, CEdit& edit, LPCSTR pStr );
	void		doClose();

	void		closeClient( unsigned int id );
	
	void		doUpdateInfo();
	void		doCreateClient();
	void		doCloseClient();
	void		doServerSend();
	void		doClientSend();
	void		onConsumeThread();
public:
	void		onRunClientService();
	void		onServerLog( const char* pStr );
	void		onClientLog( const char* pStr );
	void		onCloseClient( size_t id );

public:
	CMyASIODlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MYASIO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStartserver();
	afx_msg void OnBnClickedButtonStartclient();
	CString m_sServerHistroy;
	CString m_sClientHistroy;
	CEdit m_clientHistroy;
	CEdit m_serverHistroy;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonSendserver();
	afx_msg void OnBnClickedButtonSendclient();
	afx_msg void OnBnClickedButtonStopclient();
	afx_msg void OnBnClickedButtonStopserver();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonTestecho();
	afx_msg void OnBnClickedCheckLog();
};