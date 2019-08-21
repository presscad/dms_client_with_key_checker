// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "ClientSocket.h"
#include "Support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientSocket

CClientSocket::CClientSocket():m_isConnect(FALSE)
{

}

CClientSocket::~CClientSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClientSocket member functions

void CClientSocket::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(nErrorCode)
	{
		AfxMessageBox("连接出现错误,请您重新连接!");
		return;
	}

	m_isConnect = TRUE;
	AsyncSelect(FD_READ|FD_WRITE|FD_CLOSE);
	//((CChatClientDlg *)(AfxGetApp()->m_pMainWnd))->m_ServerIP.EnableWindow(FALSE);
	//((CChatClientDlg *)(AfxGetApp()->m_pMainWnd))->m_ButtonConnect.EnableWindow(FALSE);
	//((CChatClientDlg *)(AfxGetApp()->m_pMainWnd))->m_ButtonSend.EnableWindow();
 //   ((CChatClientDlg *)(AfxGetApp()->m_pMainWnd))->m_ListWords.AddString("连接上服务器端");
	//((CChatClientDlg *)(AfxGetApp()->m_pMainWnd))->m_ListWords.SetTopIndex(
	//((CChatClientDlg *)(AfxGetApp()->m_pMainWnd))->m_ListWords.GetCount()-1);
	CAsyncSocket::OnConnect(nErrorCode);
}

void CClientSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
/*	RECEIVE_INFO receInfo(0);
	//char szTemp[250];
	int n=Receive(&receInfo,sizeof(RECEIVE_INFO));
	//szTemp[n]=0;
	CString sTemp;
	sTemp.Format("收到:%s",receInfo.info);
	if(receInfo.type == 0){
		m_Ip = sTemp;
	}else if(receInfo.type == 1){
		((CChatClientDlg *)(AfxGetApp()->m_pMainWnd))->m_ListWords.AddString(sTemp);
	}else if(receInfo.type == 10){
		sTemp.Format("%s",receInfo.info);
		sTemp.Replace(":","");

		vector<CString> vec;
		CSupport::split(sTemp,',',vec);
		CString path = CSupport::GetEncryptPath(vec[0]);
		vec.erase(vec.begin());

	   ((CChatClientDlg *)(AfxGetApp()->m_pMainWnd))->SendFile(*this,path,vec);
	}else if(receInfo.type == 3){
	
		sTemp.Format("%s",receInfo.info);
		sTemp.Replace(":","");
		vector<CString> vec;
		CSupport::split(sTemp,',',vec);

		if(vec.size() > 1){
			CString path = vec[0];
			vec.erase(vec.begin());

			CClientSocket* pSocket =
				&((CChatClientDlg *)(AfxGetApp()->m_pMainWnd))->m_FileClientSocket;
			((CChatClientDlg *)(AfxGetApp()->m_pMainWnd))->SendFile(*pSocket,path,vec);
		}

	}else if(receInfo.type == 2){
		sTemp.Format("%s",receInfo.info);
		vector<CString> vec;
		CSupport::split(sTemp,',',vec);
		((CChatClientDlg *)(AfxGetApp()->m_pMainWnd))->CheckMonitoring(vec);
	}
	else if(receInfo.type == 4){
		sTemp.Format("%s",receInfo.info);
		((CChatClientDlg *)(AfxGetApp()->m_pMainWnd))->CancelMonitoring(sTemp);
	}else if(receInfo.type == 5){
		sTemp.Format("%s",receInfo.info);
		((CChatClientDlg *)(AfxGetApp()->m_pMainWnd))->AddMonitoring(sTemp);
	}else if(receInfo.type == 6){

	}
	((CChatClientDlg *)(AfxGetApp()->m_pMainWnd))->m_ListWords.SetTopIndex(
	((CChatClientDlg *)(AfxGetApp()->m_pMainWnd))->m_ListWords.GetCount()-1);*/


	CAsyncSocket::OnReceive(nErrorCode);
}

void CClientSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
/*	((CChatClientDlg *)(AfxGetApp()->m_pMainWnd))->m_ListWords.AddString("服务器端已经断开");
	((CChatClientDlg *)(AfxGetApp()->m_pMainWnd))->m_ListWords.SetTopIndex(
	((CChatClientDlg *)(AfxGetApp()->m_pMainWnd))->m_ListWords.GetCount()-1);
	((CChatClientDlg *)(AfxGetApp()->m_pMainWnd))->m_ServerIP.EnableWindow();
	((CChatClientDlg *)(AfxGetApp()->m_pMainWnd))->m_ButtonConnect.EnableWindow();
	((CChatClientDlg *)(AfxGetApp()->m_pMainWnd))->m_ButtonSend.EnableWindow(FALSE);*/
	Close();
	m_isConnect = FALSE;
	CAsyncSocket::OnClose(nErrorCode);
}

BOOL CClientSocket::isConnect(){
	return m_isConnect;
}