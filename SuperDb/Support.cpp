#include "stdafx.h"
#include "Support.h"


// 字符串转整型
int sToInteger(LPCSTR strVal)
{
	if (strlen(strVal) <= 0)
		return 0;

	int nVal = 0;
	sscanf(strVal, "%d", &nVal);
	return nVal;
}

// 双精度四舍五入转整型
int sToInteger(double dVal)
{
	if (dVal < 0.0)
		return int(dVal - 0.50);
	else
		return int(dVal + 0.49);
}

// 字符串转单精度
float sToFloat(LPCSTR strVal)
{
	if (strlen(strVal) <= 0)
		return 0.0F;

	float fVal = 0.0F;
	sscanf(strVal, "%f", &fVal);
	return fVal;
}

// 标准化数字，去掉前导和尾缀不可见字符
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


// 字符串转双精度
double sToDouble(LPCSTR strVal)
{
	if (strlen(strVal) <= 0)
		return 0.0;

	double dVal = 0.0;
	sscanf(strVal, "%lf", &dVal);
	return dVal;
}

// 从 _int64 类型转换为 double 类型
double sInt64ToDouble(__int64 i64Val)
{
	CString strInt64;
	double  dblVal;

	strInt64.Format("%I64d", i64Val);
	dblVal = sToDouble(strInt64);
	return dblVal;
}


// 通过分隔符提取子串，可以去掉前导符和尾缀符，可以忽略不可见字符
CString sGetSubStr(LPCSTR buf, char chSepa, int nSta, int *pnEnd,
				   LPCSTR strPrefix, LPCSTR strSuffix, BOOL bIgnoreInvisi)
{
	CString strSub = "";
	char ch;

	// 跳过前导空格
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

	// 检测结束符
	for ( ; ii < cnt; ii ++)
	{
		ch = buf[ii];
		if (bIgnoreInvisi && ch > 0x00 && ch < 0x20)
			ch = ' ';

		if (ch == chSepa)
			break;

		strSub += ch;
	}

	// 去掉尾缀空格
	strSub.TrimRight();

	// 回写结束位置
	if (pnEnd)
		*pnEnd = ii;

	// 去掉前导符
	int nPrefix = strlen(strPrefix);
	if (nPrefix > 0)
	{
		if (strSub.Left(nPrefix) == strPrefix)
			strSub = strSub.Mid(nPrefix);
	}

	// 去掉尾缀符
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

	// 跳过前导空格
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

	// 检测结束符
	for ( ; ii < cnt; ii ++)
	{
		ch = strVal.GetAt(ii);
		if (bIgnoreInvisi && ch > 0x00 && ch < 0x20)
			ch = ' ';

		if (ch == chSepa)
			break;

		strSub += ch;
	}

	// 去掉尾缀空格
	strSub.TrimRight();

	// 回写结束位置
	if (pnEnd)
		*pnEnd = ii;

	// 去掉前导符
	int nPrefix = strlen(strPrefix);
	if (nPrefix > 0)
	{
		if (strSub.Left(nPrefix) == strPrefix)
			strSub = strSub.Mid(nPrefix);
	}

	// 去掉尾缀符
	int nSuffix = strlen(strSuffix);
	if (nSuffix > 0)
	{
		if (strSub.Right(nSuffix) == strSuffix)
			strSub = strSub.Left(strSub.GetLength() - nSuffix);
	}

	return strSub;
}