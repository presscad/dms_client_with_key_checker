#pragma once
#include "ClientSocket.h"
#include "resource.h"


// ClientDlg 对话框

class ClientDlg : public CDialog
{
	DECLARE_DYNAMIC(ClientDlg)

public:
	ClientDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ClientDlg();

	CClientSocket m_ClientSocket;

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
