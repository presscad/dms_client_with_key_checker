///////////////////////////////////////////////////////////////////////////// 
// �ļ���: <ZipImplement.cpp> 
// ˵��:ѹ����ѹ���ļ��� 
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
// ����˵��: ʵ��ѹ���ļ��в��� 
// ����˵��: [in]�� 
//					pFilePath			Ҫ��ѹ�����ļ��� 
//					mZipFileFullPath	ѹ������ļ�����·�� 
//					strPW				ѹ�����룬�ɿգ�Ĭ��Ϊ�գ���ʾ��������ѹ��
// ����ֵ: �������������·���FALSE,ѹ���ɹ��󷵻�TRUE 
///////////////////////////////////////////////////////////////////////////// 
BOOL CZipImplement::Zip_PackFiles(CString& pFilePath, CString& mZipFileFullPath, CStringA strPW) 
{ 
    //�������� 
    if ((pFilePath == L"") || (mZipFileFullPath == L"")) 
    { 
        //·���쳣���� 
        return FALSE ; 
    } 

    if(!CZipImplement::FolderExist(pFilePath)) 
    { 
        //Ҫ��ѹ�����ļ��в����� 
        return FALSE ; 
    } 

    CString tZipFilePath = mZipFileFullPath.Left(mZipFileFullPath.ReverseFind('\\') + 1); 
    if(!CZipImplement::FolderExist(tZipFilePath)) 
   { 
        //ZIP�ļ���ŵ��ļ��в����ڴ����� 
		CStringW strWZipFilePath(tZipFilePath);
		if (FALSE == CreatedMultipleDirectory((LPWSTR)(LPCWSTR)strWZipFilePath)) 
		{ 
			//����Ŀ¼ʧ�� 
			return FALSE; 
		} 
    } 

    //����ļ��е����� 
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

    //����ZIP�ļ�
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
        //����Zip�ļ�ʧ�� 
        return FALSE; 
    } 

	int cnt(0);

	if(m_chType == 0){
		cnt = BrowseFile6(pFilePath);

	}else if(m_chType == 1 || m_chType == 2 || m_chType == 3){
		cnt = BrowseFile4(pFilePath);
	}else{
    //�ݹ��ļ���,����ȡ���ʼۼ���ZIP�ļ� 
		cnt = BrowseFile(pFilePath); 
	}
    //�ر�ZIP�ļ� 
    CloseZip(hz); 

    /************************************************************************/ 

    CFileFind tFFind; 
    if (!tFFind.FindFile(mZipFileFullPath)) 
    { 
        //ѹ��ʧ��(δ����ѹ������ļ�) 
        return FALSE; 
    } 

	if(cnt == 0){
		DeleteFile(mZipFileFullPath);
		return FALSE;
	}else
		return TRUE;  
} 

///////////////////////////////////////////////////////////////////////////// 
// ����˵��: ��ѹ���ļ��� 
// ����˵��: [in]��
//					mUnPackPath			��ѹ���ļ���ŵ�·�� 
//					mZipFileFullPath	ZIP�ļ���·�� 
//					strPW				��ѹ���룬�ɿգ�Ĭ��Ϊ�գ���ʾѹ����û������
///////////////////////////////////////////////////////////////////////////// 
BOOL CZipImplement::Zip_UnPackFiles(CString &mZipFileFullPath, CString& mUnPackPath, CStringA strPW) 
{ 
    //�������� 
    if ((mUnPackPath.IsEmpty()) || (mZipFileFullPath.IsEmpty())) 
    { 
        //·���쳣���� 
        return FALSE ; 
    } 

    CFileFind tFFind; 
    if (!tFFind.FindFile(mZipFileFullPath)) 
    { 
        //ѹ��ʧ��(δ����ѹ���ļ�) 
        return FALSE; 
    } 

    //�����ѹ����·�������� ��ͼ������ 
    CString tZipFilePath = mUnPackPath; 
    if(!CZipImplement::FolderExist(tZipFilePath)) 
    { 
        //��ѹ���ŵ��ļ��в����� ������         
		CStringW strWZipFilePath(tZipFilePath);
        if (FALSE == CreatedMultipleDirectory((LPWSTR)(LPCWSTR)strWZipFilePath)) 
        { 
            //����Ŀ¼ʧ�� 
            return FALSE; 
        } 
    } 
    /************************************************************************/ 
    //��ZIP�ļ�
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
        //��Zip�ļ�ʧ�� 
        return FALSE; 
    } 

    zr=SetUnzipBaseDir(hz,mUnPackPath); 
    if(zr != ZR_OK) 
    { 
        //��Zip�ļ�ʧ�� 
        CloseZip(hz); 
        return FALSE;       
    } 

    zr=GetZipItem(hz,-1,&ze); 
    if(zr != ZR_OK) 
    { 
        //��ȡZip�ļ�����ʧ�� 
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
            //��ȡZip�ļ�����ʧ�� 
            CloseZip(hz); 
            return FALSE;       
        } 
    } 

    CloseZip(hz); 
    return TRUE; 
} 

//////////////////////////////////////////////////////////////////////////// 
// ����˵��: ���ָ�����ļ����Ƿ���� 
// ����˵��: [in]��strPath �����ļ��� (�˷�����������·��ĩβ���*.*) 
// ����ֵ:BOOL����,���ڷ���TRUE,����ΪFALSE 
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
        //������ڲ��������ļ��� 
        rValue = TRUE; 
    } 

    FindClose(hFind); 
    return rValue; 
} 

///////////////////////////////////////////////////////////////////////////// 
// ����˵��: �����ļ��� 
// ����˵��: [in]��strFile �������ļ���(�˷�����������·��ĩβ���*.*) 
// ����ֵ:BOOL����,���ڷ���TRUE,����ΪFALSE 
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
            //�����һ����Ŀ¼���õݹ��������һ���� 
            CString strPath = ff.GetFilePath(); 

            CString subPath; 
            GetRelativePath(strPath,subPath); 
			subPath = m_prefixName + "\\" + subPath;
            //���ļ���ӵ�ZIP�ļ� 
            ZipAddFolder(hz,subPath); 
            BrowseFile(strPath); 
			cnt ++;
        } 
        else if(!ff.IsDirectory() && !ff.IsDots()) 
        { 
            //��ʾ��ǰ���ʵ��ļ�(����·��) 
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
            //���ļ���ӵ�ZIP�ļ� 
            ZipAdd(hz,subPath,strPath);
			cnt ++;
        } 
    } 

    //�ر� 
    ff.Close(); 

	return cnt;
} 


///////////////////////////////////////////////////////////////////////////// 
// ����˵��: �����ļ��� 
// ����˵��: [in]��strFile �������ļ���(�˷�����������·��ĩβ���*.*) 
// ����ֵ:BOOL����,���ڷ���TRUE,����ΪFALSE 
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
// ����˵��: ��ȡ���·�� 
// ����˵��: [in]��pFullPath ��ǰ�ļ�������·�� [out] : ����������·�� 
///////////////////////////////////////////////////////////////////////////// 
void CZipImplement::GetRelativePath(CString& pFullPath,CString& pSubString) 
{ //+ this->m_FolderName.GetLength()
    pSubString = pFullPath.Right(pFullPath.GetLength() - this->m_FolderPath.GetLength() -1 ); 
} 

///////////////////////////////////////////////////////////////////////////// 
// ����˵��: �����༶Ŀ¼ 
// ����˵��: [in]�� ·���ַ��� 
// ����ֵ: BOOL �ɹ�True ʧ��False 
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
