4// StartDMSService.cpp : WinMain µÄÊµÏÖ


#include "stdafx.h"
#include "resource.h"
#include "StartDMSService.h"
#include <atlstr.h>


class CStartDMSServiceModule : public CAtlExeModuleT< CStartDMSServiceModule >
{
public :
	DECLARE_LIBID(LIBID_StartDMSServiceLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_STARTDMSSERVICE, "{0FFCFD13-F5C3-4CF0-953C-1FD7F87C19A6}")
};

CStartDMSServiceModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	CString strCurPath;
	GetModuleFileName(NULL,strCurPath.GetBuffer(MAX_PATH),MAX_PATH);
	strCurPath.ReleaseBuffer();//Must ReleaseBuffer, or GetLength=0
	int pos= strCurPath.ReverseFind(_T('\\'));
	int len = strCurPath.GetLength();
	strCurPath = strCurPath.Left(pos);

	strCurPath += "\\bin\\startup.bat";

	//Sleep(1800000);
	WinExec(strCurPath, SW_HIDE);
    return _AtlModule.WinMain(nShowCmd);
}

