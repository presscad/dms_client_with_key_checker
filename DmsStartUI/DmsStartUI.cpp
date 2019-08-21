// DmsStartUI.cpp : WinMain ��ʵ��


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
		// TODO : ���� CoInitializeSecurity ��Ϊ�����ṩ�ʵ��� 
		// ��ȫ����
		// ���� - PKT ����������֤��
		// RPC_C_IMP_LEVEL_IDENTIFY ��ģ�⼶��
		// �Լ��ʵ��ķ� NULL ��ȫ˵������

		return S_OK;
	}

	PROCESS_INFORMATION m_pinfo;
	//����Ҫ��һЩ����������д  

	void OnPause() throw(); //��ͣ  
	void OnStop() throw();//ֹͣ  
	void Handler(DWORD dwOpcode) throw();//����ͬ�ķ��������Ϣ  
	void OnContinue() throw();//��������  
	HRESULT PreMessageLoop(int nShowCmd) throw();//��Ϣ��Ӧ  
	HRESULT RegisterAppId(bool bService = false) throw();//����ע��  
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
		if (IsInstalled())//�����Ѿ���װ
		{
			SC_HANDLE hSCM = ::OpenSCManager(NULL,NULL,SERVICE_CHANGE_CONFIG);//�򿪷��������
			SC_HANDLE hService = NULL;

			if (hSCM == NULL)
			{
				hr = ATL::AtlHresultFromLastError();
			}
			else
			{
				//�򿪷���m_szServiceNameΪ�����Ա����������ǰ��������
				//��������Դ�ļ��б��String Table���޸�
				hService = OpenService(hSCM,m_szServiceName,SERVICE_CHANGE_CONFIG);
				if (hService != NULL)
				{
					//�޸ķ�������
					ChangeServiceConfig(hService,
						SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS,//�������̡�������
						SERVICE_AUTO_START,//�����Զ�����
						NULL,NULL,NULL,NULL,NULL,NULL,NULL,
						m_szServiceName);

					//����������Ϣ
					SERVICE_DESCRIPTION sDescription;
					TCHAR szDescription[1024];
					ZeroMemory(szDescription,1024);
					ZeroMemory(&sDescription,sizeof(SERVICE_DESCRIPTION));

					//��������
					lstrcpy(szDescription,_T("ATLDemo������ԣ�"));
					sDescription.lpDescription = szDescription;

					//�޸ķ���������Ϣ
					ChangeServiceConfig2(hService,SERVICE_CONFIG_DESCRIPTION,&sDescription);

					//�رշ�����
					CloseServiceHandle(hService);

				}
				else
				{
					hr = ATL::AtlHresultFromLastError();
				}
			}

			//�رշ�����������
			::CloseServiceHandle(hSCM);
		}
	}
	return hr;
}



HRESULT CDmsStartUIModule::PreMessageLoop( int nShowCmd ) throw()
{
	//�÷���������ͣ�ͼ�������
	m_status.dwControlsAccepted = m_status.dwControlsAccepted|SERVICE_ACCEPT_PAUSE_CONTINUE;
	HRESULT hr = __super::PreMessageLoop(nShowCmd);
	if (hr == S_FALSE)
	{
		hr = S_OK;//������Bug,��������д��������ܼ���
	}

	//������״̬����Ϊ����
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

	CString str =time.Format("start --- %Y��%m��%d��%Hʱ%M��%S��");//��ʽ������ʱ�� 
	LogEvent(str);
	//if(ret){
	//	//д��ϵͳ��־
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
	case SERVICE_CONTROL_PAUSE://��ͣ
		{
			OnPause();
			break;
		}
	case SERVICE_CONTROL_CONTINUE://����
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
	//���÷���״̬Ϊ��ͣ
	SetServiceStatus(SERVICE_PAUSED);

	__super::OnPause();
}

void CDmsStartUIModule::OnStop() throw()
{
	//���÷���״̬Ϊֹͣ
	SetServiceStatus(SERVICE_STOPPED);

	//if(m_pinfo.hProcess)
	//	TerminateProcess(m_pinfo.hProcess,0);//�رս���
	__super::OnStop();
}

void CDmsStartUIModule::OnContinue() throw()
{
	//���÷���״̬Ϊ����
	SetServiceStatus(SERVICE_RUNNING);

	__super::OnContinue();
}

