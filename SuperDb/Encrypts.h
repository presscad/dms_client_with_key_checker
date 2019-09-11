#pragma once


BOOL __declspec(dllexport) GetBaseBoardByCmd(char *lpszBaseBoard, int len/*=128*/);

BOOL __declspec(dllexport) GetMacAddrByCmd(char *lpszBaseBoard, int len);

CString __declspec(dllexport)  GetCPUID();

class  __declspec(dllexport)  CEncrypts
{
public:
	CEncrypts(void);
public:
	~CEncrypts(void);

	BOOL OpenLicence(CString& strPath);

	BOOL OpenLicence();

	int  CheckLicence(CString strPath,char & type);

	static CStringArray m_arrStrName;

	static CStringArray& sGetArrName();

	static CString sGetLinInfo();

	static void GetMacAddress(CStringArray* arrAddress);
};
