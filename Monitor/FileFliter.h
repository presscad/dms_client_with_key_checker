#pragma once
#include "Lock.h"
class CFileFliter
{
public:
	CFileFliter(void);
	~CFileFliter(void);
	BOOL IsMatching(const CString& rootPath,const CString& str);
	static CStringArray  m_arrFliter;
	static Mutex g_Lock2; 
};

