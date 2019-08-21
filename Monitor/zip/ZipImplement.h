///////////////////////////////////////////////////////////////////////////// 
// �ļ���: <ZipImplement.h> 
// ˵��:ѹ����ѹ���ļ��� 
///////////////////////////////////////////////////////////////////////////// 

#pragma once 
//#include "stdafx.h"
#include <atlconv.h>
#include "Zip.h" 
#include "Unzip.h" 
#include "..\Crypt.h"
#include <vector>
using namespace  std;

class CZipImplement 
{ 
public: 
	CZipImplement(void); 

	CZipImplement(CString prefixName,char type); 
	~CZipImplement(void); 

private: 
	HZIP hz;//Zip�ļ���� 
	ZRESULT zr;//��������ֵ 
	ZIPENTRY ze;//Zip�ļ���� 
	CString m_FolderPath;//folder·�� 
	CString  m_FolderName;//folder��Ҫ��ѹ�����ļ����� 

	CString  m_prefixName;
	char     m_chType;
	CCrypt m_Crypt;

private: 
	//ʵ�ֱ����ļ��� 
	int BrowseFile(CString &strFile); 
	//ʵ�ֱ����ļ��� 
	int BrowseFile6(CString &strFile); 
	CString ReadTmpFile(CString& strFileName);
	int BrowseFile4(CString &strFile);

	//��ȡ���·�� 
	void GetRelativePath(CString& pFullPath, CString& pSubString); 

	//����·�� 
	BOOL CreatedMultipleDirectory(wchar_t* direct); 

	//map<CString,int>* m_pMapZipFile;

	std::vector<CString>*  m_pVecZipFile;

public: 
	//ѹ���ļ��нӿ� 
	BOOL Zip_PackFiles(CString& pFilePath, CString& mZipFileFullPath, CStringA strPW = ""); 

	//��ѹ���ļ��нӿ� 
	BOOL Zip_UnPackFiles(CString &mZipFileFullPath, CString& mUnPackPath, CStringA strPW = ""); 

	void SetZipFile(std::vector<CString>* pVec);

public: 
	//��̬�����ṩ�ļ���·����� 
	static BOOL FolderExist(CString& strPath); 
};
