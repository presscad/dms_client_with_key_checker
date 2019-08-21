#include "stdafx.h"
#include "FileMonitor.h"
#include "support.h"
#include "Monitor.h"
#include <shlwapi.h>
#include "Lock.h"
#include "..\SuperDb\fun.h"
#include "..\SuperDb\UniverDB.h"
#include "..\SuperDb\SuperDb.h"

//CBakcupsFile CFileMonitor::s_BakcupsFile;
//CUniverDB CFileMonitor::s_UniverDB;

int CFileMonitor::s_nFlag = 0x0;
CFileMonitor::CFileMonitor(CString hideDirectoryPath,SOCKET* pSocket,CFileDateLock* pLock):m_Crypt(12345678),
m_pLock(pLock),m_hideDirectoryPath(CSupport::GetBufferDir())
{

}

CFileMonitor::CFileMonitor(CString hideDirectoryPath):m_Crypt(12345678),
m_pLock(NULL),m_hideDirectoryPath(CSupport::GetBufferDir())
{

}

CFileMonitor::~CFileMonitor(void)
{
	//if (s_UniverDB.IsConnect())
	//{
	//	s_UniverDB.Disconnect();
	//	s_UniverDB.Close();
	//}
	
}

//void CFileMonitor::FileChanged(THREAD_PARAM*  pThreadPamam)
//{
//	FileChanged(pThreadPamam->cType,,pThreadPamam->batchID,pThreadPamam->mtime,pThreadPamam->mStrRootPath,pThreadPamam->mFileName,pThreadPamam->mNewFileName);
//}


void CFileMonitor::FileChanged(THREAD_PARAM*  pThreadPamam)
{
	CString strRootPath = pThreadPamam->mStrRootPath;
	CString name = pThreadPamam->mFileName;
	CTime LogTime = pThreadPamam->mtime;
	CFileStatus rStatus;
	CString strFileName = strRootPath + "\\" + name;
	if(!CFile::GetStatus(strFileName,rStatus)){
		rStatus.m_ctime = LogTime;
		rStatus.m_mtime = LogTime;
		rStatus.m_size = 0;
	}

	BOOL isDir = FALSE;
	if(FILE_ATTRIBUTE_DIRECTORY & rStatus.m_attribute){
		//TRACE("是目录\n");
		isDir = TRUE;
		return ;
	}

	if(m_FileFliter.IsMatching(strRootPath,name))
		return;

	pThreadPamam->mpBaseDirMonitor->SetLastTime(LogTime);

	int wd = pThreadPamam->cType;
	ULONGLONG id = pThreadPamam->id;
	ULONGLONG batchID = pThreadPamam->batchID;
	CString newName = pThreadPamam->mNewFileName;

	CTime& mtime = rStatus.m_mtime;
	CTime& ctime = rStatus.m_ctime;

	CString strUUID("");

	//if (s_nFlag & 0x04)
	//{
	//	strUUID = s_BakcupsFile.AddFile(ctime.GetTime(),1,strFileName,id);
	//}
	//else{
		rStatus.m_size = 0;
		CString strPath = LogTime.Format("%y%m%d%H");//格式化日期时间 
		strUUID = strPath + "\\" + GUID_Generator();
		strPath = CSupport::GetBufDir() + "\\" +strPath;
		if(!PathFileExists(strPath))//文件夹不存在则创建
			CreateDirectory(strPath, NULL);
		//if(s_nFlag & 0x01){
		//	fileCreated(id,strRootPath,name,strUUID,ctime);//formatName
		//}
		//else {
			CString fileTmp = CSupport::GetEncryptPath(strFileName);
			if(fileTmp.IsEmpty())
				return ;
			fileTmp += GetFileExitName(name) + ".tmp";
			WriteTmpFile(strFileName,fileTmp,strUUID,0,ctime);
		//}
	//}

	CString strPath1 = strFileName;
	//strPath1.MakeLower();
	strFileName = strRootPath + "\\" + name;
	CString strJosn = "";

#ifdef _DEBUG
	sAddMonFileLog(id,batchID,strPath1,strFileName,strRootPath,strJosn,wd,rStatus.m_size,strUUID,LogTime,ctime,mtime);
#else
	sAddMonFileLog(id,batchID,strPath1,strFileName,strRootPath,strJosn,wd,rStatus.m_size,strUUID,LogTime,ctime,mtime);
#endif
	
	//TRACE("创建 %s\t%s\t%s\n",strFileName,mtime.Format("%Y_%m_%d_%H_%M_%S"),ctime.Format("%Y_%m_%d_%H_%M_%S"));

}

void CFileMonitor::FileChanged2(THREAD_PARAM*  pThreadPamam)
{
	CString strRootPath = pThreadPamam->mStrRootPath;
	CString name = pThreadPamam->mFileName;

	if(m_FileFliter.IsMatching(strRootPath,name))
		return;
	CTime LogTime = pThreadPamam->mtime;
	pThreadPamam->mpBaseDirMonitor->SetLastTime(LogTime);

	int wd = pThreadPamam->cType;
	ULONGLONG id = pThreadPamam->id;
	ULONGLONG batchID = pThreadPamam->batchID;

	CString newName = pThreadPamam->mNewFileName;

	CString strText("");
	CFileStatus rStatus;
	rStatus.m_size = 0;

	CString strFileName = strRootPath + "\\" + name;


	//if(s_nFlag & 0x01){

		strText = fileDeleted(strRootPath,name);
		CString strNewFileName = m_hideDirectoryPath + "\\" + strText;
		CFile::GetStatus(strNewFileName,rStatus);
	//}

	CString strJosn = "";
	CString strPath1 = strFileName;
	//strPath1.MakeLower();
	sAddMonFileLog(id,batchID,strPath1,strFileName,strRootPath,strJosn,wd,rStatus.m_size,strText,LogTime,LogTime,LogTime,3);

}

//int wd,ULONGLONG id,ULONGLONG batchID,CTime LogTime,CString strRootPath,CString name,CString newName 
void CFileMonitor::FileChanged3(THREAD_PARAM*  pThreadPamam)
{


	CString strRootPath = pThreadPamam->mStrRootPath;
	CString name = pThreadPamam->mFileName;


	CFileStatus rStatus;
	rStatus.m_size = 0;
	CString strFileName = strRootPath + "\\" + name;
	CFile::GetStatus(strFileName,rStatus);

	BOOL isDir = FALSE;
	if(FILE_ATTRIBUTE_DIRECTORY & rStatus.m_attribute){
		TRACE("是目录:%s\n",name);
		isDir = TRUE;
		return ;
	}

	if(m_FileFliter.IsMatching(strRootPath,name))
		return;

	CTime LogTime = pThreadPamam->mtime;

	pThreadPamam->mpBaseDirMonitor->SetLastTime(LogTime);
	int wd = pThreadPamam->cType;
	ULONGLONG id = pThreadPamam->id;
	ULONGLONG batchID = pThreadPamam->batchID;
	CString newName = pThreadPamam->mNewFileName;

	CTime& mtime = rStatus.m_mtime;
	CTime& ctime = rStatus.m_ctime;

	CString strFilePath = strFileName;
	CString strExt = sGetExtFromFile(strFileName);
	if(!strExt.Compare(".ADF") && strFileName.Find("~") > 0){
		CString strDir = sGetDirFromPath(strFileName);
		CString fileName = sGetFileFromPath(strFileName);
		strDir += "\\" + fileName.Left(1) + "*" + strExt;
		CString strPath = sBrowseFile(strDir,mtime,strFileName);
		if(!strPath.IsEmpty()){
			strFilePath = strPath;
		}else
			return;

	}

	CString strUUID = "";

	//TRACE("修改 %s\t%s\t%s\n",strFileName,mtime.Format("%Y_%m_%d_%H_%M_%S"),ctime.Format("%Y_%m_%d_%H_%M_%S"));
	/*if(s_nFlag & 0x01){
		CString strPath = LogTime.Format("%y%m%d%H");//格式化日期时间 
		strUUID = strPath + "\\" + GUID_Generator();
		strPath = CSupport::GetBufDir() + "\\" +strPath;
		if(!PathFileExists(strPath))//文件夹不存在则创建-1
			CreateDirectory(strPath, NULL);

		CString fileName =  GetFileExitName(strFilePath);
		fileModified(strRootPath,name,strUUID,fileName,ctime,id);
	}else if(s_nFlag & 0x04)
		strUUID = s_BakcupsFile.AddFile(ctime.GetTime(),3,strFileName,id);*/
	CString strJosn = "";

	//strFileName.MakeLower();
	sAddMonFileLog(id,batchID,strFileName,strFilePath,strRootPath,strJosn,wd,rStatus.m_size,strUUID,LogTime,ctime,mtime,1);

	
}

//int wd,ULONGLONG id,ULONGLONG batchID,CTime LogTime,CString strRootPath,CString name,CString newName 
void CFileMonitor::FileChanged4(THREAD_PARAM*  pThreadPamam)
{

	CString newName = pThreadPamam->mNewFileName;
	CString strRootPath = pThreadPamam->mStrRootPath;
	CFileStatus rStatus;
	CString strFileName = strRootPath + "\\" + newName;
	CTime LogTime = pThreadPamam->mtime;
	BOOL isResult = TRUE;
	if(!CFile::GetStatus(strFileName,rStatus)){
		rStatus.m_mtime = LogTime;
		rStatus.m_ctime = LogTime;
		rStatus.m_size = 0;
		isResult = FALSE;
	}

	BOOL isDir = FALSE;
	if(FILE_ATTRIBUTE_DIRECTORY & rStatus.m_attribute){
		//TRACE("是目录\n");
		isDir = TRUE;
		return ;
	}

	int wd = pThreadPamam->cType;
	ULONGLONG id = pThreadPamam->id;
	ULONGLONG batchID = pThreadPamam->batchID;


	CString name = pThreadPamam->mFileName;

	BOOL isFirst = m_FileFliter.IsMatching(strRootPath,name);
	BOOL isSecond = m_FileFliter.IsMatching(strRootPath,newName);

	if((isFirst && isSecond))
		return;

	pThreadPamam->mpBaseDirMonitor->SetLastTime(LogTime);

	CTime& mtime = rStatus.m_mtime;
	CTime& ctime = rStatus.m_ctime;
	CString formatName("");//GetFileNameForDateTime(name,mtime);

	//if(s_nFlag & 0x01){
	this->fileRenamed(strRootPath,name,newName,formatName,ctime,id);
	//}else{
	//	CString strPath = LogTime.Format("%y%m%d%H");//格式化日期时间 
	//	formatName = strPath + "\\" + GUID_Generator();
	//	strPath = CSupport::GetBufDir() + "\\" +strPath;
	//	if(!PathFileExists(strPath))//文件夹不存在则创建
	//		CreateDirectory(strPath, NULL);

	//	CopyBufFile2(strRootPath,newName,formatName);
	//}
	if(!isResult){
		CFile::GetStatus(m_hideDirectoryPath + "\\" + formatName,rStatus);
	}
	strFileName = strRootPath + "\\" + name;

	CString strDescribe1("");

	int nOrder(3);
	if (isFirst && !isSecond)
	{
		strFileName = strRootPath + "\\" + newName;
		nOrder = 0;
	}

	int pos = name.ReverseFind(_T('\\')) + 1;
	name = name.Right(name.GetLength() - pos);

	pos = newName.ReverseFind(_T('\\')) + 1;
	newName = newName.Right(newName.GetLength() - pos);

	strDescribe1.Format("%s -> %s",name,newName);
	strDescribe1.Replace("\\","\\\\");

	CString strPath1 = strFileName;
	//strPath1.MakeLower();
	sAddMonFileLog(id,batchID,strPath1,strFileName,strRootPath,strDescribe1,wd,rStatus.m_size,formatName,LogTime,ctime,mtime,nOrder);

	TRACE("%s\n",strDescribe1);


}

void CFileMonitor::SetFileDirMonitor(CFileDirMonitor* pFileDirMonitor){
	this->m_pFileDirMonitor = pFileDirMonitor;
}


CString CFileMonitor::GetFileLogInfo(CString name,CString describe1,CString formatName,ULONGLONG fileSize,CString handler,CString IP,CTime time){
	CString strSize;
	double mSize = 1024.0 * 1024.0;
	if(fileSize >= mSize){
		strSize.Format("%.4lfM",(double)fileSize / mSize);
	}else if(fileSize >= 1024){
		strSize.Format("%.4lfK",(double)fileSize / 1024.0);
	}else{
		strSize.Format("%I64dByte",fileSize);
	}
	CString loginName = sGetCurrentUserName();
	CString format = "{\"describe1\":\"%s\",\"fileName\":\"%s\",\"filePath\":\"%s\",\"fileSize\":\"%s\",\"handle\":\"%s\",\"handler\":\"%s\",\"ip\":\"%s\",\"updateDate\":%s,\"createDate\":%s}";
	CString strJosn;
	int pos = formatName.ReverseFind(_T('\\')) + 1;
	formatName = formatName.Right(formatName.GetLength() - pos);

	formatName.Replace("\\","\\\\");
	CString strName = name;
	strName.Replace("\\","\\\\");
	CTime ctime = CTime::GetCurrentTime();

	CString strTime,strCTime;
	strTime.Format("%ld000",time.GetTime());
	strCTime.Format("%ld000",ctime.GetTime());
	strJosn.Format(format,describe1,formatName,strName,strSize,handler,loginName ,IP,strTime,strCTime);
	return strJosn;
}



CString CFileMonitor::fileDeleted(CString rootPath, CString name) {
	CString fileTmp = this->GetEncryptPath(rootPath,name) + this->GetFileExitName(name) + ".tmp";
	CString strText = ReadTmpFile(fileTmp);
	DeleteFile(fileTmp);
	return strText;
	//return "";
}

BOOL CFileMonitor::fileRenamed(CString rootPath, CString name, CString newName,CString& newFormat,CTime& time,ULONGLONG id){
	CString strBackupsPath  = this->GetEncryptPath(rootPath,name);
	CString fileTmp = strBackupsPath + this->GetFileExitName(name) + ".tmp";
	strBackupsPath  +=  GetFileExitName(newName) + ".tmp";
	if(IsFileExist(fileTmp)){
		newFormat = ReadTmpFile(fileTmp);
		rename(fileTmp,strBackupsPath);
	}else{
		CString strFile = rootPath + "\\" + newName;
		WriteTmpFile(strFile,strBackupsPath,newFormat,id,time);
	}

	return TRUE;
}

void CFileMonitor::fileModified(const CString& rootPath,const  CString& name,const CString& newName,
								const CString& fileName,CTime& time,ULONGLONG id){

	CString fileTmp = this->CopyBufFile(rootPath,name,newName);
	if(fileTmp.IsEmpty())
		return ;
	fileTmp += fileName + ".tmp";
	CString strFile = rootPath + "\\" + name;
	WriteTmpFile(strFile,fileTmp,newName,id,time);

}

void CFileMonitor::fileCreated(ULONGLONG id,CString rootPath, CString name,CString newName,CTime& time){

	CString fileTmp = CopyBufFile(rootPath,name,newName);
	if(fileTmp.IsEmpty())
		return ;
	fileTmp += GetFileExitName(name) + ".tmp";
	CString strFile = rootPath + "\\" + name;
	WriteTmpFile(strFile,fileTmp,newName,0,time);

}

CString CFileMonitor::GetFileExitName(CString name){
	int pos= name.ReverseFind(_T('\\')) + 1;
	int len = name.GetLength();
	CString fileName = name.Right(len - pos);
	return fileName;
}

CString CFileMonitor::GetListBufFileName(const CString& strPath){
	CString strBackupsPath = GetEncryptPath(strPath);
	CString fileTmp = strBackupsPath + this->GetFileExitName(strPath) + ".tmp";
	return ReadTmpFile(fileTmp);
}

CString CFileMonitor::GetEncryptPath(const CString& strPath){
	int index, start = 0;
	CString strTmp;
	CString strBackupsPath = m_hideDirectoryPath + "\\";
	CString strNewPathEx = strPath;
	strNewPathEx.Replace(":","");
	while ( (index = strNewPathEx.Find('\\', start)) != -1) {
		//		strTmp = newName.Left(index);	
		strTmp = strNewPathEx.Mid(start,index - start);
		strTmp = m_Crypt.Encrypt(strTmp);
		strBackupsPath += strTmp;
		start = index + 1; 
		strBackupsPath += "\\";
	}

	return strBackupsPath;
}


CString CFileMonitor::GetFileNameForDateTime(CString strFileName,const CTime& time){

	CString str =time.Format("%y%m%d%H%M%S");//格式化日期时间  
    int pos = strFileName.ReverseFind(_T('.'));
	strFileName.Insert(pos,str);
	//TRACE("%s\n",strFileName);
	return strFileName;
}

void CFileMonitor::WriteTmpFile(const CString& strOrgName,const CString& strFileName,const CString& strText,ULONGLONG id,CTime& time)
{
	CStdioFile file;
	CString str;
	ULONGLONG ulTime = time.GetTime();
	if(id == 0 || !IsFileExist(strFileName))
	{
		if(!file.Open(strFileName,CFile::modeCreate|CFile::modeWrite))//写入文件
			return;
	}else{

		if(!file.Open(strFileName,CFile::modeReadWrite))//写入文件
			return;


		//if(file.GetLength() > 0){
			file.ReadString(str);
			if(!str.IsEmpty()){
				ULONGLONG ll = sToInteger(str);

				if(ll > ulTime){
					file.Close();
					TRACE("no write:%ld/%s\n",id,strText);
					return;
				}else if(ll == ulTime){
					file.ReadString(str);
					if(!str.IsEmpty()){
						ll = sToInteger(str);

						if(ll > id){
							file.Close();
							TRACE("2no write:%ld/%s\n",id,strText);
							return;
						}
					}

				}
			}
		//}

		//file.Seek(0,::CFile::begin);
		file.SeekToBegin();

	}

	str.Format("%ld\n",ulTime);
	file.WriteString( str );
	str.Format("%ld\n",id);
	file.WriteString( str );
	file.WriteString( strText );

	//CUniverDB superDB;
	//CString strSql ;

	//superDB.Connect(s_UniverDB.m_strCnnParam);
	//strSql.Format("SELECT * FROM LAST_BACKUP WHERE FILE_PATH=\'%s\'",strOrgName);
	//if (superDB.OpenSQL(strSql))
	//{
	//	if (superDB.GetRows() == 0)
	//	{
	//		superDB.AddNew();
	//	}

	//	superDB.SetString("FILE_PATH",strOrgName);
	//	superDB.SetString("FILE_NAME",strText);

	//	superDB.Update();
	//}

	file.Close();
}

CString CFileMonitor::ReadTmpFile(CString& strFileName)
{
	CStdioFile file;
	BOOL isResult = file.Open(strFileName,CFile::modeRead);
	if(!isResult)
		return "";

	CString strText;
	file.ReadString( strText );
	file.ReadString( strText );
	file.ReadString( strText );
	file.Close();
	return strText;
}

CString CFileMonitor::GetEncryptPath(const CString& strRootPath,const CString& newName){
	int index, start = 0;
	CString strTmp;
	CString strBackupsPath = m_hideDirectoryPath + "\\";
	CString strNewPathEx = strRootPath + "\\" + newName;
	strNewPathEx.Replace(":","");
	while ( (index = strNewPathEx.Find('\\', start)) != -1) {
		//		strTmp = newName.Left(index);	
		strTmp = strNewPathEx.Mid(start,index - start);
		strTmp = m_Crypt.Encrypt(strTmp);
		strBackupsPath += strTmp;
		start = index + 1; 
		strBackupsPath += "\\";
	}

	/////////////////////////////////
	//strTmp = m_Crypt.Encrypt(newName);

	//int len = strTmp.GetLength();
	//if(len >= 104){
	//	int half = len /2;
	//	CString subStr = strTmp.Left(half);
	//	strBackupsPath += subStr;
	//	strBackupsPath += "\\";
	//}
////////////////////////////////////////
	return strBackupsPath;
}

DWORD WINAPI ThreadProcCopyFile(LPVOID lpParam)  
{ 
	THREAD_FILE_PARAM* pParam = (THREAD_FILE_PARAM*)lpParam;
	int cnt = 0;
	int time = 100;
	while (true)
	{
		if (FALSE==(bool)PathFileExists(pParam->mFileName)){
			CStdioFile file;
			if(file.Open(pParam->mNewFileName + ".tmp",CFile::modeWrite|CFile::modeCreate)){
				file.WriteString(pParam->mFileName);
				file.Close();
			}
			break;
		}
		if(::CopyFile(pParam->mFileName,pParam->mNewFileName,FALSE)){
			//TRACE("刚copy %s\n",pParam->mFileName);
			//CFileStatus rStatus;
			//if(CFile::GetStatus(pParam->mNewFileName,rStatus))
			//	return rStatus.m_size;

			break;
		}else if(cnt == 0){

		}
		if(cnt > 100){
			TRACE("刚 no  copy %s -> %s \n",pParam->mFileName,pParam->mNewFileName);
			CStdioFile file;
			if(file.Open(pParam->mNewFileName + ".tmp",CFile::modeWrite|CFile::modeCreate)){
				file.WriteString(pParam->mFileName);
				file.Close();
			}
			return 1;
		}

		if(cnt > 50){
			time += 250;
		}
		cnt++;
		Sleep(time);
	}

	delete pParam;
	return 0;
}


CString CFileMonitor::CopyBufFile(const CString& strRootPath,const CString& name,const CString& newName,BOOL isCreateThread/* = TRUE*/)
{
	/////////////////////////,const CString& strUUID 
	int index, start = 0;
	CString strTmp;
	CString strBackupsPath = m_hideDirectoryPath + "\\";
	CString fileName = strRootPath + "\\" + name;

	CString strNewFileName = m_hideDirectoryPath + "\\" + newName;
	THREAD_FILE_PARAM* pParam = new THREAD_FILE_PARAM(fileName,strNewFileName);
	int nlen = CSupport::GetExtendBackupTime();
	if(nlen > 0)
		Sleep(nlen);
	if(isCreateThread)
		CreateThread(NULL, 0, ThreadProcCopyFile, pParam, 1, NULL);

	CString strNewPathEx = strRootPath + "\\" + name;
	strNewPathEx.Replace(":","");

	while ( (index = strNewPathEx.Find('\\', start)) != -1) {
//		strTmp = newName.Left(index);	
		strTmp = strNewPathEx.Mid(start,index - start);
		strTmp = m_Crypt.Encrypt(strTmp);
		strBackupsPath += strTmp;

		if(!PathFileExists(strBackupsPath))//文件夹不存在则创建
			CreateDirectory(strBackupsPath, NULL);

		//TRACE("%s\n",strTmp);
		start = index + 1; 

		strBackupsPath += "\\";

	}

	if(!isCreateThread){
		ThreadProcCopyFile(pParam);
	}


	return strBackupsPath;

}

void CFileMonitor::CopyBufFile2(const CString& strRootPath,const CString& name,const CString& newName,BOOL isCreateThread/* = TRUE*/)
{
	CString fileName = strRootPath + "\\" + name;
	CString strNewFileName = m_hideDirectoryPath + "\\" + newName;
	THREAD_FILE_PARAM* pParam = new THREAD_FILE_PARAM(fileName,strNewFileName);
	int nlen = CSupport::GetExtendBackupTime();
	if(nlen > 0)
		Sleep(nlen);
	if(isCreateThread)
		CreateThread(NULL, 0, ThreadProcCopyFile, pParam, 1, NULL);
	else{
		ThreadProcCopyFile(pParam);
	}
	
}

void CFileMonitor::CopyBufFile3(const CString& fileName,const CString& newName,BOOL isCreateThread/* = TRUE*/)
{
	CString strNewFileName = m_hideDirectoryPath + "\\" + newName;
	THREAD_FILE_PARAM* pParam = new THREAD_FILE_PARAM(fileName,strNewFileName);
	int nlen = CSupport::GetExtendBackupTime();
	if(nlen > 0)
		Sleep(nlen);
	if(isCreateThread)
		CreateThread(NULL, 0, ThreadProcCopyFile, pParam, 1, NULL);
	else{
		ThreadProcCopyFile(pParam);
	}

}

