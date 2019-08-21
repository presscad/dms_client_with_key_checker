#pragma once
#include "Crypt.h"
#include <vector>
using namespace std;

class CFileBaseMonitor;
class   __declspec(dllexport) CSupport
{
public:
	CSupport(void);
	~CSupport(void);
	static CString GetEncryptPath(const CString& strRootPath);
	static void split(CString srcStr,char chRegex,vector<CString>& vec);
	static CCrypt s_Crypt;

	static CString s_Ip;
	static int s_LogPort;
	static int s_FilePort;
	static int s_WebPort;
	static int s_Millisecond;
	static CString s_bufDir;
	static CString GetServerIP();
	static void SetServerIP(CString ip);

	static int GetLogPort();
	static void SetLogPort(int port);
	static void SetLogPort(LPCSTR strVal);
	static int GetFilePort();
	static void SetFilePort(int port);
	static void SetFilePort(LPCSTR strVal);
	static void CSupport::SetWebPort(LPCSTR strVal);
	static int CSupport::GetWebPort();
	static CString& GetBufferDir();

	static void SetExtendBackupTime(LPCSTR Millisecond);

	static void SetExtendBackupTime(int Millisecond);
	static int  GetExtendBackupTime();
	static long toLong(LPCSTR strVal);

	static int toInt(LPCSTR strVal);

	static CString GetBufDir();
	static void SetBufDir(CString dir);

	static int GetIntsIP(int ips[]);

	static int s_coTime ;//Continuous operation time

	static int GetContinuousOperationTime();
	static void SetContinuousOperationTime(int timeLen);
	static CString GetFileNameForDateTime(CString strFileName,const COleDateTime& time);
	static CString GetFileNameForDateTime(CString strFileName,const CTime& time);

	static CFileBaseMonitor* m_pFileMonitor;


	static void sSetFileBaseMonitor(CFileBaseMonitor* pFileMonitor);

	static CFileBaseMonitor*  sGetFileBaseMonitor();


};
BOOL IsFileExist(const CString& csFile);
BOOL IsDirExist(const CString & csDir);
BOOL IsPathExist(const CString & csPath);
BOOL IsPathExist2(const CString & csPath);

