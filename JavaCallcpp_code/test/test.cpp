// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "statisticalMethod.h"
#pragma comment(lib, "javacallcpp.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	double dVals[4] = {1,1,1,1};
	double R[] = {1};
	double datas[] = {145,137,142,117,134,130,117,126,122,141,177,143,156,181,161,157,160,131,145,103,121,122,116,123,104,112,132,111,120,121,125,128,141,123,123,121,107,104,115,110,116,115,122,112,112,101,101,101,132,127};
	int count = sizeof(datas)/sizeof(double);
	int nSolution = 3;
	bool bConst = true;
	bool ret = getTrendLine(dVals, R, datas, count, nSolution, bConst);

	readLic("D:\\SDK\\apache-tomcat-8.5.23\\webapps\\dms\\");

	int  nRet[3];
	int date = checked(nRet);

	if(ret)
		printf("check ok?");
	return 0;
}

