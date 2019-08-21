#pragma once

class CFileLogInfo
{
public:
	CFileLogInfo(void);
public:
	~CFileLogInfo(void);

	CString m_strFilePath;

	CString m_sendFileLog;

	CTime m_lastTime;
};
