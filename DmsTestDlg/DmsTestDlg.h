// DmsTestDlg.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDmsTestDlgApp:
// �йش����ʵ�֣������ DmsTestDlg.cpp
//

class CDmsTestDlgApp : public CWinApp
{
public:
	CDmsTestDlgApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDmsTestDlgApp theApp;