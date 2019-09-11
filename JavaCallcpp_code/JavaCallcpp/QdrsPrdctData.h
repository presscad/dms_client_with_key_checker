#pragma once
#include <string>

using namespace std;

class CQdrsPrdctData
{
public:
	CQdrsPrdctData();
	~CQdrsPrdctData();

private:
	string m_projectNum;		// ��Ŀ���
	string m_batchNum;		// ��������
	double m_dataValue;		// ����ֵ
	string m_type;		// ���ͣ�1��ʼ���ݣ�2�������ݣ�
	string m_grade;		// ���յȼ�SLMH
	string m_result;		// ���,�����������������޵�
	double m_dValue;		// dֵ
	double m_removeD;		// �޳���dֵ
	int m_datasort;		// ����
	string m_factoryNum;		// ԭ������
	string m_remark;		// ��ע
	int m_countNum;			//������
	int m_normalNum;			//��������
	int m_anomalyNum;			//�쳣����
	string m_ratio;			//ռ��
	string m_proName;			//��Ŀ����

	string m_objId;

	bool m_bResult;
	double  m_adValue;

	double m_dMax; 			//���dֵ
	double m_dMin;			//��Сdֵ

	string m_state;

	int m_myId;


public :


	string getState();

	void setState(string state);

	string getObjId();

	void setObjId(string objId);


	int getNormalNum();

	void setNormalNum(int normalNum);

	int getAnomalyNum();

	void setAnomalyNum(int anomalyNum);

	string getRatio();

	void setRatio(string ratio);

	int getCountNum();

	void setCountNum(int countNum);

	string getProName();

	void setProName(string proName);

	string getProjectNum();

	void setProjectNum(string projectNum);

	//@Length(min = 0, max = 64, message = "���γ��ȱ������ 0 �� 64 ֮��")
	string getBatchNum();

	void setBatchNum(string batchNum);

	double getDataValue();

	void setDataValue(double dataValue);

	//@Length(min = 0, max = 1, message = "���ͣ���ʼ���ݣ��������ݣ����ȱ������ 0 �� 1 ֮��")
	string getType();

	void setType(string type);

	//@Length(min = 0, max = 5, message = "���յȼ�SLMH���ȱ������ 0 �� 5 ֮��")
	string getGrade();

	void setGrade(string grade);

	//@Length(min = 0, max = 64, message = "���,�����������������޵ȳ��ȱ������ 0 �� 1 ֮��")
	string getResult();

	void setResult(string result);

	double getDValue();

	void setDValue(double dValue);

	double getRemoveD();

	void setRemoveD(double removeD);

	//@Length(min = 0, max = 11, message = "���򳤶ȱ������ 0 �� 11 ֮��")
	int getDatasort();

	void setDatasort(int datasort);

	//@Length(min = 0, max = 64, message = "ԭ�����ų��ȱ������ 0 �� 64 ֮��")
	string getFactoryNum();

	void setFactoryNum(string factoryNum);


	string getRemark();

	void setRemark(string remark);

	double getdMax();

	void setdMax(double dMax);

	double getdMin();

	void setdMin(double dMin);



	bool isbResult();

	void setbResult(bool bResult);

	double getAdValue();

	void setAdValue(double adValue);

	string toString();

	string toDataString();

	void setMyId(int myId){
		m_myId = myId;
	}

	int getMyId(){
		return m_myId;
	}
};

