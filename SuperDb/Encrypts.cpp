#include "StdAfx.h"
#include "Encrypts.h"

CStringArray CEncrypts::m_arrStrName;
CEncrypts::CEncrypts(void)
{
}

CEncrypts::~CEncrypts(void)
{
}

BOOL CEncrypts::OpenLicence(CString& strPath){
	return TRUE;
}

int CEncrypts::CheckLicence(CString strPath,char & type){
	type = 2;
	return 2;
}

CStringArray& CEncrypts::sGetArrName(){
	return m_arrStrName;
}


void CEncrypts::GetMacAddress(CStringArray* arrAddress){

}
