// SupperDb.h : SupperDb DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif


#include "support.h"

class CUniverDB;


// ���Access���ݿ����Ӵ�
__declspec(dllexport) CString sdbGetAccessCnnStr(LPCSTR strPath, LPCSTR strUsrPwd);



// �ֽ����Ӳ���
__declspec(dllexport) CString sdbSepaCnnParam(LPCSTR strCnnParam, CString& strHostName,
											  CString& strLibName, CString& strUsrName,
											  CString& strUsrPwd);
// �ϳ����Ӳ���
__declspec(dllexport) CString sdbCompCnnParam(CString strLibType, CString strHostName,
											  CString strLibName, CString strUsrName,
											  CString strUsrPwd);

