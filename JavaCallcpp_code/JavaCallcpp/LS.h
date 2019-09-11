// LS.h: interface for the CLS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LS_H__208D279A_F391_4DA8_BBE3_3895A9800FFE__INCLUDED_)
#define AFX_LS_H__208D279A_F391_4DA8_BBE3_3895A9800FFE__INCLUDED_
#include <vector>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

using namespace std;

class CLS  
{
private:
	static CLS *_instance;

	double pVertex[100][2];
	int m;	//点的个数
	int n;	//多项式次数
	double *pSolution;	//多项式系数

	vector<double>  m_VertexY;

public:
	CLS();
	virtual ~CLS();
	static CLS *getInstance();

	void setN(int t);	//n次多项式拟合
	void addPoint(double x, double y);	//添加一个点
	void restart();
	bool Solve();
	double *getSolution() const;	//获得多项式系数
	double calcY(double x);	//根据x坐标计算y

	void addPoint(double y);	//添加一个点
	bool Solve2();
};

#endif // !defined(AFX_LS_H__208D279A_F391_4DA8_BBE3_3895A9800FFE__INCLUDED_)
