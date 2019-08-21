// SelectTimeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DmsTestDlg.h"
#include "SelectTimeDlg.h"


// CSelectTimeDlg 对话框

IMPLEMENT_DYNAMIC(CSelectTimeDlg, CDialog)

CSelectTimeDlg::CSelectTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectTimeDlg::IDD, pParent)
{

}

CSelectTimeDlg::~CSelectTimeDlg()
{
}

void CSelectTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MONTHCALENDAR1, m_MonthDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DateTimeCtrl);
	DDX_Control(pDX, IDC_CHECK1, m_ChkAllFile);
	DDX_Control(pDX, IDC_MONTHCALENDAR2, m_MonthEndDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_DateTimeCtrlEnd);
}


BEGIN_MESSAGE_MAP(CSelectTimeDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSelectTimeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK1, &CSelectTimeDlg::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CSelectTimeDlg 消息处理程序

void CSelectTimeDlg::OnBnClickedOk()
{
	COleDateTime date;
	m_MonthDate.GetCurSel(date);
	int nYear = date.GetYear();
	int nMonth = date.GetMonth();
	int nDay  = date.GetDay();

	COleDateTime time ;
	m_DateTimeCtrl.GetTime(time);

	if(m_ChkAllFile.GetCheck())
		m_StartTime = 0;
	else
		m_StartTime = CTime(nYear,nMonth,nDay,time.GetHour(),time.GetMinute(),time.GetSecond());

	m_MonthEndDate.GetCurSel(date);
	nYear = date.GetYear();
	nMonth = date.GetMonth();
	nDay  = date.GetDay();
	m_DateTimeCtrlEnd.GetTime(time);

	if(m_ChkAllFile.GetCheck())
		m_EndTime = 0;
	else
		m_EndTime = CTime(nYear,nMonth,nDay,time.GetHour(),time.GetMinute(),time.GetSecond());

	OnOK();
}

void CSelectTimeDlg::OnBnClickedCheck1()
{
	if (m_ChkAllFile.GetCheck())
	{
		m_MonthDate.EnableWindow(FALSE);
		m_DateTimeCtrl.EnableWindow(FALSE);
		m_MonthEndDate.EnableWindow(FALSE);
		m_DateTimeCtrlEnd.EnableWindow(FALSE);
	}else{
		m_MonthDate.EnableWindow(TRUE);
		m_DateTimeCtrl.EnableWindow(TRUE);
		m_MonthEndDate.EnableWindow(TRUE);
		m_DateTimeCtrlEnd.EnableWindow(TRUE);
	}
	
}
