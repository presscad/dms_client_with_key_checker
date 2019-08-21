#pragma once

// 数据库专用
#pragma warning (disable: 4146)
#import "c:\Program Files\Common Files\System\ADO\msado15.dll" rename("EOF", "AdoEOF")


class __declspec(dllexport) CUniverDB  
{
// 构造、析构函数
public:
	CUniverDB();
	~CUniverDB();

	ADODB::_ConnectionPtr m_pConnect;  // ADO连接指针
	ADODB::_RecordsetPtr m_pRecordset; // ADO记录集指针


	CString m_strSQL;       // 打开记录集的SQL语句
	CString m_strLastErr;   // 上一次操作返回的错误信息

	// 数据库参数
	CString m_strCnnParam;  

	CStringArray m_arrTblName; // 表名称列表

	// 获得错误信息，并删除异常指针
	void GetError(CException* e);

	// 原始取数据
	BOOL GetFieldValue(short shtFld, _variant_t& vtValue);
	BOOL GetFieldValue(LPCSTR strFld, _variant_t& vtValue);

	BOOL GetFieldValue(short shtFld, CString& strValue);
	BOOL GetFieldValue(LPCSTR strFld, CString& strValue);

	BOOL GetFieldValue(short shtFld, unsigned long& ulValue);
	BOOL GetFieldValue(LPCSTR strFld, unsigned long& ulValue);
	BOOL GetFieldValue(short shtFld, long& lValue);
	BOOL GetFieldValue(LPCSTR strFld, long& lValue);

	BOOL GetFieldValue(short shtFld, COleDateTime& timeValue);
	BOOL GetFieldValue(LPCSTR strFld, COleDateTime& timeValue);


	// 原始设置数据
	BOOL SetFieldValue(short shtFld, _variant_t vtValue);
	BOOL SetFieldValue(LPCSTR strFld, _variant_t vtValue);

	// 检测当前数据库是否已经连接
	BOOL IsConnect();
	// 连接当前数据库
	BOOL Connect(LPCSTR strCnnParam);
	// 撤消当前数据库连接
	void Disconnect();

	// 检测当前记录集是否打开
	BOOL IsOpen();
	// 通过 SQL 语句打开记录集
	//     pSrcDB != NULL 表示依据源表重新建表，仅仅文件库支持
	BOOL OpenSQL(LPCSTR strSql, CUniverDB* pSrcDB = NULL);
	// 通过 SQL 语句创建数据表并打开记录集
	BOOL OpenSQL(LPCSTR strSql, CStringArray* parrSrcFld);
	// 关闭记录集
	void Close();
	// 获得记录集的行数
	long GetRows();
	// 获得记录集的列数

	int GetTblCount();
	CString GetTblName(int);
	int SetPos(long);
	long GetPos(void);



	// 检测、移动、设置记录位置
	BOOL IsEOF();
	BOOL IsBOF();

	BOOL MoveFirst();
	BOOL MoveNext();
	BOOL MovePrev();
	BOOL MoveLast();

	// 取数据
	CString GetString(short shtCol);
	CString GetString(LPCSTR strComCol);


	long GetLong(short shtCol);
	long GetLong(LPCSTR strComCol);

	COleDateTime GetDateTime(short shtCol);
	COleDateTime GetDateTime(LPCSTR strComCol);

	// 设置数据
	BOOL SetString(short shtCol, LPCSTR lpValue);
	BOOL SetString(LPCSTR strComCol, LPCSTR lpValue);

	BOOL SetLong(short shtCol, long lValue);
	BOOL SetLong(LPCSTR strComCol, long lValue);


	BOOL SetDateTime(short shtCol, COleDateTime timeValue);
	BOOL SetDateTime(LPCSTR strComCol, COleDateTime timeValue);


	// 记录集编辑
	BOOL AddNew();
	BOOL Edit();
	BOOL Delete();
	BOOL Update(BOOL bUpdated = FALSE);


	// 运行非查询SQL语句
	BOOL Execute(CString strSql,BOOL isResult);


};
