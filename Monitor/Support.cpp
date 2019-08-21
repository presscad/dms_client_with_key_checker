#include "stdafx.h"
#include "Support.h"
#include "..\BaseDB\FileBaseMonitor.h"
#include <Shlwapi.h>


CCrypt CSupport::s_Crypt(12345678);
CString CSupport::s_Ip("127.0.0.1");
int CSupport::s_LogPort = 14875;
int CSupport::s_FilePort = 14876;
CString CSupport::s_bufDir("d:\\testh");
int CSupport::s_WebPort = 14878;
int CSupport::s_coTime = 10;
int  CSupport::s_Millisecond = 0;

CFileBaseMonitor* CSupport::m_pFileMonitor(NULL);
CSupport::CSupport(void)
{
}


CSupport::~CSupport(void)
{
}


CString CSupport::GetEncryptPath(const CString& strRootPath){
	int index, start = 0;
	CString strTmp;
	CString strBackupsPath = s_bufDir + "\\";
	CString strNewPathEx = strRootPath;
	strNewPathEx.Replace(":","");
	while ( (index = strNewPathEx.Find('\\', start)) != -1) {
		//		strTmp = newName.Left(index);	
		strTmp = strNewPathEx.Mid(start,index - start);
		strTmp = s_Crypt.Encrypt(strTmp);
		strBackupsPath += strTmp;
		if(!PathFileExists(strBackupsPath))//文件夹不存在则创建
			CreateDirectory(strBackupsPath, NULL);
		start = index + 1; 
		strBackupsPath += "\\";
	}

	return strBackupsPath;

}

void CSupport::split(CString srcStr,char chRegex,vector<CString>& vec){
	int index, start = 0;
	CString strTmp;
	CString strBackupsPath = "";
	while ( (index = srcStr.Find(chRegex, start)) != -1) {
		//		strTmp = newName.Left(index);	
		strTmp = srcStr.Mid(start,index - start);

		strBackupsPath += strTmp;
		start = index + 1; 
		vec.push_back(strTmp);
		//strBackupsPath += "\\";
	}
	int len = srcStr.GetLength();
	strTmp = srcStr.Right( len - start);
	vec.push_back(strTmp);

}

int CSupport::GetIntsIP(int ips[]){
	vector<CString> vec;
	split(s_Ip,'.',vec);

	ips[0] = toInt(vec[0]);
	ips[1] = toInt(vec[1]);
	ips[2] = toInt(vec[2]);
	ips[3] = toInt(vec[3]);

	return 1;
}

CString CSupport::GetServerIP()
{
	return s_Ip;
}

void CSupport::SetServerIP(CString ip)
{
	s_Ip = ip;
}

int CSupport::GetLogPort()
{
	return s_LogPort;
}

void CSupport::SetLogPort(int port)
{
	s_LogPort = port;
}

void CSupport::SetLogPort(LPCSTR strVal)
{
	s_LogPort = toInt(strVal);
}

void CSupport::SetWebPort(LPCSTR strVal)
{
	s_WebPort = toInt(strVal);
}

int CSupport::GetWebPort()
{
	return s_WebPort;
}

int CSupport::GetFilePort()
{
	return s_FilePort;
}

void CSupport::SetFilePort(int port)
{
	s_FilePort = port;
}

void CSupport::SetFilePort(LPCSTR strVal)
{
	s_FilePort = toInt(strVal);
}

long CSupport::toLong(LPCSTR strVal)
{
	if (strlen(strVal) <= 0)
		return 0;

	long nVal = 0;
	sscanf_s(strVal, "%d", &nVal);
	return nVal;
}

int CSupport::toInt(LPCSTR strVal)
{
	if (strlen(strVal) <= 0)
		return 0;

	long nVal = 0;
	sscanf_s(strVal, "%d", &nVal);
	return nVal;
}

CString CSupport::GetBufDir()
{
	return s_bufDir;
}

CString& CSupport::GetBufferDir()
{
	return s_bufDir;
}

void CSupport::SetBufDir(CString dir)
{
	s_bufDir = dir;
}

int CSupport::GetContinuousOperationTime(){
	return s_coTime;
}
void CSupport::SetContinuousOperationTime(int timeLen){
	s_coTime = timeLen;
}

CString CSupport::GetFileNameForDateTime(CString strFileName,const COleDateTime& time){

	CString str =time.Format("%Y_%m_%d_%H_%M_%S");//格式化日期时间  
	int pos = strFileName.ReverseFind(_T('.'));
	strFileName.Insert(pos,str);
	//TRACE("%s\n",strFileName);
	return strFileName;
}

CString CSupport::GetFileNameForDateTime(CString strFileName,const CTime& time){

	CString str =time.Format("%Y_%m_%d_%H_%M_%S");//格式化日期时间  
	int pos = strFileName.ReverseFind(_T('.'));
	strFileName.Insert(pos,str);
	//TRACE("%s\n",strFileName);
	return strFileName;
}

void CSupport::SetExtendBackupTime(int Millisecond){
	s_Millisecond = Millisecond;
}

void CSupport::SetExtendBackupTime(LPCSTR Millisecond){
	s_Millisecond = toInt(Millisecond);
}

int  CSupport::GetExtendBackupTime(){
	return s_Millisecond;
}

void CSupport::sSetFileBaseMonitor(CFileBaseMonitor* pFileMonitor){
	CSupport::m_pFileMonitor = pFileMonitor;
}

CFileBaseMonitor*  CSupport::sGetFileBaseMonitor(){
	return CSupport::m_pFileMonitor;
}



// 判断文件是否存在
BOOL IsFileExist(const CString& csFile)
{
	DWORD dwAttrib = GetFileAttributes(csFile);
	return INVALID_FILE_ATTRIBUTES != dwAttrib && 0 == (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}
// 判断文件夹是否存在
BOOL IsDirExist(const CString & csDir)
{
	DWORD dwAttrib = GetFileAttributes(csDir);
	return INVALID_FILE_ATTRIBUTES != dwAttrib && 0 != (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}
// 判断文件或文件夹是否存在
BOOL IsPathExist(const CString & csPath)
{
	DWORD dwAttrib = GetFileAttributes(csPath);
	return INVALID_FILE_ATTRIBUTES != dwAttrib;
}

// 变变变变变种(听说会更快一点)，见备注1
BOOL IsPathExist2(const CString & csPath)
{
	WIN32_FILE_ATTRIBUTE_DATA attrs = { 0 };
	return 0 != GetFileAttributesEx(csPath, GetFileExInfoStandard, &attrs);
}
