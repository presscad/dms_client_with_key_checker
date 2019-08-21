#pragma once

// ���ݿ�ר��
#pragma warning (disable: 4146)
#import "c:\Program Files\Common Files\System\ADO\msado15.dll" rename("EOF", "AdoEOF")


class __declspec(dllexport) CUniverDB  
{
// ���졢��������
public:
	CUniverDB();
	~CUniverDB();

	ADODB::_ConnectionPtr m_pConnect;  // ADO����ָ��
	ADODB::_RecordsetPtr m_pRecordset; // ADO��¼��ָ��


	CString m_strSQL;       // �򿪼�¼����SQL���
	CString m_strLastErr;   // ��һ�β������صĴ�����Ϣ

	// ���ݿ����
	CString m_strCnnParam;  

	CStringArray m_arrTblName; // �������б�

	// ��ô�����Ϣ����ɾ���쳣ָ��
	void GetError(CException* e);

	// ԭʼȡ����
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


	// ԭʼ��������
	BOOL SetFieldValue(short shtFld, _variant_t vtValue);
	BOOL SetFieldValue(LPCSTR strFld, _variant_t vtValue);

	// ��⵱ǰ���ݿ��Ƿ��Ѿ�����
	BOOL IsConnect();
	// ���ӵ�ǰ���ݿ�
	BOOL Connect(LPCSTR strCnnParam);
	// ������ǰ���ݿ�����
	void Disconnect();

	// ��⵱ǰ��¼���Ƿ��
	BOOL IsOpen();
	// ͨ�� SQL ���򿪼�¼��
	//     pSrcDB != NULL ��ʾ����Դ�����½��������ļ���֧��
	BOOL OpenSQL(LPCSTR strSql, CUniverDB* pSrcDB = NULL);
	// ͨ�� SQL ��䴴�����ݱ��򿪼�¼��
	BOOL OpenSQL(LPCSTR strSql, CStringArray* parrSrcFld);
	// �رռ�¼��
	void Close();
	// ��ü�¼��������
	long GetRows();
	// ��ü�¼��������

	int GetTblCount();
	CString GetTblName(int);
	int SetPos(long);
	long GetPos(void);



	// ��⡢�ƶ������ü�¼λ��
	BOOL IsEOF();
	BOOL IsBOF();

	BOOL MoveFirst();
	BOOL MoveNext();
	BOOL MovePrev();
	BOOL MoveLast();

	// ȡ����
	CString GetString(short shtCol);
	CString GetString(LPCSTR strComCol);


	long GetLong(short shtCol);
	long GetLong(LPCSTR strComCol);

	COleDateTime GetDateTime(short shtCol);
	COleDateTime GetDateTime(LPCSTR strComCol);

	// ��������
	BOOL SetString(short shtCol, LPCSTR lpValue);
	BOOL SetString(LPCSTR strComCol, LPCSTR lpValue);

	BOOL SetLong(short shtCol, long lValue);
	BOOL SetLong(LPCSTR strComCol, long lValue);


	BOOL SetDateTime(short shtCol, COleDateTime timeValue);
	BOOL SetDateTime(LPCSTR strComCol, COleDateTime timeValue);


	// ��¼���༭
	BOOL AddNew();
	BOOL Edit();
	BOOL Delete();
	BOOL Update(BOOL bUpdated = FALSE);


	// ���зǲ�ѯSQL���
	BOOL Execute(CString strSql,BOOL isResult);


};
