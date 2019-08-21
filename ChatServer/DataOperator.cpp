#include "stdafx.h"
#include "DataOperator.h"


CDataOperator::CDataOperator()
{
	try
	{
		//创建连接对象
		HRESULT hr = m_pConnection.CreateInstance(_T("ADODB.Connection"));
		if (FAILED(hr))
		{
			m_pConnection = NULL;
		}
	}
	catch (_com_error &e)
	{
		e.ErrorMessage();
	}
}

CDataOperator::~CDataOperator()
{
	if (m_pConnection)
	{
		try
		{
			//将连接对象关闭
			HRESULT hr = m_pConnection->Close();
		}
		catch (_com_error &e)
		{
			e.ErrorMessage();
		}
		//释放连接对象
		m_pConnection.Release();
		m_pConnection = NULL;
	}
}

//打开数据库的操作
BOOL CDataOperator::OpenDatabase(CString strDbName, CString strUserName, CString strUserPwd)
{
	if (NULL == m_pConnection)
	{
		return FALSE;
	}
	if (m_pConnection)
	{
		try
		{//DATABASE=%s;DSN=myodbc;OPTION=0;PWD=%s;PORT=3306;SERVER=localhost;UID=%s;
			CString strConnectionName;//DSN=dms;SERVER=localhost;UID=root;PWD=123456;DATABASE=dms;PORT=3306
			strConnectionName.Format(_T("Provider=MSDASQL.1;Persist Security Info=True;DSN=dms;SERVER=localhost;UID=root;PWD=123456;DATABASE=dms;PORT=3306;"),
				strUserPwd,strDbName, strUserName,strDbName );
			HRESULT hr = m_pConnection->Open(_bstr_t(strConnectionName.GetBuffer(0)),
				_T(""), _T(""), -1);
			if (FAILED(hr))
			{
				m_pConnection = NULL;
			}
		}
		catch (_com_error &e)
		{
			e.ErrorInfo();
			return FALSE;
		}
	}
	return true;
}

BOOL CDataOperator::ExecuteSQL(CString sql)  
{  
	if (NULL == m_pConnection)  
	{  
		return FALSE;  
	}  
	if (m_pConnection)  
	{  
		try  
		{  
			HRESULT hr = m_pConnection->Execute(_bstr_t(sql), NULL, 1);  
			if (FAILED(hr))  
			{  
				m_pConnection = NULL;  
			}  
		}  
		catch (_com_error &e)  
		{  
			e.ErrorMessage();  
			return FALSE;  
		}  
	}  
	return true;  
}

BOOL CDataOperator::SelectFromFileLog(vector<OBJ_FILE_LOG> &vecObjFileLog)
{
	if (NULL == m_pConnection)
		return FALSE;
	//记录集对象
	_RecordsetPtr m_pRecordSet;
	HRESULT hr = m_pRecordSet.CreateInstance(_T("ADODB.Recordset"));
	if (FAILED(hr))
	{
		return FALSE;
	}
	//获取数据前先清空
	vecObjFileLog.clear();
	CString strSQL = _T("select * from dms_file_log");
	hr = m_pRecordSet->Open(_bstr_t(strSQL),m_pConnection.GetInterfacePtr(),
		adOpenStatic, adLockOptimistic, adCmdText);
	if (FAILED(hr))
	{
		m_pRecordSet.Release();
		return FALSE;
	}
	//获取当前游标的位置
	VARIANT_BOOL bRet = m_pRecordSet->GetadoEOF();
	//如果游标在末尾返回失败
	//遍历数据
	while(!bRet)
	{
		OBJ_FILE_LOG objFileLog;
		//_variant_t varFileName = m_pRecordSet->GetCollect("file_name");
		//objFileLog.fileName = (char*)(_bstr_t)varFileName;
		_variant_t varIp = m_pRecordSet->GetCollect("ip");
		objFileLog.ip = (char*)(_bstr_t)varIp;
		_variant_t varFilePath = m_pRecordSet->GetCollect("file_path");
		objFileLog.filePath = (char*)(_bstr_t)varFilePath;
		//_variant_t varHandler = m_pRecordSet->GetCollect("handler");
		//objFileLog.handler = (char*)(_bstr_t)varHandler;

		_variant_t varDescribe1 = m_pRecordSet->GetCollect("describe1");
		objFileLog.describe1 = (char*)(_bstr_t)varDescribe1;
		vecObjFileLog.push_back(objFileLog);
		//游标下移
		m_pRecordSet->MoveNext();
		bRet = m_pRecordSet->GetadoEOF();
	}
	m_pRecordSet->Close();
	m_pRecordSet.Release();
	m_pRecordSet = NULL;
	return true;
}

BOOL CDataOperator::Select_From_User(vector<OBJ_USER> &vecObjUser)
{
	if (NULL == m_pConnection)
		return FALSE;
	//记录集对象
	_RecordsetPtr m_pRecordSet;
	HRESULT hr = m_pRecordSet.CreateInstance(_T("ADODB.Recordset"));
	if (FAILED(hr))
	{
		return FALSE;
	}
	//获取数据前先清空
	vecObjUser.clear();
	CString strSQL = _T("select User_ID, User_Name from user");
	hr = m_pRecordSet->Open(_bstr_t(strSQL),m_pConnection.GetInterfacePtr(),
		adOpenStatic, adLockOptimistic, adCmdText);
	if (FAILED(hr))
	{
		m_pRecordSet.Release();
		return FALSE;
	}
	//获取当前游标的位置
	VARIANT_BOOL bRet = m_pRecordSet->GetadoEOF();
	//如果游标在末尾返回失败
	//遍历数据
	while(!bRet)
	{
		_variant_t varUserID = m_pRecordSet->GetCollect("User_ID");
		_variant_t varUserName = m_pRecordSet->GetCollect("User_Name");
		OBJ_USER objUser;
		objUser.User_ID = varUserID.intVal;
		_tcscpy_s(objUser.User_Name, (TCHAR*)(_bstr_t)varUserName);
		vecObjUser.push_back(objUser);
		//游标下移
		m_pRecordSet->MoveNext();
		bRet = m_pRecordSet->GetadoEOF();
	}
	m_pRecordSet->Close();
	m_pRecordSet.Release();
	m_pRecordSet = NULL;
	return true;
}

BOOL CDataOperator::Insert_Into_User(OBJ_USER &objUser)
{
	if (NULL == m_pConnection)
		return FALSE;
	//记录集对象
	_RecordsetPtr m_pRecordSet;
	HRESULT hr = m_pRecordSet.CreateInstance(_T("ADODB.Recordset"));
	if (FAILED(hr))
	{
		return FALSE;
	}
	CString strSQL = _T("select User_ID, User_Name from user");
	hr = m_pRecordSet->Open(_bstr_t(strSQL), m_pConnection.GetInterfacePtr(),
		adOpenStatic, adLockOptimistic, adCmdText);
	if (FAILED(hr))
	{
		m_pRecordSet.Release();
		return FALSE;
	}

	try
	{
		//增加一行
		m_pRecordSet->AddNew();
	}
	catch (_com_error &e)
	{
		e.ErrorMessage();
		return FALSE;
	}
	try
	{
		m_pRecordSet->PutCollect(_T("User_ID"), _variant_t(objUser.User_ID));
		m_pRecordSet->PutCollect(_T("User_Name"), _variant_t(objUser.User_Name));
	}
	catch (_com_error &e)
	{
		m_pRecordSet->Close();
		m_pRecordSet.Release();
		e.ErrorMessage();
		return FALSE;
	}
	m_pRecordSet->Update();
	m_pRecordSet->Close();
	m_pRecordSet.Release();
	m_pRecordSet = NULL;
	return TRUE;
}

BOOL CDataOperator::Update_For_User(OBJ_USER &objUser)
{
	if (NULL == m_pConnection)
		return FALSE;
	//记录集对象
	_RecordsetPtr m_pRecordSet;
	HRESULT hr = m_pRecordSet.CreateInstance(_T("ADODB.Recordset"));
	if (FAILED(hr))
	{
		return FALSE;
	}
	CString strSQL;
	strSQL.Format(_T("select User_ID, User_Name from user where User_ID=%d"), objUser.User_ID);
	hr = m_pRecordSet->Open(_bstr_t(strSQL), m_pConnection.GetInterfacePtr(),
		adOpenStatic, adLockOptimistic, adCmdText);
	if (FAILED(hr))
	{
		m_pRecordSet.Release();
		return FALSE;
	}
	try
	{
		m_pRecordSet->PutCollect(_T("User_Name"), _variant_t(objUser.User_Name));
	}
	catch (_com_error &e)
	{
		m_pRecordSet->Close();
		m_pRecordSet.Release();
		e.ErrorMessage();
		return FALSE;
	}
	m_pRecordSet->Update();
	m_pRecordSet->Close();
	m_pRecordSet.Release();
	m_pRecordSet = NULL;
	return TRUE;
}