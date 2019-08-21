#include "stdafx.h"
#include "UniverDB.h"
#include <math.h>
#include "SuperDb.h"
#include "Support.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CUniverDB::CUniverDB()
{
	m_pConnect   = NULL;

	m_pRecordset = NULL;

	m_strSQL.Empty();
	m_strLastErr.Empty();
	m_strCnnParam.Empty(); // 数据库参数

	m_arrTblName.RemoveAll();
}

CUniverDB::~CUniverDB()
{
	Disconnect();
}

// 获得错误信息，并删除异常指针
void CUniverDB::GetError(CException* e)
{
	char err[256];
	e->GetErrorMessage(err, sizeof(err));
	e->Delete();
	m_strLastErr = err;
	if (m_strLastErr.IsEmpty())
		m_strLastErr = "未知类型错误";
}

// 原始取数据
BOOL CUniverDB::GetFieldValue(short shtFld, _variant_t& vtValue)
{
	try
	{
		vtValue = m_pRecordset->Fields->Item[_variant_t(shtFld)]->GetValue();
	}
	catch (_com_error &e)
	{
		m_strLastErr = (char*)e.Description();
		return FALSE;
	}
	catch (CException* e)
	{
		GetError(e);
		return FALSE;
	}

	m_strLastErr.Empty();
	return TRUE;
}

BOOL CUniverDB::GetFieldValue(LPCSTR strFld, _variant_t& vtValue)
{
	try
	{
		vtValue = m_pRecordset->Fields->Item[_bstr_t(strFld)]->GetValue();
	}
	catch (_com_error &e)
	{
		m_strLastErr = (char*)e.Description();
		return FALSE;
	}
	catch (CException* e)
	{
		GetError(e);
		return FALSE;
	}

	m_strLastErr.Empty();
	return TRUE;
}

BOOL CUniverDB::GetFieldValue(short shtFld, CString& strValue)
{
	strValue.Empty();

	_variant_t vtVal;
	if (!GetFieldValue(shtFld, vtVal))
		return FALSE;

	if (vtVal.vt == VT_NULL || vtVal.vt == VT_EMPTY)
		return TRUE;

	try
	{
		// 变串转宽串，宽串转串
		_bstr_t bstr(vtVal);
		strValue = (char*)bstr;
	}
	catch (_com_error &e)
	{
		m_strLastErr = (char*)e.Description();
		return FALSE;
	}
	catch (CException* e)
	{
		GetError(e);
		return FALSE;
	}

	return TRUE;
}

BOOL CUniverDB::GetFieldValue(LPCSTR strFld, CString& strValue)
{
	strValue.Empty();

	_variant_t vtVal;
	if (!GetFieldValue(strFld, vtVal))
		return FALSE;

	if (vtVal.vt == VT_NULL || vtVal.vt == VT_EMPTY)
		return TRUE;

	try
	{
		// 变串转宽串，宽串转串
		_bstr_t bstr(vtVal);
		strValue = (char*)bstr;
	}
	catch (_com_error &e)
	{
		m_strLastErr = (char*)e.Description();
		return FALSE;
	}
	catch (CException* e)
	{
		GetError(e);
		return FALSE;
	}

	return TRUE;
}

BOOL CUniverDB::GetFieldValue(short shtFld, unsigned long& ulValue)
{
	ulValue = 0;

	_variant_t vtVal;
	if (!GetFieldValue(shtFld, vtVal))
		return FALSE;

	switch (vtVal.vt)
	{
	case VT_BSTR:
		{
			// 变串转宽串，宽串转串
			_bstr_t bstr(vtVal);
			CString strVal = (char*)bstr;

			strVal = sStdNumber(strVal);
			ulValue = (unsigned long)sToInteger(strVal);
		}
		break;
	case VT_DECIMAL:
		{
			double dblVal;
			dblVal = sInt64ToDouble(vtVal.decVal.Lo64);
			dblVal *= (vtVal.decVal.sign == 128)? -1 : 1;
			dblVal /= pow(10, (float)vtVal.decVal.scale);
			ulValue = (unsigned long)dblVal;
		}
		break;
	case VT_R8:
		ulValue = (unsigned long)vtVal.dblVal;
		break;
	case VT_R4:
		ulValue = (unsigned long)vtVal.fltVal;
		break;
	case VT_UI4:
		ulValue = vtVal.ulVal;
		break;
	case VT_I4:
		ulValue = (unsigned long)vtVal.lVal;
		break;
	case VT_I2:
	case VT_UI1:
		ulValue = (unsigned long)vtVal.iVal;
		break;
	case VT_BOOL:
		if (!vtVal.boolVal) ulValue = 0;
		else ulValue = 1;
		break;
	case VT_DATE:
		{ // 仅仅转化日期
			COleDateTime dtVal(vtVal);
			ulValue = dtVal.GetYear() * 10000 +
				dtVal.GetMonth() * 100 + dtVal.GetDay();
		}
		break;
	case VT_CY:
		ulValue = (unsigned long)vtVal.cyVal.Hi;
		break;
	}

	return TRUE;
}

BOOL CUniverDB::GetFieldValue(LPCSTR strFld, unsigned long& ulValue)
{
	ulValue = 0;

	_variant_t vtVal;
	if (!GetFieldValue(strFld, vtVal))
		return FALSE;

	switch (vtVal.vt)
	{
	case VT_BSTR:
		{
			// 变串转宽串，宽串转串
			_bstr_t bstr(vtVal);
			CString strVal = (char*)bstr;

			strVal = sStdNumber(strVal);
			ulValue = (unsigned long)sToInteger(strVal);
		}
		break;
	case VT_DECIMAL:
		{
			double dblVal;
			dblVal = sInt64ToDouble(vtVal.decVal.Lo64);
			dblVal *= (vtVal.decVal.sign == 128)? -1 : 1;
			dblVal /= pow(10, (float)vtVal.decVal.scale);
			ulValue = (unsigned long)dblVal;
		}
		break;
	case VT_R8:
		ulValue = (unsigned long)vtVal.dblVal;
		break;
	case VT_R4:
		ulValue = (unsigned long)vtVal.fltVal;
		break;
	case VT_UI4:
		ulValue = vtVal.ulVal;
		break;
	case VT_I4:
		ulValue = (unsigned long)vtVal.lVal;
		break;
	case VT_I2:
	case VT_UI1:
		ulValue = (unsigned long)vtVal.iVal;
		break;
	case VT_BOOL:
		if (!vtVal.boolVal) ulValue = 0;
		else ulValue = 1;
		break;
	case VT_DATE:
		{ // 仅仅转化日期
			COleDateTime dtVal(vtVal);
			ulValue = dtVal.GetYear() * 10000 +
				dtVal.GetMonth() * 100 + dtVal.GetDay();
		}
		break;
	case VT_CY:
		ulValue = (unsigned long)vtVal.cyVal.Hi;
		break;
	}

	return TRUE;
}

BOOL CUniverDB::GetFieldValue(short shtFld, long& lValue)
{
	lValue = 0;

	_variant_t vtVal;
	if (!GetFieldValue(shtFld, vtVal))
		return FALSE;

	switch (vtVal.vt)
	{
	case VT_BSTR:
		{
			// 变串转宽串，宽串转串
			_bstr_t bstr(vtVal);
			CString strVal = (char*)bstr;

			strVal = sStdNumber(strVal);
			lValue = sToInteger(strVal);
		}
		break;
	case VT_DECIMAL:
		{
			double dblVal;
			dblVal = sInt64ToDouble(vtVal.decVal.Lo64);
			dblVal *= (vtVal.decVal.sign == 128)? -1 : 1;
			dblVal /= pow(10, (float)vtVal.decVal.scale);
			lValue = (long)dblVal;
		}
		break;
	case VT_R8:
		lValue = (long)vtVal.dblVal;
		break;
	case VT_R4:
		lValue = (long)vtVal.fltVal;
		break;
	case VT_UI4:
		lValue = (long)vtVal.ulVal;
		break;
	case VT_I4:
		lValue = vtVal.lVal;
		break;
	case VT_I2:
	case VT_UI1:
		lValue = (long)vtVal.iVal;
		break;
	case VT_BOOL:
		if (!vtVal.boolVal) lValue = 0;
		else lValue = 1;
		break;
	case VT_DATE:
		{ // 仅仅转化日期
			COleDateTime dtVal(vtVal);
			lValue = dtVal.GetYear() * 10000 +
				dtVal.GetMonth() * 100 + dtVal.GetDay();
		}
		break;
	case VT_CY:
		lValue = vtVal.cyVal.Hi;
		break;
	}

	return TRUE;
}

BOOL CUniverDB::GetFieldValue(LPCSTR strFld, long& lValue)
{
	lValue = 0;

	_variant_t vtVal;
	if (!GetFieldValue(strFld, vtVal))
		return FALSE;

	switch (vtVal.vt)
	{
	case VT_BSTR:
		{
			// 变串转宽串，宽串转串
			_bstr_t bstr(vtVal);
			CString strVal = (char*)bstr;

			strVal = sStdNumber(strVal);
			lValue = sToInteger(strVal);
		}
		break;
	case VT_DECIMAL:
		{
			double dblVal;
			dblVal = sInt64ToDouble(vtVal.decVal.Lo64);
			dblVal *= (vtVal.decVal.sign == 128)? -1 : 1;
			dblVal /= pow(10, (float)vtVal.decVal.scale);
			lValue = (long)dblVal;
		}
		break;
	case VT_R8:
		lValue = (long)vtVal.dblVal;
		break;
	case VT_R4:
		lValue = (long)vtVal.fltVal;
		break;
	case VT_UI4:
		lValue = (long)vtVal.ulVal;
		break;
	case VT_I4:
		lValue = vtVal.lVal;
		break;
	case VT_I2:
	case VT_UI1:
		lValue = (long)vtVal.iVal;
		break;
	case VT_BOOL:
		if (!vtVal.boolVal) lValue = 0;
		else lValue = 1;
		break;
	case VT_DATE:
		{ // 仅仅转化日期
			COleDateTime dtVal(vtVal);
			lValue = dtVal.GetYear() * 10000 +
				dtVal.GetMonth() * 100 + dtVal.GetDay();
		}
		break;
	case VT_CY:
		lValue = vtVal.cyVal.Hi;
		break;
	}

	return TRUE;
}


BOOL CUniverDB::GetFieldValue(short shtFld, COleDateTime& timeValue)
{
	timeValue.SetStatus(COleDateTime::null);

	_variant_t vtVal;
	if (!GetFieldValue(shtFld, vtVal))
		return FALSE;

	switch (vtVal.vt) 
	{
	case VT_DATE:
		COleDateTime dtVal(vtVal);
		timeValue = dtVal;
		break;
	}

	return TRUE;
}

BOOL CUniverDB::GetFieldValue(LPCSTR strFld, COleDateTime& timeValue)
{
	timeValue.SetStatus(COleDateTime::null);

	_variant_t vtVal;
	if (!GetFieldValue(strFld, vtVal))
		return FALSE;

	switch (vtVal.vt) 
	{
	case VT_DATE:
		COleDateTime dtVal(vtVal);
		timeValue = dtVal;
		break;
	}

	return TRUE;
}


// 原始设置数据
BOOL CUniverDB::SetFieldValue(short shtFld, _variant_t vtValue)
{
	try
	{
		m_pRecordset->Fields->Item[_variant_t(shtFld)]->PutValue(vtValue);
	}
	catch (_com_error &e)
	{
		m_strLastErr = (char*)e.Description();
		return FALSE;
	}
	catch (CException* e)
	{
		GetError(e);
		return FALSE;
	}

	m_strLastErr.Empty();
	return TRUE;
}

BOOL CUniverDB::SetFieldValue(LPCSTR strFld, _variant_t vtValue)
{
	try
	{
		m_pRecordset->Fields->Item[_bstr_t(strFld)]->PutValue(vtValue);
	}
	catch (_com_error &e)
	{
		m_strLastErr = (char*)e.Description();
		return FALSE;
	}
	catch (CException* e)
	{
		GetError(e);
		return FALSE;
	}

	m_strLastErr.Empty();
	return TRUE;
}

// 检测当前数据库是否已经连接
BOOL CUniverDB::IsConnect()
{
	BOOL bRet;


	if (m_pConnect == NULL)
	{
		m_strLastErr = "无效的连接指针";
		return FALSE;
	}

	try
	{
		bRet = (m_pConnect->GetState() & ADODB::adStateOpen);
	}
	catch (_com_error &e)
	{
		m_strLastErr = (char*)e.Description();
		return FALSE;
	}
	catch (CException* e)
	{
		GetError(e);
		return FALSE;
	}

	if (bRet)
		m_strLastErr.Empty();
	else
		m_strLastErr = "数据库尚未连接";
	return bRet;
}

// 连接当前数据库
BOOL CUniverDB::Connect(LPCSTR strCnnParam)
{
	Disconnect();

	// 分解数据库连接参数
	CString strHostName;
	CString strLibName;
	CString strUsrName;
	CString strUsrPwd;
	CString strLibType = sdbSepaCnnParam(strCnnParam, strHostName,
		strLibName, strUsrName, strUsrPwd);

	//BOOL bRet;



	CString strCnn;
    if (!strLibType.CompareNoCase("Access"))
		strCnn = sdbGetAccessCnnStr(strLibName, strUsrPwd);

	else
	{
		m_strLastErr = "非法的数据库类型";
		return FALSE;
	}

	try
	{
		m_pConnect = NULL;
		m_pConnect.CreateInstance(__uuidof(ADODB::Connection));

		if (m_pConnect == NULL)
		{
			m_pConnect.CreateInstance(__uuidof(ADODB::Connection));
			if (m_pConnect == NULL)
			{
				m_strLastErr = "连接实例化失败";
				return FALSE;
			}
		}

		m_pConnect->Open(_bstr_t(strCnn), "", "", ADODB::adConnectUnspecified);
	}
	catch (_com_error &e)
	{
		m_strLastErr = (char*)e.Description();
		return FALSE;
	}
	catch (CException* e)
	{
		GetError(e);
		return FALSE;
	}

	// 合成数据库参数
	m_strCnnParam = sdbCompCnnParam(strLibType, strHostName, strLibName, strUsrName, strUsrPwd);
	m_strLastErr.Empty();
	return TRUE;
}

// 撤消当前数据库连接
void CUniverDB::Disconnect()
{
	if (m_pRecordset != NULL)
		Close();

	m_pConnect  = NULL;

	m_strLastErr.Empty();
	m_strCnnParam.Empty(); // 数据库参数
	m_arrTblName.RemoveAll();
}

BOOL CUniverDB::IsOpen()
{
	BOOL bRet;

	if (!IsConnect())
		return FALSE;

	if (m_pRecordset == NULL)
	{
		m_strLastErr = "无效的记录集指针";
		return FALSE;
	}

	try
	{
		bRet = m_pRecordset->GetState() & ADODB::adStateOpen;
	}
	catch (_com_error &e)
	{
		m_strLastErr = (char*)e.Description();
		return FALSE;
	}
	catch (CException* e)
	{
		GetError(e);
		return FALSE;
	}

	if (bRet)
		m_strLastErr.Empty();
	else
		m_strLastErr = "数据表尚未打开";
	return bRet;
}

// 通过 SQL 语句打开记录集
//     pSrcDB != NULL 表示依据源表重新建表，仅仅文件库支持
BOOL CUniverDB::OpenSQL(LPCSTR strSql, CUniverDB* pSrcDB)
{
	//BOOL bRet;

	if (!IsConnect())
		return FALSE;

	Close();

	try
	{
		m_pRecordset.CreateInstance(__uuidof(ADODB::Recordset));

		if (m_pRecordset == NULL)
		{
			m_pRecordset.CreateInstance(__uuidof(ADODB::Recordset));
			if (m_pRecordset == NULL)
			{
				m_strLastErr = "Recordset 实例化失败";
				return FALSE;
			}
		}

		m_pRecordset->CursorLocation = ADODB::adUseClient;
		m_strSQL = strSql;
		HRESULT hr = m_pRecordset->Open(_bstr_t(m_strSQL),
			_variant_t((IDispatch*)m_pConnect, TRUE),
			ADODB::adOpenDynamic, ADODB::adLockOptimistic, ADODB::adCmdText);

		if (hr != S_OK)
		{
			m_pRecordset = NULL;
			m_strSQL.Empty();

			m_strLastErr = "打开数据表失败";
			return FALSE;
		}
	}
	catch (_com_error &e)
	{
		m_pRecordset = NULL;
		m_strSQL.Empty();

		m_strLastErr = (char*)e.Description();
		return FALSE;
	}
	catch (CException* e)
	{
		m_pRecordset = NULL;
		m_strSQL.Empty();
		GetError(e);
		return FALSE;
	}

	m_strLastErr.Empty();
	return TRUE;
}

// 通过 SQL 语句创建数据表并打开记录集
BOOL CUniverDB::OpenSQL(LPCSTR strSql, CStringArray* parrSrcFld)
{
	//BOOL bRet;

	if (!IsConnect())
		return FALSE;

	Close();

	try
	{
		m_pRecordset.CreateInstance(__uuidof(ADODB::Recordset));

		if (m_pRecordset == NULL)
		{
			m_pRecordset.CreateInstance(__uuidof(ADODB::Recordset));
			if (m_pRecordset == NULL)
			{
				m_strLastErr = "Recordset 实例化失败";
				return FALSE;
			}
		}

		m_pRecordset->CursorLocation = ADODB::adUseClient;
		m_strSQL = strSql;
		HRESULT hr = m_pRecordset->Open(_bstr_t(m_strSQL),
			_variant_t((IDispatch*)m_pConnect, TRUE),
			ADODB::adOpenDynamic, ADODB::adLockOptimistic, ADODB::adCmdText);

		if (hr != S_OK)
		{
			m_pRecordset = NULL;
			m_strSQL.Empty();

			m_strLastErr = "打开数据表失败";
			return FALSE;
		}
	}
	catch (_com_error &e)
	{
		m_pRecordset = NULL;
		m_strSQL.Empty();

		m_strLastErr = (char*)e.Description();
		return FALSE;
	}
	catch (CException* e)
	{
		m_pRecordset = NULL;
		m_strSQL.Empty();
		GetError(e);
		return FALSE;
	}

	m_strLastErr.Empty();
	return TRUE;
}

void CUniverDB::Close()
{

	m_pRecordset = NULL;
	m_strSQL.Empty();
}

// 获得记录集的行数
long CUniverDB::GetRows()
{
	long lRows;

	if (!IsOpen())
		return 0L;

	try
	{
		lRows = m_pRecordset->GetRecordCount();
		if (lRows == -1)
		{
			lRows = 0L;
			if (m_pRecordset->AdoEOF != VARIANT_TRUE)
				m_pRecordset->MoveFirst();

			while (m_pRecordset->AdoEOF != VARIANT_TRUE)
			{
				lRows++;
				m_pRecordset->MoveNext();
			}

			if (lRows > 0)
				m_pRecordset->MoveFirst();
		}
	}
	catch (_com_error &e)
	{
		m_strLastErr = (char*)e.Description();
		return 0L;
	}
	catch (CException* e)
	{
		GetError(e);
		return 0L;
	}

	m_strLastErr.Empty();
	return lRows;
}


// 检测、移动、设置记录位置
BOOL CUniverDB::IsEOF()
{
	BOOL bEOF;

	if (!IsOpen())
		return TRUE;

	try
	{
		bEOF = m_pRecordset->AdoEOF == VARIANT_TRUE;
	}
	catch (_com_error &e)
	{
		m_strLastErr = (char*)e.Description();
		return TRUE;
	}
	catch (CException* e)
	{
		GetError(e);
		return TRUE;
	}

	m_strLastErr.Empty();
	return bEOF;
}

BOOL CUniverDB::IsBOF()
{
	BOOL bBOF;

	if (!IsOpen())
		return TRUE;

	try
	{
		bBOF = m_pRecordset->BOF == VARIANT_TRUE;
	}
	catch (_com_error &e)
	{
		m_strLastErr = (char*)e.Description();
		return TRUE;
	}
	catch (CException* e)
	{
		GetError(e);
		return TRUE;
	}

	m_strLastErr.Empty();
	return bBOF;
}

BOOL CUniverDB::MoveFirst()
{
	//BOOL bMoved;

	if (!IsOpen())
		return FALSE;

	try
	{
		if (!(m_pRecordset->BOF && m_pRecordset->AdoEOF))
			m_pRecordset->MoveFirst();
		else
			return FALSE;
	}
	catch (_com_error &e)
	{
		m_strLastErr = (char*)e.Description();
		return FALSE;
	}
	catch (CException* e)
	{
		GetError(e);
		return FALSE;
	}

	m_strLastErr.Empty();
	return TRUE;
}

BOOL CUniverDB::MoveNext()
{
	//BOOL bMoved;


	if (!IsOpen())
		return FALSE;

	try
	{
		if (!m_pRecordset->AdoEOF)
			m_pRecordset->MoveNext();
	}
	catch (_com_error &e)
	{
		m_strLastErr = (char*)e.Description();
		return FALSE;
	}
	catch (CException* e)
	{
		GetError(e);
		return FALSE;
	}

	m_strLastErr.Empty();
	return TRUE;
}

BOOL CUniverDB::MovePrev()
{
	///BOOL bMoved;


	if (!IsOpen())
		return FALSE;

	try
	{
		if (!m_pRecordset->BOF)
			m_pRecordset->MovePrevious();
	}
	catch (_com_error &e)
	{
		m_strLastErr = (char*)e.Description();
		return FALSE;
	}
	catch (CException* e)
	{
		GetError(e);
		return FALSE;
	}

	m_strLastErr.Empty();
	return TRUE;
}

BOOL CUniverDB::MoveLast()
{
	//BOOL bMoved;

	if (!IsOpen())
		return FALSE;

	try
	{
		if (!(m_pRecordset->BOF && m_pRecordset->AdoEOF))
			m_pRecordset->MoveLast();
	}
	catch (_com_error &e)
	{
		m_strLastErr = (char*)e.Description();
		return FALSE;
	}
	catch (CException* e)
	{
		GetError(e);
		return FALSE;
	}

	m_strLastErr.Empty();
	return TRUE;
}


// 取数据
CString CUniverDB::GetString(short shtCol)
{
	CString strVal;

	if (!IsOpen())
		return "";

	strVal.Empty();
	GetFieldValue(shtCol, strVal);
	strVal.TrimRight();
	return strVal;
}

CString CUniverDB::GetString(LPCSTR strComCol)
{
	if (strComCol[0] == '!')
	{
		CString str = strComCol;
		return str.Mid(1);
	}
	if (strComCol[0] >= '0' && strComCol[0] <= '9')
		return GetString((short)sToInteger(strComCol));

	CString strVal;

	if (!IsOpen())
		return "";

	strVal.Empty();
	GetFieldValue(strComCol, strVal);
	strVal.TrimRight();
	return strVal;
}


long CUniverDB::GetLong(short shtCol)
{
	long lVal;

	if (!IsOpen())
		return 0L;

	GetFieldValue(shtCol, lVal);
	return lVal;
}

long CUniverDB::GetLong(LPCSTR strComCol)
{
	if (strComCol[0] == '!')
	{
		CString str = strComCol;
		return sToInteger(str.Mid(1));
	}
	if (strComCol[0] >= '0' && strComCol[0] <= '9')
		return GetLong((short)sToInteger(strComCol));

	long lVal;

	if (!IsOpen())
		return 0L;

	GetFieldValue(strComCol, lVal);
	return lVal;
}


COleDateTime CUniverDB::GetDateTime(short shtCol)
{
	COleDateTime timeVal;
	timeVal.SetStatus(COleDateTime::null);

	if (!IsOpen())
		return timeVal;


	GetFieldValue(shtCol, timeVal);
	return timeVal;
}

COleDateTime CUniverDB::GetDateTime(LPCSTR strComCol)
{
	if (strComCol[0] >= '0' && strComCol[0] <= '9')
		return GetDateTime((short)sToInteger(strComCol));

	COleDateTime timeVal;
	timeVal.SetStatus(COleDateTime::null);

	if (!IsOpen())
		return timeVal;


	GetFieldValue(strComCol, timeVal);
	return timeVal;
}

// 设置数据
BOOL CUniverDB::SetString(short shtCol, LPCSTR lpValue)
{
	BOOL bRet;


	if (!IsOpen())
		return FALSE;

	if (!lpValue || strlen(lpValue) == 0)
	{
		_variant_t vtVal;
		vtVal.vt = VT_NULL;
		bRet = SetFieldValue(shtCol, vtVal);
	}
	else
	{
		// 串转宽串，宽串转变串
		_bstr_t bstr(lpValue);
		_variant_t vtVal(bstr);
		bRet = SetFieldValue(shtCol, vtVal);
	}

	return bRet;
}

BOOL CUniverDB::SetString(LPCSTR strComCol, LPCSTR lpValue)
{
	if (strComCol[0] >= '0' && strComCol[0] <= '9')
		return SetString((short)sToInteger(strComCol), lpValue);

	BOOL bRet;


	if (!IsOpen())
		return FALSE;

	if (!lpValue || strlen(lpValue) == 0)
	{
		_variant_t vtVal;
		vtVal.vt = VT_NULL;
		bRet = SetFieldValue(strComCol, vtVal);
	}
	else
	{
		// 串转宽串，宽串转变串
		_bstr_t bstr(lpValue);
		_variant_t vtVal(bstr);
		bRet = SetFieldValue(strComCol, vtVal);
	}

	return bRet;
}


BOOL CUniverDB::SetLong(short shtCol, long lValue)
{
	BOOL bRet;


	if (!IsOpen())
		return FALSE;

	_variant_t vtVal(lValue);
	bRet = SetFieldValue(shtCol, vtVal);
	return bRet;
}

BOOL CUniverDB::SetLong(LPCSTR strComCol, long lValue)
{
	if (strComCol[0] >= '0' && strComCol[0] <= '9')
		return SetLong((short)sToInteger(strComCol), lValue);

	BOOL bRet;
	

	if (!IsOpen())
		return FALSE;

	_variant_t vtVal(lValue);
	bRet = SetFieldValue(strComCol, vtVal);
	return bRet;
}

int CUniverDB::GetTblCount(void){
	return 1;
}
CString CUniverDB::GetTblName(int){
	return "";
}

int CUniverDB::SetPos(long){
	return 0;
}

long CUniverDB::GetPos(void){
	return 0;
}

BOOL CUniverDB::SetDateTime(short shtCol, COleDateTime timeValue)
{
	if(timeValue.GetStatus() == COleDateTime::null)
		return FALSE;
	if (!IsOpen())
		return FALSE;


	_variant_t vtVal(timeValue);
	BOOL bRet = SetFieldValue(shtCol, vtVal);
	return bRet;
}

BOOL CUniverDB::SetDateTime(LPCSTR strComCol, COleDateTime timeValue)
{
	if(timeValue.GetStatus() == COleDateTime::null)
		return FALSE;
	if (strComCol[0] >= '0' && strComCol[0] <= '9')
		return SetDateTime((short)sToInteger(strComCol), timeValue);

	if (!IsOpen())
		return FALSE;

	_variant_t vtVal(timeValue);
	BOOL bRet = SetFieldValue(strComCol, vtVal);
	return bRet;
}


// 记录集编辑
BOOL CUniverDB::AddNew()
{
	BOOL bAdded;


	if (!IsOpen())
		return FALSE;

	try
	{
		if (m_pRecordset->AddNew() == S_OK)
			bAdded = TRUE;
		else
			bAdded = FALSE;
	}
	catch (_com_error &e)
	{
		m_strLastErr = (char*)e.Description();
		return FALSE;
	}
	catch (CException* e)
	{
		GetError(e);
		return FALSE;
	}

	m_strLastErr.Empty();
	return TRUE;
}

BOOL CUniverDB::Edit()
{
	//BOOL bEdited;


	if (!IsOpen())
		return FALSE;

	m_strLastErr.Empty();
	return TRUE;
}

BOOL CUniverDB::Delete()
{
	BOOL bDeleted;


	if (!IsOpen())
		return FALSE;

	try
	{
		if (m_pRecordset->Delete(ADODB::adAffectCurrent) == S_OK)
			bDeleted = TRUE;
		else
			bDeleted = FALSE;
	}
	catch (_com_error &e)
	{
		m_strLastErr = (char*)e.Description();
		return FALSE;
	}
	catch (CException* e)
	{
		GetError(e);
		return FALSE;
	}

	m_strLastErr.Empty();
	return bDeleted;
}

BOOL CUniverDB::Update(BOOL bUpdated)
{
	bUpdated = FALSE;


	if (!IsOpen())
		return FALSE;

	try
	{
		m_pRecordset->Update();
		bUpdated = TRUE;
	}
	catch (_com_error &e)
	{
		m_strLastErr = (char*)e.Description();
	}
	catch (CException* e)
	{
		GetError(e);
	}

	// Update出错必须在处理完抛出事件之后复位
	if (!bUpdated)
	{
		try
		{
			m_pRecordset->CancelUpdate();
		}
		catch (_com_error &e)
		{
			e; // 不能处理事件
		}
		catch (CException* e)
		{
			e; // 不能处理事件
		}
	}
	else
		m_strLastErr.Empty();

	return bUpdated;
}


// 运行非查询SQL语句
BOOL CUniverDB::Execute(CString strSql,BOOL isResult)
{

	if (!IsConnect())
		return FALSE;

	try
	{
		m_pConnect->Execute(_bstr_t(strSql), NULL, ADODB::adCmdText);
	}
	catch (_com_error &e)
	{
		m_strLastErr = (char*)e.Description();
		return FALSE;
	}
	catch (CException* e)
	{
		GetError(e);
		return FALSE;
	}

	m_strLastErr.Empty();
	return TRUE;
}
