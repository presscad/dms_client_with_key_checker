#pragma once
#ifndef __MAIN_H__
#define __MAIN_H__

// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� TESTJNI_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// TESTJNI_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
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



