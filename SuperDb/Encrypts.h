#pragma once

class  __declspec(dllexport)  CEncrypts
{
public:
	CEncrypts(void);
public:
	~CEncrypts(void);

	BOOL OpenLicence(CString& strPath);

	static int  CheckLicence(CString strPath,char & type);

	static CStringArray m_arrStrName;

	static CStringArray& sGetArrName();

	static void GetMacAddress(CStringArray* arrAddress);
};
