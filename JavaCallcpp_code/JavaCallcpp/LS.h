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
	int m;	//��ĸ���
	int n;	//����ʽ����
	double *pSolution;	//����ʽϵ��

	vector<double>  m_VertexY;

public:
	CLS();
	virtual ~CLS();
	static CLS *getInstance();

	void setN(int t);	//n�ζ���ʽ���
	void addPoint(double x, double y);	//���һ����
	void restart();
	bool Solve();
	double *getSolution() const;	//��ö���ʽϵ��
	double calcY(double x);	//����x�������y

	void addPoint(double y);	//���һ����
	bool Solve2();
};

#endif // !defined(AFX_LS_H__208D279A_F391_4DA8_BBE3_3895A9800FFE__INCLUDED_)
