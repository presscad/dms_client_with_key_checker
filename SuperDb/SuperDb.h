// SupperDb.h : SupperDb DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif


#include "support.h"

class CUniverDB;


// 获得Access数据库连接串
__declspec(dllexport) CString sdbGetAccessCnnStr(LPCSTR strPath, LPCSTR strUsrPwd);



// 分解连接参数
__declspec(dllexport) CString sdbSepaCnnParam(LPCSTR strCnnParam, CString& strHostName,
											  CString& strLibName, CString& strUsrName,
											  CString& strUsrPwd);
// 合成连接参数
__declspec(dllexport) CString sdbCompCnnParam(CString strLibType, CString strHostName,
											  CString strLibName, CString strUsrName,
											  CString strUsrPwd);

