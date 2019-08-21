#pragma once
#include <vector>

class CFileLogList
{
public:
	CFileLogList(void);
public:
	~CFileLogList(void);

	std::vector<CString> m_vecFileLogs;

	void AddFileLogs(CString strFileLogs);

	void GetFileLogs(CStringArray& arrFileLog);
};
