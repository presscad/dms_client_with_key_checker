// Support.h : Support DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif


// �ַ���ת����
__declspec(dllexport) int sToInteger(LPCSTR strVal);
// ˫������������ת����
__declspec(dllexport) int sToInteger(double dVal);

// �ַ���ת������
__declspec(dllexport) float sToFloat(LPCSTR strVal);

// ��׼�����֣�ȥ��ǰ����β׺���ɼ��ַ�
__declspec(dllexport) CString sStdNumber(LPCSTR strVal);

// �ַ���ת˫����
__declspec(dllexport) double sToDouble(LPCSTR strVal);
// �� _int64 ����ת��Ϊ double ����
__declspec(dllexport) double sInt64ToDouble(__int64 i64Val);

// ͨ���ָ�����ȡ�Ӵ���֧�ּ�ǰ��׺�����Բ��ɼ��ַ�
__declspec(dllexport) CString sGetSubStr(LPCSTR buf, char chSepa, int nSta = 0,
										 int *pnEnd = NULL, LPCSTR strPrefix = "",
										 LPCSTR strSuffix = "", BOOL bIgnoreInvisi = TRUE);
__declspec(dllexport) CString sGetSubStr(CString& strVal, char chSepa, int nSta = 0,
										 int *pnEnd = NULL, LPCSTR strPrefix = "",
										 LPCSTR strSuffix = "", BOOL bIgnoreInvisi = TRUE);