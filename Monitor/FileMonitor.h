#pragma once
#include <map>
#include <vector>
#include <string>
#include "ClientSocket.h"
#include "Crypt.h"
#include "FileDateLock.h"
#include "FileFliter.h"
#include "FileDirMonitor.h"
#include "BakcupsFile.h"
#include "..\BaseDB\FileBaseMonitor.h"

class CUniverDB;

typedef struct _send_info{
	char type;
	int pos;
	int len;
	char info[1024];
	char tail[16];
	//COleDateTime   time;

	_send_info(char cType){
		type = cType;
		pos = 0;
	};
	_send_info(char cType,CString strInfo){
		type = cType;
		pos = 0;
		//info = strInfo.GetBuffer();
		memset(info,0,1024);
		info[strInfo.GetLength()] = '\0'; 
		memcpy(info,strInfo.GetBuffer(strInfo.GetLength() + 1),strInfo.GetLength() + 1);
		//time = tTime;,COleDateTime tTime
	};
}SEND_INFO;


typedef struct _file_log{
	char type;
	char info[1026];

	_file_log(char cType){
		type = cType;
	}

	_file_log(char cType,CString strInfo){
		type = cType;
		memset(info,0,1026);
		info[strInfo.GetLength()] = '\0'; 
		memcpy(info,strInfo.GetBuffer(strInfo.GetLength() + 1),strInfo.GetLength() + 1);
	};
}FILE_LOG;

typedef struct _thread_file_param{
	char cType;
	PVOID   mpParam;
	CString mStrRootPath,mFileName,mNewFileName;
	ULONGLONG    fileSize;
	COleDateTime datetime;


public:
	_thread_file_param(CString fileName,CString newFileName){
		mFileName = fileName;
		mNewFileName = newFileName;
	};
	_thread_file_param(char type,PVOID pParam,CString strRootPath,CString fileName,CString newFileName){
		cType = type;
		mpParam = pParam;
		mStrRootPath = strRootPath;
		mFileName = fileName;
		mNewFileName = newFileName;
	};
}THREAD_FILE_PARAM;


class CFileMonitor : CFileBaseMonitor
{
public:
	CFileMonitor(CString hideDirectoryPath,SOCKET* pSocket,CFileDateLock* pLock);

	CFileMonitor(CString hideDirectoryPath);
	~CFileMonitor(void);
	//SOCKET* m_pClientSocket;

	CFileDirMonitor* m_pFileDirMonitor;

	CFileDateLock* m_pLock;
	CCrypt m_Crypt;
	CFileFliter m_FileFliter;

	CString& m_hideDirectoryPath;

	//static CBakcupsFile s_BakcupsFile;
	//static CUniverDB s_UniverDB;

	static int s_nFlag;

	virtual void FileChanged(THREAD_PARAM*  pThreadPamam) ;
	virtual void FileChanged2(THREAD_PARAM*  pThreadPamam);
	virtual void FileChanged3(THREAD_PARAM*  pThreadPamam);
	virtual void FileChanged4(THREAD_PARAM*  pThreadPamam);

	//virtual void FileChanged(int wd,ULONGLONG id,ULONGLONG batchID,CTime LogTime,CString strRootPath,CString name,CString newName = _T(""));
	//void FileChanged2(int wd,ULONGLONG id,ULONGLONG batchID,CTime LogTime,CString strRootPath,CString name,CString newName = _T(""));
	//void FileChanged3(int wd,ULONGLONG id,ULONGLONG batchID,CTime LogTime,CString strRootPath,CString name,CString newName = _T(""));
	//void FileChanged4(int wd,ULONGLONG id,ULONGLONG batchID,CTime LogTime,CString strRootPath,CString name,CString newName = _T(""));
	CString CopyBufFile(const CString& strRootPath,const CString& name,const CString& newName,BOOL isCreateThread = TRUE );//,const CString& strUUID
	void CopyBufFile2(const CString& strRootPath,const CString& name,const CString& newName,BOOL isCreateThread = TRUE);
	void CopyBufFile3(const CString& fileName,const CString& newName,BOOL isCreateThread = TRUE);
	void GetFileNameForTime(const CString& strFileName,const CTime& time);
	CString GetFileNameForDateTime(CString strFileName,const CTime& time);
	static	void WriteTmpFile(const CString & strOrgName,const CString& strFileName,const CString& strText,ULONGLONG id ,CTime& time);
	static	CString ReadTmpFile(CString& strFileName);
	void fileCreated(ULONGLONG id,CString rootPath, CString name,CString newName,CTime& time);
	void fileModified(const CString& rootPath,const  CString& name,const CString& newName,const CString& fileName,CTime& time,ULONGLONG id);
	BOOL fileRenamed(CString rootPath, CString name, CString newName,CString& newFormat,CTime& time,ULONGLONG id);
	CString fileDeleted(CString rootPath, CString name);
	//void test();
	//void inserData(CString filePath,CString ip,CString fileSize,CString describel,CString fileName,CString handler,COleDateTime createData,COleDateTime upDateDate);
	void inserData(CString filePath,CString ip,CString fileSize,CString describel,CString fileName,CString handler);
	CString GetFileLogInfo(CString name,CString describe1,CString formatName,ULONGLONG fileSize,CString handler,CString IP,CTime time);
	static CString GetFileExitName(CString name);
	CString GetEncryptPath(const CString& strRootPath,const CString& newName);
	//void SendFileLog(const CString& fileName,const CString& info,CString strUUID);
	//BOOL isChange(CString name,const COleDateTime& datetime);
	//void RomoveMapFile(CString name);

	void SetFileDirMonitor(CFileDirMonitor* pFileDirMonitor);

	CString GetListBufFileName(const CString& strPath);

	CString GetEncryptPath(const CString& strPath);
};

