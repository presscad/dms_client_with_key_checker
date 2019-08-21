// FileSecurityDlg.h : ͷ�ļ�
//
#include "..\BaseDb\Encrypt.h"

#pragma once


// CFileSecurityDlg �Ի���
class CFileSecurityDlg : public CDialog
{
// ����
public:
	CFileSecurityDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FILESECURITY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	CString m_strOrgFileName;
	CString m_strNewFileName;

	CEncrypt m_Encrypt;
	CButton m_chkDecryption;

	CString m_strFilter1;
	CString m_strFilter2;

	// ���ɵ���Ϣӳ�亯��
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
