
// MyASIODlg.h : ͷ�ļ�
//

#pragma once

#include <boost/asio.hpp>

//#include "TestXAsio.h"
#include "../asiowrapper/include/asio/XAsioServer.h"
#include "../asiowrapper/include/asio/XAsioClient.h"
#include "../asiowrapper/include/util/XLog.h"
#include "afxwin.h"

using namespace XASIO;

// CMyASIODlg �Ի���
class CMyASIODlg : public CDialogEx
{
// ����
	int							m_iPort;

public:
	//boost::asio::io_service				m_ioService;
	XAsioService						m_service;
	boost::shared_ptr<class XServer>	m_ptrServer;
	int		m_iMaxClient;

	mutex	m_clientMutex;
	mutex	m_serverMutex;

	boost::thread	m_createClientThread;
	boost::thread	m_closeClientThread;

	XLogUtil	m_log;

	int		m_idCounter;
	typedef boost::shared_ptr<class XClient>	CLIENT_PTR;
	std::tr1::unordered_map<UINT, CLIENT_PTR>	m_mapClient;

//	CClient m_client;
//	CServer m_server;

	void		addHistroy( CString& s, CEdit& edit, LPCSTR pStr );
	void		doClose();
	
	void		createClient();
	void		closeClient();
public:
	void		onRunClientService();
	void		onServerLog( std::string str );
	void		onClientLog( std::string str );

public:
	CMyASIODlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MYASIO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
};