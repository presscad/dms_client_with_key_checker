#pragma once
#include "afxdtctl.h"
#include "afxwin.h"


// CSelectTimeDlg �Ի���

class CSelectTimeDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectTimeDlg)

public:
	CSelectTimeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSelectTimeDlg();

// �Ի�������
	enum { IDD = IDD_SELECT_TIME_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CMonthCalCtrl m_MonthDate;
public:
	CDateTimeCtrl m_DateTimeCtrl;
public:
	afx_msg void OnBnClickedOk();

public:

	CTime m_StartTime;
	CTime m_EndTime;
public:
	CButton m_ChkAllFile;
public:
	afx_msg void OnBnClickedCheck1();
public:
	CMonthCalCtrl m_MonthEndDate;
public:
	CDateTimeCtrl m_DateTimeCtrlEnd;
};
