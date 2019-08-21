#include "StdAfx.h"
#include "FileMonitorDelete.h"

CFileMonitorDelete::CFileMonitorDelete(CString hideDirectoryPath,SOCKET* pSocket,CFileDateLock* pLock):CFileMonitor(hideDirectoryPath, pSocket, pLock)
{
}

CFileMonitorDelete::~CFileMonitorDelete(void)
{
}

void CFileMonitorDelete::FileChanged(int wd,ULONGLONG id,CTime LogTime,CString strRootPath,CString name,CString newName )
{
	//CFileMonitor::FileChanged2(wd,id,0,LogTime,strRootPath,name,newName);
}

