#include "stdafx.h"
#include "QdrsPrdctData.h"
#include <sstream>
#include <iostream>

using namespace std;


CQdrsPrdctData::CQdrsPrdctData()
{
	m_projectNum = "";		// ��Ŀ���
	m_batchNum = "";		// ��������
	m_dataValue = 0.0;		// ����ֵ
	m_type = "";		// ���ͣ�1��ʼ���ݣ�2�������ݣ�
	m_grade = "";		// ���յȼ�SLMH
	m_result = "";		// ���,�����������������޵�
	m_dValue = 0.0;		// dֵ
	m_removeD = 0.0;		// �޳���dֵ
	m_datasort = 0;		// ����
	m_factoryNum = "";		// ԭ������
	m_remark = "";		// ��ע
	m_countNum = 0;			//������
	m_normalNum = 0;			//��������
	m_anomalyNum = 0;			//�쳣����
	m_ratio = "";			//ռ��
	m_proName = "";			//��Ŀ����
	m_objId = "";

	m_bResult = false;
	m_adValue = 0.0;

	m_dMax = 0.0; 			//���dֵ
	m_dMin = 0.0;			//��Сdֵ

	string m_state = "";

	int m_myId = 0;
}


CQdrsPrdctData::~CQdrsPrdctData()
{
}


string CQdrsPrdctData::getState() {
	return m_state;
}

void CQdrsPrdctData::setState(string state) {
	m_state = state;
}

string CQdrsPrdctData::getObjId() {
	return m_objId;
}

void CQdrsPrdctData::setObjId(string objId) {
	m_objId = objId;
}


int CQdrsPrdctData::getNormalNum() {
	return m_normalNum;
}

void CQdrsPrdctData::setNormalNum(int normalNum) {
	m_normalNum = normalNum;
}

int CQdrsPrdctData::getAnomalyNum() {
	return m_anomalyNum;
}

void CQdrsPrdctData::setAnomalyNum(int anomalyNum) {
	m_anomalyNum = anomalyNum;
}

string CQdrsPrdctData::getRatio() {
	return m_ratio;
}

void CQdrsPrdctData::setRatio(string ratio) {
	m_ratio = ratio;
}

int CQdrsPrdctData::getCountNum() {
	return m_countNum;
}

void CQdrsPrdctData::setCountNum(int countNum) {
	m_countNum = countNum;
}

string CQdrsPrdctData::getProName() {
	return m_proName;
}

void CQdrsPrdctData::setProName(string proName) {
	m_proName = proName;
}


//@Length(min = 0, max = 64, message = "��Ŀ��ų��ȱ������ 0 �� 64 ֮��")
string CQdrsPrdctData::getProjectNum() {
	return m_projectNum;
}

void CQdrsPrdctData::setProjectNum(string projectNum) {
	m_projectNum = projectNum;
}

//@Length(min = 0, max = 64, message = "���γ��ȱ������ 0 �� 64 ֮��")
string CQdrsPrdctData::getBatchNum() {
	return m_batchNum;
}

void CQdrsPrdctData::setBatchNum(string batchNum) {
	m_batchNum = batchNum;
}

double CQdrsPrdctData::getDataValue() {
	return m_dataValue;
}

void CQdrsPrdctData::setDataValue(double dataValue) {
	m_dataValue = dataValue;
}

//@Length(min = 0, max = 1, message = "���ͣ���ʼ���ݣ��������ݣ����ȱ������ 0 �� 1 ֮��")
string CQdrsPrdctData::getType() {
	return m_type;
}

void CQdrsPrdctData::setType(string type) {
	m_type = type;
}

//@Length(min = 0, max = 5, message = "���յȼ�SLMH���ȱ������ 0 �� 5 ֮��")
string CQdrsPrdctData::getGrade() {
	return m_grade;
}

void CQdrsPrdctData::setGrade(string grade) {
	m_grade = grade;
}

//@Length(min = 0, max = 64, message = "���,�����������������޵ȳ��ȱ������ 0 �� 1 ֮��")
string CQdrsPrdctData::getResult() {
	return m_result;
}

void CQdrsPrdctData::setResult(string result) {
	m_result = result;
}

double CQdrsPrdctData::getDValue() {
	return m_dValue;
}

void CQdrsPrdctData::setDValue(double dValue) {
	m_dValue = dValue;
}

double CQdrsPrdctData::getRemoveD() {
	return m_removeD;
}

void CQdrsPrdctData::setRemoveD(double removeD) {
	m_removeD = removeD;
}

//@Length(min = 0, max = 11, message = "���򳤶ȱ������ 0 �� 11 ֮��")
int CQdrsPrdctData::getDatasort() {
	return m_datasort;
}

void CQdrsPrdctData::setDatasort(int datasort) {
	m_datasort = datasort;
}

//@Length(min = 0, max = 64, message = "ԭ�����ų��ȱ������ 0 �� 64 ֮��")
string CQdrsPrdctData::getFactoryNum() {
	return m_factoryNum;
}

void CQdrsPrdctData::setFactoryNum(string factoryNum) {
	m_factoryNum = factoryNum;
}


//@Length(min = 0, max = 255, message = "��ע���ȱ������ 0 �� 255 ֮��")
string CQdrsPrdctData::getRemark() {
	return m_remark;
}

void CQdrsPrdctData::setRemark(string remark) {
	m_remark = remark;
}



double CQdrsPrdctData::getdMax() {
	return m_dMax;
}

void CQdrsPrdctData::setdMax(double dMax) {
	m_dMax = dMax;
}

double CQdrsPrdctData::getdMin() {
	return m_dMin;
}

void CQdrsPrdctData::setdMin(double dMin) {
	m_dMin = dMin;
}



bool CQdrsPrdctData::isbResult() {
	return m_bResult;
}

void CQdrsPrdctData::setbResult(bool bResult) {
	m_bResult = bResult;
}

double CQdrsPrdctData::getAdValue() {
	return m_adValue;
}

void CQdrsPrdctData::setAdValue(double adValue) {
	m_adValue = adValue;
}

string CQdrsPrdctData::toString() {
	//string sDate = "";
	//if (produDate != null)
	//	sDate = format.format(produDate);

	//string str;
	//string str = string.format("%s\t%.4f\t%s\t%.4f\t", m_batchNum, m_dataValue, sDate, m_dValue);
	//str += String.format("%b\t%s\t%s\t%.4f\t%d", bResult, result, grade, removeD, datasort);


	//string strResult;
	//double a = 35.000, b = 14.967;
	//std::ostringstream   ostr;
	//ostr << "a+b=" << a << "+" << b << "=" << a + b;
	//std::string str = ostr.str();
	//strResult += str;
	//\t%d\t%s\t%s\t%.4lf\t%d
	//, m_bResult, m_result, m_grade, m_removeD, m_datasor
//	TRACE("%d\t%.4lf\t%.4lf\t%d\t%s\t%s\t%.4lf\t%d\n", m_myId, m_dataValue, m_dValue, m_bResult, m_result.data(), m_grade.data(), m_removeD,m_datasort);

	return "";
}

string CQdrsPrdctData::toDataString() {
	//string sDate = "";
	//if (produDate != null)
	//	sDate = format.format(produDate);

	//string str = string.format("%s\t%s\t%.4f\t%s", batchNum, factoryNum, doubleValue, sDate);
	//return str;

///	TRACE("%s\t%s\t%.4f\n",m_batchNum, m_factoryNum, m_dataValue);
	return "";
}
