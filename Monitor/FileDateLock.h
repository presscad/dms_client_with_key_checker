#pragma once
#include <map>
#include <vector>
#include "Lock.h"
#include "FileLogInfo.h"
#include "FileDirMonitor.h"

typedef struct file_info{
	CTime   time;
	int  dw;
	std::vector<CFileLogInfo*> mVecFileLog;
public:
	file_info(){

	};
	file_info(CTime tTime,int dwType){
		time = tTime;
		dw = dwType;
		
	};
}FILE_INFO;

class CFileDateLock
{
public:
	CFileDateLock(void);
	CFileDateLock(CFileDirMonitor* pFileDirMonitor);
public:
	~CFileDateLock(void);
private:
	std::map<CString,FILE_INFO> s_MapFile;
	Mutex m_LockMap;
public:
	BOOL IsChange(CString name,int& wd,const CTime& datetime);
	void RomoveMapFile(CString name);

	CFileDirMonitor* m_pFileDirMonitor;
};
