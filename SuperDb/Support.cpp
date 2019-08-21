#include "stdafx.h"
#include "Support.h"


// �ַ���ת����
int sToInteger(LPCSTR strVal)
{
	if (strlen(strVal) <= 0)
		return 0;

	int nVal = 0;
	sscanf(strVal, "%d", &nVal);
	return nVal;
}

// ˫������������ת����
int sToInteger(double dVal)
{
	if (dVal < 0.0)
		return int(dVal - 0.50);
	else
		return int(dVal + 0.49);
}

// �ַ���ת������
float sToFloat(LPCSTR strVal)
{
	if (strlen(strVal) <= 0)
		return 0.0F;

	float fVal = 0.0F;
	sscanf(strVal, "%f", &fVal);
	return fVal;
}

// ��׼�����֣�ȥ��ǰ����β׺���ɼ��ַ�
CString sStdNumber(LPCSTR strVal)
{
	CString strRet = "";
	int  ii, mm;
	mm = strlen(strVal);
	for (ii = 0; ii < mm; ii ++)
	{
		if (strVal[ii] > 0 && strVal[ii] < 32)
			continue;
		strRet += strVal[ii];
	}
	return strRet;
}


// �ַ���ת˫����
double sToDouble(LPCSTR strVal)
{
	if (strlen(strVal) <= 0)
		return 0.0;

	double dVal = 0.0;
	sscanf(strVal, "%lf", &dVal);
	return dVal;
}

// �� _int64 ����ת��Ϊ double ����
double sInt64ToDouble(__int64 i64Val)
{
	CString strInt64;
	double  dblVal;

	strInt64.Format("%I64d", i64Val);
	dblVal = sToDouble(strInt64);
	return dblVal;
}


// ͨ���ָ�����ȡ�Ӵ�������ȥ��ǰ������β׺�������Ժ��Բ��ɼ��ַ�
CString sGetSubStr(LPCSTR buf, char chSepa, int nSta, int *pnEnd,
				   LPCSTR strPrefix, LPCSTR strSuffix, BOOL bIgnoreInvisi)
{
	CString strSub = "";
	char ch;

	// ����ǰ���ո�
	int cnt = (int)strlen(buf);
	int ii;
	for (ii = nSta; ii < cnt; ii ++)
	{
		ch = buf[ii];
		if (bIgnoreInvisi && ch > 0x00 && ch < 0x20)
			ch = ' ';

		if (ch != ' ')
			break;
	}

	// ��������
	for ( ; ii < cnt; ii ++)
	{
		ch = buf[ii];
		if (bIgnoreInvisi && ch > 0x00 && ch < 0x20)
			ch = ' ';

		if (ch == chSepa)
			break;

		strSub += ch;
	}

	// ȥ��β׺�ո�
	strSub.TrimRight();

	// ��д����λ��
	if (pnEnd)
		*pnEnd = ii;

	// ȥ��ǰ����
	int nPrefix = strlen(strPrefix);
	if (nPrefix > 0)
	{
		if (strSub.Left(nPrefix) == strPrefix)
			strSub = strSub.Mid(nPrefix);
	}

	// ȥ��β׺��
	int nSuffix = strlen(strSuffix);
	if (nSuffix > 0)
	{
		if (strSub.Right(nSuffix) == strSuffix)
			strSub = strSub.Left(strSub.GetLength() - nSuffix);
	}

	return strSub;
}

CString sGetSubStr(CString& strVal, char chSepa, int nSta, int *pnEnd,
				   LPCSTR strPrefix, LPCSTR strSuffix, BOOL bIgnoreInvisi)
{
	CString strSub = "";
	char ch;

	// ����ǰ���ո�
	int cnt = strVal.GetLength();
	int ii;
	for (ii = nSta; ii < cnt; ii ++)
	{
		ch = strVal.GetAt(ii);
		if (bIgnoreInvisi && ch > 0x00 && ch < 0x20)
			ch = ' ';

		if (ch != ' ')
			break;
	}

	// ��������
	for ( ; ii < cnt; ii ++)
	{
		ch = strVal.GetAt(ii);
		if (bIgnoreInvisi && ch > 0x00 && ch < 0x20)
			ch = ' ';

		if (ch == chSepa)
			break;

		strSub += ch;
	}

	// ȥ��β׺�ո�
	strSub.TrimRight();

	// ��д����λ��
	if (pnEnd)
		*pnEnd = ii;

	// ȥ��ǰ����
	int nPrefix = strlen(strPrefix);
	if (nPrefix > 0)
	{
		if (strSub.Left(nPrefix) == strPrefix)
			strSub = strSub.Mid(nPrefix);
	}

	// ȥ��β׺��
	int nSuffix = strlen(strSuffix);
	if (nSuffix > 0)
	{
		if (strSub.Right(nSuffix) == strSuffix)
			strSub = strSub.Left(strSub.GetLength() - nSuffix);
	}

	return strSub;
}