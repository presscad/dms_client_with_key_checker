#pragma once
#include "ClientSocket.h"
#include "resource.h"


// ClientDlg �Ի���

class ClientDlg : public CDialog
{
	DECLARE_DYNAMIC(ClientDlg)

public:
	ClientDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ClientDlg();

	CClientSocket m_ClientSocket;

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
