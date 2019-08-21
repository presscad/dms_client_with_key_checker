#pragma once
#include "filemonitor.h"

class  __declspec(dllexport) CFileMonitorAdd :
	public CFileMonitor
{
public:
	CFileMonitorAdd(CString hideDirectoryPath,SOCKET* pSocket,CFileDateLock* pLock);
public:
	~CFileMonitorAdd(void);

	void FileChanged(int wd,ULONGLONG id,CTime LogTime,CString strRootPath,CString name,CString newName = _T(""));
};
