#include "stdafx.h"
#include "SuperDB.h"

#include "UniverDB.h"
#include <atldbcli.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE SUPER_DATABASE_DLL = { NULL, NULL };

static CString g_strOraDriver = "";      // Oracle OLEDB ������������
static CString g_strSQLDriver = "";      // SQL Server OLEDB ������������
static CString g_strJetDriver = "";      // Jet(�������ݿ�) OLEDB ������������
static CString g_strODBCDriver = "";     // ODBC ������������

// Ǩ�������������
static CStringArray* g_parrKeyVals = NULL;



extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(SUPER_DATABASE_DLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(SUPER_DATABASE_DLL);

		// ������ݿ�������������
		g_strOraDriver = "MSDAORA.1";
		g_strSQLDriver = "SQLOLEDB.1";
		g_strJetDriver = "Microsoft.Jet.OLEDB.4.0";
		g_strODBCDriver = "MSDASQL.1";
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		// Terminate the library before destructors are called
		AfxTermExtensionModule(SUPER_DATABASE_DLL);

		// ���Ǩ�������������
		if (g_parrKeyVals)
			delete [] g_parrKeyVals;
		//g_arrKeyPos.RemoveAll();
	}

	return 1;   // ok
}




// ���Access���ݿ����Ӵ�
CString sdbGetAccessCnnStr(LPCSTR strPath, LPCSTR strUsrPwd)
{
	CString strCnn;
	strCnn.Format("Provider=%s;Data Source=%s;Extended Properties=%s;Persist Security Info=False;Jet OLEDB:Database Password=%s",
		g_strJetDriver, strPath, "", strUsrPwd);
	return strCnn;
}



// �ֽ����Ӳ���
CString sdbSepaCnnParam(LPCSTR strCnnParam, CString& strHostName,
						CString& strLibName, CString& strUsrName, CString& strUsrPwd)
{
	int pos = -1;
	CString strLibType = sGetSubStr(strCnnParam, ';', pos + 1, &pos);

	strHostName = sGetSubStr(strCnnParam, ';', pos + 1, &pos);
	strLibName  = sGetSubStr(strCnnParam, ';', pos + 1, &pos);
	strUsrName  = sGetSubStr(strCnnParam, ';', pos + 1, &pos);
	strUsrPwd   = sGetSubStr(strCnnParam, ';', pos + 1, &pos);

	return strLibType;
}

// �ϳ����Ӳ���
CString sdbCompCnnParam(CString strLibType, CString strHostName,
						CString strLibName, CString strUsrName, CString strUsrPwd)
{
	return strLibType + ";" + strHostName + ";" +
		strLibName + ";" + strUsrName + ";" + strUsrPwd;
}

