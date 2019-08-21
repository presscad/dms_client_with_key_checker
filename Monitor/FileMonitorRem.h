#pragma once
#include "filemonitor.h"

class CFileMonitorRem :
	public CFileMonitor
{
public:
	CFileMonitorRem(CString hideDirectoryPath,SOCKET* pSocket,CFileDateLock* pLock);
public:
	~CFileMonitorRem(void);
	void FileChanged(int wd,ULONGLONG id,CTime LogTime,CString strRootPath,CString name,CString newName = _T(""));
};
