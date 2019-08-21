#pragma once
#include "filemonitor.h"

class CFileMonitorModify :
	public CFileMonitor
{
public:
	CFileMonitorModify(CString hideDirectoryPath,SOCKET* pSocket,CFileDateLock* pLock);
public:
	~CFileMonitorModify(void);
	void FileChanged(int wd,ULONGLONG id,CTime LogTime,CString strRootPath,CString name,CString newName = _T(""));
};
