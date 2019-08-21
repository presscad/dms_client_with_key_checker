#include "StdAfx.h"
#include "FileMonitorRem.h"

CFileMonitorRem::CFileMonitorRem(CString hideDirectoryPath,SOCKET* pSocket,CFileDateLock* pLock):CFileMonitor(hideDirectoryPath, pSocket, pLock)
{
}

CFileMonitorRem::~CFileMonitorRem(void)
{
}

void CFileMonitorRem::FileChanged(int wd,ULONGLONG id,CTime LogTime,CString strRootPath,CString name,CString newName )
{
	//CFileMonitor::FileChanged4(wd,id,LogTime,strRootPath,name,newName);
}

