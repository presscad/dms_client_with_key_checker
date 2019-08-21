// Support.h : Support DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif


// 字符串转整型
__declspec(dllexport) int sToInteger(LPCSTR strVal);
// 双精度四舍五入转整型
__declspec(dllexport) int sToInteger(double dVal);

// 字符串转单精度
__declspec(dllexport) float sToFloat(LPCSTR strVal);

// 标准化数字，去掉前导和尾缀不可见字符
__declspec(dllexport) CString sStdNumber(LPCSTR strVal);

// 字符串转双精度
__declspec(dllexport) double sToDouble(LPCSTR strVal);
// 从 _int64 类型转换为 double 类型
__declspec(dllexport) double sInt64ToDouble(__int64 i64Val);

// 通过分隔符提取子串，支持加前后缀、忽略不可见字符
__declspec(dllexport) CString sGetSubStr(LPCSTR buf, char chSepa, int nSta = 0,
										 int *pnEnd = NULL, LPCSTR strPrefix = "",
										 LPCSTR strSuffix = "", BOOL bIgnoreInvisi = TRUE);
__declspec(dllexport) CString sGetSubStr(CString& strVal, char chSepa, int nSta = 0,
										 int *pnEnd = NULL, LPCSTR strPrefix = "",
										 LPCSTR strSuffix = "", BOOL bIgnoreInvisi = TRUE);