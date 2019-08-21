// FileSecurityDlg.h : 头文件
//
#include "..\BaseDb\Encrypt.h"

#pragma once


// CFileSecurityDlg 对话框
class CFileSecurityDlg : public CDialog
{
// 构造
public:
	CFileSecurityDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FILESECURITY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	CString m_strOrgFileName;
	CString m_strNewFileName;

	CEncrypt m_Encrypt;
	CButton m_chkDecryption;

	CString m_strFilter1;
	CString m_strFilter2;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedCheck1();
};
