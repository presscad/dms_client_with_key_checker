// ListenSocket.cpp : implementation file
//

#include "stdafx.h"
#include "ChatServer.h"
#include "ListenSocket.h"
#include "ChatServerDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListenSocket

CListenSocket::CListenSocket()
{
}

CListenSocket::~CListenSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CListenSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CListenSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CListenSocket member functions

void CListenSocket::OnAccept(int nErrorCode) 
{
	CChatServerDlg* pChatServerDlg = ((CChatServerDlg *)(AfxGetApp()->m_pMainWnd));
	//if(pChatServerDlg->m_isConnect == FALSE ){
	//	// TODO: Add your specialized code here and/or call the base class
	//	Accept(pChatServerDlg->m_ServerSocket);
	//	pChatServerDlg->m_ButtonSend.EnableWindow();
	//	pChatServerDlg->m_ServerSocket.AsyncSelect(
	//		FD_READ|FD_WRITE|FD_CLOSE);
	//	pChatServerDlg->m_ListWords.AddString(_T("接受客户端连接请求"));
	//	pChatServerDlg->m_ListWords.SetTopIndex(
	//		pChatServerDlg->m_ListWords.GetCount()-1);

	//	CString address;
	//	UINT  port;
	//	pChatServerDlg->m_ServerSocket.GetPeerName(address,port);
	//	SEND_INFO sendInfo(0,address);
	//	pChatServerDlg->m_ServerSocket.Send(&sendInfo,address.GetLength() + 2);		

	//	//CString rSocketAddress;
	//	//UINT rSocketPort;
	//	//pChatServerDlg->m_ServerSocket.GetSockName(rSocketAddress, rSocketPort);

	//	pChatServerDlg->m_isConnect = TRUE;
	//}else{
		CServerSocket* pServerSocket = NULL;
		CArray<CServerSocket*,CServerSocket*>& arrayServerSockets = pChatServerDlg->m_arrayServerSockets;

		int last = arrayServerSockets.GetCount() - 1;
		for(int ii = last  ; ii >= 0 ; ii --)
		{
			if(!arrayServerSockets[ii]->isConnect())
				delete arrayServerSockets[ii];
				arrayServerSockets.RemoveAt(ii);
		}

		pServerSocket = new CServerSocket();
		arrayServerSockets.Add(pServerSocket);
		
		Accept(*pServerSocket);
		pServerSocket->AsyncSelect(
			FD_READ|FD_WRITE|FD_CLOSE);

		CString address;
		UINT  port;
		pServerSocket->GetPeerName(address,port);
		SEND_INFO sendInfo(0,address);
		pServerSocket->Send(&sendInfo,address.GetLength() + 2);	

		pChatServerDlg->m_ListWords.AddString(address + _T("接受客户端连接请求"));
		pChatServerDlg->m_ListWords.SetTopIndex(
			pChatServerDlg->m_ListWords.GetCount()-1);

	//}

	 //CServerSocket m_ServerSocket;

	CAsyncSocket::OnAccept(nErrorCode);
}

