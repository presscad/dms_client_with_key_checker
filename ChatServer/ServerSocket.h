#if !defined(AFX_SERVERSOCKET_H__19FCDE66_2848_42CB_9F4E_B01E892690D8__INCLUDED_)
#define AFX_SERVERSOCKET_H__19FCDE66_2848_42CB_9F4E_B01E892690D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerSocket.h : header file
//
#include <map>

typedef struct _receive_info{
	char type;
	int pos;
	int len;
	char info[1024];
	char tail[16];
	_receive_info(char cType){
		type = cType;
		memset(info,0,1024);
		pos = 0;
	};
	_receive_info(char cType,CString strInfo){
		type = cType;
		pos = 0;
		memset(info,0,1024);
		info[strInfo.GetLength()] = '\0'; 
		memcpy(info,strInfo.GetBuffer(strInfo.GetLength()),strInfo.GetLength() + 1);
		//time = tTime;,COleDateTime tTime
	};
}RECEIVE_INF;

typedef struct _send_info{
	char type;
	char info[1024];

	_send_info(char cType){
		type = cType;
	};
	_send_info(char cType,CString strInfo){
		type = cType;
		memset(info,0,1024);
		info[strInfo.GetLength()] = '\0'; 
		memcpy(info,strInfo.GetBuffer(strInfo.GetLength() + 1),strInfo.GetLength() + 1);
	};
}SEND_INFO;

/////////////////////////////////////////////////////////////////////////////
// CServerSocket command target

class CServerSocket : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CServerSocket();
	virtual ~CServerSocket();
	void ClearArrayFileData();
	int GetPacketCount(ULONGLONG length);
	CArray<char*,char*> m_arrFileData;
	CArray<int,int>     m_arrFileDataLength;
	CString m_strFileName;
	CFile m_file;
	BOOL m_isOpen;

	RECEIVE_INF** m_ppReceiveInfo;
	int           m_nReceiveInfoCnt;

	std::map<ULONGLONG,RECEIVE_INF*> m_MapData;
// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CServerSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	BOOL isConnect();
// Implementation
protected:
	BOOL m_isConnect;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERSOCKET_H__19FCDE66_2848_42CB_9F4E_B01E892690D8__INCLUDED_)
