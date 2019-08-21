#pragma once
#include <map>
#include "Lock.h"
#include "crypt.h"


typedef struct back_file_info{
	ULONGLONG   time;
	ULONGLONG   id;
	int  dw;
	
	CString strFileName;
	CString strNewName;

	BOOL isCopyFinsh;
public:
	back_file_info(){
		isCopyFinsh = FALSE;
	};
	back_file_info(ULONGLONG tTime,int dwType,CString strFile,CString strNew){
		time = tTime;
		dw = dwType;
		strFileName = strFile;
		strNewName = strNew;
		isCopyFinsh = FALSE;
	};
}BACK_FILE_INFO;

class CBakcupsFile
{
public:
	CBakcupsFile(void);
public:
	~CBakcupsFile(void);

	std::map<CString,BACK_FILE_INFO> m_MapFile;

	void IsCopyFile();

	CString AddFile(CTime tTime,int dwType,CString strFile,ULONGLONG id);

private:
	Mutex m_Lock;
	HANDLE m_hThread;	//MonitorDirectoryChanges() thread handle
	DWORD  m_dwThreadID;

	static UINT CopyFileThread(LPVOID lpvThis);

	CCrypt m_Crypt;
	void StartThread();

	CString GetFileExitName(CString name);

	CString GetFileTmpPath(const CString& oldName);
};
