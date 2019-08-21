#include "StdAfx.h"
#include "FileMonitorModify.h"
#include "..\SuperDb\fun.h"

CFileMonitorModify::CFileMonitorModify(CString hideDirectoryPath,SOCKET* pSocket,CFileDateLock* pLock):CFileMonitor(hideDirectoryPath, pSocket, pLock)
{
}

CFileMonitorModify::~CFileMonitorModify(void)
{
}

void CFileMonitorModify::FileChanged(int wd,ULONGLONG id,CTime LogTime,CString strRootPath,CString name,CString newName )
{
	//CString ext = sGetExtFromFile(name);
	//if(!ext.CompareNoCase(".BKH"))
	//	return;

	//CFileMonitor::FileChanged3(3,id,LogTime,strRootPath,name,newName);
	//CString strNew = name;
	//if(!ext.CompareNoCase(".AIF")){
	//	strNew.Replace(ext,".BKH");
	//	Sleep(500);
	//	CFileMonitor::FileChanged3(3,id + 1,LogTime,strRootPath,strNew,newName);
	//}

}

