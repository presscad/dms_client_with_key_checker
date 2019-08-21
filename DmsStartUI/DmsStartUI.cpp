// DmsStartUI.cpp : WinMain 的实现


#include "stdafx.h"
#include "resource.h"
#include "DmsStartUI.h"
#include <atlstr.h>
#include <atltime.h>
#include <stdio.h>

class CDmsStartUIModule : public CAtlServiceModuleT< CDmsStartUIModule, IDS_SERVICENAME >
{
public :
	DECLARE_LIBID(LIBID_DmsStartUILib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DMSSTARTUI, "{B6A0096E-FD9E-440A-B124-A36E20D37134}")
	HRESULT InitializeSecurity() throw()
	{
		// TODO : 调用 CoInitializeSecurity 并为服务提供适当的 
		// 安全设置
		// 建议 - PKT 级别的身份验证、
		// RPC_C_IMP_LEVEL_IDENTIFY 的模拟级别
		// 以及适当的非 NULL 安全说明符。

		return S_OK;
	}

	PROCESS_INFORMATION m_pinfo;
	//这里要对一些函数进行重写  

	void OnPause() throw(); //暂停  
	void OnStop() throw();//停止  
	void Handler(DWORD dwOpcode) throw();//处理不同的服务控制消息  
	void OnContinue() throw();//继续运行  
	HRESULT PreMessageLoop(int nShowCmd) throw();//消息响应  
	HRESULT RegisterAppId(bool bService = false) throw();//服务注册  
};

CDmsStartUIModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}



HRESULT CDmsStartUIModule::RegisterAppId( bool bService /*= false*/ ) throw()
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
					lstrcpy(szDescription,_T("ATLDemo服务测试！"));
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



HRESULT CDmsStartUIModule::PreMessageLoop( int nShowCmd ) throw()
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



	//CHAR szFileName[256];
	//memset(szFileName,'0',sizeof(szFileName));

	//::GetModuleFileName(NULL,szFileName, sizeof(szFileName)); 

	CString strCurPath;
	GetModuleFileName(NULL,strCurPath.GetBuffer(MAX_PATH),MAX_PATH);
	strCurPath.ReleaseBuffer();//Must ReleaseBuffer, or GetLength=0
	int pos= strCurPath.ReverseFind(_T('\\'));
	int len = strCurPath.GetLength();
	strCurPath = strCurPath.Left(pos);

	strCurPath += "\\bin\\startup.bat";

	//Sleep(1800000);
	//WinExec(strCurPath, SW_HIDE);

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

	

	
	CTime time = CTime::GetCurrentTime();

	CString str =time.Format("start --- %Y年%m月%d日%H时%M分%S秒");//格式化日期时间 
	LogEvent(str);
	//if(ret){
	//	//写入系统日志
	//	LogEvent(_T("ATLDemo Service Start Successfully~!.DmsClient Start Successfully!"));
	//}else{
	//	LogEvent(_T("ATLDemo Service Start Successfully~!.DmsClient Start Failure!"));
	//}

	//CFileMonitor fm ;
	//fm.Test();
	//LogEvent();

	return hr;
}


void CDmsStartUIModule::Handler( DWORD dwOpcode ) throw()
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

void CDmsStartUIModule::OnPause() throw()
{
	//设置服务状态为暂停
	SetServiceStatus(SERVICE_PAUSED);

	__super::OnPause();
}

void CDmsStartUIModule::OnStop() throw()
{
	//设置服务状态为停止
	SetServiceStatus(SERVICE_STOPPED);

	//if(m_pinfo.hProcess)
	//	TerminateProcess(m_pinfo.hProcess,0);//关闭进程
	__super::OnStop();
}

void CDmsStartUIModule::OnContinue() throw()
{
	//设置服务状态为启动
	SetServiceStatus(SERVICE_RUNNING);

	__super::OnContinue();
}

