#include "stdafx.h"
#include "StatisticalMethod.h"
#include "QdrsPrdctData.h"
#include "ls.h"
#include<iostream>
#include<algorithm>
#include <vector>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <openssl/opensslv.h>
#include <openssl/rsa.h>
#include <openssl/des.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <process.h>
using namespace std;

#ifdef _WIN64
#pragma comment(lib, "crypto/lib64/libcryptoMD.lib")
#else
#pragma comment(lib, "crypto/lib/libcryptoMD.lib")
#endif


int L_Staticstical_expried = 0;
int L_Staticstaial_count = 0;
int L_Staticstaial_type = 0;
int L_Staticstaial_users = 0;
char *L_Staticstaial_Name = NULL;

using namespace std;


CStatisticalMethod::CStatisticalMethod()
{

}


CStatisticalMethod::~CStatisticalMethod()
{
	
}

///////////////////////

//void printData(vector<CQdrsPrdctData*> list, CString title) {
//	TRACE("%s--------------------------------------------\n", title);
//	TRACE("myid\t����\tDֵ\t���\tResult\t���յȼ�\t�޳���dֵ\t����\n");
//	for (int ii = 0; ii < list.size(); ii++) {
//		list[ii]->toString();
//	}
//}

bool comp(CQdrsPrdctData& a,CQdrsPrdctData& b)
{
	return a.getDataValue() < b.getDataValue();
}

double Round(double number, unsigned int bits) {
	//stringstream ss;
	//ss << fixed << setprecision(bits) << number;
	//ss >> number;
	//return number;

	int n = bits;
	double temp = number* pow(10.0, n);//��n<0�������ʾ��ȷ��С������|n|λ
	temp = floor(temp + 0.5);//��n=0����ʾ��������ֻ����������������
	return (temp * pow(10.0, -n));
}


vector<double> preCheckError(vector<CQdrsPrdctData*> list, double results[], int calculationAccuracy) {
	double sum = 0.0;
	vector<double> dataList;// = new vector<double>();
	double dValue;

	for (int ii = 0; ii < list.size(); ii++) {
		CQdrsPrdctData& qpd = *list[ii];
		qpd.setMyId(ii);
		dValue = qpd.getDataValue();
		dataList.push_back(dValue);
		sum += dValue;
	}

	sort(dataList.begin(), dataList.end());

	results[2] = Round(sum / (double)list.size(), calculationAccuracy);

	//Collections.sort(dataList);dddddd
	results[0] = dataList[dataList.size() - 1];
	results[1] = dataList[0];

	return dataList;
}

bool comp1(CQdrsPrdctData* a, CQdrsPrdctData* b)
{
	return a->getDValue() > b->getDValue();
}

/**
* @param list ���ݼ���
* @return	������������
*/
vector<CQdrsPrdctData*> getDDescList(vector<CQdrsPrdctData*> list){
	vector<CQdrsPrdctData*> newList;
	for (int ii = 0; ii < list.size(); ii ++) {
		newList.push_back(list[ii]);
	}

	sort(newList.begin(), newList.end(),comp1);

	return newList;
}



bool compAD(CQdrsPrdctData* a, CQdrsPrdctData* b)
{
	return a->getAdValue() > b->getAdValue();
}

/**
* @param list ���ݼ���
* @return	������������
*/
vector<CQdrsPrdctData*> getADDescList(vector<CQdrsPrdctData*> list){
	vector<CQdrsPrdctData*> newList;
	for (int ii = 0; ii < list.size(); ii++) {
		newList.push_back(list[ii]);
	}

	sort(newList.begin(), newList.end(), compAD);

	return newList;
}


bool compData(CQdrsPrdctData* a, CQdrsPrdctData* b)
{
	return a->getDataValue() > b->getDataValue();
}

//bool compData2(CQdrsPrdctData* a, CQdrsPrdctData* b){
//	if (a->getDataValue() > b->getDataValue())
//		        return true;
//		else if (a.score == b.score  && a.age > b.age)
//		        return true;
//		else                ///�����else return false�ǳ���Ҫ����������
//		        return false;
//		
//}

/**
* @param list ���ݼ���
* @return	������������
*/
vector<CQdrsPrdctData*> getDataDescList(vector<CQdrsPrdctData*> list){
	vector<CQdrsPrdctData*> newList;
	for (int ii = 0; ii < list.size(); ii++) {
		newList.push_back(list[ii]);
	}

	sort(newList.begin(), newList.end(), compData);

	return newList;
}

vector<CQdrsPrdctData*> ReMarkRes(vector<CQdrsPrdctData*> list, int iErrorCount, string sortname){
	vector<CQdrsPrdctData*> newList;
	//��ȡ��������������
	if (!sortname.compare("Dֵ desc")) {
		newList = getDDescList(list);
	}
	else {
		newList = getDataDescList(list);
	}
	int totalCount = newList.size();

	for (int i = 0; i < totalCount; i++){
		CQdrsPrdctData& qpd = *newList[i];
		if (i > 0){
			if (qpd.getDValue() == (newList[i - 1]->getDValue())) {
				qpd.setDatasort(newList[i - 1]->getDatasort());
			}
			else {
				qpd.setDatasort(i + 1);
			}
		}
		else{
			qpd.setDatasort(i + 1);
		}

		//�жϣ���ǰ��������С���޳�����ʱ���򽫵�ǰֵ�ĳ��޳�״̬
		if (i < iErrorCount){
			qpd.setRemoveD(0.0);
			qpd.setbResult(false);
			qpd.setResult("1");//�޳�
		}//����ǰֵΪ��Чֵ
		else{
			qpd.setbResult(true);
			qpd.setResult("0");//��Ч
			qpd.setRemoveD(qpd.getDValue());
		}
	}

//	printData(newList, L"ReMarkRes");
	return newList;

}

vector<CQdrsPrdctData*> getQdrsPrdctDataResutList(vector<CQdrsPrdctData*> list){
	vector<CQdrsPrdctData*> newList;
	for (int ii = 0; ii < list.size(); ii ++) {
		if (list[ii]->isbResult()) {
			newList.push_back(list[ii]);
		}
	}
	return newList;
}

double AVEDEV(vector<CQdrsPrdctData*> list, double fz, bool isfz)
{
	double avg = 0.0;
	int okNo = 0;
	CQdrsPrdctData* qpd = NULL;
	//double aveNewAbsSum = 0.0;
	for (int i = 0; i < list.size(); i++)
	{
		qpd = list[i];
		if (qpd->isbResult())
		{
			//aveNewAbsSum += round(qpd.getDataValue(),2);
			okNo++;
		}
	}
	if (isfz){
		avg = fz;
	}
	else{
		avg = fz;
	}
	double sumpjz = 0.0;
	for (int i = 0; i < list.size(); i++)
	{
		qpd = list[i];
		if (qpd->isbResult())
		{
			sumpjz += Round(abs((double)(qpd->getDataValue() - avg)), 2);
		}
	}
	return (sumpjz / ((double)okNo));
}

double countXn(vector<double> dataList)
{
	double dSum = 0.0;
	for (int ii = 0; ii < dataList.size(); ii ++) {
		dSum += dataList[ii];
	}
	return dSum / (double)dataList.size();
}



double chinaRound(double value, int decimals){
	if (value < 0.0){
		value = (double)(value + (5.0 / pow(10.0, (double)(decimals + 1))));
		return value;
	}

	//BigDecimal decimal = new BigDecimal(value);
	//return decimal.setScale(decimals, BigDecimal.ROUND_HALF_UP).doubleValue();

	return Round(value, decimals);
}

double countXn(double* pVals,int cnt,double& dSum)
{
	dSum = 0.0;
	for (int ii = 0; ii < cnt; ii ++) {
		dSum += pVals[ii];
	}
	return dSum / (double)cnt;
}

bool CountMath(double dVals[],double dAPlus, double dAMinus, double datas[], int count, int fz, int accuracy, bool needDelete,int calculationAccuracy)
{
	CQdrsPrdctData* pDatas = new CQdrsPrdctData[count];
	vector<CQdrsPrdctData*> list;
	for (int ii = 0; ii < count; ii++){
		pDatas[ii].setDataValue(datas[ii]);
		pDatas[ii].setMyId(ii + 1);
		list.push_back(&pDatas[ii]);
	}

	int iErrorCount = 0;
	vector<CQdrsPrdctData*> newList;

	double M = (dAPlus + dAMinus) / 2.0;

	//��ȡ��Ҫ�޳�����������
	if (needDelete){
		iErrorCount = (int)chinaRound((count * 5.0) / 100.0, 0);
	}

	double dSum(0.0);
	//˫�� ��ֵ��
	if (fz == 1){
		//ѭ������
		for (int i = 0; i < count; i++){
			CQdrsPrdctData& qpd = pDatas[i];
			//��ȡDֵ�þ���ֵ������ֵ  - �����м�ֵ��
			double dd = abs((double)(qpd.getDataValue() - M));
			//double��ֵ����λ��
			dd = Round(dd, calculationAccuracy);
			dSum += qpd.getDataValue();
			qpd.setDValue(dd);
		}

		//��ȡ�����޳����ݺ�ļ���
		newList = ReMarkRes(list, iErrorCount, "Dֵ desc");
	}else //if (fz == 2 || fz == 3)
	{
		double Xn = chinaRound(countXn(datas,count,dSum), calculationAccuracy);
		for (int i = 0; i < list.size(); i++){
			CQdrsPrdctData& qpd = *list[i];
			double doubleData = qpd.getDataValue();
			//��ȡ����Dֵ
			qpd.setDValue(Round(abs((double)(doubleData - Xn)), calculationAccuracy));
		}
		//��ȡ�����޳����ݺ�ļ���
		newList = ReMarkRes(list, iErrorCount, "Dֵ desc");

	}
	//printData(newList,L"newList");
	int okNo(count - iErrorCount);
	double MAD(0.0);
	double dataMax(0.0);

	double dDelSub(0.0);
	for (int i = 0; i < newList.size(); i++){

		if (newList[i]->isbResult()){
			MAD = newList[i]->getDValue();
			dataMax = newList[i]->getDataValue();
			break;
		}else{
			dDelSub += newList[i]->getDataValue();
		}
	}
	//QdrsPrdctData qpd = null;
	double N = 0.0;//��ƫ��ϵ��
	double AD = 0.0;//ƽ������ƫ��
	double dAvg(0.0);

	vector<CQdrsPrdctData*> vecADDesc;
	////˫�� ��ֵ��
	//if (fz == 1){
	//	vector<CQdrsPrdctData*> sublist = getQdrsPrdctDataResutList(newList);
	//	okNo = sublist.size();
	//	AD = AVEDEV(sublist, M, true); //ʣ�����������ƽ������ƫ��
	//	N = chinaRound(MAD / AD, 0);//f.�����ƫ��ϵ��N=�޳������������Dֵ/�޳����ݺ�������ADֵ����������ȡ������

	//}
	//else 
	//	if (fz == 2 || fz == 5 || fz == 3) //˫�� ��ֵ��
	{
		dAvg = Round((dSum - dDelSub) / ((double)okNo), calculationAccuracy);

		for (int i = 0; i < newList.size(); i++)
		{
			CQdrsPrdctData& qpd = *newList[i];
			double dd = abs(qpd.getDataValue() - dAvg);
			qpd.setAdValue(dd);//chinaRound(, calculationAccuracy)
		}

		dSum = 0.0;
		//okNo = 0;
		for (int i = 0; i < newList.size(); i++)
		{
			CQdrsPrdctData& qpd = *newList[i];
			if (qpd.isbResult())
			{
				dSum += qpd.getAdValue();
				//okNo++;
			}
		}
		AD = (double)(dSum / ((double)okNo));//chinaRound(Round((double)(sum / ((double)okNo)), 4), calculationAccuracy);
		N = chinaRound(MAD / AD, 0);
		if (fz != 1)
		{
			M = dAvg;
		}


	}
	//printData(newList,L"newList");
	//double dWorringMax = M + ((0.5 * N) * AD);
	//double dWorringMin = M - ((0.5 * N) * AD);
	//double dJPMax = M + (N * AD);
	//double dJPMin = M - (N * AD);
	//double dNKMax = M + ((1.5 * N) * AD);
	//double dNKMin = M - ((1.5 * N) * AD);

	double dWorringMax = M + (N * AD);
	double dWorringMin = M - (N * AD);
	double dJPMax = M + (1.5 * N * AD);
	double dJPMin = M - (1.5 * N * AD);
	double dNKMax = M + (2.0 * N * AD);
	double dNKMin = M - (2.0 * N * AD);

	//TRACE("��������\t��������\t��ƫ����\t��ƫ����\t�ڿ�����\t�ڿ�����\n");
	//TRACE("%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\n", dWorringMax, dWorringMin, dJPMax, dJPMin, dNKMax, dNKMin);

	int jd = accuracy;


	dVals[0] = dWorringMax = Round(dWorringMax, jd);
	dVals[1] = dWorringMin = Round(dWorringMin, jd);
	dVals[2] = dJPMax = Round(dJPMax, jd);
	dVals[3] = dJPMin = Round(dJPMin, jd);
	dVals[4] = dNKMax = Round(dNKMax, jd);
	dVals[5] = dNKMin = Round(dNKMin, jd);
	dAPlus = chinaRound(dAPlus, jd);
	dAMinus = chinaRound(dAMinus, jd);

	for (int ii = 0; ii < count; ii++){

		if (pDatas[ii].isbResult()){
			datas[ii] = 0.0;
		}
		else{
			datas[ii] = 1.0;
		}
	}

	delete [] pDatas;
	return true;
}


/*
bool CountMath(double dVals[],double dAPlus, double dAMinus, double datas[], int count, int fz, int accuracy, bool needDelete,int calculationAccuracy)
{
	if (CEncryptTest::s_result <= 0)
		return false;
	double dataMax;
	double dataMin;
	double dataAvg;
	double dWorringMax;
	double dWorringMin;
	double dJPMax;
	double dJPMin;
	double dNKMax;
	double dNKMin;

	if(calculationAccuracy < 0){
		if (accuracy >= 0){
			calculationAccuracy = accuracy + 1;
		}else{
			calculationAccuracy = 3;
		}
	}

	double M = (dAPlus + dAMinus) / 2.0;

	CQdrsPrdctData* pDatas = new CQdrsPrdctData[count];
	vector<CQdrsPrdctData*> list;
	for (int ii = 0; ii < count; ii++){
		pDatas[ii].setDataValue(datas[ii]);
		list.push_back(&pDatas[ii]);
	}
	double results[3];
	vector<double> dataList = preCheckError(list, results, calculationAccuracy);
	dataMax = results[0];
	dataMin = results[1];
	dataAvg = results[2];
	int iErrorCount = 0;
	vector<CQdrsPrdctData*> newList;


	//��ȡ��Ҫ�޳�����������
	if (needDelete){
		iErrorCount = (int)chinaRound((dataList.size() * 5.0) / 100.0, 0);
	}

//	printData(list, L"   org");

	//˫�� ��ֵ��
	if (fz == 1){
		//ѭ������
		for (int i = 0; i < count; i++){
			CQdrsPrdctData& qpd = pDatas[i];
			//��ȡDֵ�þ���ֵ������ֵ  - �����м�ֵ��
			double dd = abs((double)(qpd.getDataValue() - M));
			//double��ֵ����λ��
			dd = Round(dd, calculationAccuracy);
			qpd.setDValue(dd);
		}

		//��ȡ�����޳����ݺ�ļ���
		newList = ReMarkRes(list, iErrorCount, "Dֵ desc");
	}
	else if (((fz == 2) || (fz == 4)) || (fz == 5)){//˫�� �������� �������� ��ֵ��
		double Xn = chinaRound(countXn(dataList), calculationAccuracy);//�����Ѿ������˲�֪��Ϊʲô�ּ�����ƽ��ֵ
		for (int i = 0; i < list.size(); i++){
			CQdrsPrdctData& qpd = *list[i];
			double doubleData = qpd.getDataValue();
			//��ȡ����Dֵ
			qpd.setDValue(Round(abs((double)(doubleData - Xn)), calculationAccuracy));
		}
		//��ȡ�����޳����ݺ�ļ���
		newList = ReMarkRes(list, iErrorCount, "Dֵ desc");
	}
	else if (fz == 3){ //�������� ��ֵ��
		double Xn = chinaRound(countXn(dataList), calculationAccuracy);//�����Ѿ������˲�֪��Ϊʲô�ּ�����ƽ��ֵ
		for (int i = 0; i < list.size(); i++){
			CQdrsPrdctData& qpd = *list[i];
			//��ȡ����Dֵ
			qpd.setDValue(Round(abs(qpd.getDataValue() - Xn), calculationAccuracy));
		}
		//��ȡ�����޳����ݺ�ļ���
		newList = ReMarkRes(list, iErrorCount, "���� desc");
	}
	int okNo = 0;
	double MAD = 0.0;
	for (int i = 0; i < newList.size(); i++){
		if (newList[i]->isbResult()){
			MAD = newList[i]->getDValue();
			dataMax = newList[i]->getDataValue();
			break;
		}
	}
	//QdrsPrdctData qpd = null;
	double N = 0.0;//��ƫ��ϵ��
	double AD = 0.0;//ƽ������ƫ��

	//˫�� ��ֵ��
	if (fz == 1){
		vector<CQdrsPrdctData*> sublist = getQdrsPrdctDataResutList(newList);
		okNo = sublist.size();
		AD = AVEDEV(sublist, M, true); //ʣ�����������ƽ������ƫ��
		N = chinaRound(MAD / AD, 0);//f.�����ƫ��ϵ��N=�޳������������Dֵ/�޳����ݺ�������ADֵ����������ȡ������

	} //�������� ��ֵ��
	else if (fz == 3){
		double sum = 0.0;

		for (int i = 0; i < newList.size(); i++){
			CQdrsPrdctData& qpd = *newList[i];
			if (qpd.isbResult()){
				sum += qpd.getDataValue();
				okNo++;
			}
		}
		double Xn_i = Round((double)(sum / ((double)okNo)), calculationAccuracy);//�����޳�������ƽ��ֵ
		//                dtData.Columns.Add("ADֵ", typeof(double));
		for (int i = 0; i < newList.size(); i++){
			CQdrsPrdctData& qpd = *newList[i];
			//�����޳������ݵ�ADֵ��
			double adValue = chinaRound(abs((double)(qpd.getDataValue() - Xn_i)), calculationAccuracy);
			qpd.setAdValue(adValue);
		}
		newList = getADDescList(list);
		for (int i = 0; i < newList.size(); i++){
			CQdrsPrdctData& qpd = *newList[i];
			if (qpd.isbResult()){
				MAD = qpd.getAdValue();
				break;
			}
		}
		sum = 0.0;
		okNo = 0;
		for (int i = 0; i < newList.size(); i++){
			CQdrsPrdctData& qpd = *newList[i];
			if (qpd.isbResult()) {
				sum += qpd.getAdValue();
				okNo++;
			}
		}
		//               // e.�����޳������ݵ�ADֵ��ƽ������ƫ���
		//               f.�����ƫ��ϵ��N =���޳����������ֵ - �޳�������ƽ��ֵ��/ �޳����ݺ�ADֵ����������ȡ������
		AD = chinaRound(Round((double)(sum / ((double)okNo)), 4), calculationAccuracy);
		N = chinaRound((dataMax - Xn_i) / AD, 0);
		M = Xn_i;
	}
	//�������� ��ֵ��
	else if (fz == 4){
		double sum = 0.0;
		for (int i = 0; i < newList.size(); i++){
			CQdrsPrdctData& qpd = *newList[i];
			if (qpd.isbResult()){
				sum += qpd.getDataValue();
				okNo++;
			}
		}
		double Xn_i = Round((double)(sum / ((double)okNo)), calculationAccuracy);

		for (int i = 0; i < newList.size(); i++){
			CQdrsPrdctData& qpd = *newList[i];
			double adValue = chinaRound(abs((double)(qpd.getDataValue() - Xn_i)), calculationAccuracy);
			qpd.setAdValue(adValue);
		}

		newList = getADDescList(newList);
		for (int i = 0; i < newList.size(); i++){
			CQdrsPrdctData& qpd = *newList[i];
			if (qpd.isbResult()) {
				MAD = qpd.getAdValue();
				break;
			}
		}
		sum = 0.0;
		okNo = 0;
		dataMin = 999999999999.0;
		for (int i = 0; i < newList.size(); i++){
			CQdrsPrdctData& qpd = *newList[i];
			if (qpd.isbResult()) {
				sum += qpd.getAdValue();
				okNo++;
				if (dataMin > qpd.getDataValue()) {
					dataMin = qpd.getDataValue();
				}
			}
		}
		AD = chinaRound(Round((double)(sum / ((double)okNo)), 4), calculationAccuracy);
		N = chinaRound((Xn_i - dataMin) / AD, 0);
		M = Xn_i;
	}
	else if ((fz == 2) || (fz == 5)) //˫�� ��ֵ��
	{
		double sum = 0.0;
		for (int i = 0; i < newList.size(); i++)
		{
			CQdrsPrdctData& qpd = *newList[i];
			if (qpd.isbResult())
			{
				sum += qpd.getDataValue();
				okNo++;
			}
		}
		double Xn_i = Round((double)(sum / ((double)okNo)), calculationAccuracy);
		for (int i = 0; i < newList.size(); i++)
		{
			CQdrsPrdctData& qpd = *newList[i];
			qpd.setAdValue(chinaRound(abs((double)(qpd.getDataValue() - Xn_i)), calculationAccuracy));
		}

		newList = getADDescList(list);
		for (int i = 0; i < newList.size(); i++)
		{
			CQdrsPrdctData& qpd = *newList[i];
			if (qpd.isbResult())
			{
				MAD = qpd.getAdValue();
				break;
			}
		}
		sum = 0.0;
		okNo = 0;
		for (int i = 0; i < newList.size(); i++)
		{
			CQdrsPrdctData& qpd = *newList[i];
			if (qpd.isbResult())
			{
				sum += qpd.getAdValue();
				okNo++;
			}
		}
		AD = chinaRound(Round((double)(sum / ((double)okNo)), 4), calculationAccuracy);
		N = chinaRound(MAD / AD, 0);
		M = Xn_i;
		//printData(newList, "AD");
	}

	dWorringMax = M + ((0.5 * N) * AD);
	dWorringMin = M - ((0.5 * N) * AD);
	dJPMax = M + (N * AD);
	dJPMin = M - (N * AD);
	dNKMax = M + ((1.5 * N) * AD);
	dNKMin = M - ((1.5 * N) * AD);

	//TRACE("��������\t��������\t��ƫ����\t��ƫ����\t�ڿ�����\t�ڿ�����\n");
	//TRACE("%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\n", dWorringMax, dWorringMin, dJPMax, dJPMin, dNKMax, dNKMin);

	int jd = accuracy;
	

	dVals[0] = dWorringMax = Round(dWorringMax, jd);
	dVals[1] = dWorringMin = Round(dWorringMin, jd);
	dVals[2] = dJPMax = Round(dJPMax, jd);
	dVals[3] = dJPMin = Round(dJPMin, jd);
	dVals[4] = dNKMax = Round(dNKMax, jd);
	dVals[5] = dNKMin = Round(dNKMin, jd);
	dAPlus = chinaRound(dAPlus, jd);
	dAMinus = chinaRound(dAMinus, jd);

	//TRACE("��������\t��������\t��ƫ����\t��ƫ����\t�ڿ�����\t�ڿ�����\n");
	//TRACE("%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\n", dWorringMax, dWorringMin, dJPMax, dJPMin, dNKMax, dNKMin);

	for (int ii = 0; ii < count; ii++){

		if (pDatas[ii].isbResult()){
			datas[ii] = 0.0;
		}
		else{
			datas[ii] = 1.0;
		}
	}

	delete[] pDatas;

	//return dVals;

	return true;
}

*/


void TestMain(){
	//double dd = Round(1.235, 2);

	//CQdrsPrdctData datas[4];

	//vector<CQdrsPrdctData*> list;
	//double rsults[3];

	//for (int ii = 0; ii < 4; ii++)
	//	list.push_back(&datas[ii]);


	//datas[0].setDataValue(1.2);
	//datas[1].setDataValue(1.1);
	//datas[2].setDataValue(1.5);
	//datas[3].setDataValue(1.8);

	//for (int ii = 0; ii < 4; ii ++ ){
	//	datas[ii].setDValue(datas[ii].getDataValue());
	//	datas[ii].setAdValue(datas[ii].getDataValue());
	//}



	//vector<double> vals = preCheckError(list, rsults, 3);

	//vector<CQdrsPrdctData*> newList = getDDescList(list);


	//for (int ii = 0; ii < newList.size(); ii++){

	//	TRACE("%lf\n", newList[ii]->getDValue());

	//}


	//double datas[]{1.460000
	//	, 1.510000, 1.300000, 1.360000, 1.250000, 1.250000, 1.160000, 1.280000, 1.280000, 1.210000, 1.350000
	//	, 1.330000, 1.380000, 1.410000, 1.360000, 1.250000, 1.340000};
	
	//CountMath(1.6000, 1.2000, datas, 17, 1, 5, true);


}


bool getTrendLine(double dVals[], double R[], double datas[], int count, int nSolution,bool bConst){

	CLS ls;

	double avg = 0.0;
	double sstotal = 0.0;
	double ssresid = 0.0;
	double total = 0.0;
	for (int ii = 0; ii < count; ii++){
		ls.addPoint( datas[ii]);//(double)(ii + 1),
	}

	if (bConst){
		for (int ii = 0; ii < count; ii++){
			total += datas[ii];
		}
		avg = total / count;
	}

	ls.setN(nSolution);
	ls.Solve2();

	double* pSoleution = ls.getSolution();

	for (int ii = 0; ii <= nSolution; ii++){
		dVals[ii] = pSoleution[nSolution-ii];
	}

	double dYY;

	for (int ii = 0; ii < count; ii++){

		dYY = ls.calcY(ii + 1.0);
		ssresid += pow(datas[ii] - dYY, 2);
		sstotal += pow(datas[ii] - avg, 2);
		datas[ii] = dYY;
	}

	R[0] = 1.0 - ssresid / sstotal;


	return true;
}

string getRSAKey(string ff)
{
	string ret;
	ifstream infile(ff, ios_base::in | ios_base::binary);
	ostringstream inbuf; inbuf << infile.rdbuf();
	ret = inbuf.str();
	infile.close();
	return ret;
}

// ��Կ����    
std::string rsa_pub_decrypt(const std::string &encText, std::string &sKey)
{
	std::string strRet;
	BIO *keybio = BIO_new_mem_buf((unsigned char *)sKey.c_str(), -1);

	EVP_PKEY* evpkey = NULL;
	PEM_read_bio_PUBKEY(keybio, &evpkey, NULL, NULL);
	RSA* rsa = EVP_PKEY_get1_RSA(evpkey);

	int len = RSA_size(rsa);
	char *clearText = (char *)malloc(len + 1);
	memset(clearText, 0, len + 1);

	//����
	int ret = RSA_public_decrypt(encText.length(), (const unsigned char*)encText.c_str(), (unsigned char*)clearText, rsa, RSA_PKCS1_PADDING);
	if (ret >= 0){ strRet = std::string(clearText, ret); }
	else{ char err[128]; printf("%s\n", ERR_error_string(ERR_get_error(), err)); }


	// �ͷ��ڴ�  
	free(clearText);
	BIO_free_all(keybio);
	//RSA_free(rsa);
	EVP_cleanup();
	return strRet;
}


//��Կ���� + ��Ƭ
std::string rsa_pub_split128_decrypt(const std::string &encText, std::string &sKey)
{
	std::string result;
	std::string input;
	result.clear();
	for (int i = 0; i< encText.length() / 128; i++)
	{
		input.clear();
		input.assign(encText.begin() + i * 128, encText.begin() + i * 128 + 128);

		result = result + rsa_pub_decrypt(input, sKey);
	}
	if (encText.length() % 128 != 0)
	{
		int tem1 = encText.length() / 128 * 128;
		int tem2 = encText.length() - tem1;
		input.clear();
		input.assign(encText.begin() + tem1, encText.end());
		result = result + rsa_pub_decrypt(input, sKey);
	}
	return result;
}

int verify(const unsigned char* msg, size_t mlen, const unsigned char* sig, size_t slen, unsigned char* pkey)
{
	int result = -1;
	if (!msg || !mlen || !sig || !slen || !pkey) return result;

	EVP_PKEY *evpkey = EVP_PKEY_new();
	BIO *bp = BIO_new(BIO_s_mem());
	BIO_write(bp, pkey, strlen((char*)pkey));
	evpkey = PEM_read_bio_PUBKEY(bp, &evpkey, NULL, NULL);

	EVP_MD_CTX* ctx = EVP_MD_CTX_create();
	do
	{
		const EVP_MD* md = EVP_get_digestbyname("SHA1");
		int rc = EVP_DigestInit_ex(ctx, md, NULL);
		rc = EVP_DigestVerifyInit(ctx, NULL, md, NULL, evpkey);
		rc = EVP_DigestVerifyUpdate(ctx, msg, mlen);
		/* Clear any errors for the call below */
		ERR_clear_error();
		rc = EVP_DigestVerifyFinal(ctx, sig, slen);
		if (!rc) { char err[128]; printf("%s\n", ERR_error_string(ERR_get_error(), err)); }
		else result = 1;
	} while (0);
	if (ctx) {
		EVP_MD_CTX_destroy(ctx);
		ctx = NULL;
	}
	return !!result;
}


BOOL GetBaseBoardByCmd(char *lpszBaseBoard, int len/*=128*/)
{
	const long MAX_COMMAND_SIZE = 10000; // ��������������С	
	CHAR szFetCmd[] = "wmic BaseBoard get SerialNumber"; // ��ȡ�������к�������	
	const string strEnSearch = "SerialNumber"; // �������кŵ�ǰ����Ϣ

	BOOL   bret = FALSE;
	HANDLE hReadPipe = NULL; //��ȡ�ܵ�
	HANDLE hWritePipe = NULL; //д��ܵ�	
	PROCESS_INFORMATION pi;   //������Ϣ	
	STARTUPINFOA			si;	  //���������д�����Ϣ
	SECURITY_ATTRIBUTES sa;   //��ȫ����

	char			szBuffer[MAX_COMMAND_SIZE + 1] = { 0 }; // ���������н�������������
	string			strBuffer;
	unsigned long	count = 0;
	long			ipos = 0;

	memset(&pi, 0, sizeof(pi));
	memset(&si, 0, sizeof(si));
	memset(&sa, 0, sizeof(sa));

	pi.hProcess = NULL;
	pi.hThread = NULL;
	si.cb = sizeof(STARTUPINFOA);
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	//1.0 �����ܵ�
	bret = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
	if (!bret)
	{
		goto END;
	}

	//2.0 ���������д��ڵ���ϢΪָ���Ķ�д�ܵ�
	//GetStartupInfoA(&si);
	si.hStdError = hWritePipe;
	si.hStdOutput = hWritePipe;
	si.wShowWindow = SW_HIDE; //���������д���
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	//3.0 ������ȡ�����еĽ���
	bret = CreateProcessA(NULL, szFetCmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
	if (!bret)
	{
		goto END;
	}

	//4.0 ��ȡ���ص�����
	WaitForSingleObject(pi.hProcess, 500/*INFINITE*/);
	bret = ReadFile(hReadPipe, szBuffer, MAX_COMMAND_SIZE, &count, 0);
	if (!bret)
	{
		goto END;
	}

	//5.0 �����������к�
	bret = FALSE;
	strBuffer = szBuffer;
	ipos = strBuffer.find(strEnSearch);

	if (ipos < 0) // û���ҵ�
	{
		goto END;
	}
	else
	{
		strBuffer = strBuffer.substr(ipos + strEnSearch.length());
	}

	memset(szBuffer, 0x00, sizeof(szBuffer));
	strcpy_s(szBuffer, strBuffer.c_str());

	//ȥ���м�Ŀո� \r \n
	int j = 0;
	for (int i = 0; i < strlen(szBuffer); i++)
	{
		if (szBuffer[i] != ' ' && szBuffer[i] != '\n' && szBuffer[i] != '\r')
		{
			lpszBaseBoard[j] = szBuffer[i];
			j++;
		}
	}

	lpszBaseBoard[j] = '\0';

	bret = TRUE;

END:
	//�ر����еľ��
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return(bret);
}


string Format(const char* format, ...)
{
	va_list args;
	va_start(args, format);
#ifndef _MSC_VER
	size_t size = std::snprintf(nullptr, 0, format, args) + 1; // Extra space for '\0'
	std::unique_ptr<char[]> buf(new char[size]);
	std::vsnprintf(buf.get(), size, format, args);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
#else
	int size = _vscprintf(format, args);
	std::string result(++size, 0);
	vsnprintf_s((char*)result.data(), size, _TRUNCATE, format, args);
	return result;
#endif
	va_end(args);
}


string GetCPUID()
{
	string CPUID;
	unsigned long s1, s2;
	unsigned char vendor_id[] = "------------";
	char sel;
	sel = '1';
	string VernderID;
	string MyCpuID, CPUID1, CPUID2;
	INT32 dwBuf[4];
	__cpuidex(dwBuf, 1, 1);
	CPUID1 = Format("%08X%08X", dwBuf[3], dwBuf[0]);

	MyCpuID = CPUID1;//+CPUID2;
	CPUID = MyCpuID;
	return CPUID;
}

//--------------------------------------------------------------
//						MACADDR ��ȡ����ʱΪ None
//--------------------------------------------------------------
unsigned __stdcall GetMacAddrByCmd(void *pBaseBoard)
{
	const long MAX_COMMAND_SIZE = 1000; // ��������������С	
	CHAR szFetCmd[] = "getmac /NH"; // mac	
	//CHAR szFetCmd[] = "notepad.exe"; // mac	

	char* lpszBaseBoard = (char*)pBaseBoard;

	BOOL   bret = FALSE;
	HANDLE hReadPipe = NULL; //��ȡ�ܵ�
	HANDLE hWritePipe = NULL; //д��ܵ�	
	PROCESS_INFORMATION pi;   //������Ϣ	
	STARTUPINFOA			si;	  //���������д�����Ϣ
	SECURITY_ATTRIBUTES sa;   //��ȫ����

	char			szBuffer[MAX_COMMAND_SIZE + 1] = { 0 }; // ���������н�������������
	char*			ptr = szBuffer;
	string			strBuffer;
	unsigned long	count = 0;
	long			ipos = 0;

	memset(&pi, 0, sizeof(pi));
	memset(&si, 0, sizeof(si));
	memset(&sa, 0, sizeof(sa));

	pi.hProcess = NULL;
	pi.hThread = NULL;
	si.cb = sizeof(STARTUPINFOA);
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	//1.0 �����ܵ�
	bret = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
	if (!bret)
	{
		goto END;
	}


	//2.0 ���������д��ڵ���ϢΪָ���Ķ�д�ܵ�
	//GetStartupInfoA(&si);
	si.hStdError = hWritePipe;
	si.hStdOutput = hWritePipe;
	si.wShowWindow = SW_HIDE; //���������д���
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	//3.0 ������ȡ�����еĽ���
	bret = CreateProcessA(NULL, szFetCmd, NULL, NULL, TRUE, 
		CREATE_NEW_CONSOLE | CREATE_BREAKAWAY_FROM_JOB | CREATE_DEFAULT_ERROR_MODE | CREATE_NEW_PROCESS_GROUP,
		NULL, NULL, &si, &pi);
	if (!bret)
	{
		goto END;
	}

	//4.0 ��ȡ���ص�����
	WaitForSingleObject(pi.hProcess, INFINITE);


	bret = ReadFile(hReadPipe, szBuffer, MAX_COMMAND_SIZE, &count, 0);
	if (!bret)
	{
		goto END;
	}

	//5.0 �������к�
	bret = FALSE;
	strBuffer = szBuffer;
	ipos = strBuffer.find("\r\n");
	if (ipos < 0) // û���ҵ�
	{
		goto END;
	}
	else
	{
		strBuffer = strBuffer.substr(ipos + 2);
		ptr = szBuffer + ipos + 2;
	}
	//ipos = strBuffer.find("\r\n");
	//if (ipos < 0) // û���ҵ�
	//{		
	//	goto END;
	//}
	//else
	//{
	//	strBuffer = strBuffer.substr(ipos+2);
	//	ptr= szBuffer + ipos + 2;
	//}
	ipos = strBuffer.find(" ");
	strBuffer = strBuffer.substr(0, ipos);


	memset(szBuffer, 0x00, sizeof(szBuffer));
	strcpy_s(szBuffer, strBuffer.c_str());
	int j = 0;
	for (int i = 0; i < strlen(szBuffer); i++)
	{
		if (szBuffer[i] != ' ' && szBuffer[i] != '\n' && szBuffer[i] != '\r' && szBuffer[i] != '-')
		{
			lpszBaseBoard[j] = szBuffer[i];
			j++;
		}
	}

	lpszBaseBoard[j] = '\0';


	bret = TRUE;

END:
	//�ر����еľ��
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	//_endthreadex(0);

	return(bret);
}

void GetMacAddress(string& arrAddress){

	char addrs[1000];
	unsigned int threadID;
	HANDLE	hThread = (HANDLE)_beginthreadex(NULL, 0, &GetMacAddrByCmd, addrs, 0, &threadID);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	arrAddress = addrs;
}

string sGetLinInfo(){
	string strResult = "";

	string arrMac;
	GetMacAddress(arrMac);
	strResult = "MacAddress = "; strResult += arrMac; strResult += "\r\n";

	char lpszBaseBoard[128];
	if (GetBaseBoardByCmd(lpszBaseBoard, 128)){
	}
	strResult += "MotherboardSN = ";  strResult += lpszBaseBoard;  strResult += "\r\n";
	strResult += "CPUSerial = "; strResult += GetCPUID(); strResult += "\r\n";
	return strResult;
}

int GetLinInfo(char* info)
{
	string rea = sGetLinInfo();
	const char* rec = rea.c_str();
	int len = strlen(rec);
	for (int i = 0; i < len; i++)
	{
		info[i] = rec[i];
	}
	info[len] = '\0';
	return true;
}


int readLic(const char* licFileName) {//, const char* datFile
	bool ret = 0; time_t expired = 0;
	string str = licFileName; 	str += "lic/pubkey.pem";
	string strsa = getRSAKey(str);
	str = licFileName; 	str+= "lic/Dms.lic";
	ifstream infile(str, ios_base::in | ios_base::binary);
	infile.seekg(0, ios_base::end);
	int len = infile.tellg();
	infile.seekg(ios_base::beg);
	char * buffer = new char[len];
	infile.read(buffer, len);
	string out;
	out = rsa_pub_split128_decrypt(string(buffer, len), strsa);
	size_t pos = out.find("\n\r\n\r");
	string firstEncoderData(out.begin(), out.begin() + pos);
	string secondSignData(out.begin() + pos + 4, out.end());
	ret = verify((unsigned char*)firstEncoderData.c_str(), firstEncoderData.size(), (unsigned char*)secondSignData.c_str(), secondSignData.size(), (unsigned char*)strsa.c_str());
	if (!ret) return ret;
	printf("The licFile path is: %s\n", str.c_str());
	out = rsa_pub_split128_decrypt(firstEncoderData, strsa);
	istringstream ss; ss.str(out);  string minfo = sGetLinInfo();
	BOOL bMacOK = FALSE; BOOL bBoardOK = FALSE; BOOL bCPUOK = FALSE; BOOL hasMAC = FALSE; 
	BOOL hasBoard = FALSE; BOOL hasCPU = FALSE;
	for (string line; getline(ss, line);){
		for_each(line.begin(), line.begin() + 9, [](char & c){ c = ::toupper(c); });
		string label = string(line.begin(), line.begin() + 9); auto itr = line.begin(); char cc[32] = { 0 };
		if (label == "LICENSEID"){
			// DMS license 1, das license 2 
			itr += 10;
			continue;
		}
		else if (label == "LICENSETY"){
			//֤������;
			itr += 12;
			L_Staticstaial_type = atoi(&*itr);
			continue;
		}
		else if (label == "PRINTCLIE")
		{
			//���������ͷ�����
			itr += 17;
			L_Staticstaial_count = atoi(&*itr); continue;
		}
		else if (label == "USERCOUNT"){
			itr += 10; L_Staticstaial_users = atoi(&*itr); continue;
		}
		else if (label == "EXPIREDAY")
		{
			//��������
			itr += 10; cc[0] = 0x40;
			time_t rawtime;
			struct tm timeinfo{ 0 };
			char buffer[80];
			time(&rawtime);
			sscanf(&*itr, "%d-%d-%d", &timeinfo.tm_year, &timeinfo.tm_mon, &timeinfo.tm_mday);
			timeinfo.tm_hour = 0; timeinfo.tm_min = 0; timeinfo.tm_sec = 0;
			timeinfo.tm_year -= 1900; timeinfo.tm_mon -= 1;
			expired = mktime(&timeinfo);
			expired = (expired - rawtime) / (60 * 60 * 24);
			if (L_Staticstaial_type == 2) L_Staticstical_expried = 2;
			else L_Staticstical_expried = expired+9;
		}
		else if (label == "LICENSENA")
		{
			itr += 12;   L_Staticstaial_Name = &*itr; continue;
		}
		else if (label == "MACADDRES")
		{
			itr += 13;
			if (line.end() - itr == 0) continue;
			char* pos = strstr(&*itr, ";");
			if (!pos) {
				pos = strstr(&*itr, ",");
			}
			if (!pos) pos = &(*line.rbegin());
			string mac(&*itr, pos);
			ret = (string::npos != minfo.find(mac));
			if (ret) bMacOK = TRUE;
			hasMAC = TRUE;
			continue;
		}
		else if (label == "CPUSERIAL")
		{
			itr += 13;
			if (line.end() - itr == 0) continue;
			char* pos = strstr(&*itr, ";");
			if (!pos) {
				pos = strstr(&*itr, ",");
			}
			if (!pos) pos = &(*line.rbegin());
			string sscpu(&*itr, pos);
			ret = (string::npos != minfo.find(sscpu));
			if (ret) bCPUOK = TRUE;
			hasCPU = TRUE;
			continue;
		}
		else if (label == "MOTHERBOA")
		{
			itr += 17;   
			if(line.end() - itr == 0) continue;
			char* pos = strstr(&*itr, ";");
			if (!pos) {
				pos = strstr(&*itr, ",");
			}
			if (!pos) pos = &(*line.rbegin());
			string ssbd(&*itr, pos);
			ret = (string::npos != minfo.find(ssbd));
			if (ret) bBoardOK = TRUE;
			hasBoard = TRUE;
			continue;
		}
	}
	bBoardOK = hasBoard & bBoardOK | !hasBoard;
	bCPUOK = hasCPU & bCPUOK | !hasCPU;
	bMacOK = hasMAC & bMacOK | !hasMAC;
	ret = bMacOK & bCPUOK & bBoardOK;
	if (!ret){
		L_Staticstaial_count = 0;
		L_Staticstaial_users = 0;
		L_Staticstical_expried = 0;
	}
	printf("return value is %d and the expried is %d\n", L_Staticstaial_count, L_Staticstical_expried);
	return L_Staticstaial_count;
}

int check() {
	if (L_Staticstaial_type == 2) return L_Staticstaial_type;
	else return L_Staticstical_expried;
}

int checked(int nResults[]) {
	nResults[0] = L_Staticstaial_users;
	nResults[1] = L_Staticstaial_count;
	nResults[2] = L_Staticstical_expried;
	return L_Staticstical_expried;
}

