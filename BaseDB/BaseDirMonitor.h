#pragma once
//#include "DirectoryChanges.h"


class CDirectoryChangeWatcher;
class __declspec(dllexport) CBaseDirMonitor
{
public:
	CBaseDirMonitor(void);
public:
	~CBaseDirMonitor(void);

	void StartFileMonitor(DWORD 	dwNotifyFilter,int password);
	void StopFileMonitor();

	static 	CDirectoryChangeWatcher	m_DirWatcher;

	static CListBox* m_pListInfo;

	void SetListBox(CListBox* pList);

	virtual void SetLastTime(CTime time) = 0;

	ULONGLONG* m_startID;

	virtual void SetStartID(ULONGLONG startID) = 0;

	virtual ULONGLONG GetStartID() = 0;

	virtual CString GetMonitorDir() = 0;
};
