// DMSstart.cpp : WinMain µÄÊµÏÖ


#include "stdafx.h"
#include "resource.h"
#include "DMSstart.h"
#include <atlstr.h>


class CDMSstartModule : public CAtlExeModuleT< CDMSstartModule >
{
public :
	DECLARE_LIBID(LIBID_DMSstartLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DMSSTART, "{29B8F79C-E00F-43D7-AA6F-182300397720}")
};

CDMSstartModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                                LPTSTR lpCmdLine, int nShowCmd)
{
	CString str = (CString)lpCmdLine;
	if(str.IsEmpty()){
		str = "http://localhost/das";//qdrsmaster
		//ShellExecute(NULL,   _T("open"),   str,   NULL,NULL,SW_SHOWMAXIMIZED);

	}
	ShellExecute(NULL,   _T("open"),   str,   NULL,NULL,SW_SHOWMAXIMIZED);
	/*else{


		STARTUPINFO StartInfo;

		PROCESS_INFORMATION m_pinfo;
		memset(&StartInfo,0,sizeof(STARTUPINFO));
		StartInfo.cb = sizeof(STARTUPINFO);
		StartInfo.dwFlags = STARTF_USESHOWWINDOW;
		StartInfo.wShowWindow = FALSE;
		m_pinfo.hProcess = NULL;

		CString lpCommandLine("http://localhost:8080/master");
		CreateProcess((LPWSTR)(LPCTSTR)str, (LPWSTR)(LPCTSTR)lpCommandLine,NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS,NULL,NULL,&StartInfo,&m_pinfo);
	}*/

    return _AtlModule.WinMain(nShowCmd);
}

