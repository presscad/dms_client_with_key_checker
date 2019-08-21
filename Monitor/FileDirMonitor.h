#pragma once

#include "Lock.h"
#include "FileLogList.h"
#include "..\BaseDB\BaseDirMonitor.h"
#include <map>
#include <set>

//
//typedef struct _thread_param_mon{
////	char cType;
//	ULONGLONG  id;
//	PVOID   mpParam;
//	CTime mtime;
//	CString mStrRootPath,mFileName,mNewFileName;
//	ULONGLONG batchID;
//	CBaseDirMonitor* mpBaseDirMonitor;
//public:
//	_thread_param_mon(char type,ULONGLONG nid,ULONGLONG bID,PVOID pParam,CTime time,
//		CString strRootPath,CString fileName,CString newFileName,CBaseDirMonitor* pBaseDirMonitor){
//			id = nid;
//			batchID = bID;
//			cType = type;
//			mpParam = pParam;
//			mtime = time;
//			mStrRootPath = strRootPath;
//			mFileName = fileName;
//			mNewFileName = newFileName;
//			mpBaseDirMonitor = pBaseDirMonitor;
//
//	};
//}THREAD_PARAM_MON;



typedef struct _scanned_file{
	CString strDirName;

	std::map<CString,CFileStatus> mapFileStatus;

}SCANNED_FILE;


class CFileDirMonitor : public CBaseDirMonitor
{
public:
	CFileDirMonitor(CString strDir);
public:
	~CFileDirMonitor(void);

	CString m_strDir;
	HANDLE m_ThreadHandle;
	HANDLE m_FileHandle;

	CString m_strSql;

	CString m_strFilePath;

	std::set<CString> s_setDataFlagFile;

	std::set<CString> s_setOtherFileEx;

	//HANDLE m_ThreadHandle2;
	//HANDLE m_FileHandle2;

	void StartFileMonitor1(DWORD 	dw);
	void StopFileMonitor1();
	std::map<CString,SCANNED_FILE> m_mapDirFileStatus;


	CString m_strFileSql;
	CString m_strFileDelSql;
	int m_FileLogCount;

	CTime  m_LastTime;

	Mutex m_FileLogLock;
	int m_index;

	void BrowseFile(CString &strFile) ;

	static DWORD WINAPI ThreadProc(LPVOID lpParam);

	static DWORD WINAPI FileMonitor1(LPVOID lpParam); 
	static DWORD WINAPI FileMonitor4(LPVOID lpParam);
	static DWORD WINAPI FileMonitor3(LPVOID lpParam);
	static DWORD WINAPI FileMonitor2(LPVOID lpParam);

	void SetLastTime(CTime time);

	CTime GetLastTime();

	void SetStartID(ULONGLONG startID);

	ULONGLONG GetStartID();

	CString GetMonitorDir();

	ULONGLONG m_startID;

};
