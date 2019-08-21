#include "StdAfx.h"
#include "FileDirMonitor.h"
#include "FileMonitorAdd.h"
#include "FileMonitorDelete.h"
#include "FileMonitorModify.h"
#include "FileMonitorRem.h"
#include "Support.h"
#include "Monitor.h"
#include "../BaseDB/BaseSupport.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD 	dwNotifyFilter = 	FILE_NOTIFY_CHANGE_FILE_NAME |  // 修改文件名  
FILE_NOTIFY_CHANGE_ATTRIBUTES | // 修改文件属性  
FILE_NOTIFY_CHANGE_CREATION|
FILE_NOTIFY_CHANGE_LAST_WRITE | // 最后一次写入 
FILE_NOTIFY_CHANGE_DIR_NAME	//重命名文件夹 
;

CFileDirMonitor::CFileDirMonitor(CString strDir):m_strSql(""),m_strDir(strDir),m_index(0),m_strFilePath("FILE_PATH")
{
	StartFileMonitor(dwNotifyFilter,1);
}

CFileDirMonitor::~CFileDirMonitor(void)
{
	StopFileMonitor();
}

void CFileDirMonitor::SetLastTime(CTime time){
	m_LastTime = time;
}

CTime CFileDirMonitor::GetLastTime(){
	return m_LastTime;
}

CString CFileDirMonitor::GetMonitorDir(){
	return m_strDir;
}

void CFileDirMonitor::BrowseFile(CString &strFile) 
{ 
	CFileFind ff; 
	CString szDir = strFile; 

	if(szDir.Right(1) != L"\\") 
		szDir += L"\\"; 

	szDir += L"*.*"; 

	BOOL res = ff.FindFile(szDir); 
	while(res) 
	{ 
		res = ff.FindNextFile(); 
		if(ff.IsDirectory() && !ff.IsDots()) 
		{ 
			//如果是一个子目录，用递归继续往深一层找 
			CString strPath = ff.GetFilePath(); 

			CString subPath; 

			m_mapDirFileStatus[strPath] = SCANNED_FILE();

			BrowseFile(strPath); 
		} 
		else if(!ff.IsDirectory() && !ff.IsDots()) 
		{ 
			//显示当前访问的文件(完整路径) 
			CString strPath = ff.GetFilePath(); 

			CFileStatus rStatus;
			if(CFile::GetStatus(strPath,rStatus)){
				TRACE("%s\n",strPath);
				m_mapDirFileStatus[strFile].mapFileStatus[strPath] = rStatus;
				//if(m_mapDirFileStatus.find(strFile) == fi)
			}

			
		} 
	} 

	//关闭 
	ff.Close(); 
} 

void CFileDirMonitor::StartFileMonitor1(DWORD 	dw){

	HANDLE hThread = CreateThread(NULL, 0, CFileDirMonitor::ThreadProc, this, 0, NULL);
	if(INVALID_HANDLE_VALUE != hThread){
		m_ThreadHandle = hThread;
	}

}

void CFileDirMonitor::SetStartID(ULONGLONG startID){
	m_startID = startID;
}

ULONGLONG CFileDirMonitor::GetStartID(){
	return m_startID;
}

void CFileDirMonitor::StopFileMonitor1(){

	TerminateThread(m_ThreadHandle,0);
	CloseHandle(m_ThreadHandle);
	CloseHandle(m_FileHandle); 

}

//DWORD WINAPI CFileDirMonitor::ThreadProcMonitor(LPVOID lpParam)  
//{ 
//	THREAD_PARAM_MON*  pThreadPamam = (THREAD_PARAM_MON*)lpParam;
//	CFileMonitor* fileMonitor = (CFileMonitor*)pThreadPamam->mpParam;
//	fileMonitor->FileChanged(pThreadPamam);
//	//fileMonitor->FileChanged(pThreadPamam->cType,pThreadPamam->id,pThreadPamam->batchID,pThreadPamam->mtime,pThreadPamam->mStrRootPath,pThreadPamam->mFileName,pThreadPamam->mNewFileName);
//	//TRACE("%d\t%s\t%s\n",pThreadPamam->cType,pThreadPamam->mFileName,pThreadPamam->mNewFileName);
//	delete pThreadPamam;
//	return 0;
//}

DWORD WINAPI CFileDirMonitor::FileMonitor1(LPVOID lpParam)  
{ 
	THREAD_PARAM*  pThreadPamam = (THREAD_PARAM*)lpParam;
	CFileBaseMonitor* fileMonitor = (CFileBaseMonitor*)pThreadPamam->mpParam;
	fileMonitor->FileChanged(pThreadPamam);
	delete pThreadPamam;
	return 0;
}


DWORD WINAPI CFileDirMonitor::FileMonitor2(LPVOID lpParam)  
{ 
	THREAD_PARAM*  pThreadPamam = (THREAD_PARAM*)lpParam;
	CFileBaseMonitor* fileMonitor = (CFileBaseMonitor*)pThreadPamam->mpParam;
	fileMonitor->FileChanged2(pThreadPamam);
	//TRACE("%d\t%s\t%s\n",pThreadPamam->cType,pThreadPamam->mFileName,pThreadPamam->mNewFileName);
	delete pThreadPamam;
	return 0;
}


DWORD WINAPI CFileDirMonitor::FileMonitor3(LPVOID lpParam)  
{ 
	THREAD_PARAM*  pThreadPamam = (THREAD_PARAM*)lpParam;
	CFileBaseMonitor* fileMonitor = (CFileBaseMonitor*)pThreadPamam->mpParam;
	fileMonitor->FileChanged3(pThreadPamam);
	//TRACE("%d\t%s\t%s\n",pThreadPamam->cType,pThreadPamam->mFileName,pThreadPamam->mNewFileName);
	delete pThreadPamam;
	return 0;
}

DWORD WINAPI CFileDirMonitor::FileMonitor4(LPVOID lpParam)  
{ 
	THREAD_PARAM*  pThreadPamam = (THREAD_PARAM*)lpParam;
	CFileBaseMonitor* fileMonitor = (CFileBaseMonitor*)pThreadPamam->mpParam;
	fileMonitor->FileChanged4(pThreadPamam);
	//TRACE("%d\t%s\t%s\n",pThreadPamam->cType,pThreadPamam->mFileName,pThreadPamam->mNewFileName);
	delete pThreadPamam;
	return 0;
}

DWORD WINAPI CFileDirMonitor::ThreadProc(LPVOID lpParam)  
{  
	BOOL bRet = FALSE;  
	BYTE Buffer[1024] = { 0 }; 
	CFileDirMonitor* pFileDirMonitor = (CFileDirMonitor*)lpParam;

	CString strRootPath = pFileDirMonitor->m_strDir;
	if(strRootPath.IsEmpty())
		return 1;
	FILE_NOTIFY_INFORMATION *pBuffer = (FILE_NOTIFY_INFORMATION *)Buffer;  
	DWORD BytesReturned = 0;  
	//HANDLE hFile = CreateFile(strRootPath,  
	//	FILE_LIST_DIRECTORY,   
	//	FILE_SHARE_READ|FILE_SHARE_DELETE|FILE_SHARE_WRITE,  
	//	NULL,  
	//	OPEN_EXISTING,  
	//	FILE_FLAG_BACKUP_SEMANTICS,  
	//	NULL); 

	HANDLE hFile = CreateFile(strRootPath, 
		FILE_LIST_DIRECTORY, 
		FILE_SHARE_READ | FILE_SHARE_WRITE ,//| FILE_SHARE_DELETE, <-- removing FILE_SHARE_DELETE prevents the user or someone else from renaming or deleting the watched directory. This is a good thing to prevent.
		NULL, //security attributes
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS | //<- the required priviliges for this flag are: SE_BACKUP_NAME and SE_RESTORE_NAME.  CPrivilegeEnabler takes care of that.
		FILE_FLAG_OVERLAPPED, //OVERLAPPED!
		NULL);

	if ( INVALID_HANDLE_VALUE == hFile )  
	{  
		return 1;  
	} 

	pFileDirMonitor->m_FileHandle = hFile;
	pFileDirMonitor->m_FileLogCount = 1;
	CString bufDir = CSupport::GetBufDir();
	CFileDateLock  lock(pFileDirMonitor);
	CFileMonitorAdd    fileMonitor1(bufDir,&s_Client,&lock);
	CFileMonitorDelete fileMonitor2(bufDir,&s_Client,&lock);
	CFileMonitorModify fileMonitor3(bufDir,&s_Client,&lock);
	CFileMonitorRem    fileMonitor4(bufDir,&s_Client,&lock);
	CString fileName = "";
	CString newFileName = "";

	//printf("monitor... \r\n");  

	CString strInfo = "";
	CString str;

	ULONGLONG startId;
	//sInitFileDirID(startId,strRootPath);

	//int cnt = 0;

	pFileDirMonitor->m_LastTime = CTime::GetCurrentTime();

	CTime LastTime = CTime::GetCurrentTime();
	ULONGLONG startID = 0;
	int cnt  = 0;
	while (TRUE)  
	{  
		ZeroMemory(Buffer, 1024);  

		ULONGLONG batchID = pFileDirMonitor->GetStartID();

		CTime time = CTime::GetCurrentTime();
		ULONGLONG id = time.GetTime() - batchID;
		//id -= 1529000000;
		id *= 1000;
		if(time > LastTime){
			cnt = 0;
		}else{
			cnt ++;
		}

		id += cnt * 10;

		LastTime = time;
		//子线程一直在读取目录的改变  
		//调用系统ReadDirectoryChangesW API  
		bRet = ReadDirectoryChangesW(hFile,  
			&Buffer,  
			sizeof(Buffer),  
			TRUE,  
			dwNotifyFilter,
			&BytesReturned,  
			NULL, NULL);  

		if ( bRet == TRUE )  
		{  
			char szFileName[MAX_PATH] = { 0 };  

			CString strFileName = "";
			strInfo = "";
			// 宽字符转换多字节  
			WideCharToMultiByte(CP_ACP,  
				0,  
				pBuffer->FileName,  
				pBuffer->FileNameLength / 2,  
				szFileName,  
				MAX_PATH,  
				NULL,  
				NULL);  

			fileName.Format(_T("%s"), szFileName);



			switch(pBuffer->Action)   
			{   
				// 添加  
			case FILE_ACTION_ADDED:   
				{  
					TRACE(_T("添加 : %s\r\n"), szFileName); 
					THREAD_PARAM* pParam = new THREAD_PARAM(1,id ++,batchID,CBaseSupport::sGetFileBaseMonitor(),time,strRootPath,fileName,"",pFileDirMonitor);
					//id ++;
					HANDLE hThread = CreateThread(NULL, 0, FileMonitor1, pParam, 0, NULL);
					if(INVALID_HANDLE_VALUE == hThread){
						delete pParam;
					}
					break;  
				}  
				// 删除  
			case FILE_ACTION_REMOVED:   
				{  
					TRACE(_T("删除 : %s\r\n"), szFileName);

					THREAD_PARAM* pParam = new THREAD_PARAM(2,id++,batchID,CBaseSupport::sGetFileBaseMonitor(),time,strRootPath,fileName,"",pFileDirMonitor);
					HANDLE hThread = CreateThread(NULL, 0, FileMonitor2, pParam, 0, NULL);
					if(INVALID_HANDLE_VALUE == hThread){
						delete pParam;
					}
					break;   
				}
			case FILE_ACTION_MODIFIED:   
				{  
					TRACE(_T("2修改 : %s\r\n"), szFileName);

					THREAD_PARAM* pParam = new THREAD_PARAM(3,id ++,batchID,CBaseSupport::sGetFileBaseMonitor(),time,strRootPath,fileName,"",pFileDirMonitor);
					HANDLE hThread = CreateThread(NULL, 0, FileMonitor3, pParam, 0, NULL);
					if(INVALID_HANDLE_VALUE == hThread){
						delete pParam;
					}
					break;   
				}
				// 重命名  
			case FILE_ACTION_RENAMED_OLD_NAME:   
				{  
					TRACE(_T("重命名 : %s"), szFileName);  

					if ( pBuffer->NextEntryOffset != 0 )  
					{  
						FILE_NOTIFY_INFORMATION *tmpBuffer = (FILE_NOTIFY_INFORMATION *)((DWORD)pBuffer + pBuffer->NextEntryOffset);  
						switch ( tmpBuffer->Action )  
						{  
						case FILE_ACTION_RENAMED_NEW_NAME:  
							{          
								ZeroMemory(szFileName, MAX_PATH);  
								WideCharToMultiByte(CP_ACP,  
									0,  
									tmpBuffer->FileName,  
									tmpBuffer->FileNameLength / 2,  
									szFileName,  
									MAX_PATH,  
									NULL,  
									NULL);  

								TRACE(_T(" ->  : %s \r\n"), szFileName);  
								newFileName.Format(_T("%s"), szFileName);
								break;  
							}  
						} 


					}  


					THREAD_PARAM* pParam = new THREAD_PARAM(4,id++,batchID,CBaseSupport::sGetFileBaseMonitor(),time,strRootPath,fileName,newFileName,pFileDirMonitor);
					HANDLE hThread = CreateThread(NULL, 0, FileMonitor4, pParam, 0, NULL);
					if(INVALID_HANDLE_VALUE == hThread){
						delete pParam;
					}
					break;   
				}  
			case FILE_ACTION_RENAMED_NEW_NAME:   
				{  
					//printf("重命名(new) : %s\r\n", szFileName);  
				} 


			} 


		}  
	}  

	CloseHandle(hFile);  

	return 0;  
}











