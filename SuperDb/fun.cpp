#include "StdAfx.h"
#include "fun.h"


///////////////////////////////////////////////////////////////////
// ���ߺ���

// ��·������ȡĿ¼��ĩβ������б��
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

// ��·������ȡ�ļ�����������չ��
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

// ���ļ�������ȡ���ļ�������������չ��
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

// ���ļ�������ȡ��չ����������
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

// ͨ���ָ�����ȡ�Ӵ�������ȥ��ǰ������β׺�������Ժ��Բ��ɼ��ַ�
CString sGetSubStr(LPCSTR buf, char chSepa, int nSta, int *pnEnd)
{
	CString strSub = "";
	char ch;

	// ����ǰ���ո�
	int ii, cnt = (int)strlen(buf);
	for (ii = nSta; ii < cnt; ii ++)
	{
		ch = buf[ii];
		if (ch > 0x00 && ch < 0x20)
			ch = ' ';

		if (ch != ' ')
			break;
	}

	// ��������
	for ( ; ii < cnt; ii ++)
	{
		ch = buf[ii];
		if (ch > 0x00 && ch < 0x20)
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

	return strSub;
}

//// �ַ���ת����
//int sToInteger(LPCSTR strVal)
//{
//	if (strlen(strVal) <= 0)
//		return 0;
//
//	int nVal = 0;
//	sscanf_s(strVal, "%d", &nVal);
//	return nVal;
//}

// ��ö�������ǰ���ո�
CString sPrefixLenStr(LPCSTR strVal, int nLen)
{
	CString strRet = "";

	int num = nLen - (int)strlen(strVal);
	for (int ii = 0; ii < num; ii ++)
		strRet += ' ';

	strRet += strVal;
	return strRet;
}

// ��ö��������󲹿ո�
CString sSuffixLenStr(LPCSTR strVal, int nLen, char chSepa)
{
	CString strRet = strVal;

	int num = nLen - (int)strlen(strVal);
	for (int ii = 0; ii < num; ii ++)
		strRet += chSepa;

	return strRet;
}
