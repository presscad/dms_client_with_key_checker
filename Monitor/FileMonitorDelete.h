#pragma once
#include "filemonitor.h"

class CFileMonitorDelete :
	public CFileMonitor
{
public:
	CFileMonitorDelete(CString hideDirectoryPath,SOCKET* pSocket,CFileDateLock* pLock);
public:
	~CFileMonitorDelete(void);
	void FileChanged(int wd,ULONGLONG id,CTime LogTime,CString strRootPath,CString name,CString newName = _T(""));
};
