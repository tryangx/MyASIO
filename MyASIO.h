
// MyASIO.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMyASIOApp:
// �йش����ʵ�֣������ MyASIO.cpp
//

class CMyASIOApp : public CWinApp
{
public:
	CMyASIOApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMyASIOApp theApp;