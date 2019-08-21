#pragma once
#include <vector>
#include <map>
using namespace std;

//typedef void (*PFunSetWebPID)(int pid); 

//typedef void (*sInitFileDirIDEx)(ULONGLONG& id,const CString& strDir);

//void (*pSetWebPID)(sInitFileDirID ) = NULL;

//static sInitFileDirIDEx  s_pInitFileDirID = NULL;

class CFileBaseMonitor;
class __declspec(dllexport) CBaseSupport
{
public:
	CBaseSupport(void);
	~CBaseSupport(void);

	static CFileBaseMonitor* m_pFileMonitor;

	//static map<CString,CTime>*  m_pLastFileTime;


	static void sSetFileBaseMonitor(CFileBaseMonitor* pFileMonitor);

	static CFileBaseMonitor*  sGetFileBaseMonitor();

	//static void SetFunInitFileDirID(sInitFileDirIDEx fun);

	//static void InitFileDirID(ULONGLONG& id,const CString& strDir);

	//static void SetMonitorLastFileTime(const CString& strDir,CTime  time);

	//static void SetMonitorLastFileTime(map<CString,CTime>*  pLastFileTime);

};


