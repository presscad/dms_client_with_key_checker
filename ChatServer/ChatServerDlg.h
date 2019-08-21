// ChatServerDlg.h : header file
//

#if !defined(AFX_CHATSERVERDLG_H__7070A13E_60CA_424B_9F33_6534FB816A8F__INCLUDED_)
#define AFX_CHATSERVERDLG_H__7070A13E_60CA_424B_9F33_6534FB816A8F__INCLUDED_

#include "ListenSocket.h"	// Added by ClassView
#include "ServerSocket.h"	// Added by ClassView
#include "afxwin.h"
//#include "C:\\Program Files\\MySQL\MySQL Server 5.7\\include\\mysql.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//#pragma comment(lib, "C:\\Program Files\\MySQL\\MySQL Server 5.7\\lib\\libmysql.lib")

/////////////////////////////////////////////////////////////////////////////
// CChatServerDlg dialog

class CChatServerDlg : public CDialog
{
// Construction
public:
	CServerSocket m_ServerSocket;
	CListenSocket m_ListenSocket;
	CChatServerDlg(CWnd* pParent = NULL);	// standard constructor

	~CChatServerDlg();	// standard constructor

	CArray<CServerSocket*,CServerSocket*> m_arrayServerSockets;
	//CMap<CString,CServerSocket*>  m_mapServerSockets;

	BOOL m_isConnect;
	//////////////////////////
	//MYSQL   *m_pMySQL;//MySQL的连接句柄
	//MYSQL_RES *pResRet;//返回的结果集
	//my_ulonglong RecordCount;//共有几条数据
	//MYSQL_ROW pRow;//某一行的数据


	///////////////////////////

// Dialog Data
	//{{AFX_DATA(CChatServerDlg)
	enum { IDD = IDD_CHATSERVER_DIALOG };
	CListBox	m_ListWords;
	CButton	m_ButtonSend;
	CString	m_sWords;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	void addServerSockets();
// Implementation
protected:
	HICON m_hIcon;

	CEdit m_editDir;
	CEdit m_editPort;

	u_short m_usPort;

	

	// Generated message map functions
	//{{AFX_MSG(CChatServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonSend();
	afx_msg void OnButtonClear();
	afx_msg void OnButtonAbout();
	afx_msg void OnBnClickedButtonDir();
	afx_msg void OnBnClickedButtonSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void ReadConfig();
	int toInt(LPCSTR strVal);
	
public:
	CEdit m_editText;
	CString GetBufDir();
	int m_maxClient;
	int m_maxLogin;
	int CheckLicence();
	// 打开服务器许可
	BOOL OpenServerLicense();
	//void GetLicInfo();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATSERVERDLG_H__7070A13E_60CA_424B_9F33_6534FB816A8F__INCLUDED_)
