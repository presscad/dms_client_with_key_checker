#if !defined(AFX_CLIENTSOCKET_H__F802821A_8575_4F39_80C6_144E48655E74__INCLUDED_)
#define AFX_CLIENTSOCKET_H__F802821A_8575_4F39_80C6_144E48655E74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientSocket.h : header file
//

#include "afxsock.h"


typedef struct _receive_info{
	char type;
	char info[1024];
	int len;
	_receive_info(char cType){
		type = cType;
	};
	_receive_info(char cType,CString strInfo){
		type = cType;
		memset(info,0,1024);
		info[strInfo.GetLength()] = '\0'; 
		memcpy(info,strInfo.GetBuffer(strInfo.GetLength() + 1),strInfo.GetLength() + 1);
	};
}RECEIVE_INFO;

/////////////////////////////////////////////////////////////////////////////
// CClientSocket command target

class CClientSocket : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CClientSocket();
	virtual ~CClientSocket();
	BOOL isConnect();
	CString m_Ip;
// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientSocket)
	public:
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CClientSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	BOOL m_isConnect;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKET_H__F802821A_8575_4F39_80C6_144E48655E74__INCLUDED_)
