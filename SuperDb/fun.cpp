#include "StdAfx.h"
#include "fun.h"


///////////////////////////////////////////////////////////////////
// 工具函数

// 从路径中提取目录，末尾不带反斜杠
CString sGetDirFromPath(CString strPath)
{
	int ii, cnt = strPath.GetLength();
	for (ii = cnt - 1; ii >= 0; ii --)
	{
		if (strPath[ii] == '\\' || strPath[ii] == ':')
			break;
	}

	if (ii < 0)
		return "";

	if (strPath[ii] == ':')
		ii ++;
	return strPath.Left(ii);
}

// 从路径中提取文件名，包括扩展名
CString sGetFileFromPath(CString strPath)
{
	int ii, cnt = strPath.GetLength();
	for (ii = cnt - 1; ii >= 0; ii --)
	{
		if (strPath[ii] == '\\' || strPath[ii] == ':') 
			break;
	}

	return strPath.Mid(ii + 1);
}

// 从文件名中提取主文件名，不包括扩展名
CString sGetNameFromFile(CString strFile)
{
	int ii, cnt = strFile.GetLength();
	for (ii = cnt - 1; ii >= 0; ii --)
	{
		if (strFile[ii] == '.')
			break;
	}

	if (ii < 0)
		return strFile;

	return strFile.Left(ii);
}

// 从文件名中提取扩展名，包括点
CString sGetExtFromFile(CString strFile)
{
	int ii, cnt = strFile.GetLength();
	for (ii = cnt - 1; ii >= 0; ii --)
	{
		if (strFile[ii] == '.')
			break;
	}

	if (ii < 0)
		return "";

	return strFile.Mid(ii);
}

// 通过分隔符提取子串，可以去掉前导符和尾缀符，可以忽略不可见字符
CString sGetSubStr(LPCSTR buf, char chSepa, int nSta, int *pnEnd)
{
	CString strSub = "";
	char ch;

	// 跳过前导空格
	int ii, cnt = (int)strlen(buf);
	for (ii = nSta; ii < cnt; ii ++)
	{
		ch = buf[ii];
		if (ch > 0x00 && ch < 0x20)
			ch = ' ';

		if (ch != ' ')
			break;
	}

	// 检测结束符
	for ( ; ii < cnt; ii ++)
	{
		ch = buf[ii];
		if (ch > 0x00 && ch < 0x20)
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

	return strSub;
}

//// 字符串转整型
//int sToInteger(LPCSTR strVal)
//{
//	if (strlen(strVal) <= 0)
//		return 0;
//
//	int nVal = 0;
//	sscanf_s(strVal, "%d", &nVal);
//	return nVal;
//}

// 获得定长串，前补空格
CString sPrefixLenStr(LPCSTR strVal, int nLen)
{
	CString strRet = "";

	int num = nLen - (int)strlen(strVal);
	for (int ii = 0; ii < num; ii ++)
		strRet += ' ';

	strRet += strVal;
	return strRet;
}

// 获得定长串，后补空格
CString sSuffixLenStr(LPCSTR strVal, int nLen, char chSepa)
{
	CString strRet = strVal;

	int num = nLen - (int)strlen(strVal);
	for (int ii = 0; ii < num; ii ++)
		strRet += chSepa;

	return strRet;
}
