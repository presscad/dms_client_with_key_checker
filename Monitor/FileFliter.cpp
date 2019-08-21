#include "StdAfx.h"
#include "FileFliter.h"
#include "fun.h"

CStringArray  CFileFliter::m_arrFliter;
Mutex CFileFliter::g_Lock2; 
CFileFliter::CFileFliter(void)
{
	//m_arrFliter.Add("d:\\test\\.*.tmp$");

	//m_arrFliter.Add("data\\(~.*)");
	//m_arrFliter.Add("data\\(~.*)");
	//m_arrFliter.Add("data\\.*.lock$");
	//m_arrFliter.Add("data\\.*.tmp$");
	//m_arrFliter.Add("data\\.*.\$\$\$$");

	////m_arrFliter.Add(".*.lock$");
	////m_arrFliter.Add(".*.tmp$");
	////m_arrFliter.Add(".*.\$\$\$$");
	//m_arrFliter.Add("D:\\test(.*)\\USB001");

}


CFileFliter::~CFileFliter(void)
{
	
}


BOOL CFileFliter::IsMatching(const CString& rootPath,const CString& strFileName){


	int cnt = (int)m_arrFliter.GetCount();
	CString strRegex = "";
	CString strExt = sGetExtFromFile(strFileName);
	//if(!strExt.CompareNoCase(".ini"))
	//	return TRUE;
	CString fileName = sGetFileFromPath(strFileName);
	if(!fileName.CompareNoCase("desktop.ini"))
		return TRUE;

	if(!fileName.CompareNoCase("$RECYCLE.BIN"))
		return TRUE;

	CString strRegExt("");
	CLock lock(CFileFliter::g_Lock2);
	for (int ii = 0 ; ii < cnt ; ii ++)
	{
		strRegex = m_arrFliter[ii];
		strRegex.Replace("\\\\","\\");
		int pos = strRegex.Find(".*.");
		int lastPos = strRegex.GetLength() - 1;
		int pos2 = strRegex.Find("(~.*)");

		if(strRegex.GetAt(lastPos) == '$'){
			if(pos > 0){
				if(strRegex.GetAt(1)== ':'){
					strRegExt = strRegex.Mid(pos + 2, lastPos - pos - 2);
					if(!strExt.CompareNoCase(strRegExt)){
						CString path = rootPath + "\\" + sGetDirFromPath(strFileName);
						CString strRegPath = strRegex.Left(pos - 1);
						if(!path.CompareNoCase(strRegPath))
							return TRUE;
					}
				}else{

					strRegExt = strRegex.Mid(pos + 2, lastPos - pos - 2);
					if(!strExt.CompareNoCase(strRegExt)){
						CString path = sGetDirFromPath(strFileName);
						CString strRegPath = strRegex.Left(pos -1);
						if(!path.CompareNoCase(strRegPath))
							return TRUE;
					}
				}
			}else if(pos == 0){
				strRegExt = strRegex.Mid(pos + 2, lastPos - pos - 2);
				if(!strExt.CompareNoCase(strRegExt))
					return TRUE;

			}
		}
		else if(pos2 == 0){
			if(fileName.GetAt(0)== '~')
				return TRUE;

		}else if(pos2 > 0){
			if(strRegex.GetAt(1)== ':'){
				strRegExt = strRegex.Mid(pos2 + 2, lastPos - pos2 - 2);
				if(!strExt.CompareNoCase(strRegExt)){
					CString path = rootPath + "\\" + sGetDirFromPath(strFileName);
					CString strRegPath = strRegex.Left(pos2);
					if(!path.CompareNoCase(strRegPath))
						return TRUE;
				}
			}else{

				strRegExt = strRegex.Mid(pos2 + 2, lastPos - pos2 - 2);
				if(!strExt.CompareNoCase(strRegExt)){
					CString path = sGetDirFromPath(strFileName);
					CString strRegPath = strRegex.Left(pos2 -1);
					if(!path.CompareNoCase(strRegPath))
						return TRUE;
				}
			}
		}else if(int pos3 = strRegex.Find("(.*)")){
			//sGetDirFromPath()
			CString path = strFileName;
			if(strRegex.GetAt(1)== ':'){
				path = rootPath + "\\" + strFileName;
			}
			CString strLeft = strRegex.Left(pos3);
			int len = strRegex.GetLength() - pos3 - 4;
			CString strRight = strRegex.Right(len);
			//if(len > path.GetLength()){
				CString strLeft1 = path.Left(pos3);
				CString strRight2 = path.Right(len);
				if(!strLeft.CompareNoCase(strLeft1) && !strRight.CompareNoCase(strRight2))
					return TRUE;
		//	}

		}else if(strRegex.Find("(") && strRegex.Find(")")){
			CString path = rootPath + "\\" + strFileName;
			CString strRegxPath = strRegex;
			strRegxPath.Replace("(","(");
			strRegxPath.Replace(")",")");

			if(!strRegxPath.CompareNoCase(path))
				return TRUE;
		}else if(strRegex.GetAt(1) == ':'){
			CString path = rootPath + "\\" + strFileName;
			if(!strRegex.CompareNoCase(path))
				return TRUE;
		}else if(!strRegex.CompareNoCase(strFileName))
			return TRUE;
	


	}

	
	return FALSE;
}