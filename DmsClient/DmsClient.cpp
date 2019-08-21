// DmsClient.cpp : WinMain 的实现


#include "stdafx.h"
#include "resource.h"
#include "DmsClient.h"
#include <atlstr.h>
#include <stdio.h>

BOOL (*startMonitorFile)(char* ) = NULL;
BOOL (*stopMonitorFile)(void) = NULL;
int (*sConnectServer)(void) = NULL;
//void (*sSetWebPID)(int pid) = NULL;


//typedef void (*PFunSetWebPID)(int pid); 
//
//void (*pSetWebPID)(PFunSetWebPID ) = NULL;
//
//int s_WebPID = 0;
//void sSetWebPID(int pid){
//	s_WebPID = pid;
//	if(pid  == 0)
//	  WinExec("c:\\TestWeb.bat", SW_SHOW);
//}

class CDmsClientModule : public CAtlServiceModuleT< CDmsClientModule, IDS_SERVICENAME >
{
public :
	DECLARE_LIBID(LIBID_DmsClientLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DMSCLIENT, "{09D8D218-66E6-4C88-8C5B-5F7262306D31}")
	HRESULT InitializeSecurity() throw()
	{
		// TODO : 调用 CoInitializeSecurity 并为服务提供适当的 
		// 安全设置
		// 建议 - PKT 级别的身份验证、
		// RPC_C_IMP_LEVEL_IDENTIFY 的模拟级别
		// 以及适当的非 NULL 安全说明符。

		//return S_OK;
		return CoInitializeSecurity(NULL,-1,NULL,NULL,  
			RPC_C_AUTHN_LEVEL_NONE,  
			RPC_C_IMP_LEVEL_IDENTIFY,  
			NULL,EOAC_NONE,NULL);//创建安全说明符  
	}

	PROCESS_INFORMATION m_pinfo;
	//这里要对一些函数进行重写  

	void StartMonitorFile();
	void OnPause() throw(); //暂停  
	void OnStop() throw();//停止  
	void Handler(DWORD dwOpcode) throw();//处理不同的服务控制消息  
	void OnContinue() throw();//继续运行  
	HRESULT PreMessageLoop(int nShowCmd) throw();//消息响应  
	HRESULT RegisterAppId(bool bService = false) throw();//服务注册  

};

HRESULT CDmsClientModule::RegisterAppId( bool bService /*= false*/ ) throw()
{
	HRESULT hr = S_OK;
	BOOL res = __super::RegisterAppId(bService);
	if (bService)
	{
		if (IsInstalled())//服务已经安装
		{
			SC_HANDLE hSCM = ::OpenSCManager(NULL,NULL,SERVICE_CHANGE_CONFIG);//打开服务管理器
			SC_HANDLE hService = NULL;

			if (hSCM == NULL)
			{
				hr = ATL::AtlHresultFromLastError();
			}
			else
			{
				//打开服务，m_szServiceName为基类成员变量，代表当前服务名称
				//可以在资源文件列表的String Table中修改
				hService = OpenService(hSCM,m_szServiceName,SERVICE_CHANGE_CONFIG);
				if (hService != NULL)
				{
					//修改服务配置
					ChangeServiceConfig(hService,
						SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS,//独立进程、允许交互
						SERVICE_AUTO_START,//服务自动启动
						NULL,NULL,NULL,NULL,NULL,NULL,NULL,
						m_szServiceName);

					//服务描述信息
					SERVICE_DESCRIPTION sDescription;
					TCHAR szDescription[1024];
					ZeroMemory(szDescription,1024);
					ZeroMemory(&sDescription,sizeof(SERVICE_DESCRIPTION));

					//服务描述
					lstrcpy(szDescription,_T("DMS服务！"));
					sDescription.lpDescription = szDescription;

					//修改服务描述信息
					ChangeServiceConfig2(hService,SERVICE_CONFIG_DESCRIPTION,&sDescription);

					//关闭服务句柄
					CloseServiceHandle(hService);

				}
				else
				{
					hr = ATL::AtlHresultFromLastError();
				}
			}

			//关闭服务管理器句柄
			::CloseServiceHandle(hSCM);
		}
	}
	return hr;
}



HRESULT CDmsClientModule::PreMessageLoop( int nShowCmd ) throw()
{
	//让服务允许暂停和继续操作
	m_status.dwControlsAccepted = m_status.dwControlsAccepted|SERVICE_ACCEPT_PAUSE_CONTINUE;
	HRESULT hr = __super::PreMessageLoop(nShowCmd);
	if (hr == S_FALSE)
	{
		hr = S_OK;//这里有Bug,必须这样写，后面才能继续
	}

	//将服务状态设置为启动
	SetServiceStatus(SERVICE_RUNNING);
	CHAR szFileName[256];
	memset(szFileName,'0',sizeof(szFileName));

	::GetModuleFileName(NULL,szFileName, sizeof(szFileName)); 

	CString strCurPath;
	GetModuleFileName(NULL,strCurPath.GetBuffer(MAX_PATH),MAX_PATH);
	strCurPath.ReleaseBuffer();//Must ReleaseBuffer, or GetLength=0
	int pos= strCurPath.ReverseFind(_T('\\'));
	int len = strCurPath.GetLength();

	strCurPath = strCurPath.Left(pos);

	//BOOL ret = WinExec("c:\\TestWeb.bat", SW_SHOW);

	//CString lpApplicationName = strCurPath;

	//lpApplicationName.Format(_T("%s\\jre\\bin\\java.exe"),strCurPath);

	//CString  lpCommandLine;
	//lpCommandLine.Format(_T(" -jar %s\\dms_client.jar %s\\"),strCurPath ,strCurPath);
	//
	//STARTUPINFO StartInfo;
	//
	//memset(&StartInfo,0,sizeof(STARTUPINFO));
	//StartInfo.cb = sizeof(STARTUPINFO);
	//StartInfo.dwFlags = STARTF_USESHOWWINDOW;
	//StartInfo.wShowWindow = FALSE;
	//m_pinfo.hProcess = NULL;
	BOOL ret = FALSE;// CreateProcess((LPWSTR)(LPCTSTR)lpApplicationName, (LPWSTR)(LPCTSTR)lpCommandLine,NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS,NULL,NULL,&StartInfo,&m_pinfo);

	
	//LogEvent(lpApplicationName);
	//LogEvent(lpCommandLine);

	StartMonitorFile();

	if(sConnectServer)
		ret = !sConnectServer();

	if(ret){
		//写入系统日志
		LogEvent(_T("ATLDemo Service Start Successfully~!.DmsClient Start Successfully!"));
	}else{
		LogEvent(_T("ATLDemo Service Start Successfully~!.DmsClient Start Failure!"));
	}

    //CFileMonitor fm ;
	//fm.Test();
	//LogEvent();

	return hr;
}

void CDmsClientModule::StartMonitorFile(){

	CHAR szFileName[256];
	memset(szFileName,'0',sizeof(szFileName));

	::GetModuleFileName(NULL,szFileName, sizeof(szFileName)); 

	CString strCurPath;
	GetModuleFileName(NULL,strCurPath.GetBuffer(MAX_PATH),MAX_PATH);
	strCurPath.ReleaseBuffer();//Must ReleaseBuffer, or GetLength=0
	int pos= strCurPath.ReverseFind(_T('\\'));
	strCurPath = strCurPath.Left(pos);

	HMODULE hModule = ::LoadLibrary(strCurPath + "\\Monitor.dll");

	if(hModule)
	{
		startMonitorFile = (BOOL  (*)(char*))::GetProcAddress(hModule,_T("startMonitorFile"));

		stopMonitorFile = (BOOL(*)(void))::GetProcAddress(hModule,_T("stopMonitorFile"));
		sConnectServer  = (int (*)(void))::GetProcAddress(hModule,_T("sConnectServer"));

		//pSetWebPID = (void (*)(PFunSetWebPID ))::GetProcAddress(hModule,_T("sSetWebPID"));
		//if(pSetWebPID){
		//	pSetWebPID(sSetWebPID);
		//}

		startMonitorFile(szFileName);
		LogEvent(_T("ATLDemo Service Start Successfully~!.DmsClient Start Successfully!"));
	}else{
		LogEvent(_T("ATLDemo Service Start Successfully~!.DmsClient Start Failure!"));
	}
}

void CDmsClientModule::Handler( DWORD dwOpcode ) throw()
{
	switch(dwOpcode)
	{
	case SERVICE_CONTROL_PAUSE://暂停
		{
			OnPause();
			break;
		}
	case SERVICE_CONTROL_CONTINUE://继续
		{
			OnContinue();
			break;
		}
	default:
		break;
	}

	__super::Handler(dwOpcode);
}

void CDmsClientModule::OnPause() throw()
{
	//设置服务状态为暂停
	SetServiceStatus(SERVICE_PAUSED);

	__super::OnPause();
}

void CDmsClientModule::OnStop() throw()
{
	//设置服务状态为停止
	SetServiceStatus(SERVICE_STOPPED);

	if(stopMonitorFile)
		stopMonitorFile();

	//if(m_pinfo.hProcess)
	//	TerminateProcess(m_pinfo.hProcess,0);//关闭进程
	__super::OnStop();
}

void CDmsClientModule::OnContinue() throw()
{
	//设置服务状态为启动
	SetServiceStatus(SERVICE_RUNNING);

	__super::OnContinue();
}



CDmsClientModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	//_AtlModule.StartMonitorFile();
    return _AtlModule.WinMain(nShowCmd);
}



