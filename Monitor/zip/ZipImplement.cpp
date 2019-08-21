///////////////////////////////////////////////////////////////////////////// 
// 文件名: <ZipImplement.cpp> 
// 说明:压缩解压缩文件夹 
///////////////////////////////////////////////////////////////////////////// 
#include "StdAfx.h" 
#include "zipimplement.h" 
#include <direct.h> 
#include "..\Support.h"
#include <vector> 
#include <xstring> 
//#include "..\..\BaseDb\fun.h"

CZipImplement::CZipImplement(void):m_prefixName(""),m_pVecZipFile(NULL),m_Crypt(12345678)
{ 
	m_chType = 0;
} 

CZipImplement::CZipImplement(CString prefixName,char type):m_prefixName(prefixName),m_pVecZipFile(NULL),m_Crypt(12345678)
{
	m_chType = type;
}

CZipImplement::~CZipImplement(void) 
{ 
} 

///////////////////////////////////////////////////////////////////////////// 
// 函数说明: 实现压缩文件夹操作 
// 参数说明: [in]： 
//					pFilePath			要被压缩的文件夹 
//					mZipFileFullPath	压缩后的文件名和路径 
//					strPW				压缩密码，可空，默认为空，表示不带密码压缩
// 返回值: 参数有误的情况下返回FALSE,压缩成功后返回TRUE 
///////////////////////////////////////////////////////////////////////////// 
BOOL CZipImplement::Zip_PackFiles(CString& pFilePath, CString& mZipFileFullPath, CStringA strPW) 
{ 
    //参数错误 
    if ((pFilePath == L"") || (mZipFileFullPath == L"")) 
    { 
        //路径异常返回 
        return FALSE ; 
    } 

    if(!CZipImplement::FolderExist(pFilePath)) 
    { 
        //要被压缩的文件夹不存在 
        return FALSE ; 
    } 

    CString tZipFilePath = mZipFileFullPath.Left(mZipFileFullPath.ReverseFind('\\') + 1); 
    if(!CZipImplement::FolderExist(tZipFilePath)) 
   { 
        //ZIP文件存放的文件夹不存在创建它 
		CStringW strWZipFilePath(tZipFilePath);
		if (FALSE == CreatedMultipleDirectory((LPWSTR)(LPCWSTR)strWZipFilePath)) 
		{ 
			//创建目录失败 
			return FALSE; 
		} 
    } 

    //获得文件夹的名字 
    if(pFilePath.Right(1) == L"\\") 
    { 
        this->m_FolderPath = pFilePath.Left(pFilePath.GetLength() - 1); 
        m_FolderName = m_FolderPath.Right(m_FolderPath.GetLength() - m_FolderPath.ReverseFind('\\') - 1); 
    } 
    else 
    { 
        this->m_FolderPath = pFilePath; 
       m_FolderName = pFilePath.Right(pFilePath.GetLength() - pFilePath.ReverseFind('\\') - 1); 
    } 

    /************************************************************************/ 

    //创建ZIP文件
	if (strPW.IsEmpty())
	{
		hz=CreateZip(mZipFileFullPath,0); 
	}
	else
	{
		hz=CreateZip(mZipFileFullPath,(LPCSTR)strPW); 
	}    
    if(hz == 0) 
    { 
        //创建Zip文件失败 
        return FALSE; 
    } 

	int cnt(0);

	if(m_chType == 0){
		cnt = BrowseFile6(pFilePath);

	}else if(m_chType == 1 || m_chType == 2 || m_chType == 3){
		cnt = BrowseFile4(pFilePath);
	}else{
    //递归文件夹,将获取的问价加入ZIP文件 
		cnt = BrowseFile(pFilePath); 
	}
    //关闭ZIP文件 
    CloseZip(hz); 

    /************************************************************************/ 

    CFileFind tFFind; 
    if (!tFFind.FindFile(mZipFileFullPath)) 
    { 
        //压缩失败(未发现压缩后的文件) 
        return FALSE; 
    } 

	if(cnt == 0){
		DeleteFile(mZipFileFullPath);
		return FALSE;
	}else
		return TRUE;  
} 

///////////////////////////////////////////////////////////////////////////// 
// 函数说明: 解压缩文件夹 
// 参数说明: [in]：
//					mUnPackPath			解压后文件存放的路径 
//					mZipFileFullPath	ZIP文件的路径 
//					strPW				解压密码，可空，默认为空，表示压缩包没有密码
///////////////////////////////////////////////////////////////////////////// 
BOOL CZipImplement::Zip_UnPackFiles(CString &mZipFileFullPath, CString& mUnPackPath, CStringA strPW) 
{ 
    //参数错误 
    if ((mUnPackPath.IsEmpty()) || (mZipFileFullPath.IsEmpty())) 
    { 
        //路径异常返回 
        return FALSE ; 
    } 

    CFileFind tFFind; 
    if (!tFFind.FindFile(mZipFileFullPath)) 
    { 
        //压缩失败(未发现压缩文件) 
        return FALSE; 
    } 

    //如果解压缩的路径不存在 试图创建它 
    CString tZipFilePath = mUnPackPath; 
    if(!CZipImplement::FolderExist(tZipFilePath)) 
    { 
        //解压后存放的文件夹不存在 创建它         
		CStringW strWZipFilePath(tZipFilePath);
        if (FALSE == CreatedMultipleDirectory((LPWSTR)(LPCWSTR)strWZipFilePath)) 
        { 
            //创建目录失败 
            return FALSE; 
        } 
    } 
    /************************************************************************/ 
    //打开ZIP文件
	if (strPW.IsEmpty())
	{
		hz=OpenZip(mZipFileFullPath,0);
	}
	else
	{
		hz=OpenZip(mZipFileFullPath,(LPCSTR)strPW);
	}     
    if(hz == 0) 
    { 
        //打开Zip文件失败 
        return FALSE; 
    } 

    zr=SetUnzipBaseDir(hz,mUnPackPath); 
    if(zr != ZR_OK) 
    { 
        //打开Zip文件失败 
        CloseZip(hz); 
        return FALSE;       
    } 

    zr=GetZipItem(hz,-1,&ze); 
    if(zr != ZR_OK) 
    { 
        //获取Zip文件内容失败 
        CloseZip(hz); 
        return FALSE;       
    } 

    int numitems=ze.index; 
    for (int i=0; i<numitems; i++) 
    { 
        zr=GetZipItem(hz,i,&ze); 
        zr=UnzipItem(hz,i,ze.name); 
		TRACE("%s\n",ze.name);
        if(zr != ZR_OK) 
        { 
            //获取Zip文件内容失败 
            CloseZip(hz); 
            return FALSE;       
        } 
    } 

    CloseZip(hz); 
    return TRUE; 
} 

//////////////////////////////////////////////////////////////////////////// 
// 函数说明: 检查指定的文件夹是否存在 
// 参数说明: [in]：strPath 检查的文件夹 (此方法会主动向路径末尾添加*.*) 
// 返回值:BOOL类型,存在返回TRUE,否则为FALSE 
///////////////////////////////////////////////////////////////////////////// 
BOOL CZipImplement::FolderExist(CString& strPath) 
{ 
    CString sCheckPath = strPath; 

    if(sCheckPath.Right(1) != L"\\") 
        sCheckPath += L"\\"; 

    sCheckPath += L"*.*"; 

    WIN32_FIND_DATA wfd; 
    BOOL rValue = FALSE; 

    HANDLE hFind = FindFirstFile(sCheckPath, &wfd); 

    if ((hFind!=INVALID_HANDLE_VALUE) && 
        (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) || (wfd.dwFileAttributes&FILE_ATTRIBUTE_ARCHIVE)) 
    { 
        //如果存在并类型是文件夹 
        rValue = TRUE; 
    } 

    FindClose(hFind); 
    return rValue; 
} 

///////////////////////////////////////////////////////////////////////////// 
// 函数说明: 遍历文件夹 
// 参数说明: [in]：strFile 遍历的文件夹(此方法会主动向路径末尾添加*.*) 
// 返回值:BOOL类型,存在返回TRUE,否则为FALSE 
///////////////////////////////////////////////////////////////////////////// 
int CZipImplement::BrowseFile(CString &strFile) 
{ 
    CFileFind ff; 
    CString szDir = strFile; 

    if(szDir.Right(1) != L"\\") 
        szDir += L"\\"; 

    szDir += L"*.*"; 
	int cnt = 0;
    BOOL res = ff.FindFile(szDir); 
    while(res) 
    { 
        res = ff.FindNextFile(); 
        if(ff.IsDirectory() && !ff.IsDots()) 
        { 
            //如果是一个子目录，用递归继续往深一层找 
            CString strPath = ff.GetFilePath(); 

            CString subPath; 
            GetRelativePath(strPath,subPath); 
			subPath = m_prefixName + "\\" + subPath;
            //将文件添加到ZIP文件 
            ZipAddFolder(hz,subPath); 
            BrowseFile(strPath); 
			cnt ++;
        } 
        else if(!ff.IsDirectory() && !ff.IsDots()) 
        { 
            //显示当前访问的文件(完整路径) 
            CString strPath = ff.GetFilePath(); 

			if(strPath.Find(".tmp") > 1)
				continue;

			CString strKey = strPath.MakeUpper();

			//if(m_pMapZipFile != NULL){// && (*m_pMapZipFile)[strKey] != 1)
			//	std::map<CString,int>::iterator it = m_pMapZipFile->find(strKey);
			//	if(it== m_pMapZipFile->end()){
			//		continue;
			//	}
			//}
            CString subPath; 

            GetRelativePath(strPath,subPath); 
			subPath = m_prefixName + "\\" + subPath;
            //将文件添加到ZIP文件 
            ZipAdd(hz,subPath,strPath);
			cnt ++;
        } 
    } 

    //关闭 
    ff.Close(); 

	return cnt;
} 


///////////////////////////////////////////////////////////////////////////// 
// 函数说明: 遍历文件夹 
// 参数说明: [in]：strFile 遍历的文件夹(此方法会主动向路径末尾添加*.*) 
// 返回值:BOOL类型,存在返回TRUE,否则为FALSE 
///////////////////////////////////////////////////////////////////////////// 
int CZipImplement::BrowseFile6(CString &strFile) 
{ 
	std::vector<CString> ::iterator iter;
	CString subPath; 
	CString strAddPath = "";
	int cnt = 0;


	for(iter = m_pVecZipFile->begin(); iter != m_pVecZipFile->end(); iter ++){
		CString key = *iter;

		CString subPath = key;
		if(key.GetAt(1) != ':')
			key.Insert(1,":");
		else{
			subPath.Replace(":","");
		}
		if(IsFileExist(key))
		{
			TRACE("%s\n",key);
			cnt ++;
			ZipAdd(hz,m_prefixName + "\\" + subPath,key);
		}
	}

	return cnt ;
} 

CString CZipImplement::ReadTmpFile(CString& strFileName)
{
	CStdioFile file;
	BOOL isResult = file.Open(strFileName,CFile::modeRead);
	if(!isResult)
		return "";
	CString strText;
	file.ReadString( strText );
	file.Close();
	return strText;
}

int CZipImplement::BrowseFile4(CString &strFile) 
{ 
	std::vector<CString> ::iterator iter = m_pVecZipFile->begin();
	CString subPath; 
	CString strAddPath = "";
	CString strSrcFile = "";
	int cnt = 0;
	CString key;
	if (m_chType == 2 || m_chType == 3)
	{
		key = *iter;
		//CString strLogFile = CSupport::GetBufDir() + "\\LogFile.txt";
		if(IsFileExist(key))
			ZipAdd(hz,m_prefixName + "\\LogFile.txt" ,key);
		iter ++;
		cnt ++;
	}

	for(iter; iter != m_pVecZipFile->end(); iter ++){
		key = *iter;
		strSrcFile = strFile  + key;
		TRACE("%s\n",key);
		if(IsFileExist(strSrcFile)){

			ZipAdd(hz,m_prefixName + "\\" + key,strSrcFile);
			cnt ++;
		}else{
			if(IsFileExist(strSrcFile + ".tmp")){
				CString strOrgPath = ReadTmpFile(strSrcFile);
				if(::CopyFile(strOrgPath,strSrcFile,FALSE)){
					ZipAdd(hz,m_prefixName + "\\" + key,strSrcFile);
					cnt ++;
				}
			}

		}

	}

	return cnt;

}

///////////////////////////////////////////////////////////////////////////// 
// 函数说明: 获取相对路径 
// 参数说明: [in]：pFullPath 当前文件的完整路径 [out] : 解析后的相对路径 
///////////////////////////////////////////////////////////////////////////// 
void CZipImplement::GetRelativePath(CString& pFullPath,CString& pSubString) 
{ //+ this->m_FolderName.GetLength()
    pSubString = pFullPath.Right(pFullPath.GetLength() - this->m_FolderPath.GetLength() -1 ); 
} 

///////////////////////////////////////////////////////////////////////////// 
// 函数说明: 创建多级目录 
// 参数说明: [in]： 路径字符串 
// 返回值: BOOL 成功True 失败False 
///////////////////////////////////////////////////////////////////////////// 
BOOL CZipImplement::CreatedMultipleDirectory(wchar_t* direct) 
{ 
    std::wstring Directoryname = direct; 

    if (Directoryname[Directoryname.length() - 1] !=  '\\') 
    { 
        Directoryname.append(1, '\\'); 
    } 
    std::vector< std::wstring> vpath; 
    std::wstring strtemp; 
    BOOL  bSuccess = FALSE; 
    for (int i = 0; i < Directoryname.length(); i++) 
    { 
        if ( Directoryname[i] != '\\') 
        { 
            strtemp.append(1,Directoryname[i]);    
        } 
        else 
        { 
            vpath.push_back(strtemp); 
            strtemp.append(1, '\\'); 
        } 
    } 
    std::vector<std::wstring>:: const_iterator vIter; 
    for (vIter = vpath.begin();vIter != vpath.end(); vIter++) 
	{
		USES_CONVERSION;
		bSuccess = CreateDirectoryA( W2A(vIter->c_str()), NULL ) ? true :false;
	
	}

    return bSuccess; 
}

void CZipImplement::SetZipFile(vector<CString>* pVec)
{
	this->m_pVecZipFile = pVec;
}
