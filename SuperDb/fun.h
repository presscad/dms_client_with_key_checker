#include "stdafx.h"


// ��·������ȡĿ¼��ĩβ������б��
__declspec(dllexport) CString sGetDirFromPath(CString strPath);
// ��·������ȡ�ļ�����������չ��
__declspec(dllexport) CString sGetFileFromPath(CString strPath);
// ���ļ�������ȡ���ļ�������������չ��
__declspec(dllexport) CString sGetNameFromFile(CString strFile);
// ���ļ�������ȡ��չ����������
__declspec(dllexport) CString sGetExtFromFile(CString strFile);
// ͨ���ָ�����ȡ�Ӵ���֧�ּ�ǰ��׺�����Բ��ɼ��ַ�
__declspec(dllexport) CString sGetSubStr(LPCSTR buf, char chSepa, int nSta = 0, int *pnEnd = NULL);
// �ַ���ת����
//__declspec(dllexport) int sToInteger(LPCSTR strVal);
// ��ö�������ǰ���ո�
__declspec(dllexport) CString sPrefixLenStr(LPCSTR strVal, int nLen);
// ��ö��������󲹿ո�
__declspec(dllexport) CString sSuffixLenStr(LPCSTR strVal, int nLen, char chSepa = ' ');
