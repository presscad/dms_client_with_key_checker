#include "StdAfx.h"
#include "StatFileLogs.h"
#include "FileDirMonitor.h"
#include "FileTransferClient.h"
#include "..\SuperDb\UniverDB.h"
#include "FileMonitor.h"
#include "Monitor.h"
#include "support.h"

#include <Shlwapi.h>

BOOL CStatFileLogs::s_isAutoCopyDB(FALSE);
CListBox* CStatFileLogs::m_pListInfo(NULL);
CStatFileLogs::CStatFileLogs(void)
{
}

CStatFileLogs::~CStatFileLogs(void)
{
}

COleDateTime CStatFileLogs::StatFileLogs(CFileDirMonitor* pMonitor,CUniverDB& SuperDB,const CString& TableName,SOCKET Client){
	//CString strCnnParam = "Access;;" +strAccessPath;

	CString key = pMonitor->m_strDir;
	CString strWhereSql;
	strWhereSql.Format(" file_dir = \'%s\' and BATCH_NUMBER <> %I64d ",key,pMonitor->GetStartID());

	CString strSql;
	vector<ULONGLONG> vecBatchNumber;
	strSql.Format("SELECT distinct(BATCH_NUMBER) FROM %s  WHERE %s",TableName,strWhereSql);

	if (!SuperDB.OpenSQL(strSql))
	{
		goto over;
	}


	while (!SuperDB.IsEOF())
	{
		LONGLONG bn = SuperDB.GetLong("BATCH_NUMBER");
		if (bn != 0)
		{
			vecBatchNumber.push_back(bn);
		}

		SuperDB.MoveNext();
	}

	COleDateTime lastTime(1970,1,1,0,0,0); 

	if (vecBatchNumber.size() > 0)
	{

		CString strSqlMaxID;
		strSqlMaxID.Format("SELECT MAX(ID) AS IDS,FILE_PATH2 AS FILE_PATH3 ,BATCH_NUMBER AS BATCH_NUMBER2 FROM %s WHERE HANDLE = 3  AND %s",
			TableName,strWhereSql);
		strSqlMaxID += " GROUP BY FILE_PATH2,BATCH_NUMBER";

		TRACE("%s\n",strSqlMaxID);

		CString strSqlLeft;
		strSqlLeft.Format("SELECT * FROM (SELECT * FROM (SELECT * FROM %s WHERE %s AND BATCH_NUMBER <> 0) AS A LEFT JOIN (",
			TableName,strWhereSql);

		strSqlLeft += strSqlMaxID ;
		strSqlLeft += ") AS B ON A.ID=B.IDS AND A.BATCH_NUMBER = B.BATCH_NUMBER2 AND A.FILE_PATH2 = B.FILE_PATH3 ";
		strSqlLeft += "ORDER BY ID) WHERE  (HANDLE = 3 AND IDS >0 ) OR HANDLE <> 3  ";

		CString strFilePath = pMonitor->m_strFilePath;
		if (!strFilePath.CompareNoCase("FILE_PATH2"))
		{
			//strFilePath += "2";
			strSqlLeft += "ORDER BY BATCH_NUMBER ,FILE_LOG_DATE,FILE_PATH2,ID";//,IS_CREATE,
		}else{
			strSqlLeft += "ORDER BY BATCH_NUMBER ,FILE_LOG_DATE,FILE_PATH,ID";//,,IS_CREATE,
		}

		TRACE("%s\n",strSqlLeft);
		
		CStatFileLogs::SendFileLog(pMonitor ,SuperDB,strSqlLeft,strWhereSql,TableName,Client,strFilePath,lastTime);


	}else{
		lastTime = COleDateTime::GetCurrentTime();
	}

over:
	if(SuperDB.IsConnect()){
		SuperDB.Close();
		SuperDB.Disconnect();

	}

	return lastTime;
}


CString CStatFileLogs::GetFileLogInfo(CString name,CString describe1,CString formatName,ULONGLONG fileSize,CString handler,CString loginName,CString IP,CTime& ctime,CTime& time){
	CString strSize;
	double mSize = 1024.0 * 1024.0;
	if(fileSize >= mSize){
		strSize.Format("%.4lfM",(double)fileSize / mSize);
	}else if(fileSize >= 1024){
		strSize.Format("%.4lfK",(double)fileSize / 1024.0);
	}else{
		strSize.Format("%I64dByte",fileSize);
	}

	CString format = "{\"describe1\":\"%s\",\"fileName\":\"%s\",\"filePath\":\"%s\",\"fileSize\":\"%s\",\"handler\":\"%s\",\"ip\":\"%s\",\"updateDate\":%s,\"createDate\":%s,\"handle\":\"%s\"}";
	CString strJosn;

	formatName.Replace("\\","\\\\");
	CString strName = name;
	strName.Replace("\\","\\\\");

	CString strTime,strCTime;
	strTime.Format("%I64d000",time.GetTime());
	strCTime.Format("%I64d000",ctime.GetTime());
	strJosn.Format(format,describe1,formatName,strName,strSize,loginName ,IP,strTime,strCTime,handler);
	return strJosn;
}

DWORD CStatFileLogs::SendFileLog(CFileDirMonitor* pMonitor ,CUniverDB& SuperDB,CString& strSubTable,
								 const CString& strWhereSql,const CString& TableName,SOCKET Client,CString& strFilePath,COleDateTime& lastTime)
{	

	CString key = pMonitor->m_strDir;

		CString strSql = pMonitor->m_strSql;

		if(m_pListInfo){
			m_pListInfo->AddString("ZIP log:" + strSql);
		}

		SuperDB.Update();

		CTime time = CTime::GetCurrentTime();

		COleDateTime minTime = COleDateTime::GetCurrentTime();

		if(!SuperDB.OpenSQL(strSubTable)){
			if(s_pListInfo){
				s_pListInfo->AddString(pMonitor->m_strSql);
				s_pListInfo->AddString(SuperDB.m_strLastErr);
			}
			return -1;
		}

		TRACE("\n");
		TRACE(strSubTable);
		TRACE("\n");

		int cnt = SuperDB.GetRows();

		if(cnt <= 0)
			return -1;
		SuperDB.MoveFirst();

		CString lastPath = "";
		LONG nHandle;
		CString strHandle = "",loginName;
		CString lastFileLog,strFileDir,describe1,fileName;
		ULONGLONG fileSize;
		COleDateTime ctime, utime ,createTime;
		CString strHandles[] = {"","创建","删除","修改","重命名"};

		FILE_SETS* fileSets = new FILE_SETS();
		std::set<CString>&  setFilePath = fileSets->mSetFilePath;
		std::set<CString>&  setFileName = fileSets->mSetFileName;
		ULONGLONG_ARRAY arrId;
		ULONGLONG id = 0;
		//std::map<CTime,CString> mapDafFileName;

		vector<int> vecType;
		std::map<CString,vector<int>> map; 
		std::map<CString,vector<int>> map21; 
		CString keyFileTime,keyFileTime2;
		CString strExt = "";

		CString strPath("");
		//mapDafFileName.clear();
		//CString strUUID;
		if((CFileMonitor::s_nFlag & 0x03) == 0x0){
			CTime time = CTime::GetCurrentTime();
			strPath = time.Format("%y%m%d%H");//格式化日期时间 
			//strUUID = strPath + "\\" + GUID_Generator();
			CString strPathDir = CSupport::GetBufDir() + "\\" +strPath;
			if(!PathFileExists(strPathDir))//文件夹不存在则创建
				CreateDirectory(strPathDir, NULL);
		}

		CStringArray arrSrcFile;
		CStringArray arrDesFile;
		vector<int>  vecPosFile;


		CString bufDir = CSupport::GetBufDir();
		CString strNewFileName("");

		std::map<CString,CString> mapBufFile; 
		std::map<CString,ULONGLONG> mapBufFileSize; 
		//std::map<CString,int> mapCreateFile; 
		CStringArray arrSendFileLogs;
		arrSendFileLogs.RemoveAll();

		std::map<CString,int> mapModified;

		const int typeNo(11);
		while(!SuperDB.IsEOF()){
			id = SuperDB.GetLong("id");
			if(id == 0){
				SuperDB.MoveNext();
				continue;
			}

			lastPath   = SuperDB.GetString(strFilePath);//"FILE_PATH"
			strFileDir = SuperDB.GetString("FILE_DIR");
			if(strFileDir.CompareNoCase(key)){
				SuperDB.MoveNext();
				continue;
			}
			nHandle  = SuperDB.GetLong("HANDLE");
			loginName = SuperDB.GetString("HANDLER");
			describe1 =  SuperDB.GetString("DESCRIBE1");
			fileName = SuperDB.GetString("FILE_NAME");
			fileSize = SuperDB.GetLong("FILE_SIZE");
			ctime = SuperDB.GetDateTime("FILE_LOG_DATE");
			utime = SuperDB.GetDateTime("UPDATE_DATE");
			createTime = SuperDB.GetDateTime("CREATE_DATE");

			if(ctime > lastTime){
				lastTime = ctime;
			}


			CTime cctime;
			CTime uutime;
			CTime crtime;

			if(ctime.m_dt == 0.0){
				cctime = CTime::GetCurrentTime();
			}else{
				cctime = CTime(ctime.GetYear(),ctime.GetMonth(),ctime.GetDay(),ctime.GetHour(),ctime.GetMinute(),ctime.GetSecond());
			}

			if (utime.m_dt == 0.0)
			{ 
				uutime = CTime::GetCurrentTime();
			}else{
				uutime = CTime(utime.GetYear(),utime.GetMonth(),utime.GetDay(),utime.GetHour(),utime.GetMinute(),utime.GetSecond());
			}

			if (createTime.m_dt == 0.0)
			{
				crtime = CTime::GetCurrentTime();
			}else{
				crtime = CTime(createTime.GetYear(),createTime.GetMonth(),createTime.GetDay(),createTime.GetHour(),createTime.GetMinute(),createTime.GetSecond());
			}

			if(nHandle != 3 && !fileName.IsEmpty()){
				CFileStatus rStatus;
				CString strFileName = bufDir + "\\" + fileName;
				if(CFile::GetStatus(strFileName,rStatus)){
					fileSize = rStatus.m_size;
				}
			}


			if(CFileMonitor::s_nFlag == 0){
				if(mapBufFile.find(lastPath) == mapBufFile.end()){

					if (fileName.IsEmpty())
					{
						fileName = strPath + "\\" + GUID_Generator();
					}

					CFileStatus rStatus;
					if(CFile::GetStatus(lastPath,rStatus)){
						fileSize = rStatus.m_size;
					}
					mapBufFile[lastPath] = fileName;
					mapBufFileSize[lastPath] = fileSize;

					strNewFileName = bufDir + "\\" + fileName;
					if(::CopyFile(lastPath,strNewFileName,FALSE)){

						CString fileTmp =  CSupport::GetEncryptPath(lastPath);
						if(fileTmp.IsEmpty())
							continue ;

						fileTmp += CFileMonitor::GetFileExitName(lastPath) + ".tmp";

						CFileMonitor::WriteTmpFile(lastPath,fileTmp,fileName,0,cctime);
					}
				}else{
					fileName = mapBufFile[lastPath];
					fileSize = mapBufFileSize[lastPath];
				}
			}

			lastFileLog = GetFileLogInfo(lastPath,describe1,fileName,fileSize,strHandles[nHandle],loginName,"",cctime,uutime);



			/////////////////////////
			keyFileTime.Format("%s%ld",lastPath,cctime.GetTime());
			keyFileTime2.Format("%s%ld",lastPath,cctime.GetTime() - 1);
			int pos;
			if (nHandle == 3)
			{
				pos = arrSendFileLogs.Add(lastFileLog);//-
				mapModified[keyFileTime] = pos;
			}
			else if (nHandle == 1 && mapModified.find(keyFileTime) != mapModified.end())
			{
				//if (s_pListInfo)
				//{
				//	CString str;
				//	str.Format("Create:%s",lastPath);
				//	s_pListInfo->AddString(str );
				//}

				int posPrev = mapModified[keyFileTime];
				CString strPrev = arrSendFileLogs[posPrev];
				arrSendFileLogs.SetAt(posPrev,lastFileLog);
				lastFileLog = strPrev;
				mapModified.erase(keyFileTime);

				pos = arrSendFileLogs.Add(lastFileLog);//-
			}else{
				pos = arrSendFileLogs.Add(lastFileLog);//-
			}

			vecType.push_back(nHandle);//-


			if(nHandle == 1){//同一秒和前一秒内创建删除的无效
				//keyFileTime.Format("%s%ld",lastPath,cctime.GetTime());
				if(map.find(keyFileTime) == map.end()){
					map[keyFileTime] = vector<int>();
					map[keyFileTime].push_back(pos);
				}else{
					map[keyFileTime].clear();
					map[keyFileTime].push_back(pos);
				}

				if(map21.find(keyFileTime) != map21.end()){
					vector<int>& vecPos = map21[keyFileTime];
					for (int ii = 0 ; ii < vecPos.size(); ii ++)
					{
						vecType[vecPos[ii]] = typeNo;
					}
					map21.erase(keyFileTime);
				}


				if(map21.find(keyFileTime2) != map21.end()){
					vector<int>& vecPos = map21[keyFileTime2];
					for (int ii = 0 ; ii < vecPos.size(); ii ++)
					{
						vecType[vecPos[ii]] = typeNo;
					}
					map21.erase(keyFileTime2);
				}

			}else if(nHandle == 2){
				if(map.find(keyFileTime) != map.end()){
					vector<int>& vecPos = map[keyFileTime];

					for (int ii = 0 ; ii < vecPos.size(); ii ++)
					{
						vecType[vecPos[ii]] = typeNo;
					}

					vecType[pos] = typeNo;

					map.erase(keyFileTime);
					//if (mapCreateFile.find(lastPath) != mapCreateFile.end())//
					//{
					//	mapCreateFile.erase(lastPath);
					//}
				}else if(crtime == uutime && crtime == cctime){
					map21[keyFileTime] = vector<int>();
					map21[keyFileTime].push_back(pos);
				}

			}else{
				if(map.find(keyFileTime) != map.end()){
					map[keyFileTime].push_back(pos);
				}
			}

			setFilePath.insert(lastPath);
			setFileName.insert(fileName);
			arrId.Add(id);//-
			arrDesFile.Add(fileName);//-

			//SuperDB.Delete();
			SuperDB.MoveNext();

		}

		//SuperDB.Update(FALSE);
		mapModified.clear();
		CString strLogFilePathDir = CSupport::GetBufDir() + "\\LogFile";
		if(!PathFileExists(strLogFilePathDir))//文件夹不存在则创建
			CreateDirectory(strLogFilePathDir, NULL);

		strPath = time.Format("%y%m%d%H");//格式化日期时间 	
		strLogFilePathDir = CSupport::GetBufDir() + "\\LogFile\\" +strPath;
		if(!PathFileExists(strLogFilePathDir))//文件夹不存在则创建
			CreateDirectory(strLogFilePathDir, NULL);

		fileName = time.Format("\\%Y_%m_%d_%H_%M_%S.txt");//格式化日期时间 
		fileSets->strLogFile =strLogFilePathDir + fileName;

		strPath += fileName;

		BOOL IsWeb = TRUE;

		if(!CSupport::GetServerIP().CompareNoCase("127.0.0.1")){
			strPath.Empty();
			IsWeb = FALSE;
		}

		if (CFileTransferClient::s_version <= 1)
		{
			strPath.Empty();
		}

		CString strBufdir = CSupport::GetBufDir();
		if(!PathFileExists(strBufdir))//文件夹不存在则创建
			CreateDirectory(strBufdir, NULL);

		//
		if(!SendFileLog(vecType,typeNo,fileSets->strLogFile,Client,arrSendFileLogs,strPath)){
			return -1;
		}


		if(IsWeb){
			HANDLE hThread = CreateThread(NULL, 0, ThreadProcSendOnceFile, fileSets, 0, NULL);
			if(INVALID_HANDLE_VALUE == hThread){
				delete fileSets;
				return -1;
			}
		}else{

			if (CFileTransferClient::s_version >= 1)
			{
				FILE_LOG sendInfo(10,fileSets->strLogFile);
				int result = send(Client,(LPSTR)&sendInfo,fileSets->strLogFile.GetLength() + 2,0);
				if (m_pListInfo)
				{
					m_pListInfo->AddString(fileSets->strLogFile);
				}
			}
			delete fileSets;
		}


		//mapCreateFile.clear();
		std::set<CString> setUpLoad;

		if (CFileMonitor::s_nFlag & 0x01)
		{
			cnt = arrDesFile.GetCount();


			for (int ii = 0 ; ii < cnt ;ii ++)
			{
				if(vecType[ii] == 3)
					setUpLoad.insert(arrDesFile[ii]);
				else if (vecType[ii] == typeNo 
					&& !arrDesFile[ii].IsEmpty())
				{
					DeleteFile(bufDir + "\\" + arrDesFile[ii]);
					DeleteFile(bufDir + "\\" + arrDesFile[ii] + ".tmp");
					TRACE("delete : %s\n",arrDesFile[ii]);
				}

			}

			DeleteOthersFileLog(SuperDB,key,setUpLoad,strWhereSql,TableName);
		}else if (CFileMonitor::s_nFlag & 0x04)
		{
			cnt = arrDesFile.GetCount();


			for (int ii = 0 ; ii < cnt ;ii ++)
			{
				if(vecType[ii] != typeNo)
					setUpLoad.insert(arrDesFile[ii]);

			}

			DeleteOthersFileLog(SuperDB,key,setUpLoad,strWhereSql,TableName);
		}

		setUpLoad.clear();
		arrDesFile.RemoveAll();
		arrId.RemoveAll();
		vecType.clear();


		strSql.Format("DELETE FROM %s ",TableName);//where (%s) or id = 0,strWhereSql
		SuperDB.Execute(strSql,FALSE);
		SuperDB.Update(FALSE);

		//pMonitor->SetStartID( CTime::GetCurrentTime().GetTime() - 10);


	return 0;
}


BOOL CStatFileLogs::DeleteOthersFileLog(CUniverDB& SuperDB,CString key,std::set<CString> setUpload ,const CString& strSubSql,const CString& TableName){


	CString strSql;
	strSql.Format("DELETE FROM %s WHERE HANDLE <> 3 AND %s",TableName,strSubSql);

	if (!SuperDB.Execute(strSql,FALSE))
	{

		return FALSE;
	}else{
		SuperDB.Update();
	}


	CString strSqlDel = "";
	strSqlDel.Format("DELETE FROM %s where %s and file_name in (",TableName,strSubSql);

	CString strSub = "";
	int total = 0;

	set<CString>::iterator it; //定义前向迭代器
	for(it = setUpload.begin(); it != setUpload.end(); it++)
	{

		CString strFileName = *it;
		strSub.Format("\'%s\',",strFileName);
		strSqlDel += strSub;
		total ++;
		if(total == 200){
			int last = strSqlDel.GetLength() - 1;
			strSqlDel.SetAt(last,')');
			if(!SuperDB.Execute(strSqlDel,FALSE)){
				//SuperDB.CancelUpdate();
				return FALSE;
			}
			strSqlDel.Format("DELETE FROM %s where %s and file_name in (",TableName,strSubSql);
			SuperDB.Update(FALSE);
			total = 0;
		}

	}

	if(total > 0){
		int last = strSqlDel.GetLength() - 1;
		strSqlDel.SetAt(last,')');
		if(!SuperDB.Execute(strSqlDel,FALSE)){
			//SuperDB.CancelUpdate();
			return FALSE;
		}
		SuperDB.Update(FALSE);
	}

	CString strBufDir = CSupport::GetBufDir() + "\\";

	CString strFileName("");
	CString strFilePath("");

	strSql.Format("SELECT FILE_NAME,FILE_PATH FROM %s WHERE %s",TableName,strSubSql);

	if (!SuperDB.OpenSQL(strSql))
	{
		return FALSE;
	}

	while (!SuperDB.IsEOF())
	{
		strFileName = SuperDB.GetString("FILE_NAME");
		strFilePath = SuperDB.GetString("FILE_PATH");

		DeleteFile(strBufDir + strFileName);
		DeleteFile(strBufDir + strFileName + ".tmp");
		TRACE("delete : %s\n",strFileName);
		SuperDB.MoveNext();
	}

	return TRUE;
}



BOOL CStatFileLogs::SendFileLog(vector<int>& vecType,const int& typeNo,CString& logFileName,SOCKET Client, CStringArray& arrSendFileLogs,CString& logFile){
	int cnt = (int)arrSendFileLogs.GetCount();
	CString strFileLog("");
	/////////////

	if (CFileTransferClient::s_version >= 1)
	{
		CStdioFile file;
		if(file.Open(logFileName,CFile::typeText|CFile::modeWrite|CFile::modeNoTruncate| CFile::modeCreate | CFile::shareDenyWrite))
		{
			if (!logFile.IsEmpty())
			{
				//CString strTime;
				//strTime.Format("%ld000\n",time.GetTime());
				file.WriteString(logFile  + "\n");
			}
			for(int ii = 0 ; ii < cnt;  ii ++){
				if(vecType[ii] == typeNo)
					continue;
				strFileLog = arrSendFileLogs[ii] + "\n";
				file.WriteString(strFileLog);
			}
			file.Close();
			return TRUE;
		}else
			return FALSE;
	}


	CString str("");
	//return TRUE;
	for(int ii = 0 ; ii < cnt;  ii ++){
		strFileLog = arrSendFileLogs[ii];
		if(vecType[ii] == typeNo)
			continue;
		TRACE("%d\t%s\n",ii,strFileLog);

		//if(s_pListInfo){
		//	s_pListInfo->AddString(strFileLog);
		//}
		if(str.GetLength() + strFileLog.GetLength() > 1020)
		{
			if(!str.IsEmpty()){
				str += "]";
				FILE_LOG sendInfo(3,str);
				//if(str.GetLength() > 1020)
				//	TRACE("%d\t%d\t%s\n",str.GetLength(),strFileLog.GetLength(),str);
				int result = send(Client,(LPSTR)&sendInfo,str.GetLength() + 2,0);
				if(result<=0){
					return FALSE;
				}
				str.Empty();
				Sleep(500);
			}

			str =  "[" + strFileLog;

		}else if(str.IsEmpty()){
			str += "[" + strFileLog;

		}else{
			str += "," + strFileLog;
		}
	}

	if(!str.IsEmpty()){
		str += "]";
		FILE_LOG sendInfo(3,str);
		//TRACE("%s\n",str);
		int result = send(Client,(LPSTR)&sendInfo,str.GetLength() + 1,0);

		if(result<=0){
			return FALSE;
		}
	}

	arrSendFileLogs.RemoveAll();
	return TRUE;
}


