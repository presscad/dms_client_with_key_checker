// lientDlg.cpp : 实现文件
//

#include "stdafx.h"
//#include "Monitor.h"
#include "lientDlg.h"


// ClientDlg 对话框

IMPLEMENT_DYNAMIC(ClientDlg, CDialog)

ClientDlg::ClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ClientDlg::IDD, pParent)
{

}

ClientDlg::~ClientDlg()
{
}

void ClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ClientDlg, CDialog)
END_MESSAGE_MAP()


// ClientDlg 消息处理程序
