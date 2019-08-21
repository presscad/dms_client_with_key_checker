#include "stdafx.h"


// 从路径中提取目录，末尾不带反斜杠
__declspec(dllexport) CString sGetDirFromPath(CString strPath);
// 从路径中提取文件名，包括扩展名
__declspec(dllexport) CString sGetFileFromPath(CString strPath);
// 从文件名中提取主文件名，不包括扩展名
__declspec(dllexport) CString sGetNameFromFile(CString strFile);
// 从文件名中提取扩展名，包括点
__declspec(dllexport) CString sGetExtFromFile(CString strFile);
// 通过分隔符提取子串，支持加前后缀、忽略不可见字符
__declspec(dllexport) CString sGetSubStr(LPCSTR buf, char chSepa, int nSta = 0, int *pnEnd = NULL);
// 字符串转整型
//__declspec(dllexport) int sToInteger(LPCSTR strVal);
// 获得定长串，前补空格
__declspec(dllexport) CString sPrefixLenStr(LPCSTR strVal, int nLen);
// 获得定长串，后补空格
__declspec(dllexport) CString sSuffixLenStr(LPCSTR strVal, int nLen, char chSepa = ' ');
