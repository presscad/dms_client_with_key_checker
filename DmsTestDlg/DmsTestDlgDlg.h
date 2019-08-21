// DmsTestDlgDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDmsTestDlgDlg 对话框
class CDmsTestDlgDlg : public CDialog
{
// 构造
public:
	CDmsTestDlgDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DMSTESTDLG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	BOOL StartMonitorFile();
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
	afx_msg void OnBnClickedOk();
public:
	CListBox m_listInfo;
public:
	CEdit m_editPort;
public:
	CEdit m_editFilePort;
public:
	afx_msg void OnBnClickedWiznext();
public:
	afx_msg void OnBnClickedButton2();
public:
	CIPAddressCtrl m_AddressCtrl;
public:
	CEdit m_editDir;
	CEdit m_editFlagFile;
public:
	afx_msg void OnBnClickedCancel();
public:
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedClearList();
public:
	CButton m_BtnConnect;
public:
	CEdit m_editWebPort;
public:
	afx_msg void OnBnClickedButton4();
public:
	afx_msg void OnBnClickedCheck1();
public:
	CButton m_checkCopyDb;
public:
	CEdit m_editOtherExt;
public:
	//int m_nExBackupTime;

public:
	CComboBox m_cbxBackup;
public:
	afx_msg void OnBnClickedButton5();
public:
	afx_msg void OnBnClickedButton6();
};
