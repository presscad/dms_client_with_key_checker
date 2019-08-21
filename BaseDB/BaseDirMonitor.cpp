#include "StdAfx.h"
#include "BaseDirMonitor.h"
#include "DirectoryChanges.h"

CDirectoryChangeWatcher	CBaseDirMonitor::m_DirWatcher;
CListBox* CBaseDirMonitor::m_pListInfo(NULL);
CBaseDirMonitor::CBaseDirMonitor(void)
{
}

CBaseDirMonitor::~CBaseDirMonitor(void)
{
}

void CBaseDirMonitor::StartFileMonitor(DWORD 	dwNotifyFilter,int password){

	//CString m_strIncludeFilter2(""),m_strExcludeFilter2("");
	DWORD dwWatch;
	if( ERROR_SUCCESS != (dwWatch = m_DirWatcher.WatchDirectory(GetMonitorDir(), 
		dwNotifyFilter,
		this,
		TRUE)) )
	{
		TRACE(_T("Failed to start watch:\n"));
	}



}

void CBaseDirMonitor::SetListBox(CListBox* pList){
	m_pListInfo = pList;
}


void CBaseDirMonitor::StopFileMonitor(){

	m_DirWatcher.UnwatchDirectory(GetMonitorDir());

}


