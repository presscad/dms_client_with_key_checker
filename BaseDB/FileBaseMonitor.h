#pragma once

#include "DirectoryChanges.h"

class  __declspec(dllexport) CFileBaseMonitor
{
public:
	CFileBaseMonitor(void);
public:
	~CFileBaseMonitor(void);

	//virtual void FileChanged(int wd,ULONGLONG id,ULONGLONG batchID,CTime LogTime,CString strRootPath,CString name,CString newName = _T("")) = 0;
	//virtual void FileChanged2(int wd,ULONGLONG id,ULONGLONG batchID,CTime LogTime,CString strRootPath,CString name,CString newName = _T("")) = 0;
	//virtual void FileChanged3(int wd,ULONGLONG id,ULONGLONG batchID,CTime LogTime,CString strRootPath,CString name,CString newName = _T("")) = 0;
	//virtual void FileChanged4(int wd,ULONGLONG id,ULONGLONG batchID,CTime LogTime,CString strRootPath,CString name,CString newName = _T("")) = 0;


	virtual void FileChanged(THREAD_PARAM*  pThreadPamam) = 0;
	virtual void FileChanged2(THREAD_PARAM*  pThreadPamam) = 0;
	virtual void FileChanged3(THREAD_PARAM*  pThreadPamam) = 0;
	virtual void FileChanged4(THREAD_PARAM*  pThreadPamam) = 0;


};
