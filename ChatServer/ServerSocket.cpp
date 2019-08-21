// ServerSocket.cpp : implementation file
//

#include "stdafx.h"
#include "ChatServer.h"
#include "ServerSocket.h"
#include "ChatServerDlg.h"
#include "zip\ZipImplement.h"
#include <math.h>
#include <Shlwapi.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerSocket

CServerSocket::CServerSocket()
{
	m_isOpen = FALSE;
	m_ppReceiveInfo = NULL;
	m_nReceiveInfoCnt = 0;
	m_isConnect = TRUE;
}

CServerSocket::~CServerSocket()
{
	ClearArrayFileData();
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CServerSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CServerSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0
//int cnt = 0;

void CServerSocket::ClearArrayFileData(){
	m_arrFileData.RemoveAll();
	int cnt = (int)m_arrFileData.GetCount();
	for (int ii = 0 ; ii < cnt ; ii ++)
	{
		delete [] m_arrFileData[ii];
	}
	m_arrFileDataLength.RemoveAll();
	std::map<ULONGLONG,RECEIVE_INF*> ::iterator iter;//定义一个迭代指针iter
	for(iter=m_MapData.begin(); iter!=m_MapData.end(); iter++){
		delete iter->second;
	}
	m_MapData.clear();


	if(m_ppReceiveInfo != NULL){
		for (int ii = 0 ; ii < this->m_nReceiveInfoCnt; ii ++)
		{
			if(m_ppReceiveInfo[ii])
				delete m_ppReceiveInfo[ii];
		}

		delete [] m_ppReceiveInfo;
		m_ppReceiveInfo = NULL;
		m_nReceiveInfoCnt = 0;
	}
}

//DWORD WINAPI ThreadProc(LPVOID lpParam)  
//{
//	CServerSocket* pServerSocket = (CServerSocket*)lpParam;
//
//	CFile file;
//	file.Open("D:\\file\\" + pServerSocket->m_strFileName,CFile::modeWrite|CFile::modeCreate|CFile::typeBinary);
//
//	int cnt = (int)pServerSocket->m_arrFileData.GetCount();
//	for(int ii = 0 ; ii < cnt ; ii ++){
//		file.Write(pServerSocket->m_arrFileData[ii],pServerSocket->m_arrFileDataLength[ii]);
//	}
//	//int a = pServerSocket->m_arrFileDataLength[cnt  - 1];
//	pServerSocket->ClearArrayFileData();
//	//TRACE("%d\t%d次\n",cnt,a);
//	file.Close();
//	return 0;
//}

DWORD WINAPI ThreadProc2(LPVOID lpParam)  
{
	CServerSocket* pServerSocket = (CServerSocket*)lpParam;
	CString fileName = ".\\" + pServerSocket->m_strFileName;
	CFile file; 
	if(!file.Open(fileName,CFile::modeWrite|CFile::modeCreate|CFile::typeBinary)){
		return 1;
	}
	char datas[1024];
	memset(datas,0,1024);

	for(int ii = 0 ; ii < pServerSocket->m_nReceiveInfoCnt ; ii ++){
		if(pServerSocket->m_ppReceiveInfo[ii] != NULL){
			RECEIVE_INF* pSendInfo = pServerSocket->m_ppReceiveInfo[ii];
			if(pSendInfo)
				file.Write(pSendInfo->info,pSendInfo->len );
		}else{
			file.Write(datas,1024);
		}
	}

	pServerSocket->ClearArrayFileData();
	//TRACE("%d\t%d次\n",cnt,a);
	file.Close();

	CZipImplement cZipImplement;
	CString mUnPackPath = "";
	mUnPackPath = ((CChatServerDlg *)(AfxGetApp()->m_pMainWnd))->GetBufDir();

	mUnPackPath += "\\backups\\";
	if(!PathFileExists(mUnPackPath))//文件夹不存在则创建
		CreateDirectory(mUnPackPath, NULL);

	cZipImplement.Zip_UnPackFiles(fileName,mUnPackPath,"");
	return 0;
}

DWORD WINAPI ThreadProc3(LPVOID lpParam)  
{
	CServerSocket* pServerSocket = (CServerSocket*)lpParam;
	CString fileName = ".\\" + pServerSocket->m_strFileName;
	CFile file; 
	if(!file.Open(fileName,CFile::modeWrite|CFile::modeCreate|CFile::typeBinary)){
		return 1;
	}
	char datas[1024];
	memset(datas,0,1024);

	for(int ii = 0 ; ii < pServerSocket->m_nReceiveInfoCnt ; ii ++){
		if(pServerSocket->m_ppReceiveInfo[ii] != NULL){
			RECEIVE_INF* pSendInfo = pServerSocket->m_ppReceiveInfo[ii];
			if(pSendInfo)
				file.Write(pSendInfo->info,pSendInfo->len );
		}else{
			file.Write(datas,1024);
		}
	}

	pServerSocket->ClearArrayFileData();
	//TRACE("%d\t%d次\n",cnt,a);
	file.Close();

	CZipImplement cZipImplement;
	CString mUnPackPath = "d:\\filetest\\";
	mUnPackPath = ((CChatServerDlg *)(AfxGetApp()->m_pMainWnd))->GetBufDir();

	
	cZipImplement.Zip_UnPackFiles(fileName,mUnPackPath,"");
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CServerSocket member functions
int s_cnt = 0;

int CServerSocket::GetPacketCount(ULONGLONG length){
	double  dCnt = length /1024;
	int nCnt = floor(dCnt);
	int last = 0;
	if(length >  nCnt * 1024)
		last = 1;
	return nCnt + last;
}

void CServerSocket::OnReceive(int nErrorCode) 
{
	//char* szTemp;
	//int size = sizeof(char) + sizeof(ULONGLONG);
	RECEIVE_INF* pInfo = new RECEIVE_INF(0);

	int n = Receive(pInfo,sizeof(RECEIVE_INF));

	RECEIVE_INF& info = *pInfo;
	if(info.type == 2){
		m_strFileName = "";
		m_strFileName.Format("%s",info.info);
		TRACE(m_strFileName);
		m_isOpen = TRUE;
		ClearArrayFileData();
		m_nReceiveInfoCnt = info.pos;;
		m_ppReceiveInfo = new RECEIVE_INF*[m_nReceiveInfoCnt];
		memset(m_ppReceiveInfo,0,sizeof(RECEIVE_INF*)* m_nReceiveInfoCnt);
		delete pInfo;
		s_cnt = 0;
	}else if(info.type == 3 && m_isOpen){
		if(n != -1){
			m_ppReceiveInfo[info.pos] = pInfo;
			TRACE("%d\n",info.pos);
		}
	}else if(info.type == 4  && m_isOpen){

		HANDLE hThread = CreateThread(NULL, 0, ThreadProc2, this, 0, NULL);
		m_isOpen = FALSE;
		delete pInfo;
		
	}else if(info.type == 6  && m_isOpen){

		HANDLE hThread = CreateThread(NULL, 0, ThreadProc3, this, 0, NULL);
		m_isOpen = FALSE;
		delete pInfo;

	}else if(info.type == 18){

		CChatServerDlg* pChatServerDlg = ((CChatServerDlg *)(AfxGetApp()->m_pMainWnd));
		int time = pChatServerDlg->CheckLicence();
		CString str;
		str.Format("%d,%d,%d",time,pChatServerDlg->m_maxLogin,pChatServerDlg->m_maxClient);

		SEND_INFO sendInfo(28,str);
		Send(&sendInfo,str.GetLength() + 1);
		delete pInfo;

	}else{
		delete pInfo;
	}
	
	//else{
		//szTemp = info.info;
		//szTemp[n] = 0;

		//char szTemp[256];
		//int n = Receive(szTemp,256);
		//CString sTemp;
		//sTemp.Format(_T("收到:%s"),szTemp);
		//((CChatServerDlg *)(AfxGetApp()->m_pMainWnd))->m_ListWords.AddString(sTemp);
		//((CChatServerDlg *)(AfxGetApp()->m_pMainWnd))->m_ListWords.SetTopIndex(
		//((CChatServerDlg *)(AfxGetApp()->m_pMainWnd))->m_ListWords.GetCount()-1);
	//}
	CAsyncSocket::OnReceive(nErrorCode);
}

void CServerSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	((CChatServerDlg *)(AfxGetApp()->m_pMainWnd))->m_ListWords.AddString(_T("客户端已经断开"));
	((CChatServerDlg *)(AfxGetApp()->m_pMainWnd))->m_ListWords.SetTopIndex(
		((CChatServerDlg *)(AfxGetApp()->m_pMainWnd))->m_ListWords.GetCount()-1);
	//	((CChatServerDlg *)(AfxGetApp()->m_pMainWnd))->ServerIP.EnableWindow();
	//	((CChatServerDlg *)(AfxGetApp()->m_pMainWnd))->m_ButtonConnect.EnableWindow();
	((CChatServerDlg *)(AfxGetApp()->m_pMainWnd))->m_ButtonSend.EnableWindow(FALSE);
	((CChatServerDlg *)(AfxGetApp()->m_pMainWnd))->m_isConnect = FALSE;

	//CArray<CServerSocket*,CServerSocket*>& arrayServerSockets = ((CChatServerDlg *)(AfxGetApp()->m_pMainWnd))->m_arrayServerSockets;
	//int cnt = (int)arrayServerSockets.GetCount();
	//for (int ii = 0 ; ii < cnt  ; ii ++)
	//{
	//	if(arrayServerSockets[ii] == this){
	//		arrayServerSockets.RemoveAt(ii);
	//		delete this;
	//		break;
	//	}
	//}

	m_isConnect = FALSE;
	CAsyncSocket::OnClose(nErrorCode);
}

BOOL CServerSocket::isConnect()
{
	return m_isConnect;
}

void CServerSocket::OnConnect(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class
	if(nErrorCode)
	{
		AfxMessageBox("连接出现错误,请您重新连接!");
		return;
	}

	m_isConnect = TRUE;
	CAsyncSocket::OnConnect(nErrorCode);
}
