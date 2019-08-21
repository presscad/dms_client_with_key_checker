#include "StdAfx.h"
#include "FileLogList.h"

CFileLogList::CFileLogList(void)
{
}

CFileLogList::~CFileLogList(void)
{
}

void CFileLogList::AddFileLogs(CString strFileLogs)
{
	this->m_vecFileLogs.push_back(strFileLogs);
}

void CFileLogList::GetFileLogs(CStringArray& arrFileLog){
	std::vector<CString>::size_type cnt = this->m_vecFileLogs.size();
	if(cnt == 1){
		arrFileLog.Add(m_vecFileLogs[0]);
		m_vecFileLogs.clear();
	}else if(cnt > 2){
		//std::vector<CString>::iterator first = m_vecFileLogs.begin();
		//std::vector<CString>::iterator last = m_vecFileLogs.end() - 1;
		int last = cnt - 1;
		arrFileLog.Add(m_vecFileLogs[0]);
		arrFileLog.Add(m_vecFileLogs[last]);
		m_vecFileLogs.clear();
	}
}
