#include "stdafx.h"


// ��·������ȡĿ¼��ĩβ������б��
CString sGetDirFromPath(CString strPath);
// ��·������ȡ�ļ�����������չ��
CString sGetFileFromPath(CString strPath);
// ���ļ�������ȡ���ļ�������������չ��
CString sGetNameFromFile(CString strFile);
// ���ļ�������ȡ��չ����������
CString sGetExtFromFile(CString strFile);
// ͨ���ָ�����ȡ�Ӵ���֧�ּ�ǰ��׺�����Բ��ɼ��ַ�
CString sGetSubStr(LPCSTR buf, char chSepa, int nSta = 0, int *pnEnd = NULL);
// �ַ���ת����
int sToInteger(LPCSTR strVal);
// ��ö�������ǰ���ո�
CString sPrefixLenStr(LPCSTR strVal, int nLen);
// ��ö��������󲹿ո�
CString sSuffixLenStr(LPCSTR strVal, int nLen, char chSepa = ' ');
