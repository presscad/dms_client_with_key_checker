#include "stdafx.h"
#include "BaseSupport.h"
#include "FileBaseMonitor.h"


CFileBaseMonitor* CBaseSupport::m_pFileMonitor(NULL);
//map<CString,CTime>*  CBaseSupport::m_pLastFileTime(NULL);
CBaseSupport::CBaseSupport(void)
{
}


CBaseSupport::~CBaseSupport(void)
{
}

void CBaseSupport::sSetFileBaseMonitor(CFileBaseMonitor* pFileMonitor){
	CBaseSupport::m_pFileMonitor = pFileMonitor;
}

CFileBaseMonitor*  CBaseSupport::sGetFileBaseMonitor(){
	return CBaseSupport::m_pFileMonitor;
}

//void CBaseSupport::SetFunInitFileDirID(sInitFileDirIDEx fun)
//{
//	s_pInitFileDirID = fun;
//}

//void CBaseSupport::InitFileDirID(ULONGLONG& id,const CString& strDir)
//{
//	if (s_pInitFileDirID){
//		s_pInitFileDirID(id,strDir);
//	}
//}

//void CBaseSupport::SetMonitorLastFileTime(const CString& strDir,CTime  time)
//{
//	if (m_pLastFileTime)
//	{
//		(*m_pLastFileTime)[strDir] = time;
//	}
//	
//}
//
//void CBaseSupport::SetMonitorLastFileTime(map<CString,CTime>*  pLastFileTime)
//{
//	m_pLastFileTime = pLastFileTime;
//}
