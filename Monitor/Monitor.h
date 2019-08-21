#include "stdafx.h"
#include  <vector>
#include <afxsock.h>
#include <map>
#include <set>
#include "FileMonitor.h"
#include "clientSocket.h"

using namespace std;

class CUniverDB;

typedef void (*PFunReceInfo)(RECEIVE_INFO& receInfo); 

static PFunReceInfo   m_PFunReceInfos[40]; 
void initFunReceInfo();
void ReceFileLogSql(RECEIVE_INFO& receInfo);
void ReceDelFileLogSql(RECEIVE_INFO& receInfo);
void sHeartbeatAndSynchroFrequency(RECEIVE_INFO& receInfo);
void sHeartbeat(RECEIVE_INFO& receInfo);
void sRenewalLicense(RECEIVE_INFO& receInfo);
void sSendLinInfo(RECEIVE_INFO& receInfo);
void sFileFliter28(RECEIVE_INFO& receInfo);
void sFileFliter30(RECEIVE_INFO& receInfo);
void sFileFliter29(RECEIVE_INFO& receInfo);
void sSetRoomId(RECEIVE_INFO& receInfo);
void sSendFileToServer(RECEIVE_INFO& receInfo);
void sAddStringListInfo(RECEIVE_INFO& receInfo);
void sRenewalLicense(RECEIVE_INFO& receInfo);
void sSetAutoConnect(RECEIVE_INFO& receInfo);


static std::map<CString,CString>  s_MapFileSql;
static std::map<CString,int>  s_MapFileCount;
static CString s_strFlagFile("");
static CString s_strOtherSharedExt("");
//static std::set<CString> s_setDataFlagFile;

static std::map<CString,CFileDirMonitor*> s_mapFileDirMonitor;
typedef CArray<ULONGLONG, ULONGLONG> ULONGLONG_ARRAY;


static SOCKET s_Client;
static BOOL   s_isConnect = FALSE;
static CListBox* s_pListInfo = NULL;
static CString s_RoomId("");

__declspec(dllexport) void startMonitor(char szFileName[]);
int sConnectServer();
BOOL isConnect();

void sCancelMonitoring(RECEIVE_INFO& receInfo);
void sAddMonitoring(RECEIVE_INFO& receInfo);
void sAddMonitoring(CString& dir);
void sCheckMonitoring(RECEIVE_INFO& receInfo);


DWORD WINAPI ThreadProcServer(LPVOID lpParam) ;

void saveFileConfig();
BOOL stopMonitorFile();
//CString sGetLoginName();

// 打开服务器许可
BOOL sOpenServerLicense();
int sCheckLicence(char& licType);
CString GetCPUID();
BOOL GetBaseBoardByCmd(char *lpszBaseBoard, int len/*=128*/);
CString sGetLinInfo();

int sGetFileLogM();

void sAddTimeLogInfoM(const CString & fileLog,const CTime& time);
void sClearDBTIMELogInfoM();

DWORD WINAPI ThreadProcSendFile(LPVOID lpParam);

CString sGetCurrentUserName();


DWORD WINAPI ThreadProcSendFileLog(LPVOID lpParam);

CString GUID_Generator();

DWORD WINAPI ThreadProcCheckSendFileLog(LPVOID lpParam);

DWORD WINAPI ThreadProcSendDeleteDBFile(LPVOID lpParam);

void sAddMonFileLog(ULONGLONG id,ULONGLONG batchID,CString& key,CString& strFilePath,CString strRootPath,CString& strFileLog,
					int nHandle,ULONGLONG fileSize,CString strFileName,CTime& LogDate,CTime& createDate,CTime& updateDate,int nOrder = 0);

CString sBrowseFile(CString &strFile,CTime time,CString &strMyFile) ;

DWORD WINAPI ThreadProcSendOnceFile(LPVOID lpParam);
void SendOldZipFile();
CListBox* sGetListBox();

void SetLastLogDate(CUniverDB& UniverDB,COleDateTime& time,CString& strFileDir);

int sAscFind(ULONGLONG_ARRAY* parrVal, ULONGLONG nVal, int nSta = 0, int nEnd = -1);
