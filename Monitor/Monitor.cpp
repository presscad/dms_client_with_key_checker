// Monitor.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "Support.h"
#include ".\zip\ZipImplement.h"
#include <math.h>
#include <shlwapi.h>
#include <algorithm>
#include "..\SuperDb\fun.h"
#include "..\SuperDb\Encrypts.h"
#include "..\SuperDb\UniverDB.h"
#include "..\SuperDb\SuperDb.h"
#include "..\BaseDB\BaseSupport.h"
#include "FileTransferClient.h"
#include "FileMonitorRem.h"
#include "StatFileLogs.h"
#include "Lock.h"
#include <map>
#include"tlhelp32.h"

//#include <afxdllx.h>
//#include "ClientSocket.h"

#pragma comment(lib, "SuperDB.lib");

#include "monitor.h"

#ifdef _MANAGED
#error 请阅读 Monitor.cpp 中的说明以使用 /clr 进行编译
// 如果要向您的项目中添加 /clr，必须执行下列操作:
//	1. 移除上述对 afxdllx.h 的包含
//	2. 向没有使用 /clr 且已禁用预编译头的
//	   项目添加一个 .cpp 文件，其中含有下列文本:
//			#include <afxwin.h>
//			#include <afxdllx.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE MonitorDLL = { NULL, NULL };

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL s_isRun = FALSE;
BOOL s_isAutoConnect = TRUE;
int  s_Heartbeat = 10;

HANDLE s_hThread = NULL;
HANDLE s_hThreadListen = NULL;
SOCKET s_Server;        //服务器套接字 
//SOCKET s_FileClient = NULL;

CTime s_mtime ;
//创建一个互斥对象  
Mutex g_Lock; 
Mutex g_TimeLock; 
Mutex g_CheckMoniorLock;
Mutex g_SendOldZipFileLock;
CString s_strBaseBoard("");
CString s_strCPUID("");
u_short s_usPort;
int s_maxLic = 1;
int s_maxSvc = 1;
int s_licensedDays = 0;

//BOOL s_IsWebLicence = FALSE;
//int  s_AddFileLogCount = 1;


CString s_defaultSql = "";

//static HANDLE thread_exit = CreateEvent(NULL, TRUE, FALSE, NULL); // 原子量，用于控制线程结束  
//std::map<CString,CFileDirMonitor*>    s_mapFileDirMonitor;
//std::vector<CString>    s_vecFileLogInfo;
//CClientSocket s_ClientSocket;

std::vector<CString>    s_vecTimeLogInfo;
CString s_strCurExePath;
int s_nSynchroFrequency = 10;//Synchro frequency
static CUniverDB   s_SuperDB;
static CUniverDB   s_MonSuperDB;
//static CUniverDB   s_UniverDB;
//CTime s_BrokenNetTime = 0;

//CStringArray s_arrFileLogs;
//Mutex g_FileLogLock; 
Mutex g_MonFileLogLock; 

//Mutex g_DelDBLock; 
//std::map<CString,CString> s_mapFileLogs;

CStringArray s_arrSendFileLogs;
Mutex g_SendFileLogLock; 
Mutex g_DBLock; 
//int s_WebPID = 0;
//HANDLE s_hWebProcess = NULL;

BOOL g_isFirst = TRUE;
BOOL g_isInsertCreateLog = FALSE;
//CString g_strDBPassword("123");
CString g_strCnnParam("");

ULONG   g_BatchNumber = 1;

//int g_version = 0;

static CString g_StrCurUploadTableName("DMS_FILE_LOGS2");

static CString g_strLastFileDir("");

extern "C" int APIENTRY

DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// 如果使用 lpReserved，请将此移除
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("Monitor.DLL 正在初始化!\n");
		
		// 扩展 DLL 一次性初始化
		if (!AfxInitExtensionModule(MonitorDLL, hInstance))
			return 0;

		// 将此 DLL 插入到资源链中
		// 注意: 如果此扩展 DLL 由
		//  MFC 规则 DLL (如 ActiveX 控件)隐式链接到，
		//  而不是由 MFC 应用程序链接到，则需要
		//  将此行从 DllMain 中移除并将其放置在一个
		//  从此扩展 DLL 导出的单独的函数中。使用此扩展 DLL 的
		//  规则 DLL 然后应显式
		//  调用该函数以初始化此扩展 DLL。否则，
		//  CDynLinkLibrary 对象不会附加到
		//  规则 DLL 的资源链，并将导致严重的
		//  问题。

		new CDynLinkLibrary(MonitorDLL);
		//connectServer();
		//s_hThreadListen = CreateThread(NULL, 0, ThreadProcServer, NULL, 0, NULL);
		OleInitialize(NULL);
		CoInitialize(NULL);

		//CHAR szFileName[256];
		//memset(szFileName,'0',sizeof(szFileName));

		//memset(m_PFunReceInfos,'0',sizeof(m_PFunReceInfos) * 40);


		//::GetModuleFileName(NULL,szFileName, sizeof(szFileName)); 

		CString strCurPath;
		GetModuleFileName(NULL,strCurPath.GetBuffer(MAX_PATH),MAX_PATH);
		strCurPath.ReleaseBuffer();//Must ReleaseBuffer, or GetLength=0
		int pos= strCurPath.ReverseFind(_T('\\'));
		strCurPath = strCurPath.Left(pos);

		//HMODULE hModule = ::LoadLibrary(strCurPath + "\\SendDMSFile.dll");
		//if(hModule){
  //          sSendZipFile = (int (*)(const CString& ,int ,const CString& ))::GetProcAddress(hModule,_T("sSendZipFile"));
		//}

		initFunReceInfo();

		WSADATA wsaData;            
		// Initialize Windows socket library      
		WSAStartup(0x0202, &wsaData); 

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("Monitor.DLL 正在终止!\n");
		//closesocket(sClient); 
		//SaveFileLog();
		//if(s_SuperDB.IsConnect()){
		//	s_SuperDB.Disconnect();
		//	s_SuperDB.Close();
		//}

		//if(s_MonSuperDB.IsConnect()){
		//	s_MonSuperDB.Disconnect();
		//	s_MonSuperDB.Close();
		//}

		std::map<CString,CFileDirMonitor*> ::iterator iter;
		for(iter = s_mapFileDirMonitor.begin(); iter != s_mapFileDirMonitor.end(); iter ++){
			if (iter->second)
			{
				delete iter->second;
			}
			
		}
		
		WSACleanup();
		OleUninitialize();
		CoUninitialize();
		// 在调用析构函数之前终止该库
		AfxTermExtensionModule(MonitorDLL);
	}
	return 1;   // 确定
}

#ifdef _MANAGED
#pragma managed(pop)
#endif
    
#define MSGSIZE        1025         //收发缓冲区的大小      

DWORD WINAPI ThreadProcSendOldFileLog(LPVOID lpParam)  
{ 
	Sleep(500);
	CLock lock(g_Lock);
	vector<CString>::iterator it;
	CString str = "";
	CString strFileLog = "";
	int cnt = s_vecTimeLogInfo.size();
    str = "";
	for (std::vector<CString>::size_type  ii = 0 ; ii < cnt ; ii ++)
	{
		strFileLog = s_vecTimeLogInfo[ii];
		if(str.GetLength() + strFileLog.GetLength() > 1024)
		{
			if(!str.IsEmpty()){
				FILE_LOG sendInfo(7,str);
				send(s_Client,(LPSTR)&sendInfo,str.GetLength() + 1,0);
				if(s_pListInfo){
					s_pListInfo->AddString(str);
				}
				Sleep(500);
			}
			str =  strFileLog;
		}else if(str.IsEmpty()){
			str += strFileLog;

		}else{
			str += ";" + strFileLog;
		}

	}

	if(!str.IsEmpty()){
		FILE_LOG sendInfo(7,str);
		if(s_pListInfo){
			s_pListInfo->AddString(str);
		}
		send(s_Client,(LPSTR)&sendInfo,str.GetLength() + 1,0);
	}

	s_vecTimeLogInfo.clear();
	sClearDBTIMELogInfoM();

	SendOldZipFile();
	return 0;
}

void setCListBox (CListBox& listInfo){
	CStatFileLogs::m_pListInfo = s_pListInfo = &listInfo; 
}

void setAutoCopyDB(BOOL isAuto){
	CStatFileLogs::s_isAutoCopyDB = isAuto;
}

DWORD WINAPI ThreadProcOnTime(LPVOID lpParam){
	//COleDateTime time = COleDateTime::get
	CTime time = CTime::GetCurrentTime();
	ULONG cnt = 0;
	Sleep(3000);
	while (TRUE)  
	{  
		if(s_isAutoConnect)
			sConnectServer();
		Sleep(s_nSynchroFrequency * 1000);

		if(s_isConnect && CSupport::GetServerIP().CompareNoCase("127.0.0.1")){
			//sOpenServerLicense();
			//if(sCheckLicence()){
				char chType = 2;
				send(s_Client,(LPSTR)&chType,1,0);
			//}


		}
		cnt += s_nSynchroFrequency;
		if(cnt >= 6000 && !CSupport::GetServerIP().CompareNoCase("127.0.0.1")){
			sOpenServerLicense();
			char chLicType;
			s_licensedDays = sCheckLicence(chLicType);
			cnt = 0;
		}
	}
	return 0;
}

DWORD WINAPI ThreadProcCloseClientOnTime(LPVOID lpParam){
	//COleDateTime time = COleDateTime::get
	CTime time = CTime::GetCurrentTime();
	while (TRUE)  
	{  
		Sleep(5000);
		CTime mtime = CTime::GetCurrentTime();
		ULONGLONG  ll = mtime.GetTime() - time.GetTime();
		if(s_isAutoConnect)
			break;
		if(ll >  90){
			if(!closesocket(s_Client)){
				s_Client = NULL;
				if(s_hThreadListen == NULL)
					s_hThreadListen = CreateThread(NULL, 0, ThreadProcServer, NULL, 0, NULL);
				break;
			}
		}
	}
	return 0;
}

DWORD WINAPI ThreadProcOnTime2(LPVOID lpParam){
	//COleDateTime time = COleDateTime::get
	s_mtime = CTime::GetCurrentTime();
	while (TRUE)  
	{  
		Sleep(1000);
		CLock lock(g_TimeLock);
		s_mtime += 1;
		//if(s_pListInfo){
		//	CString str =time.Format("%Y_%m_%d_%H_%M_%S");//格式化日期时间 
		//	s_pListInfo->ResetContent();
		//	s_pListInfo->AddString(str);
		//}

	}
	return 0;
}

DWORD WINAPI ThreadProcOnTimeSend(LPVOID lpParam){

	while (TRUE)  
	{  
		Sleep(1000);
		//mtime += s_Heartbeat ;
		//TRACE(s_mtime.Format("old%Y_%m_%d_%H_%M_%S\n"));
		//COleDateTime datetime(s_mtime.GetYear(),s_mtime.GetMonth(),s_mtime.GetDay(),s_mtime.GetHour(),s_mtime.GetMinute(),s_mtime.GetSecond());
		//COleDateTime currTime = COleDateTime::GetCurrentTime();
		CTime mtime = CTime::GetCurrentTime();
		//TRACE(mtime.Format("new%Y_%m_%d_%H_%M_%S\n"));
		long  ll = mtime.GetTime() - s_mtime.GetTime();
		CString str,str2;
		if(abs(ll) > 80){
			str.Format("%ld000",s_mtime.GetTime());
			str2.Format(",%ld000",mtime.GetTime());
			str += str2 + "," + sGetCurrentUserName();

			FILE_LOG sendInfo(6,str);
			if(s_isConnect){
				int iRet = send(s_Client,(LPSTR)&sendInfo,str.GetLength()  +1,0);
				if (SOCKET_ERROR == iRet)
				{
					DWORD dwRet = WSAGetLastError();
					switch( dwRet )
					{       
					case WSAEWOULDBLOCK:
						break;
					default:
						s_isConnect = FALSE;
						closesocket(s_Client);
						break;
					}
				}
			}

			else{
				s_vecTimeLogInfo.push_back(str);
				sAddTimeLogInfoM(str,s_mtime);
			}

			if(s_pListInfo){
				str2.Format("%d-%d,%s->%s",s_isConnect,s_Client,s_mtime.Format("old time:%Y-%m-%d-%H:%M:%S\n"),mtime.Format("new time:%Y-%m-%d-%H:%M:%S\n"));
				s_pListInfo->AddString(str2);
			}

			//CLock lock(g_TimeLock);
			//s_mtime = mtime;
			//TRACE(s_mtime.Format("old%Y_%m_%d_%H_%M_%S\n"));
		}//else

		if(abs(ll) > 30){
			CLock lock(g_TimeLock);
			s_mtime = mtime;
		}
		
		//CFileMonitor::s_BakcupsFile.IsCopyFile();

	}

	return 0;
}

DWORD WINAPI ThreadProcOnTimeHeartbeat(LPVOID lpParam){
	//COleDateTime time = COleDateTime::get
	//
	CString str = "";
	while (TRUE)  
	{  
		Sleep(s_Heartbeat * 1000);
	//{
			if(s_isConnect){
				if(CSupport::GetServerIP().CompareNoCase("127.0.0.1")){
					char type = 5;
					send(s_Client,(LPSTR)&type,sizeof(char),0);
				}else{
					str.Format("%d,%d,%d",s_licensedDays,s_maxSvc,s_maxLic);
					FILE_LOG sendInfo(5,str);
					send(s_Client,(LPSTR)&sendInfo,str.GetLength()  +1,0);
				}
				//CLock lock(g_FileLogLock);
				//int cnt = s_mapFileLogs.size();
				//if(cnt > 0)
				//	CreateThread(NULL, 0, ThreadProcSendFileLog, NULL, 0, NULL);
			}
			CLock lock(g_TimeLock);
			s_mtime = CTime::GetCurrentTime();

			
		}
	//}
	return 0;
} 

void sDebugInfo(RECEIVE_INFO& receInfo){
	TRACE("%d:\t%s\n",receInfo.type,receInfo.info);
}

void sDeleteDBFile(RECEIVE_INFO& receInfo){
	TRACE("%d:\t%s\n",receInfo.type,receInfo.info);
	vector<CString> vec;
	CSupport::split(receInfo.info,';',vec);
	CString strBuf = CSupport::GetBufDir();
	vector<CString>::iterator it;
	for (it = vec.begin(); it != vec.end(); it++){
		TRACE("%d:\t%s\n",receInfo.type,*it);
		DeleteFile(strBuf + "\\" + *it);
	}
}

void sVersionInfo(RECEIVE_INFO& receInfo){
	TRACE("%d:\t%s\n",receInfo.type,receInfo.info);

	CString str;
	str.Format("%s",receInfo.info);

	CFileTransferClient::s_version  = sToInteger(str);

	char info[2];
	info[0] = 9;
	info[1] = 3;

	send(s_Client,(LPSTR)info,2,0);

}

//void sAddConnectInfo(RECEIVE_INFO& receInfo){
//	if(s_pListInfo){
//		CString sTemp;
//		sTemp.Format("收到:%s",receInfo.info);
//		s_pListInfo->AddString(sTemp);
//	}
//}

void initFunReceInfo(){

	for (int ii = 31 ; ii < 40; ii ++)
	{
		m_PFunReceInfos[ii] = sDebugInfo;
	}

	m_PFunReceInfos[0] = sDebugInfo;   
	m_PFunReceInfos[1] = sDebugInfo;//sAddStringListInfo;
	m_PFunReceInfos[2] = sCheckMonitoring;  // check monitor dir. if the dir is the same, cancel. or add it the log.
	m_PFunReceInfos[3] = sSendFileToServer;  // send a file, a zip file, current history backup files.
	m_PFunReceInfos[4] = sCancelMonitoring;  // cancle a dir of monitor dir.
	m_PFunReceInfos[5] = sAddMonitoring;     // add
	m_PFunReceInfos[6] = sSendFileToServer;  // web 
	m_PFunReceInfos[8] = ReceFileLogSql;     // renew the sql, filter the files that no used 
	m_PFunReceInfos[9] = ReceDelFileLogSql;	  // delete 
	m_PFunReceInfos[10] = sDeleteDBFile;     // clean the database file, reserve the last 7 
	m_PFunReceInfos[11] = sDebugInfo;        
	m_PFunReceInfos[12] = sDebugInfo;
	m_PFunReceInfos[13] = sDebugInfo;
	m_PFunReceInfos[14] = sHeartbeatAndSynchroFrequency;  //heartbeat
 	m_PFunReceInfos[15] = sSetAutoConnect;                //auto connect
	m_PFunReceInfos[16] = sHeartbeat;                     //heart beart
	m_PFunReceInfos[17] = sVersionInfo;                //ver
	m_PFunReceInfos[18] = sDebugInfo;            
	m_PFunReceInfos[19] = sDebugInfo;
	m_PFunReceInfos[20] = sDebugInfo;
	m_PFunReceInfos[21] = sDebugInfo;
	m_PFunReceInfos[22] = sDebugInfo;
	m_PFunReceInfos[23] = sDebugInfo;
	m_PFunReceInfos[24] = sDebugInfo;
	m_PFunReceInfos[25] = sDebugInfo;
	m_PFunReceInfos[26] = sRenewalLicense;              // renew license
	m_PFunReceInfos[27] = sSendLinInfo;                //send the info of request a license
	m_PFunReceInfos[28] = sFileFliter28;               //filter str 
	m_PFunReceInfos[29] = sFileFliter29;
	m_PFunReceInfos[30] = sFileFliter30;
	m_PFunReceInfos[38] = sSetRoomId;                //romid that server sended to client
}

void sAddStringListInfo(RECEIVE_INFO& receInfo){
						
	//if(s_pListInfo){
	//	CString sTemp;
	//	sTemp.Format("收到:%s",receInfo.info);
	//	s_pListInfo->AddString(sTemp);
	//}
}

void sSendFileToServer(RECEIVE_INFO& receInfo){
	CString sTemp;
	if(receInfo.type == 3)
		sTemp.Format("%s,1",receInfo.info);
	else
		sTemp.Format("%s,0",receInfo.info);
	CString* pStr = new CString(sTemp);
	HANDLE hThread = CreateThread(NULL, 0, ThreadProcSendFile, pStr, 0, NULL);
	if(INVALID_HANDLE_VALUE == hThread){
		delete pStr;
	}
}

void ReceFileLogSql(RECEIVE_INFO& receInfo){
	CString sTemp;
	sTemp.Format("%s",receInfo.info);
	vector<CString> vec;

	CSupport::split(sTemp,'^',vec);
	CLock SendLock(g_SendFileLogLock);
	CString key("");
	if(vec.size() == 2){
		key = vec[0].MakeLower();
		if(!vec[1].IsEmpty()){
			s_MapFileSql[key] = vec[1];
		}

	}

	if(vec.size() == 3){
		key = vec[0].MakeLower();

		if(vec[1] == "0"){
			if(!vec[2].IsEmpty())
				s_MapFileSql[key] = vec[2];
		}else{
			if(!vec[2].IsEmpty())
				s_MapFileSql[key].Replace("@1",vec[2]);
		}

	}

	if(vec.size() < 2)
		return;

	CString strCfg = s_strCurExePath + "\\config\\fileLogsql.sql";
	CStdioFile file;

	CString strPath = "FILE_PATH";
	if (vec[1].Find("DMS_FILE_LOGS_VIEW") > 0)
	{
		strPath = "FILE_PATH2";
	}

	int size = s_mapFileDirMonitor.size();
	std::map<CString,CFileDirMonitor*> ::iterator iter;

	//int cnt(0);
	CFileDirMonitor* pDirMonitor(NULL);
	if (s_mapFileDirMonitor.find(key) != s_mapFileDirMonitor.end())
	{
		pDirMonitor = s_mapFileDirMonitor[key];
		pDirMonitor->m_strFilePath = strPath;
		pDirMonitor->m_strSql  = vec[1];
	}

	int cnt(0);
	for(iter = s_mapFileDirMonitor.begin(); iter != s_mapFileDirMonitor.end(); iter ++){
		pDirMonitor = iter->second;
		if(!pDirMonitor->m_strSql.IsEmpty()){
			cnt ++;
		}
	}

	if (cnt == size)
	{
		BOOL isResult = file.Open(strCfg,CFile::modeWrite|CFile::modeCreate);
		if(isResult){
			for(iter = s_mapFileDirMonitor.begin(); iter != s_mapFileDirMonitor.end(); iter ++){

				strCfg.Format("%s^%s\n",iter->first,iter->second->m_strFilePath);
				file.WriteString(strCfg);
			}
			file.Close();
		}
	}

}

void ReceDelFileLogSql(RECEIVE_INFO& receInfo){
	//CString sTemp;
	//sTemp.Format("%s",receInfo.info);
	//vector<CString> vec;
	//CSupport::split(sTemp,'^',vec);
	//CLock SendLock(g_SendFileLogLock);
	//if(vec.size() == 2){
	//	CString key = vec[0].MakeLower();
	//	if(!vec[1].IsEmpty())
	//		s_MapFileDelSql[key] = vec[1];
	//}

	//if(vec.size() == 3){
	//	CString key = vec[0].MakeLower();
	//	if(vec[1] == "0"){
	//		if(!vec[2].IsEmpty())
	//			s_MapFileDelSql[key] = vec[2];

	//	}else{
	//		if(!vec[2].IsEmpty())
	//			s_MapFileDelSql[key].Replace("@1",vec[2]);
	//	}

	//}
}

void sHeartbeatAndSynchroFrequency(RECEIVE_INFO& receInfo){

	if (s_isConnect)
	{
		char info[2];
		info[0] = 9;
		info[1] = 3;
		send(s_Client,(LPSTR)info,2,0);
	}


	CString sTemp;
	sTemp.Format("%s",receInfo.info);
	vector<CString> vec;
	CSupport::split(sTemp,',',vec);

	if(vec.size() >= 2){
		int time = CSupport::toInt(vec[0]);
		if(time > 0)
			CSupport::SetContinuousOperationTime(time);
		time = CSupport::toInt(vec[1]);
		if(time > 0)
			s_nSynchroFrequency = time;
		//sConnectFileServer(path,vec,6);
	}

	if(vec.size() == 3){
		int heartbeat = CSupport::toInt(vec[2]);
		if(heartbeat > 10){
			s_Heartbeat = heartbeat;
		}
	}

	g_isFirst = FALSE;

	if (receInfo.type == 0)
	{
		return;
	}

	CStdioFile file;
	CString strCfg = s_strCurExePath + "\\config\\filelog.cfg";
	BOOL isResult = file.Open(strCfg,CFile::modeWrite|CFile::modeCreate);
	if(isResult){
		file.WriteString(sTemp);
		file.Close();
	}
}

void sHeartbeat(RECEIVE_INFO& receInfo){
	CString sTemp;
	sTemp.Format("%s",receInfo.info);
	int heartbeat = CSupport::toInt(sTemp);
	if(heartbeat > 10){
		s_Heartbeat = heartbeat;
	}
}

void sSendLinInfo(RECEIVE_INFO& receInfo){
	CString sTemp = sGetLinInfo();
	FILE_LOG sendInfo(27,sTemp);
	send(s_Client,(LPSTR)&sendInfo,sTemp.GetLength()  +2,0);
}

void sFileFliter28(RECEIVE_INFO& receInfo){
	CString sTemp;
	sTemp.Format("%s",receInfo.info);
	vector<CString> vec;
	CSupport::split(sTemp,';',vec);
	CLock lock(CFileFliter::g_Lock2); 
	if(vec.size() == 2){
		int cnt = (int)CFileFliter::m_arrFliter.GetCount();
		for (int ii = 0 ; ii < cnt ; ii ++)
		{
			if(!CFileFliter::m_arrFliter[ii].CompareNoCase(vec[0])){
				CFileFliter::m_arrFliter.RemoveAt(ii);
				break;
			}
		}

		CFileFliter::m_arrFliter.Add(vec[1]);	
	}else{
		CFileFliter::m_arrFliter.Add(sTemp);	
	}
}

void sFileFliter30(RECEIVE_INFO& receInfo){

	char info[2];
	info[0] = 9;
	info[1] = 2;

	send(s_Client,(LPSTR)info,2,0);

	CString sTemp;
	sTemp.Format("%s",receInfo.info);
	vector<CString> vec;
	CSupport::split(sTemp,';',vec);
	CLock lock(CFileFliter::g_Lock2);
	int cnt = vec.size();
	CFileFliter::m_arrFliter.RemoveAll();
	for (int ii = 0 ; ii < cnt ;ii ++)
	{
		if(!vec[ii].IsEmpty())
			CFileFliter::m_arrFliter.Add(vec[ii]);	
	}
	if (g_isFirst)
	{
		Sleep(500);
		char chType(4);
		send(s_Client,(LPSTR)&chType,1,0);
	}

}

//
void sFileFliter29(RECEIVE_INFO& receInfo){
	CString sTemp;
	sTemp.Format("%s",receInfo.info);
	int cnt = (int)CFileFliter::m_arrFliter.GetCount();
	for (int ii = 0 ; ii < cnt ; ii ++)
	{
		if(!CFileFliter::m_arrFliter[ii].CompareNoCase(sTemp)){
			CLock lock(CFileFliter::g_Lock2);
			CFileFliter::m_arrFliter.RemoveAt(ii);
			break;
		}
	}
}

void sSetRoomId(RECEIVE_INFO& receInfo){

	if (s_isConnect)
	{
		char info[2];
		info[0] = 9;
		info[1] = 1;
		send(s_Client,(LPSTR)info,2,0);
	}


	CString sTemp;
	sTemp.Format("%s",receInfo.info);
	vector<CString> vec;
	CSupport::split(sTemp,',',vec);

	s_RoomId = vec[0];
	//s_RoomId.Format("%s",receInfo.info);

	int size = vec.size();
	if (size >= 2)
	{
		CFileTransferClient::s_version  = CSupport::toInt(vec[1]);
	}
	if (size >= 5)
	{
		

		int time = CSupport::toInt(vec[2]);
		if(time > 0)
			CSupport::SetContinuousOperationTime(time);
		time = CSupport::toInt(vec[3]);
		if(time > 0)
			s_nSynchroFrequency = time;
		//sConnectFileServer(path,vec,6);

		int heartbeat = CSupport::toInt(vec[4]);
		if(heartbeat > 10){
			s_Heartbeat = heartbeat;
		}
	}

	if (receInfo.type == 0)
	{
		return;
	}

	CStdioFile file;
	CString strCfg = s_strCurExePath + "\\config\\configServer.cfg";
	BOOL isResult = file.Open(strCfg,CFile::modeWrite|CFile::modeCreate);
	if(isResult){
		file.WriteString(sTemp);
		file.Close();
	}



}

void sSetAutoConnect(RECEIVE_INFO& receInfo){
	s_isAutoConnect = FALSE;
}

void sRenewalLicense(RECEIVE_INFO& receInfo){
	CString str = s_strCurExePath + "\\config\\Dms.lic";
	CStdioFile file;
	BOOL isResult = file.Open(str,CFile::modeWrite|CFile::modeCreate);
	if(isResult){
		file.Write(receInfo.info,receInfo.len);
		file.Close();
		if(sOpenServerLicense()){
			char chLicType;
			if(sCheckLicence(chLicType)){
				CString sTemp = "";
				FILE_LOG sendInfo(27,sTemp);
				send(s_Client,(LPSTR)&sendInfo,sTemp.GetLength()  +2,0);
				s_isAutoConnect = true;
			}
		}
	}
}

DWORD WINAPI ThreadProcFun(LPVOID lpParam)  
{ 
	RECEIVE_INFO* preceInfo = (RECEIVE_INFO*)lpParam;

	RECEIVE_INFO& receInfo = *preceInfo;
	CString sTemp;
	if(s_pListInfo && receInfo.type < 40 ){
		if(receInfo.type == 15)
			sTemp.Format("%d:\tclose auto connect",receInfo.type);
		else if(receInfo.type == 26 )
			sTemp.Format("%d:\tLicense",receInfo.type);
		else if(receInfo.type != 3 && receInfo.type != 6)
			sTemp.Format("%d:%d:\t%s",receInfo.type,s_Client,receInfo.info);

		s_pListInfo->AddString(sTemp);
	}

	if(receInfo.type < 40 && receInfo.type >= 0)
		m_PFunReceInfos[receInfo.type](receInfo);

	delete preceInfo;
	return 1;
}

DWORD WINAPI ThreadProcRead(LPVOID lpParam)  
{ 
	char szMessage[MSGSIZE]; 
	int ret;
	s_isRun = TRUE;
	char chType(4);
	send(s_Client,(LPSTR)&chType,1,0);
	
	while(s_isRun){
		ret = recv(s_Client, szMessage, MSGSIZE, 0); 
		if(ret <= 0){
			//stopMonitorFile();
			s_isRun = FALSE;
			s_isConnect = FALSE;
			if(s_pListInfo)
				s_pListInfo->AddString("断开连接");
			if(s_hThreadListen == NULL)
				s_hThreadListen = CreateThread(NULL, 0, ThreadProcServer, NULL, 0, NULL);
			TRACE("断开连接:\t%d",s_Client);
			break;
		}
		//szMessage[ret] = '\0';
		// TODO: Add your specialized code here and/or call the base class
		RECEIVE_INFO* receInfo = new RECEIVE_INFO(0);

		memcpy(receInfo,szMessage,ret);
		receInfo->len = ret;

		HANDLE hThread = CreateThread(NULL, 0, ThreadProcFun, receInfo, 0, NULL);
		if(INVALID_HANDLE_VALUE == hThread){
			delete receInfo;
		}
		


	}
	
	TRACE("closesocket:\t%d",s_Client);
	closesocket(s_Client); 
	TRACE("closesocked:\t%d",s_Client);
	//WSACleanup();
	s_hThread = NULL;
	return 1;
}

BOOL isConnect(){
	if(s_hThread != NULL)
		return TRUE;
	else
		return FALSE;
}

void sCloseUniverDB(){
	if(s_SuperDB.IsConnect()){
		s_SuperDB.Close();
		s_SuperDB.Disconnect();
		
	}

	if(s_MonSuperDB.IsConnect()){
		s_MonSuperDB.Close();
		s_MonSuperDB.Disconnect();

	}

	//if (s_UniverDB.IsConnect())
	//{
	//	s_UniverDB.Close();
	//	s_UniverDB.Disconnect();
	//}
	
}

int sConnectServer()      
{  
	char chLicType;
	s_licensedDays = sCheckLicence(chLicType);
	if(s_hThread )
		return 0;
	if(s_licensedDays < 1)
	{
		if (CSupport::GetServerIP().CompareNoCase("127.0.0.1")){
			if(s_hThreadListen == NULL){
				s_hThreadListen = CreateThread(NULL, 0, ThreadProcServer, NULL, 0, NULL);
				s_isAutoConnect = FALSE;
			}
			return 0; 
		}
	}
	//连接所用套节字      
	      
	//保存远程服务器的地址信息      
	SOCKADDR_IN server;           

	// 创建客户端套节字 

	s_isConnect = FALSE;
	s_Client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //AF_INET指明使用TCP/IP协议族；      
	//SOCK_STREAM, IPPROTO_TCP具体指明使用TCP协议      
	// 指明远程服务器的地址信息(端口号、IP地址等)      
	memset(&server, 0, sizeof(SOCKADDR_IN)); //先将保存地址的server置为全0      
	server.sin_family = PF_INET; //声明地址格式是TCP/IP地址格式      
	server.sin_port = htons(CSupport::GetWebPort()); //PORT;//指明连接服务器的端口号，htons()用于 converts values between the host and network byte order      
	server.sin_addr.s_addr = inet_addr(CSupport::GetServerIP()); //指明连接服务器的IP地址      
	//结构SOCKADDR_IN的sin_addr字段用于保存IP地址，sin_addr字段也是一个结构体，sin_addr.s_addr用于最终保存IP地址      
	//inet_addr()用于将 形如的"127.0.0.1"字符串转换为IP地址格式      
	//连到刚才指明的服务器上      
	int result = connect(s_Client, (struct sockaddr *) &server, sizeof(SOCKADDR_IN)); //连接后可以用sClient来使用这个连接      

	if(INVALID_SOCKET == s_Client || result == -1)  
	{  
		s_Client = NULL;
		if(s_hThreadListen == NULL)
			s_hThreadListen = CreateThread(NULL, 0, ThreadProcServer, NULL, 0, NULL);
		return -1;  
	} else{
		s_isConnect = TRUE;
		//s_BrokenNetTime = 0;
		s_hThread = CreateThread(NULL, 0, ThreadProcRead, NULL, 0, NULL);
		CreateThread(NULL, 0, ThreadProcSendOldFileLog, NULL, 0, NULL);

	} 


	return 0;      
} 

DWORD WINAPI ThreadProcSendFile(LPVOID lpParam){

	CString* pStr = (CString*)lpParam;
	CString sTemp = *pStr;
	delete pStr;

	sTemp.Replace(":","");
	vector<CString> vec;
	CSupport::split(sTemp,',',vec);

	int size = vec.size();
	if(size > 1){
		CString path = vec[0];
		int type = CSupport::toInt(vec[size - 1]);
		vec.erase(vec.begin());
		
		CFileTransferClient::ConnectFileServer(path,vec,type,s_isConnect);
	}
	
	return 1;
}

//
//DWORD WINAPI ThreadProcFileRead(LPVOID lpParam)  
//{ 
//
//	SOCKET FileClient = (SOCKET)lpParam;
//	char szMessage[MSGSIZE]; 
//	int ret;
//	BOOL isRun = TRUE;
//
//	CString strInfo;
//
//	if (s_pListInfo)
//	{
//		strInfo.Format("%d connect",FileClient);
//		s_pListInfo->AddString(strInfo);
//	}
//	while(isRun){
//		ret = recv(FileClient, szMessage, MSGSIZE, 0); 
//		if(ret <= 0){
//			//stopMonitorFile();
//			isRun = FALSE;
//
//			if (s_pListInfo)
//			{
//				strInfo.Format("%d Disconnect",FileClient);
//				s_pListInfo->AddString(strInfo);
//			}
//
//			TRACE("断开连接:\t%d",FileClient);
//			break;
//		}
//		//szMessage[ret] = '\0';
//		// TODO: Add your specialized code here and/or call the base class
//		RECEIVE_INFO receInfo(0);
//
//		memcpy(&receInfo,szMessage,ret);
//		receInfo.len = ret;
//		CString sTemp;
//
//
//	}
//
//	TRACE("closesocket:\t%d",FileClient);
//	closesocket(FileClient); 
//	TRACE("closesocked:\t%d",FileClient);
//
//	return 1;
//}

BOOL stopMonitorFile(){
	shutdown(s_Client,SD_BOTH);
	closesocket(s_Client);
	s_isRun = FALSE;
	TerminateThread(s_hThread, 0);

	s_hThread = NULL;

	WSACleanup();

	sCloseUniverDB();

	std::map<CString,CFileDirMonitor*> ::iterator iter;
	for(iter = s_mapFileDirMonitor.begin(); iter != s_mapFileDirMonitor.end(); iter ++){
		if (iter->second)
		{
			delete iter->second;
		}
	}

	s_mapFileDirMonitor.clear();

	return TRUE;
}

short dbCheckAndCreateTable(LPCTSTR strTable)
{

	CUniverDB SuperDB;
	//CString strAccessPath = s_strCurExePath + "\\Config\\DMS_FILE_LOG.mdb;;" + g_strDBPassword;
	//CString strCnnParam = "Access;;" +strAccessPath;
	BOOL isConnected = SuperDB.Connect(g_strCnnParam);
	if(!isConnected){
		return (short)0;
	}

	// 获得数据表个数
	int cnt = SuperDB.GetTblCount();
	CString str =(LPCSTR)strTable; 
	CString strTableName;

	//if(!SuperDB.Execute("drop table LITHOLOGY",FALSE))
	//	return (short)0;
	//else
	//	return (short)1;

	//CString strSql = "drop table DMS_FILE_LOGS";// + strTableName;
	//if(!SuperDB.Execute(strSql,FALSE))
	//	return (short)0;
	//else
	//	return (short)1;


	int ii;
	for (ii = 0; ii < cnt ; ii ++)
	{
		// 取数据表名称
		strTableName = SuperDB.GetTblName(ii);
		//CString strSql = "drop table " + strTableName;
		//if(!SuperDB.Execute(strSql,FALSE))
		//	return (short)0;
		//else
		//	return (short)1;

		TRACE("%s\n",strTableName);

		if(!strTableName.CompareNoCase(str))
			break;
	}
	if(ii >= cnt)
	{
		if(!str.CompareNoCase("DMS_FILE_LOGS"))
		{
			CString strSql = "CREATE TABLE DMS_FILE_LOGS (";
			strSql += "ID	 integer,";
			strSql += "BATCH_NUMBER integer,";
			strSql += "FILE_PATH  text(255),";
			strSql += "FILE_PATH2  text(255),";
			strSql += "FILE_DIR  text(255),";
			strSql += "FILE_LOG  text(255),";

			strSql += "HANDLE  integer,";

			strSql += "IS_CREATE  integer,";
			strSql += "DESCRIBE1  text(255),";

			strSql += "HANDLER  text(255),";

			strSql += "FILE_SIZE   integer,";
			strSql += "FILE_NAME  text(255),";
			strSql += "CREATE_TIME    integer,";
			strSql += "FILE_LOG_DATE  DATETIME,";
			strSql += "CREATE_DATE    DATETIME,";

			strSql += "UPDATE_DATE    DATETIME);";//,primary key(ID)

			if(!SuperDB.Execute(strSql,FALSE))
				return (short)0;
			else
				return (short)1;

		}

		if(!str.CompareNoCase("DMS_FILE_LOGS2"))
		{
			CString strSql = "CREATE TABLE DMS_FILE_LOGS2 (";
			strSql += "ID	 integer,";
			strSql += "BATCH_NUMBER integer,";
			strSql += "FILE_PATH  text(255),";
			strSql += "FILE_PATH2  text(255),";
			strSql += "FILE_DIR  text(255),";
			strSql += "FILE_LOG  text(255),";

			strSql += "HANDLE  integer,";

			strSql += "IS_CREATE  integer,";
			strSql += "DESCRIBE1  text(255),";

			strSql += "HANDLER  text(255),";

			strSql += "FILE_SIZE   integer,";
			strSql += "FILE_NAME  text(255),";
			strSql += "CREATE_TIME    integer,";
			strSql += "FILE_LOG_DATE  DATETIME,";
			strSql += "CREATE_DATE    DATETIME,";

			strSql += "UPDATE_DATE    DATETIME);";//,primary key(ID)

			if(!SuperDB.Execute(strSql,FALSE))
				return (short)0;
			else
				return (short)1;

		}

		if(!str.CompareNoCase("DMS_FILE_LOG"))
		{
			CString strSql = "CREATE TABLE DMS_FILE_LOG(";
			strSql += "ID	 AUTOINCREMENT,";
			strSql += "FILE_NAME  text(255),";
			strSql += "FILE_LOG  text(255),";

			strSql += "CREATE_DATE    DATETIME,primary key(ID));";

			if(!SuperDB.Execute(strSql,FALSE))
				return (short)0;
			else
				return (short)1;

		}

		if(!str.CompareNoCase("CURRENT_QUERY"))
		{
			CString strSql = "CREATE TABLE CURRENT_QUERY (";
			strSql += "BATCH_NUMBER integer,";
			strSql += "FILE_PATH  text(255),";
			strSql += "FILE_NAME  text(255),";
			strSql += "FILE_DIR  text(255));";//,primary key(ID)

			if(!SuperDB.Execute(strSql,FALSE))
				return (short)0;
			else
				return (short)1;

		}

		if(!str.CompareNoCase("LAST_DATE"))
		{
			CString strSql = "CREATE TABLE LAST_DATE (";
			strSql += "OPTION_NAME  text(255),";
			strSql += "HANDLER  text(255),";
			strSql += "FILE_DIR  text(255),";
			strSql += "OPTION_DATE  DATETIME);";//,primary key(ID)

			if(!SuperDB.Execute(strSql,FALSE))
				return (short)0;
			else
				return (short)1;

		}


		if(!str.CompareNoCase("DMS_INFO"))
		{
			CString strSql = "CREATE TABLE DMS_INFO (";
			strSql += "SHUTDOWN_HANDLER  text(255),";
			strSql += "SHUTDOWN_DATE  DATETIME,";
			strSql += "LAST_LOG_HANDLER  text(255),";
			strSql += "LAST_LOG_DATE  DATETIME);";//,primary key(ID)

			if(!SuperDB.Execute(strSql,FALSE))
				return (short)0;
			else
				return (short)1;

		}

	}

	return 0;
}

inline void InsertFileLog(CStringArray& arrFileInfo,ULONGLONG_ARRAY& arrCreateDate,CString& strPath,CString& loginName,CFileStatus& rStatus,FILE_SETS& file_Sets,CTime& startTime){

	CTime time = CTime::GetCurrentTime();
	CString strNewPath = time.Format("%y%m%d%H");//格式化日期时间 
	CString strUUID = strNewPath + "\\" + GUID_Generator();
	strNewPath = CSupport::GetBufDir() + "\\" + strNewPath;
	if(!PathFileExists(strNewPath))//文件夹不存在则创建
		CreateDirectory(strNewPath, NULL);

	CString strNewFileName = CSupport::GetBufDir() + "\\" + strUUID;

	if(::CopyFile(strPath,strNewFileName,FALSE)){

		CString fileTmp =  CSupport::GetEncryptPath(strPath);
		if(!fileTmp.IsEmpty())
		{
			fileTmp += CFileMonitor::GetFileExitName(strPath) + ".tmp";
			CFileMonitor::WriteTmpFile(strPath,fileTmp,strUUID,0,rStatus.m_mtime);
		}

	}


	//vec.push_back(strUUID);

	file_Sets.mSetFileName.insert(strUUID);

	ULONGLONG llTime;
	CString strInfo;
	int pos;
	if (rStatus.m_mtime >= rStatus.m_ctime)//创建
	{
		if (rStatus.m_ctime >= startTime)
		{
			llTime = rStatus.m_ctime.GetTime();
			strInfo = CStatFileLogs::GetFileLogInfo(strPath,"",strUUID,rStatus.m_size,"创建",loginName,"",rStatus.m_ctime,rStatus.m_mtime);
			pos = sAscFind(&arrCreateDate,llTime);
			if(pos < 0){
				pos = -pos -1;
			}

			arrFileInfo.InsertAt(pos,strInfo);
			arrCreateDate.InsertAt(pos,llTime);
		}


		llTime = rStatus.m_mtime.GetTime() + 1;
		strInfo = CStatFileLogs::GetFileLogInfo(strPath,"",strUUID,rStatus.m_size,"修改",loginName,"",rStatus.m_mtime,rStatus.m_mtime);
		pos = sAscFind(&arrCreateDate,llTime);
		if(pos < 0){
			pos = -pos -1;
		}

		arrFileInfo.InsertAt(pos,strInfo);
		arrCreateDate.InsertAt(pos,llTime);
	}else{

		if (rStatus.m_ctime >= startTime)
		{
			llTime = rStatus.m_ctime.GetTime();
			strInfo = CStatFileLogs::GetFileLogInfo(strPath,"",strUUID,rStatus.m_size,"创建",loginName,"",rStatus.m_ctime,rStatus.m_mtime);
			pos = sAscFind(&arrCreateDate,llTime);
			if(pos < 0){
				pos = -pos -1;
			}

			arrFileInfo.InsertAt(pos,strInfo);
			arrCreateDate.InsertAt(pos,llTime);


			strInfo = CStatFileLogs::GetFileLogInfo(strPath,"",strUUID,rStatus.m_size,"修改",loginName,"",rStatus.m_ctime,rStatus.m_mtime);
			pos++;
			arrFileInfo.InsertAt(pos,strInfo);
			arrCreateDate.InsertAt(pos,llTime);
		}

	}
}


///////////////////////////////////////////////////////////////////////////// 
// 函数说明: 遍历文件夹 
// 参数说明: [in]：strFile 遍历的文件夹(此方法会主动向路径末尾添加*.*) 
// 返回值:BOOL类型,存在返回TRUE,否则为FALSE 
///////////////////////////////////////////////////////////////////////////// 
int BrowseFile(CString &strFile,CStringArray& arrFileInfo,ULONGLONG_ARRAY& arrCreateDate,CTime& startTime,CString& loginName,FILE_SETS& file_Sets) 
{ 
	CFileFind ff; 
	CString szDir = strFile; 

	if(szDir.Right(1) != L"\\") 
		szDir += L"\\"; 

	szDir += L"*.*"; 
	int cnt = 0;
	BOOL res = ff.FindFile(szDir); 
	CString strTime,strCTime;
	CString strInfo;
	while(res) 
	{ 
		res = ff.FindNextFile(); 
		if(ff.IsDirectory() && !ff.IsDots()) 
		{ 
			//如果是一个子目录，用递归继续往深一层找 
			CString strPath = ff.GetFilePath(); 

			BrowseFile(strPath,arrFileInfo,arrCreateDate,startTime,loginName,file_Sets); 
			cnt ++;
		} 
		else if(!ff.IsDirectory() && !ff.IsDots()) 
		{ 
			//显示当前访问的文件(完整路径) 
			CString strPath = ff.GetFilePath();

			CFileStatus rStatus;
			if(!CFile::GetStatus(strPath,rStatus)){
				continue;
			}

			if (rStatus.m_mtime < startTime && rStatus.m_ctime < startTime)
			{
				continue;
			}

			file_Sets.mSetFilePath.insert(strPath);

			InsertFileLog(arrFileInfo,arrCreateDate,strPath,loginName,rStatus,file_Sets,startTime);

			TRACE(strInfo);
			TRACE("\n");

			cnt ++;
		} 
	} 

	//关闭 
	ff.Close(); 

	return cnt;
} 


DWORD WINAPI ThreadScanningLogFile2(LPVOID lpParam)  
{ 
	CUniverDB UniverDB;

	BOOL isConnected = UniverDB.Connect(g_strCnnParam);

	if (!isConnected)
	{
		return 0;
	}

	CString strSql("SELECT Max(A.FILE_LOG_DATE) AS MAX_DATE, A.FILE_DIR FROM ");
	
		strSql += "(SELECT * FROM  DMS_FILE_LOGS UNION  SELECT * FROM  DMS_FILE_LOGS2) AS A ";
		strSql += "WHERE A.FILE_DIR<>'' GROUP BY A.FILE_DIR";


	COleDateTime maxLogTime;
	COleDateTime LogTime;
	CString strFileDir;
	BOOL isHave = FALSE;
	if (UniverDB.OpenSQL(strSql))
	{
		CString str;
		CString loginName = sGetCurrentUserName();

		while (!UniverDB.IsEOF())
		{
			LogTime = UniverDB.GetDateTime("MAX_DATE");

			if (!isHave)
			{
				maxLogTime = LogTime;
				strFileDir = UniverDB.GetString("FILE_DIR");
			}else{
				if (LogTime > maxLogTime)
				{
					maxLogTime = LogTime;
					strFileDir = UniverDB.GetString("FILE_DIR");
				}
			}
			isHave = TRUE;
			UniverDB.MoveNext();
		}


		if (isHave)
		{

			FILE_SETS fileSets;
			CStringArray arrFileInfo;
			ULONGLONG_ARRAY arrCreateDate;
			CTime time(maxLogTime.GetYear(),maxLogTime.GetMonth(),maxLogTime.GetDay(),maxLogTime.GetHour(),maxLogTime.GetMinute(),maxLogTime.GetSecond());
			ULONGLONG llTime = time.GetTime();

			//llTime -= 10;

			CTime startTime(llTime);

			//CString  strDir;
			//strDir.Format("%s\\3\\",CSupport::GetBufDir());
			//if(!PathFileExists(strDir))//文件夹不存在则创建-1
			//	CreateDirectory(strDir, NULL);

			time = CTime::GetCurrentTime();
			//CString str =time.Format("%Y_%m_%d_%H_%M_%S");//格式化日期时间  
			//CString strFilePath;
			//strFilePath.Format("%supload%s.txt",strDir ,str);

			CString strPath = time.Format("%y%m%d%H");//格式化日期时间 	
			CString strLogFilePathDir = CSupport::GetBufDir() + "\\LogFile\\" +strPath;
			if(!PathFileExists(strLogFilePathDir))//文件夹不存在则创建
				CreateDirectory(strLogFilePathDir, NULL);

			CString fileName = time.Format("\\%Y_%m_%d_%H_%M_%S.txt");//格式化日期时间 

			strLogFilePathDir += fileName;

			vector<CString> vec;
			vec.push_back(strLogFilePathDir);
			//std::map<CString,CFileDirMonitor*> ::iterator iter;

			BrowseFile(strFileDir,arrFileInfo,arrCreateDate,startTime,loginName,fileSets);
			
			if (arrFileInfo.GetCount() > 0)
			{

				CStdioFile file;
				if(file.Open(strLogFilePathDir,CFile::typeText|CFile::modeWrite| CFile::modeCreate | CFile::shareDenyWrite))
				{
					if (CFileTransferClient::s_version >= 2)
					{
						//CString strTime;
						//strTime.Format("%s%s",strPath,fileName);
						//strTime.Format("%I64d000\n",time.GetTime());
						//file.WriteString(strTime);
						strPath += fileName ;
						file.WriteString(strPath + "\n");
					}
					int cnt = (int)arrFileInfo.GetCount();
					CString strFileLog;
					for(int ii = 0 ; ii < cnt;  ii ++){

						strFileLog = arrFileInfo[ii] + "\n";
						file.WriteString(strFileLog);
					}
					file.Close();

					if(CSupport::GetServerIP().CompareNoCase("127.0.0.1")){
					fileSets.strLogFile = strLogFilePathDir;
					CFileTransferClient::SendOnceFile(s_RoomId,fileSets,s_isConnect,3);
					}else{

						if (CFileTransferClient::s_version >= 1)
						{
							FILE_LOG sendInfo(10,fileSets.strLogFile);
							int result = send(s_Client,(LPSTR)&sendInfo,fileSets.strLogFile.GetLength() + 2,0);
						}

					}

				}
			}

		}


		UniverDB.Disconnect();

	}
	return 1;
}

///////////////////////////////////////////////////////////////////////////// 
// 函数说明: 遍历文件夹 
// 参数说明: [in]：strFile 遍历的文件夹(此方法会主动向路径末尾添加*.*) 
// 返回值:BOOL类型,存在返回TRUE,否则为FALSE 
///////////////////////////////////////////////////////////////////////////// 
int BrowseFile(CString &strFile,CStringArray& arrFileInfo,ULONGLONG_ARRAY& arrCreateDate,CString& loginName,FILE_SETS& file_Sets,CTime& startTime,CTime& endTime) 
{ 
	CFileFind ff; 
	CString szDir = strFile; 

	if(szDir.Right(1) != L"\\") 
		szDir += L"\\"; 

	szDir += L"*.*"; 
	int cnt = 0;
	BOOL res = ff.FindFile(szDir); 
	CString strTime,strCTime;
	CString strInfo;
	while(res) 
	{ 
		res = ff.FindNextFile(); 
		if(ff.IsDirectory() && !ff.IsDots()) 
		{ 
			//如果是一个子目录，用递归继续往深一层找 
			CString strPath = ff.GetFilePath(); 

			BrowseFile(strPath,arrFileInfo,arrCreateDate,loginName,file_Sets,startTime,endTime); 
			cnt ++;
		} 
		else if(!ff.IsDirectory() && !ff.IsDots()) 
		{ 
			//显示当前访问的文件(完整路径) 
			CString strPath = ff.GetFilePath();

			CFileStatus rStatus;
			if(!CFile::GetStatus(strPath,rStatus)){
				continue;
			}

			if (startTime > 0 && rStatus.m_mtime < startTime && rStatus.m_ctime < startTime)
			{
				continue;
			}

			if (endTime > 0 &&  rStatus.m_mtime > endTime && rStatus.m_ctime > endTime)
			{
				continue;
			}

			file_Sets.mSetFilePath.insert(strPath);

			InsertFileLog(arrFileInfo,arrCreateDate,strPath,loginName,rStatus,file_Sets,startTime);

			TRACE(strInfo);
			TRACE("\n");

			cnt ++;
			

		} 
	} 

	//关闭 
	ff.Close(); 

	return cnt;
} 

CTime sStartTime;
CTime sEndTime;

BOOL SendFileLog(CStringArray& arrSendFileLogs){
	int cnt = (int)arrSendFileLogs.GetCount();
	CString strFileLog("");



	CString str("");
	//return TRUE;
	for(int ii = 0 ; ii < cnt;  ii ++){
		strFileLog = arrSendFileLogs[ii];

		TRACE("%d\t%s\n",ii,strFileLog);

		//if(s_pListInfo){
		//	s_pListInfo->AddString(strFileLog);
		//}
		if(str.GetLength() + strFileLog.GetLength() > 1020)
		{
			if(!str.IsEmpty()){
				str += "]";
				FILE_LOG sendInfo(3,str);
				//if(str.GetLength() > 1020)
				//	TRACE("%d\t%d\t%s\n",str.GetLength(),strFileLog.GetLength(),str);
				int result = send(s_Client,(LPSTR)&sendInfo,str.GetLength() + 2,0);
				if(result<=0){
					return FALSE;
				}
				str.Empty();
				Sleep(1000);
			}

			str =  "[" + strFileLog;

		}else if(str.IsEmpty()){
			str += "[" + strFileLog;

		}else{
			str += "," + strFileLog;
		}
	}

	if(!str.IsEmpty()){
		str += "]";
		FILE_LOG sendInfo(3,str);
		//TRACE("%s\n",str);
		int result = send(s_Client,(LPSTR)&sendInfo,str.GetLength() + 1,0);

		if(result<=0){
			return FALSE;
		}
	}

	arrSendFileLogs.RemoveAll();
	return TRUE;
}

DWORD WINAPI ThreadScanningLogFile(LPVOID lpParam)  
{ 
	FILE_SETS fileSets;
	CStringArray arrFileInfo;
	ULONGLONG_ARRAY arrCreateDate;
	CString loginName = sGetCurrentUserName();

	CTime time = CTime::GetCurrentTime();
	CString strLogFilePathDir = CSupport::GetBufDir() + "\\LogFile\\";
	if(!PathFileExists(strLogFilePathDir))//文件夹不存在则创建
		CreateDirectory(strLogFilePathDir, NULL);

	CString strPath = time.Format("%y%m%d%H");//格式化日期时间 	
	strLogFilePathDir += strPath;
	if(!PathFileExists(strLogFilePathDir))//文件夹不存在则创建
		CreateDirectory(strLogFilePathDir, NULL);

	CString fileName = time.Format("\\%Y_%m_%d_%H_%M_%S.txt");//格式化日期时间 

	strLogFilePathDir += fileName;

	vector<CString> vec;
	vec.push_back(strLogFilePathDir);
	std::map<CString,CFileDirMonitor*> ::iterator iter;

	if (s_pListInfo)
	{
		s_pListInfo->AddString("Start Scanning File.");
	}
	for(iter = s_mapFileDirMonitor.begin(); iter != s_mapFileDirMonitor.end(); iter ++){
		CFileDirMonitor* pMon = (CFileDirMonitor*)iter->second;
		CString baseDir(pMon->m_strDir);
		BrowseFile(baseDir,arrFileInfo,arrCreateDate,loginName,fileSets,sStartTime,sEndTime);
	}
	
	int cnt = (int)arrFileInfo.GetCount();
	if (s_pListInfo)
	{
		loginName.Format("Scanned %d files",cnt);
		s_pListInfo->AddString(loginName);
	}

	if (cnt > 0)
	{

		if(CFileTransferClient::s_version <= 1){
			SendFileLog(arrFileInfo);
		}else{
			CStdioFile file;
			if(file.Open(strLogFilePathDir,CFile::typeText|CFile::modeWrite| CFile::modeCreate | CFile::shareDenyWrite))
			{
				if (CFileTransferClient::s_version >= 2)
				{
					//CString strTime;
					//strTime.Format("%s%s",strPath,fileName);
					//strTime.Format("%I64d000\n",time.GetTime());

					strPath += fileName ;
					file.WriteString(strPath + "\n");
				}

				CString strFileLog;
				for(int ii = 0 ; ii < cnt;  ii ++){

					strFileLog = arrFileInfo[ii] + "\n";
					file.WriteString(strFileLog);
				}
				file.Close();

				fileSets.strLogFile = strLogFilePathDir;

				if (s_pListInfo)
				{
					s_pListInfo->AddString("Start uploading files");
				}

				if(CSupport::GetServerIP().CompareNoCase("127.0.0.1")){
					fileSets.strLogFile = strLogFilePathDir;
					CFileTransferClient::SendOnceFile(s_RoomId,fileSets,s_isConnect,2);
				}else{

					if (CFileTransferClient::s_version >= 1)
					{
						FILE_LOG sendInfo(10,fileSets.strLogFile);
						int result = send(s_Client,(LPSTR)&sendInfo,fileSets.strLogFile.GetLength() + 2,0);
					}
				}

				if (s_pListInfo)
				{
					s_pListInfo->AddString("End of file upload");
				}
			}
		}


	}




	return 1;
}

void ScanningLogFile(CTime& time,CTime& EndTime){
	sStartTime = time;
	sEndTime = EndTime;
	CreateThread(NULL, 0, ThreadScanningLogFile, NULL, 0, NULL);
}


CFileMonitor fileMonitor("");
BOOL startMonitorFile(char szFileName[]){
	CString strCurPath;
	strCurPath.Format("%s",szFileName);

	CBaseSupport::sSetFileBaseMonitor((CFileBaseMonitor*)&fileMonitor);


	strCurPath.ReleaseBuffer();//Must ReleaseBuffer, or GetLength=0
	int pos= strCurPath.ReverseFind(_T('\\'));
	s_strCurExePath = strCurPath.Left(pos);


	CString strAccessPath = s_strCurExePath + "\\Config\\DMS_FILE_LOG.mdb;;";

#ifdef _DEBUG
	strAccessPath += "";
#else
	strAccessPath += "jhtx8989";
#endif

	g_strCnnParam= "Access;;" +strAccessPath;

	CString strCfg = s_strCurExePath + "\\config\\config.cfg";

	char lpszBaseBoard[128];
	if(GetBaseBoardByCmd(lpszBaseBoard, 128)){
		s_strBaseBoard.Format("%s",lpszBaseBoard);
	}

	s_strCPUID = GetCPUID();

	CStdioFile file;
	BOOL isResult = file.Open(strCfg,CFile::modeRead);
	if(isResult){

		CString strText;
		while(file.ReadString( strText )){
			if(strText.Find(_T("ServerIP")) >= 0){
				pos = strText.Find(_T("=")) + 1;
				strText = strText.Right(strText.GetLength() - pos);
				strText.Trim();
				CSupport::SetServerIP(strText);
				TRACE(_T("ServerIP:%s\n"),strText);
			}else if(strText.Find(_T("dir")) >= 0){
				pos = strText.Find(_T("=")) + 1;
				strText = strText.Right(strText.GetLength() - pos);
				TRACE(_T("dir:%s\n"),strText);
			}else if(strText.Find(_T("logPort")) >= 0){
				pos = strText.Find(_T("=")) + 1;
				strText = strText.Right(strText.GetLength() - pos);
				strText.Trim();
				CSupport::SetLogPort(strText);
				TRACE(_T("logPort:%s\n"),strText);
			}else if(strText.Find(_T("webPort")) >= 0){
				pos = strText.Find(_T("=")) + 1;
				strText = strText.Right(strText.GetLength() - pos);
				strText.Trim();
				CSupport::SetWebPort(strText);
				TRACE(_T("logPort:%s\n"),strText);
			}else if(strText.Find(_T("filePort")) >= 0){
				pos = strText.Find(_T("=")) + 1;
				strText = strText.Right(strText.GetLength() - pos);
				strText.Trim();
				CSupport::SetFilePort(strText);
				TRACE(_T("filePort:%s\n"),strText);
			}else if(strText.Find(_T("bufDir")) >= 0){
				pos = strText.Find(_T("=")) + 1;
				strText = strText.Right(strText.GetLength() - pos);
				strText.Trim();
				CSupport::SetBufDir(strText);
				TRACE(_T("bufDir:%s\n"),strText);
			}else if(strText.Find(_T("otherExt")) >= 0){
				pos = strText.Find(_T("=")) + 1;
				strText = strText.Right(strText.GetLength() - pos);
				strText.Trim();
				s_strOtherSharedExt = strText;
				TRACE(_T("otherExt:%s\n"),strText);
			}else if(strText.Find(_T("BackupFlag")) >= 0){
				pos = strText.Find(_T("=")) + 1;
				strText = strText.Right(strText.GetLength() - pos);
				strText.Trim();
				//CFileMonitor::s_nFlag = CSupport::toInt(strText);
				TRACE(_T("BackupFlag:%s\n"),strText);
			}else if(strText.Find(_T("InsertCreateLog")) >= 0){
				pos = strText.Find(_T("=")) + 1;
				strText = strText.Right(strText.GetLength() - pos);
				strText.Trim();
				if (strText.CompareNoCase("TRUE"))
				{
					g_isInsertCreateLog = FALSE;
				}else{
					g_isInsertCreateLog = TRUE;
				}

				TRACE(_T("BackupFlag:%s\n"),strText);
			}else if(strText.Find(_T("IsCopyDB")) >= 0){
				pos = strText.Find(_T("=")) + 1;
				strText = strText.Right(strText.GetLength() - pos);
				strText.Trim();
				if (strText.CompareNoCase("TRUE"))
				{
					g_isInsertCreateLog = FALSE;
				}else{
					g_isInsertCreateLog = TRUE;
				}

				TRACE(_T("BackupFlag:%s\n"),strText);
			}else if(strText.Find(_T("FlagFile")) >= 0){
				pos = strText.Find(_T("=")) + 1;
				strText = strText.Right(strText.GetLength() - pos);
				strText.Trim();
				s_strFlagFile = strText;

				TRACE(_T("BackupFlag:%s\n"),strText);
			}
			// 
		}
		file.Close();

	}

	strCfg = s_strCurExePath + "\\config\\configServer.cfg";
	isResult = file.Open(strCfg,CFile::modeRead);
	if(isResult){
		CString sTemp;
		file.ReadString(sTemp);
		RECEIVE_INFO receInfo(0,sTemp);
		sSetRoomId(receInfo);

		file.Close();
	}

	strCfg = s_strCurExePath + "\\config\\filelog.cfg";

	isResult = file.Open(strCfg,CFile::modeRead);
	if(isResult){
		CString sTemp;
		file.ReadString(sTemp);
		RECEIVE_INFO receInfo(0,sTemp);
		sHeartbeatAndSynchroFrequency(receInfo);

		file.Close();
	}

	//fileMonitor.m_hideDirectoryPath = CSupport::GetBufDir();
	sOpenServerLicense();
	
	sGetFileLogM();

	strCfg = s_strCurExePath + "\\config\\file.cfg";
	isResult = file.Open(strCfg,CFile::modeRead);
	if(isResult){
		CString strText;
		while(file.ReadString( strText )){
			sAddMonitoring(strText);
		}
		file.Close();
	}

	CreateThread(NULL, 0, ThreadScanningLogFile2, NULL, 0, NULL);

    //s_hThreadListen = CreateThread(NULL, 0, ThreadProcServer, NULL, 0, NULL);
	CreateThread(NULL, 0, ThreadProcOnTime, NULL, 0, NULL);
	
	CreateThread(NULL, 0, ThreadProcOnTimeHeartbeat, NULL, 0, NULL);

	CreateThread(NULL, 0, ThreadProcOnTime2, NULL, 0, NULL);

	CreateThread(NULL, 0, ThreadProcCheckSendFileLog, NULL, 0, NULL);

	CreateThread(NULL, 0, ThreadProcOnTimeSend, NULL, 0, NULL);


	short ll = dbCheckAndCreateTable("DMS_FILE_LOGS");
	dbCheckAndCreateTable("DMS_FILE_LOGS2");
	dbCheckAndCreateTable("DMS_FILE_LOG");
	//dbCheckAndCreateTable("CURRENT_QUERY");
	dbCheckAndCreateTable("LAST_DATE");

	strCfg = s_strCurExePath + "\\config\\fileLogsql.sql";

	//CStdioFile file;
	if(!file.Open(strCfg,CFile::modeRead))//写入文件
		return TRUE;


	CString str("");
	vector<CString> vec;
	while(file.ReadString(str)){
		vec.clear();
		CSupport::split(str,'^',vec);
		if (vec.size() == 2)
		{
			if (s_mapFileDirMonitor.find(vec[0]) != s_mapFileDirMonitor.end())
			{
				s_mapFileDirMonitor[vec[0]]->m_strFilePath = vec[1];
			}
		}
		//s_defaultSql += strs
		//s_defaultSql += " ";
	}
	TRACE(s_defaultSql + "\n");
	file.Close();


	//strCfg = s_strCurExePath + "\\config\\test.txt";
	////CStdioFile file;
	//isResult = file.Open(strCfg,CFile::modeWrite|CFile::modeCreate);
	//if(isResult){
	//	CString strUser = sGetCurrentUserName();
	//	file.WriteString(strUser);
	//	file.Close();
	//}

	return TRUE;
}

CString sGetLinInfo(){
	CStringArray arrMac;
	CEncrypts::GetMacAddress(&arrMac);
	CString strResult = "";
	int cnt = (int)arrMac.GetCount();
	for (int ii = 0 ; ii < cnt ; ii ++)
	{
		strResult += "MacAddress = ";
		strResult += arrMac[ii] + "\r\n";
	}
	strResult += "MotherboardSN = ";
	strResult += s_strBaseBoard + "\r\n" ;
	strResult += "CPUSerial = ";
	strResult += s_strCPUID + "\r\n" ;
	return strResult;
}

void sGetMonitoringParam(int ips[],int& port,int& webPort,int& filePort,CString& bufDir,CString& strOtherSharedExt,CString& strFileFlag){
	CSupport::GetIntsIP(ips);
	port = CSupport::GetLogPort();
	filePort = CSupport::GetFilePort();
	bufDir = CSupport::GetBufDir();
	webPort = CSupport::GetWebPort();
	//ExBackupTime = CSupport::GetExtendBackupTime();
	//nFlag = CFileMonitor::s_nFlag;
	strOtherSharedExt = s_strOtherSharedExt;
	strFileFlag = s_strFlagFile;
}

void sSetMonitoringParam(const CString& ip,const CString& port,const CString&  filePort,
						 const CString& bufDir,const CString& strOtherSharedExt,const CString& strFileFlag){
	CSupport::SetServerIP(ip);
	CSupport::SetLogPort(port);
	CSupport::SetFilePort(filePort);
	CSupport::SetBufDir(bufDir);
	//CSupport::SetExtendBackupTime(ExBackupTime);
	//CFileMonitor::s_nFlag = nFlag;
	s_strOtherSharedExt = strOtherSharedExt;
	s_strFlagFile = strFileFlag;
}


void sCancelMonitoring(RECEIVE_INFO& receInfo){

	CString dir;
	dir.Format("%s",receInfo.info);

	dir.MakeLower();

	if(s_mapFileDirMonitor.find(dir) != s_mapFileDirMonitor.end()){
		delete s_mapFileDirMonitor[dir];
		s_mapFileDirMonitor.erase(dir);
	}

	saveFileConfig();


}

void sAddMonitoring(RECEIVE_INFO& receInfo){
	CString dir;
	dir.Format("%s",receInfo.info);
	sAddMonitoring(dir);
}

void sAddMonitoring(CString& dir){
	dir.MakeLower();

	if(IsDirExist(dir) && s_mapFileDirMonitor.find(dir) == s_mapFileDirMonitor.end()){
		CFileDirMonitor* pmon = new CFileDirMonitor(dir);
		s_mapFileDirMonitor[dir] = pmon;
		CTime time = CTime::GetCurrentTime();
		pmon->SetLastTime(time);
		pmon->SetStartID(time.GetTime());
		//FILE_LOG sendInfo(8,dir);
		//send(s_Client,(LPSTR)&sendInfo,dir.GetLength() + 2,0);
	}
	return;
}

void sCheckMonitoring(RECEIVE_INFO& receInfo){
	CString sTemp;

	sTemp.Format("%s",receInfo.info);
	vector<CString> vec;
	CSupport::split(sTemp,',',vec);

	std::map<CString,int> map;
	vector<CString>::iterator it;
	for (it = vec.begin(); it != vec.end(); it++){
		CString key = *it;
		if (!key.IsEmpty() && IsDirExist(key))
		{
			key.MakeLower();
			map[key] = 1;
		}

	}
	std::map<CString,CFileDirMonitor*> ::iterator iter;
	{

		CLock lock(g_CheckMoniorLock);
		set<CString> setDel;
	
		for(iter = s_mapFileDirMonitor.begin(); iter != s_mapFileDirMonitor.end(); iter ++){
			CString key = iter->first;
			key.MakeLower();
			std::map<CString,int>::iterator it = map.find(key);
			if(it== map.end()){
				delete iter->second;
				setDel.insert(key);

			}else{
				map.erase(key);
			}
		}

		set<CString>::iterator itSet; //定义前向迭代器
		for(itSet = setDel.begin(); itSet != setDel.end(); itSet++)
		{
			s_mapFileDirMonitor.erase(*itSet);
		}
	}


	std::map<CString,int> ::iterator iterAdd;
	for(iterAdd = map.begin(); iterAdd != map.end(); iterAdd ++){
		CString key = iterAdd->first;
		key.MakeLower();
		iter = s_mapFileDirMonitor.find(key);
		if(iter == s_mapFileDirMonitor.end() && IsDirExist(iterAdd->first)){
			CFileDirMonitor* pFileDriMonitor = new CFileDirMonitor(key);
			s_mapFileDirMonitor[key] = pFileDriMonitor;
			CTime time = CTime::GetCurrentTime();
			pFileDriMonitor->SetLastTime(time);
			pFileDriMonitor->SetStartID(time.GetTime());
		}
	}


	for(iter = s_mapFileDirMonitor.begin(); iter != s_mapFileDirMonitor.end(); iter ++){
		if(iter->second == NULL)
			continue;
		CString key = iter->first;
		key.MakeLower();
		FILE_LOG sendInfo(8,key);
		send(s_Client,(LPSTR)&sendInfo,key.GetLength() + 2,0);
	}

	saveFileConfig();

	//if (s_mapFileDirMonitor.size() == 0)
	//{
		char info[2];
		info[0] = 9;
		info[1] = 0;

		send(s_Client,(LPSTR)info,2,0);
	//}

	return;
}

void saveFileConfig(){
	CString strCfg = s_strCurExePath + "\\config\\file.cfg";
	CStdioFile file;
	BOOL isResult = file.Open(strCfg,CFile::modeWrite | CFile::modeCreate);
	if(isResult){

		std::map<CString,CFileDirMonitor*> ::iterator iter;
		for (iter = s_mapFileDirMonitor.begin(); iter != s_mapFileDirMonitor.end(); iter ++){
			if(iter->second != NULL)
				file.WriteString(iter->first + "\n");
		}

		file.Close();
	}
}

//根据进程名称得到进程token
BOOL GetTokenByName(HANDLE &hToken,LPSTR lpName)
{
	if(!lpName)
	{
		return FALSE;
	}
	HANDLE         hProcessSnap = NULL; 
	BOOL           bRet      = FALSE; 
	PROCESSENTRY32 pe32      = {0}; 

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) 
		return (FALSE); 

	pe32.dwSize = sizeof(PROCESSENTRY32); 

	if (Process32First(hProcessSnap, &pe32)) 
	{
		do 
		{
			if(!strcmp(_strupr(pe32.szExeFile),_strupr(lpName)))
			{
				HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION,
					FALSE,pe32.th32ProcessID);
				bRet = OpenProcessToken(hProcess, TOKEN_ALL_ACCESS,&hToken);
				CloseHandle (hProcessSnap); 
				return (bRet);
			}
		} 
		while (Process32Next(hProcessSnap, &pe32)); 
		bRet = TRUE; 
	} 
	else 
		bRet = FALSE;
	CloseHandle (hProcessSnap); 
	return (bRet);
}

CString sGetCurrentUserName()
{
	HANDLE hToken;

	//得到shell的token
	if(!GetTokenByName(hToken,"EXPLORER.EXE"))
	{
		return FALSE;
	}
	DWORD        cbti     = 0;
	PTOKEN_USER  ptiUser  = NULL;
	SID_NAME_USE snu;

	//取得所需空间大小
	if (GetTokenInformation(hToken, TokenUser, NULL, 0, &cbti)) 
	{
		CloseHandle(hToken);
		return FALSE;
	}

	//分配空间
	ptiUser = (PTOKEN_USER) HeapAlloc(GetProcessHeap(), 0, cbti);
	if(!ptiUser)
	{
		CloseHandle(hToken);
		return FALSE;
	}

	//取得token信息
	if (!GetTokenInformation(hToken, TokenUser, ptiUser, cbti, &cbti))
	{
		CloseHandle(hToken);
		HeapFree(GetProcessHeap(), 0, ptiUser);
		return FALSE;
	}

	char szUser[50];
	char szDomain[50];
	DWORD nUser = 50;
	DWORD nDomain = 50;

	//根据用户的sid得到用户名和domain

	if (!LookupAccountSid(NULL, ptiUser->User.Sid, szUser, &nUser, 
		szDomain, &nDomain, &snu))
	{
		CloseHandle(hToken);
		HeapFree(GetProcessHeap(), 0, ptiUser);
		return FALSE;
	}
	//OutputDebugString(szUser);
	//OutputDebugString("\r\n");
	//OutputDebugString(szDomain);
	//OutputDebugString("\r\n");

	CloseHandle(hToken);
	HeapFree(GetProcessHeap(), 0, ptiUser);

	CString strUserName;
	strUserName.Format("%s",szUser);
	return strUserName;
}

DWORD WINAPI ThreadProcServer(LPVOID lpParam)  
{  
	const int BUF_SIZE = 64;  
	//WSADATA         wsd;            //WSADATA变量  
	//SOCKET          sServer;        //服务器套接字  
	//SOCKET          sClient;        //客户端套接字  
	SOCKADDR_IN     addrServ;;      //服务器地址  
	//char            buf[BUF_SIZE];  //接收数据缓冲区  
	//char            sendBuf[BUF_SIZE];//返回给客户端得数据  
	int             retVal;         //返回值  
	//初始化套结字动态库  
	//if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)  
	//{  
	//	//cout << "WSAStartup failed!" << endl;  
	//	return 1;  
	//}  

	//创建套接字  
	s_Server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);      
	if(INVALID_SOCKET == s_Server)  
	{  
		//cout << "socket failed!" << endl;  
		//WSACleanup();//释放套接字资源;  
		return  -1;  
	}  

	//服务器套接字地址   
	addrServ.sin_family = AF_INET;  
	addrServ.sin_port = htons(CSupport::GetLogPort());  
	addrServ.sin_addr.s_addr = INADDR_ANY;        
	//绑定套接字  
	retVal = bind(s_Server, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));  
	if(SOCKET_ERROR == retVal)  
	{     
		//cout << "bind failed!" << endl;  
		closesocket(s_Server);   //关闭套接字  
		//WSACleanup();           //释放套接字资源;  
		return -1;  
	}  

	while(TRUE){

		//开始监听   
		retVal = listen(s_Server, 1);  
		if(SOCKET_ERROR == retVal)  
		{  
			//cout << "listen failed!" << endl;         
			closesocket(s_Server);   //关闭套接字  
			//WSACleanup();           //释放套接字资源;  
			return -1;  
		}  

		if(!s_hThread ){//&& 

			//接受客户端请求  
			sockaddr_in addrClient;  
			int addrClientlen = sizeof(addrClient);  
			s_Client = accept(s_Server,(sockaddr FAR*)&addrClient, &addrClientlen);  
			if(INVALID_SOCKET == s_Client)  
			{  
				//cout << "accept failed!" << endl;         
				//closesocket(s_Server);   //关闭套接字  
				//WSACleanup();           //释放套接字资源;  
				s_Client = NULL;
				return -1;  
			} else{
				s_hThread = CreateThread(NULL, 0, ThreadProcRead, NULL, 0, NULL);
				s_isConnect = TRUE;
				s_isAutoConnect = TRUE;
				Sleep(4000);
				CreateThread(NULL, 0, ThreadProcSendOldFileLog, NULL, 0, NULL);
				closesocket(s_Server);   //关闭套接字 
				s_hThreadListen = NULL;
				char chLicType;
				if(!sCheckLicence(chLicType)){
					CString sTemp = sGetLinInfo();
					s_isAutoConnect = FALSE;
					FILE_LOG sendInfo(27,sTemp);
					send(s_Client,(LPSTR)&sendInfo,sTemp.GetLength()  +2,0);
					CreateThread(NULL, 0, ThreadProcCloseClientOnTime, NULL, 0, NULL);
				}
				break;
				
			} 
		}
	} 

	return 0;  
}

void SendOldZipFile(){

	if(!s_isConnect){
		return;
	}

	char chLicType;
	if(!sCheckLicence(chLicType)){
		return;
	}

	CLock lock(g_SendOldZipFileLock);

	CFileTransferClient::SendOldZipFile();

}

int sCheckLicence(char& licType){
	CEncrypts enc;
	return  enc.CheckLicence("DmsClien",licType);
}

// 打开服务器许可
BOOL sOpenServerLicense()
{
	CEncrypts enc;
	if (!enc.OpenLicence(s_strCurExePath))
	{
		return FALSE;
	}

	char licType;
	if (enc.CheckLicence("DmsClien",licType) == 0)
	{
		return FALSE;
	}

	char ch;
	CString str;
	int ii, pos, val;
	CStringArray& arrName = CEncrypts::sGetArrName();
	int cnt = (int)arrName.GetCount();
	for (ii = 0; ii < cnt; ii ++)
	{
		str = arrName[ii];

		ch = str[0];
		if (ch == '#') // 服务端端口号
			s_usPort = htons((u_short)sToInteger(str.Mid(1)));
		else if (ch == '@') // 最大许可数和服务数
		{
			pos = 0;
			val = sToInteger(sGetSubStr(str, ';', pos + 1, &pos));
			if (val > 0)
				s_maxLic = val;

			val = sToInteger(sGetSubStr(str, ';', pos + 1));
			if (val > 0)
				s_maxSvc = val;
		}
	}

	//if (s_usPort == 0)
	//{
	//	return FALSE;
	//}

	return TRUE;
}

void sGetLicInfo()
{
	CFileDialog dlg(FALSE, "lin", NULL,
		OFN_EXPLORER | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
		"申请许可资料文件(*.lin)|*.lin||");

	CString str = "保存申请许可资料文件";
	dlg.m_pOFN->lpstrTitle = str;
	if (dlg.DoModal() != IDOK)
		return;

	str = dlg.GetPathName();

	CStringArray arrAddress;
	CEncrypts::GetMacAddress(&arrAddress);

	int cnt = (int)arrAddress.GetCount();
	if (cnt == 0)
	{
		AfxMessageBox("不能获得申请许可资料，请与销售商或开发商联系。");
		return;
	}

	FILE* fp(NULL);
	fopen_s(&fp, str, "wb");
	if (fp == NULL)
	{
		AfxMessageBox("不能创建申请许可资料文件");
		return;
	}

	CString strText;
	for (int ii = 0; ii < cnt; ii ++)
	{
		strText.Format("MacAddress = %s\r\n", arrAddress[ii]);
		fwrite(strText, 1, strText.GetLength(), fp);
	}

	//strText = "MotherboardSN = " + s_strBaseBoard;
	//fwrite(strText, 1, strText.GetLength(), fp);

	strText = "\r\nCPUSerial = " + s_strCPUID;
	fwrite(strText, 1, strText.GetLength(), fp);

	//strText = "\r\nVersion = 3";
	//fwrite(strText, 1, strText.GetLength(), fp);

	fclose(fp);


}


int sGetFileLogM(){

	BOOL isConnected = s_SuperDB.Connect(g_strCnnParam);
	
	BOOL isFirst = TRUE;
	
	if(isConnected){

		
		if (s_SuperDB.OpenSQL("SELECT COUNT(*),FILE_DIR FROM DMS_FILE_LOGS where id <> 0  GROUP BY FILE_DIR"))
		{
			int cnt ;CString strDir;
			
				while(!s_SuperDB.IsEOF()){
					cnt = s_SuperDB.GetLong(short(0));
					strDir = s_SuperDB.GetString(short(1)); 
					s_MapFileCount[strDir] = s_SuperDB.GetLong(short(0));
					s_SuperDB.MoveNext();
				}

		}

		if (s_SuperDB.OpenSQL("SELECT COUNT(*),FILE_DIR FROM DMS_FILE_LOGS2 where id <> 0  GROUP BY FILE_DIR"))
		{
			int cnt ;CString strDir;

			while(!s_SuperDB.IsEOF()){
				cnt = s_SuperDB.GetLong(short(0));
				strDir = s_SuperDB.GetString(short(1)); 
				s_MapFileCount[strDir] = s_SuperDB.GetLong(short(0));
				s_SuperDB.MoveNext();
				isFirst = FALSE;
			}

		}

		s_SuperDB.OpenSQL("SELECT * FROM DMS_FILE_LOG");
		s_SuperDB.MoveFirst();

		while(!s_SuperDB.IsEOF()){
			CString fileName = s_SuperDB.GetString("FILE_NAME");
			CString fileLog  = s_SuperDB.GetString("FILE_LOG");
			s_vecTimeLogInfo.push_back(fileLog);
			s_SuperDB.MoveNext();
		}

	}else{
		return 0;
	}

	isConnected = s_MonSuperDB.Connect(g_strCnnParam);
	if(isConnected){
		if (isFirst)
		{
			s_MonSuperDB.OpenSQL("SELECT * FROM DMS_FILE_LOGS");
			g_StrCurUploadTableName = "DMS_FILE_LOGS2";
		}else{
			s_MonSuperDB.OpenSQL("SELECT * FROM DMS_FILE_LOGS2");
			g_StrCurUploadTableName = "DMS_FILE_LOGS";
		}
		

	}else{
		return 0;
	}


	//isConnected = s_UniverDB.Connect(g_strCnnParam);

	//if (!isConnected)
	//{
	//	if (s_pListInfo)
	//	{
	//		s_pListInfo->AddString("s_UniverDB connected error");
	//	}
	//	return 0;
	//}

	//if (!s_UniverDB.OpenSQL("SELECT * FROM LAST_DATE")){
	//	if (s_pListInfo)
	//	{
	//		s_pListInfo->AddString("s_UniverDB open error");
	//	}
	//	return 0;
	//}
	return 1;
}


void sAddTimeLogInfoM(const CString & fileLog,const CTime& time){
	CLock lock(g_DBLock);
	s_SuperDB.AddNew();
	s_SuperDB.SetString("FILE_LOG",fileLog);
	COleDateTime OTime(time.GetYear(),time.GetMonth(),
		time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());
	s_SuperDB.SetDateTime("CREATE_DATE",OTime);
	s_SuperDB.Update(FALSE);
}

void sClearDBTIMELogInfoM(){
	CLock lock(g_DBLock);
	s_SuperDB.Execute("DELETE FROM DMS_FILE_LOG WHERE FILE_NAME  IS NULL",FALSE);
	s_SuperDB.Update(FALSE);
}


void sAddMonFileLog(ULONGLONG id,ULONGLONG batchID,CString& key,CString& strFilePath,CString strRootPath,CString& strFileLog,int nHandle,
					ULONGLONG fileSize,CString strFileName,CTime& LogDate,CTime& createDate,CTime& updateDate,int nOrder){
	CLock lock(g_MonFileLogLock);
	s_MonSuperDB.AddNew();
	s_MonSuperDB.SetLong("ID",id);
	//ULONGLONG batch = *s_MapPFileID[strRootPath];
	s_MonSuperDB.SetLong("BATCH_NUMBER",batchID);
	s_MonSuperDB.SetString("FILE_PATH",key);
	s_MonSuperDB.SetString("FILE_PATH2",strFilePath);
	s_MonSuperDB.SetString("FILE_DIR",strRootPath);
	s_MonSuperDB.SetString("DESCRIBE1",strFileLog);
	s_MonSuperDB.SetLong("HANDLE",nHandle);
	s_MonSuperDB.SetLong("IS_CREATE",nOrder);

	CString loginName = sGetCurrentUserName();

	s_MonSuperDB.SetString("HANDLER",loginName);
	s_MonSuperDB.SetString("FILE_NAME",strFileName);
	s_MonSuperDB.SetLong("FILE_SIZE",fileSize);

	COleDateTime logTime(LogDate.GetYear(),LogDate.GetMonth(),
		LogDate.GetDay(),LogDate.GetHour(),LogDate.GetMinute(),LogDate.GetSecond());
	s_MonSuperDB.SetDateTime("FILE_LOG_DATE",logTime);

	COleDateTime cTime(createDate.GetYear(),createDate.GetMonth(),
		createDate.GetDay(),createDate.GetHour(),createDate.GetMinute(),createDate.GetSecond());
	s_MonSuperDB.SetDateTime("CREATE_DATE",cTime);

	COleDateTime OTime(updateDate.GetYear(),updateDate.GetMonth(),
		updateDate.GetDay(),updateDate.GetHour(),updateDate.GetMinute(),updateDate.GetSecond());
	s_MonSuperDB.SetDateTime("UPDATE_DATE",OTime);

	ULONGLONG createTime = LogDate.GetTime();
	s_MonSuperDB.SetLong("CREATE_TIME",createTime);
	BOOL isResu = s_MonSuperDB.Update(FALSE);
	if(isResu){
		if(s_pListInfo){
			CString strInfo;
			CString strid;
			strid.Format("%ld",id);
			strInfo.Format("%s--%d--%s",strid,nHandle,strFilePath);
			s_pListInfo->AddString(strInfo);
		}

		s_MapFileCount[strRootPath]++;

		s_mapFileDirMonitor[strRootPath]->SetLastTime(LogDate);

	}
	if(s_pListInfo && !s_MonSuperDB.m_strLastErr.IsEmpty())
		s_pListInfo->AddString(s_MonSuperDB.m_strLastErr);
	//TRACE("%s\t",s_MonSuperDB.m_strLastErr);



    strRootPath.MakeLower();

	if(s_mapFileDirMonitor.find(strRootPath) == s_mapFileDirMonitor.end())
		return ;

	
	if(!s_strFlagFile.IsEmpty() ){
		CString extName = sGetExtFromFile(key);
		extName.MakeLower();
		if(s_strFlagFile.Find(extName) >= 0){
			std::set<CString>& s_setDataFlagFile = s_mapFileDirMonitor[strRootPath]->s_setDataFlagFile;
			if (nHandle == 2 || (nHandle == 4 && nOrder == 3))
			{
				set<CString>::iterator it = s_setDataFlagFile.find(key);
				if (it != s_setDataFlagFile.end())
				{
					s_setDataFlagFile.erase(it);
				}
			}else{
				s_setDataFlagFile.insert(key);
			}
			
		}
	}

	if (!s_strOtherSharedExt.IsEmpty())
	{
		CString extName = sGetExtFromFile(key);
		extName.MakeLower();
		if(s_strOtherSharedExt.Find(extName) >= 0){
			std::set<CString>& s_setDataFlagFile = s_mapFileDirMonitor[strRootPath]->s_setOtherFileEx;
			if (nHandle == 2 || (nHandle == 4 && nOrder == 3))
			{
				set<CString>::iterator it = s_setDataFlagFile.find(key);
				if (it != s_setDataFlagFile.end())
				{
					s_setDataFlagFile.erase(it);
				}
			}else{
				s_setDataFlagFile.insert(key);
			}

		}
	}
	
}


DWORD WINAPI ThreadProcSendOnceFile(LPVOID lpParam)  
{ 
	FILE_SETS* fileSets = (FILE_SETS*)lpParam;

	int cnt = 0 ;
	while(s_RoomId.IsEmpty()){
		if(s_Client){
			char chType(31);
			send(s_Client,(LPSTR)&chType,1,0);
		}
		Sleep(5000);
		cnt ++;
		if(cnt > 30){
			delete fileSets;
			return 0;
		}

	}
	CLock lock(g_SendOldZipFileLock);

	CFileTransferClient::SendOnceFile(s_RoomId,*fileSets,s_isConnect);
	delete fileSets;

	return 0;
}

BOOL isConflictFile(const CString& key,CFileDirMonitor* pFileDirMon){

	BOOL isRes(TRUE);
	std::set<CString>& s_setDataFlagFile = pFileDirMon->s_setDataFlagFile;

	int DataFileCnt = s_setDataFlagFile.size();

	if (DataFileCnt > 0)
	{
		set<CString>::iterator it; //定义前向迭代器
		//中序遍历集合中的所有元素
		int nCount(0);int nLen(0);CString subDir("");
		for(it = s_setDataFlagFile.begin(); it != s_setDataFlagFile.end(); it++)
		{
			CString strFileName = *it;

			nLen = key.GetLength();

			subDir = strFileName.Left(nLen);

			if(subDir.CompareNoCase(key)){
				continue;
			}

			if (!IsFileExist(strFileName))
			{
				nCount++;
				continue;
			}

			CFile fp;
			isRes = fp.Open(strFileName, CFile::shareDenyWrite | CFile::shareDenyRead);
			if (isRes)
			{
				fp.Close();
				nCount++;
			}else{
				break;
			}
		}

		if (DataFileCnt == nCount)
		{
			isRes = TRUE;
		}else{
			isRes = FALSE;
		}


	}

	std::set<CString>& setOtherFileEx = pFileDirMon->s_setOtherFileEx;
	DataFileCnt = setOtherFileEx.size();
	if (DataFileCnt > 0 && isRes)
	{
		set<CString>::iterator it; //定义前向迭代器
		//中序遍历集合中的所有元素
		int nCount(0);int nLen(0);CString subDir("");
		for(it = setOtherFileEx.begin(); it != setOtherFileEx.end(); it++)
		{
			CString strFileName = *it;

			nLen = key.GetLength();

			subDir = strFileName.Left(nLen);

			if(subDir.CompareNoCase(key)){
				continue;
			}

			if (!IsFileExist(strFileName))
			{
				nCount++;
				continue;
			}

			CFile fp;
			isRes = fp.Open(strFileName, CFile::shareDenyWrite | CFile::shareDenyRead);
			if (isRes)
			{
				fp.Close();
				nCount++;
			}else{
				break;
			}
		}

		if (DataFileCnt == nCount)
		{
			isRes = TRUE;
		}else{
			isRes = FALSE;
		}


	}



	return isRes;
}



DWORD WINAPI ThreadProcCheckSendFileLog(LPVOID lpParam)  
{
	Sleep(5000);
	while(TRUE){
		Sleep(1000);
		CLock lock(g_CheckMoniorLock);
			CTime curTime = CTime::GetCurrentTime();
			std::map<CString,CFileDirMonitor*> ::iterator iter;

			int fileCount(1);
			//if (FALSE)
			{
				CLock dbLock(g_MonFileLogLock);
				CString strDBPath = s_strCurExePath + "\\Config\\DMS_FILE_LOG.mdb";
				CFileStatus rStatus;
				if(CFile::GetStatus(strDBPath,rStatus) ){

					int size = rStatus.m_size / 1048576;
					if (size > 50)
					{

						CUniverDB SuperDB;
						BOOL	isConnected = SuperDB.Connect(g_strCnnParam);

						if (isConnected )
						{
							CString sql("SELECT COUNT(*) AS CNT FROM (SELECT * FROM  DMS_FILE_LOGS UNION  SELECT * FROM  DMS_FILE_LOGS2) AS A WHERE A.FILE_DIR <> ''");

							if (SuperDB.OpenSQL(sql))
							{
								fileCount = SuperDB.GetLong("CNT");
							}

						}

						if (fileCount == 0 )
						{
							//CLock lock(g_DelDBLock);
							CLock lock(g_DBLock);
							vector<CString> sysTime;
							vector<COleDateTime> vecTime;
							s_SuperDB.MoveFirst();
							while (!s_SuperDB.IsEOF())
							{
								sysTime.push_back(s_SuperDB.GetString("FILE_LOG"));
								vecTime.push_back(s_SuperDB.GetDateTime("CREATE_DATE"));
								s_SuperDB.MoveNext();
							}


							sCloseUniverDB();
							DeleteFile(strDBPath);
							CopyFile(s_strCurExePath + "\\Config\\Base.mdb",strDBPath,FALSE);
							sGetFileLogM();
							CString strFileLog;
							CTime time = CTime::GetCurrentTime();
							int cnt = sysTime.size();
							for (std::vector<CString>::size_type  ii = 0 ; ii < cnt ; ii ++)
							{
								s_SuperDB.AddNew();
								s_SuperDB.SetString("FILE_LOG",sysTime[ii]);
								s_SuperDB.SetDateTime("CREATE_DATE",vecTime[ii]);
								s_SuperDB.Update(FALSE);
							}
							sysTime.clear();
							vecTime.clear();

						}


					}


				}
			}
			


			for(iter = s_mapFileDirMonitor.begin(); iter != s_mapFileDirMonitor.end(); iter ++){
				CString strInfo;
				CFileDirMonitor* pFileDirMon = iter->second;
				ULONG cnt = curTime.GetTime() - pFileDirMon->GetLastTime().GetTime();

				CString key = iter->first;

				//int maxCnt = 0;
				//FILE_LOG sendInfo((char)8,key);
				////s_MapFileSql.erase(key);
				//while (s_MapFileSql.find(key) == s_MapFileSql.end())
				//{
				//	send(s_Client,(LPSTR)&sendInfo,key.GetLength() + 2,0);
				//	Sleep(1000);
				//	maxCnt ++;
				//	if(maxCnt >= 4){
				//		s_MapFileSql[key] = s_defaultSql;
				//	}
				//}

				fileCount = s_MapFileCount[iter->first];
				
				BOOL isRes(FALSE) ;
				
				if(cnt >= CSupport::GetContinuousOperationTime() ){
					isRes = isConflictFile(key,pFileDirMon);

					if (!isRes)
					{
						pFileDirMon->SetLastTime(curTime);
					}
				}

				if(isRes && fileCount > 0){ // && stopThread == FALSE ||fileCount >= 1000)
					
					TRACE("--------------------------\n");

					{
						CLock dbLock(g_MonFileLogLock);

						pFileDirMon->SetStartID( CTime::GetCurrentTime().GetTime() - 10);

						s_MonSuperDB.AddNew();
						s_MonSuperDB.SetLong("ID",0);
						//s_MonSuperDB.SetString("FILE_DIR",key);
						s_MonSuperDB.Update(FALSE);

						CString strFormat =
						"INSERT INTO %s ( ID, BATCH_NUMBER, FILE_PATH, FILE_PATH2, FILE_DIR, FILE_LOG, HANDLE, IS_CREATE, DESCRIBE1, HANDLER, FILE_SIZE, FILE_NAME, CREATE_TIME, FILE_LOG_DATE, CREATE_DATE, UPDATE_DATE )";
							strFormat +=" SELECT ID, BATCH_NUMBER, FILE_PATH, FILE_PATH2, FILE_DIR, FILE_LOG, HANDLE, IS_CREATE, DESCRIBE1, HANDLER, FILE_SIZE, FILE_NAME, CREATE_TIME, FILE_LOG_DATE, CREATE_DATE, UPDATE_DATE";
							strFormat +=" FROM %s WHERE FILE_DIR <>\'";
							strFormat += key;
							strFormat += "\' AND ID > 0 ";


						if (!g_StrCurUploadTableName.CompareNoCase("DMS_FILE_LOGS2"))
						{
							g_StrCurUploadTableName = "DMS_FILE_LOGS";
							s_MonSuperDB.Execute("DELETE FROM DMS_FILE_LOGS2",FALSE);
							s_MonSuperDB.Update(FALSE);

							CString strSql;
							strSql.Format(strFormat,"DMS_FILE_LOGS2","DMS_FILE_LOGS");

							if(s_MonSuperDB.Execute(strSql,FALSE))
								s_MonSuperDB.Update(FALSE);

							s_MonSuperDB.OpenSQL("SELECT * FROM DMS_FILE_LOGS2");

						}else{
							g_StrCurUploadTableName = "DMS_FILE_LOGS2";
							s_MonSuperDB.Execute("DELETE FROM DMS_FILE_LOGS",FALSE);
							s_MonSuperDB.Update(FALSE);

							CString strSql;
							strSql.Format(strFormat,"DMS_FILE_LOGS","DMS_FILE_LOGS2");

							if(s_MonSuperDB.Execute(strSql,FALSE))
								s_MonSuperDB.Update(FALSE);

							s_MonSuperDB.OpenSQL("SELECT * FROM DMS_FILE_LOGS");

						}

						s_MapFileCount[key] = 0;

					}
					
					//Sleep(1000);
					//stopThread = TRUE;
					//ThreadProcSendFileLog(pFileDirMon);
					{
						CLock SendLock(g_SendFileLogLock);
						//CFileMonitor::s_BakcupsFile.IsCopyFile();
						CUniverDB SuperDB;
						BOOL	isConnected = SuperDB.Connect(g_strCnnParam);

						if(s_pListInfo && CStatFileLogs::s_isAutoCopyDB){
							CTime time = CTime::GetCurrentTime();
							CLock lock(g_MonFileLogLock);
							CString strAccessPath = s_strCurExePath + "\\Config\\DMS_FILE_LOG.mdb";

							CString str =CSupport::GetBufDir() + "\\" + time.Format("%Y_%m_%d_%H_%M_%S.mdb");//格式化日期时间 
							::CopyFile(strAccessPath,str,FALSE);

							s_pListInfo->AddString(str);
						}

						//CString	strQuSql = s_MapFileSql[key];
						//strQuSql = s_MapFileSql[key];
						//strQuSql.MakeUpper();
						//pFileDirMon->m_strSql = strQuSql;

						COleDateTime lastTime = CStatFileLogs::StatFileLogs(pFileDirMon,SuperDB,g_StrCurUploadTableName,s_Client);
						//g_strLastFileDir = pFileDirMon->m_strDir;
						//SetLastLogDate(s_UniverDB,lastTime,g_strLastFileDir);

					}

					if (pFileDirMon->s_setDataFlagFile.size() > 0)
					{
						CreateThread(NULL, 0, ThreadProcSendDeleteDBFile, (LPVOID)&pFileDirMon->s_setDataFlagFile, 0, NULL);
					}

					pFileDirMon->s_setOtherFileEx.clear();
					
					//s_setDataFlagFile.clear();
					//HANDLE hThread = CreateThread(NULL, 0, ThreadProcSendFileLog, s_mapFileDirMonitor[iter->first], 0, NULL);

				}else{

					//if (!g_strLastFileDir.IsEmpty())
					//{
					//	g_strLastFileDir = "";
					//	COleDateTime lastTime = COleDateTime::GetCurrentTime();
					//	SetLastLogDate(s_UniverDB,lastTime,g_strLastFileDir);
					//}

					

				}

			}
	}
	return 1;
}

void SetLastLogDate(CUniverDB& UniverDB,COleDateTime& time,CString& strFileDir){
	CString str;
	UniverDB.MoveFirst();
	CString loginName = sGetCurrentUserName();
	while (!UniverDB.IsEOF())
	{
		str = UniverDB.GetString("OPTION_NAME");
		if (!str.CompareNoCase("LAST_LOG"))
		{
			//COleDateTime oleTime = COleDateTime::GetCurrentTime();
			UniverDB.Edit();
			UniverDB.SetDateTime("OPTION_DATE",time);
			UniverDB.SetString("FILE_DIR",strFileDir);
			UniverDB.SetString("HANDLER",loginName);
			UniverDB.Update(FALSE);
			break;
		}

		//if (!str.CompareNoCase("SHUTDOWN"))
		//{
		//	strOptin2= str;
		//}

		UniverDB.MoveNext();
	}
}
//
//DWORD WINAPI ThreadProcSendFileLog(LPVOID lpParam) {
//
//
//	return 0;
//}

CString sBrowseFile(CString &strFile,CTime time,CString &strMyFile ) 
{ 
	CFileFind ff; 
	CString szDir = strFile; 

	//if(szDir.Right(1) != L"\\") 
	//	szDir += L"\\"; 

	//szDir += strFilter;//L"*.adf"; 

	BOOL res = ff.FindFile(szDir); 
	while(res) 
	{ 
		res = ff.FindNextFile(); 
		if(ff.IsDirectory() && !ff.IsDots()) 
		{ 
			//如果是一个子目录，用递归继续往深一层找 
			CString strPath = ff.GetFilePath(); 

			CString subPath; 

			//sBrowseFile(strPath); 
		} 
		else if(!ff.IsDirectory() && !ff.IsDots()) 
		{ 
			//显示当前访问的文件(完整路径) 
			CString strPath = ff.GetFilePath(); 

			CFileStatus rStatus;
			if(strMyFile.CompareNoCase(strPath) && CFile::GetStatus(strPath,rStatus)){
				if(rStatus.m_mtime == time){
					CString str;
					str.Format("%s",rStatus.m_szFullName);
					return str;
				}
			}


		} 
	} 

	//关闭 
	ff.Close(); 

	return "";
} 


CString GUID_Generator()
{
	char buf[ 128 ] = {0};
	GUID guid;
	//CoInitialize(NULL);
	if (S_OK == ::CoCreateGuid(&guid))
	{
		_snprintf( buf, sizeof(buf),
			"%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X",//"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
			guid.Data1,
			guid.Data2,
			guid.Data3,
			guid.Data4[0], guid.Data4[1],
			guid.Data4[2], guid.Data4[3],
			guid.Data4[4], guid.Data4[5],
			guid.Data4[6], guid.Data4[7]);
	}
	//CoUninitialize();
	CString str;
	str.Format("%s",buf);
	return str; 
}


CListBox* sGetListBox(){
	return s_pListInfo;;
}

//CString GetFlagFile(){
//	return s_strFlagFile;
//}
//
//void SetFlagFile(CString& flagFile)
//{
//	s_strFlagFile = flagFile;
//}

DWORD WINAPI ThreadProcSendDeleteDBFile(LPVOID lpParam) {

	std::set<CString>* s_setDataFlagFile = (std::set<CString>*)lpParam;
	CString str("");
	CString strFileLog("");

	set<CString>::iterator it; //定义前向迭代器
	//中序遍历集合中的所有元素
	int nCount(0);int nLen(0);CString subDir("");
	for(it = s_setDataFlagFile->begin(); it != s_setDataFlagFile->end(); it++)
	{
		strFileLog = *it;

		if(str.GetLength() + strFileLog.GetLength() > 1020)
		{
			if(!str.IsEmpty()){
				str += "]";
				FILE_LOG sendInfo(11,str);

				int result = send(s_Client,(LPSTR)&sendInfo,str.GetLength() + 2,0);
				if(result<=0){
					return FALSE;
				}
				str.Empty();
				Sleep(500);
			}

			str =  ";" + strFileLog;

		}else if(str.IsEmpty()){
			str += strFileLog;
		}else{
			str += ";" + strFileLog;
		}
	}

	if(!str.IsEmpty()){

		FILE_LOG sendInfo(11,str);
		//TRACE("%s\n",str);
		int result = send(s_Client,(LPSTR)&sendInfo,str.GetLength() + 1,0);

		if(result<=0){
			return FALSE;
		}
	}


	s_setDataFlagFile->clear();

	return TRUE;
}



int sAscFind(ULONGLONG_ARRAY* parrVal, ULONGLONG nVal, int nSta, int nEnd)
{
	if (nEnd < 0)
		nEnd = (int)parrVal->GetCount();

	ULONGLONG nV;

	int len = nEnd - nSta;
	if (len <= 0)
		return -nSta - 1;
	else if (len == 1)
	{
		nV = parrVal->GetAt(nSta);
		if (nV == nVal)
			return nSta;
		else if (nV < nVal)
			return -nEnd - 1;
		else
			return -nSta - 1;
	}

	int nMid = (nSta + nEnd) / 2;
	nV = parrVal->GetAt(nMid);
	if (nV == nVal)
		return nMid;
	else if (nV < nVal)
		return sAscFind(parrVal, nVal, nMid, nEnd);
	else
		return sAscFind(parrVal, nVal, nSta, nMid);
}

// 全局变量
HANDLE h1 = NULL;                // 线程句柄
DWORD pid = 0;                   // 保存cmd窗口的PID
CString inputCmd, outputCmd;     // 保存cmd命令和返回的结果
CEdit * m_edit;                  // 输出框


/************************************************************************/
/* 线程函数                                                             */
/************************************************************************/
DWORD WINAPI ThreadFun(LPVOID lpParam)
{
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead,hWrite;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);    
	sa.lpSecurityDescriptor = NULL;        //使用系统默认的安全描述符    
	sa.bInheritHandle = TRUE;              //创建的进程继承句柄

	if (!CreatePipe(&hRead,&hWrite,&sa,0)) //创建匿名管道
	{        
		::MessageBox(NULL,"CreatePipe Failed!","提示",MB_OK | MB_ICONWARNING);        
		return 0;
	}

	STARTUPINFO si;    
	PROCESS_INFORMATION pi;

	ZeroMemory(&si,sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);    
	GetStartupInfo(&si);    
	si.hStdError = hWrite;    
	si.hStdOutput = hWrite;    //新创建进程的标准输出连在写管道一端
	si.wShowWindow = SW_HIDE;  //隐藏窗口    
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;


	char cmdline[200]; 
	CString tmp,stredit2;

	tmp.Format("cmd /C %s",inputCmd);  // inputCmd为输入的cmd命令
	sprintf(cmdline,"%s",tmp);

	if (!CreateProcess(NULL,cmdline,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi)) //创建子进程
	{
		::MessageBox(NULL,"CreateProcess Failed!","提示",MB_OK | MB_ICONWARNING);        
		return 0;
	}

	pid = pi.dwProcessId;

	CloseHandle(hWrite); //关闭管道句柄

	char buffer[4096] = {0};
	DWORD bytesRead;

	while (true) 
	{
		if (ReadFile(hRead,buffer,4095,&bytesRead,NULL) == NULL) //读取管道
			break;

		CString temp;
		temp = buffer; // outpuuCmd为输出的结果

		temp += _T("\r\n\r\n");

		outputCmd += temp;

		//显示输出信息到编辑框,并刷新窗口
		int len = outputCmd.GetLength();
		m_edit->SetWindowText(outputCmd);
		m_edit->SetSel(len, len);

	}
	CloseHandle(hRead);

	return 1;
}










