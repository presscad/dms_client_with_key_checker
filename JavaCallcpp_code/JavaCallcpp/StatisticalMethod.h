#pragma once
#ifndef __MAIN_H__
#define __MAIN_H__

// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 TESTJNI_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// TESTJNI_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef TESTJNI_EXPORTS
#define TESTJNI_API __declspec(dllexport)
#else
#define TESTJNI_API __declspec(dllimport)
#endif

class  TESTJNI_API CStatisticalMethod
{
public:
	CStatisticalMethod();
	~CStatisticalMethod();
};



void TestMain();


#ifdef __cplusplus
extern "C"
{
#endif

	extern TESTJNI_API bool CountMath(double dVals[],double dAPlus, double dAMinus, double datas[],
		int count, int fz, int accuracy, bool needDelete,int calculationAccuracy);

	extern TESTJNI_API bool getTrendLine(double dVals[], double R[], double datas[], int count, int nSolution, bool bConst);

	extern TESTJNI_API bool readLic(const char* licFileName);//, const char* datFile

	extern TESTJNI_API int check();

	extern TESTJNI_API int checked(int nResults[]);

#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__  



