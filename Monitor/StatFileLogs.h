#pragma once
#include <afxsock.h>
#include <vector>
#include <set>

using namespace std;

class CUniverDB;
class CFileDirMonitor;
class CStatFileLogs
{
public:
	CStatFileLogs(void);
public:
	~CStatFileLogs(void);

	//static CStringArray s_arrSendFileLogs;
	static COleDateTime CStatFileLogs::StatFileLogs(CFileDirMonitor* pMonitor,CUniverDB& SuperDB,const CString& TableName,SOCKET Client);
	static DWORD CStatFileLogs::SendFileLog(CFileDirMonitor* pMonitor ,CUniverDB& SuperDB,CString& strSubTable,
		const CString& strWhereSql,const CString& TableName,SOCKET Client,CString& strFilePath,COleDateTime& lastTime);
	static BOOL CStatFileLogs::SendFileLog(vector<int>& vecType,const int& typeNo,CString& logFileName ,
		SOCKET Client ,CStringArray& arrSendFileLogs,CString& logFile);
	static BOOL DeleteOthersFileLog(CUniverDB& SuperDB,CString key,std::set<CString> setUpload ,
		const CString& strSubSql,const CString& TableName);
	static CString GetFileLogInfo(CString name,CString describe1,CString formatName,ULONGLONG fileSize,
		CString handler,CString loginName,CString IP,CTime& ctime,CTime& time);

	static BOOL s_isAutoCopyDB;
	static CListBox* m_pListInfo;

};
