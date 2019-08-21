#include "StdAfx.h"
#include "FileDateLock.h"
#include "Support.h"

CFileDateLock::CFileDateLock(void)
{
}

CFileDateLock::CFileDateLock(CFileDirMonitor* pFileDirMonitor){
	m_pFileDirMonitor = pFileDirMonitor;
}

CFileDateLock::~CFileDateLock(void)
{
	s_MapFile.clear();
}

BOOL CFileDateLock::IsChange(CString name,int& wd,const CTime& datetime){
	BOOL isChange = FALSE;
	//return TRUE;
	name = name.MakeLower();
	CLock lock(m_LockMap);

	TRACE("--------%s\t%d\n",name,wd);
	std::map<CString,FILE_INFO>::iterator it = s_MapFile.find(name);
	if(it== s_MapFile.end()){
		s_MapFile[name] = FILE_INFO(datetime,wd);
		isChange = TRUE;
	}else{
		long log = datetime.GetTime()  - s_MapFile[name].time.GetTime() ;
		//if(log == 0 && s_MapFile[name].dw == 3 && wd == 1){
		//	return TRUE;
		//}
		s_MapFile[name].time = datetime;
		if( log > CSupport::GetContinuousOperationTime())
			isChange = TRUE;
	}

	return isChange;
}

void CFileDateLock::RomoveMapFile(CString name){
	name = name.MakeLower();
	CLock lock(m_LockMap); 
	std::map<CString,FILE_INFO>::iterator it = s_MapFile.find(name);
	if(it!= s_MapFile.end()){
		s_MapFile.erase(it);
	}
}
