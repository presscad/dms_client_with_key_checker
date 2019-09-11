#pragma once
#include <string>

using namespace std;

class CQdrsPrdctData
{
public:
	CQdrsPrdctData();
	~CQdrsPrdctData();

private:
	string m_projectNum;		// 项目编号
	string m_batchNum;		// 进场批号
	double m_dataValue;		// 数据值
	string m_type;		// 类型（1初始数据，2计算数据）
	string m_grade;		// 风险等级SLMH
	string m_result;		// 结果,正常、超出法标下限等
	double m_dValue;		// d值
	double m_removeD;		// 剔除后d值
	int m_datasort;		// 排序
	string m_factoryNum;		// 原厂批号
	string m_remark;		// 备注
	int m_countNum;			//总批次
	int m_normalNum;			//正常批次
	int m_anomalyNum;			//异常批次
	string m_ratio;			//占比
	string m_proName;			//项目名称

	string m_objId;

	bool m_bResult;
	double  m_adValue;

	double m_dMax; 			//最大d值
	double m_dMin;			//最小d值

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

	//@Length(min = 0, max = 64, message = "批次长度必须介于 0 和 64 之间")
	string getBatchNum();

	void setBatchNum(string batchNum);

	double getDataValue();

	void setDataValue(double dataValue);

	//@Length(min = 0, max = 1, message = "类型（初始数据，测试数据）长度必须介于 0 和 1 之间")
	string getType();

	void setType(string type);

	//@Length(min = 0, max = 5, message = "风险等级SLMH长度必须介于 0 和 5 之间")
	string getGrade();

	void setGrade(string grade);

	//@Length(min = 0, max = 64, message = "结果,正常、超出法标下限等长度必须介于 0 和 1 之间")
	string getResult();

	void setResult(string result);

	double getDValue();

	void setDValue(double dValue);

	double getRemoveD();

	void setRemoveD(double removeD);

	//@Length(min = 0, max = 11, message = "排序长度必须介于 0 和 11 之间")
	int getDatasort();

	void setDatasort(int datasort);

	//@Length(min = 0, max = 64, message = "原厂批号长度必须介于 0 和 64 之间")
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

