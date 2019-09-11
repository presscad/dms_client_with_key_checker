// DmsTestDlgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DmsTestDlg.h"
#include "DmsTestDlgDlg.h"
#include "SelectTimeDlg.h"
//#include <Wtsapi32.h>
//#pragma comment(lib, "Wtsapi32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
CString s_strHomeDir = "";
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	CStatic m_staLicence;
// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedButton2();
	int CheckLicLicence();
public:
	afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_staLicence);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CAboutDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CAboutDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDmsTestDlgDlg �Ի���

CListBox* s_pListInfo = NULL;

BOOL (*startMonitorFile)(char* ) = NULL;
BOOL (*stopMonitorFile)(void) = NULL;
void (*sCloseUniverDB)(void) = NULL;
void (*sGetLicInfo)(void) = NULL;
int (*sCheckLicence)(char&) = NULL;
void (*sGetMonitoringParam)(int [] ,int&,int& ,int& ,CString& ,CString& ,CString&) = NULL;
void (*sSetMonitoringParam)(const CString& ,const CString& ,const CString&  ,const CString& ,const CString&,const CString&) = NULL;
void (*setCListBox)(CListBox& ) = NULL;
int (*sConnectServer)(void) = NULL;
void (*ScanningLogFile)(CTime&,CTime&) = NULL;

//CString (*GetFlagFile)(void) = NULL;
//void (*SetFlagFile)(CString&) = NULL;

//int sSendZipFile(const CString& ip,int port,const CString& path);

void (*setAutoCopyDB)(BOOL isAuto) = NULL;
//
//typedef void (*PFunSetWebPID)(int pid); 
//
//void (*pSetWebPID)(PFunSetWebPID ) = NULL;

int s_WebPID = 0;
void sSetWebPID(int pid){
	s_WebPID = pid;
	if(s_pListInfo){
		CString str;
		str.Format("PID=%d",pid);
		s_pListInfo->AddString(str);
	}
}

//typedef int (*FunSendZipFile)(const CString& ip,int port,const CString& path);

//void (*sSetFunTest)(FunSendZipFile ) = NULL;
CDmsTestDlgDlg::CDmsTestDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDmsTestDlgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDmsTestDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listInfo);
	DDX_Control(pDX, IDC_EDIT1, m_editPort);
	DDX_Control(pDX, IDC_EDIT2, m_editFilePort);
	DDX_Control(pDX, IDC_IPADDRESS1, m_AddressCtrl);
	DDX_Control(pDX, IDC_EDIT3, m_editDir);
	DDX_Control(pDX, ID_WIZNEXT, m_BtnConnect);
	DDX_Control(pDX, IDC_EDIT4, m_editWebPort);
	DDX_Control(pDX, IDC_CHECK1, m_checkCopyDb);
	DDX_Control(pDX, IDC_EDIT5, m_editOtherExt);
	//DDX_Text(pDX,IDC_EDIT5,m_nExBackupTime);
	DDX_Control(pDX,IDC_EDIT6,m_editFlagFile);
	//DDX_Control(pDX, IDC_COMBO1, m_cbxBackup);
}

BEGIN_MESSAGE_MAP(CDmsTestDlgDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CDmsTestDlgDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_WIZNEXT, &CDmsTestDlgDlg::OnBnClickedWiznext)
	ON_BN_CLICKED(IDC_BUTTON2, &CDmsTestDlgDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDCANCEL, &CDmsTestDlgDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON3, &CDmsTestDlgDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CDmsTestDlgDlg::OnBnClickedClearList)
	ON_BN_CLICKED(IDC_BUTTON4, &CDmsTestDlgDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_CHECK1, &CDmsTestDlgDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON5, &CDmsTestDlgDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CDmsTestDlgDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CDmsTestDlgDlg ��Ϣ�������

BOOL CDmsTestDlgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	//  BOOL ret = WinExec("c:\\TestWeb.bat", SW_SHOW);
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	s_pListInfo = &m_listInfo;
	if(StartMonitorFile()){

		m_checkCopyDb.SetCheck(FALSE);
		setAutoCopyDB(FALSE);
		//m_checkClearDb.ShowWindow(SW_HIDE);

		//::ShowWindow(m_checkClearDb.m_hWnd,SW_HIDE);
		//m_editPort.EnableWindow(FALSE);
		//m_editFilePort.EnableWindow(FALSE);
		//m_AddressCtrl.EnableWindow(FALSE);
		//m_BtnConnect.SetWindowText("�Ͽ�");
	}

	//CString strFileName("Monitor.dll");
	//CStdioFile file;
	//BOOL isResult = file.Open("C:\\2\\run\\config\\Dms.lic",CFile::modeRead|CFile::typeBinary);
	//if(isResult){
	//	ULONGLONG ll = file.GetLength();
	//	char* chRead = new char[ll];
	//	if(file.Read(chRead,ll) == ll){
	//		TRACE("aaaaa");

	//		file.Close();
	//		isResult = file.Open("C:\\2\\config\\Dms.lic",CFile::modeWrite|CFile::modeCreate|CFile::typeBinary);

	//		if (isResult)
	//		{
	//			file.Write(chRead,ll);

	//			SYSTEMTIME st;
	//			st.wYear = 2018;
	//			st.wMonth = 7;
	//			st.wDay = 18;
	//			st.wHour = 11;
	//			st.wMinute = 5;
	//			st.wSecond = 0;

	//			st.wMilliseconds = 0;

	//			SetSystemTime(&st);
	//			file.Close();
	//		}
	//	}

	//	delete [] chRead;
	//}



	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDmsTestDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

BOOL CDmsTestDlgDlg::StartMonitorFile(){

	CHAR szFileName[256];
	memset(szFileName,'0',sizeof(szFileName));

	::GetModuleFileName(NULL,szFileName, sizeof(szFileName)); 

	CString strCurPath;
	GetModuleFileName(NULL,strCurPath.GetBuffer(MAX_PATH),MAX_PATH);
	strCurPath.ReleaseBuffer();//Must ReleaseBuffer, or GetLength=0
	int pos= strCurPath.ReverseFind(_T('\\'));
	strCurPath = strCurPath.Left(pos);
	s_strHomeDir = strCurPath;
	HMODULE hModule = ::LoadLibrary(strCurPath + "\\Monitor.dll");

	BOOL isResult = FALSE;

	if(hModule)
	{
		startMonitorFile = (BOOL  (*)(char*))::GetProcAddress(hModule,_T("startMonitorFile"));
		
		stopMonitorFile = (BOOL  (*)(void))::GetProcAddress(hModule,_T("stopMonitorFile"));
		sGetMonitoringParam = (void (*)(int [],int&,int& ,int& ,CString&,CString&,CString&))::GetProcAddress(hModule,_T("sGetMonitoringParam"));
		sSetMonitoringParam = (void (*)(const CString& ,const CString& ,const CString& ,const CString& ,const CString&,const CString&))::GetProcAddress(hModule,_T("sSetMonitoringParam"));
		sConnectServer = (int (*)(void))::GetProcAddress(hModule,_T("sConnectServer"));
		setCListBox = (void (*)(CListBox& ))::GetProcAddress(hModule,_T("setCListBox"));
		sGetLicInfo = (void (*)(void))::GetProcAddress(hModule,_T("sGetLicInfo"));
		sCheckLicence = (int (*)(char&))::GetProcAddress(hModule,_T("sCheckLicence"));
		sCloseUniverDB = (void(*)(void))::GetProcAddress(hModule,_T("sCloseUniverDB"));
		setAutoCopyDB = (void(*)(BOOL))::GetProcAddress(hModule,_T("setAutoCopyDB"));

		ScanningLogFile =  (void(*)(CTime&,CTime&))::GetProcAddress(hModule,_T("ScanningLogFile"));

		//GetFlagFile = (CString(*)(void))::GetProcAddress(hModule,_T("GetFlagFile"));;
		//SetFlagFile = (void(*)(CString&))::GetProcAddress(hModule,_T("SetFlagFile"));;

		//pSetWebPID = (void (*)(PFunSetWebPID ))::GetProcAddress(hModule,_T("sSetWebPID"));
		//if(pSetWebPID){
		//	pSetWebPID(sSetWebPID);
		//}

		if(setCListBox){
			setCListBox(m_listInfo);
		}

		isResult = startMonitorFile(szFileName);
		if(sGetMonitoringParam){
			CString addRess,bufDir,strFileFlag,strOtherExt;
			int port,filePort,webPort;
			int ips[4];
			int nFlag;

			int del = 0;
			sGetMonitoringParam(ips,port,webPort,filePort,bufDir,strOtherExt,strFileFlag);

			m_AddressCtrl.SetAddress(ips[0],ips[1],ips[2],ips[3]);
			CString strPort;
			strPort.Format("%d",port);
			m_editPort.SetWindowText(strPort);

			strPort.Format("%d",webPort);
			m_editWebPort.SetWindowText(strPort);

			strPort.Format("%d",filePort);
			m_editFilePort.SetWindowText(strPort);
			m_editDir.SetWindowText(bufDir);

			m_editOtherExt.SetWindowText(strOtherExt);

			//strPort.Format("%d",0);
			//m_ExtendBackupTime.SetWindowText(strPort);

			//bufDir = GetFlagFile();

			strPort.Format("%s",strFileFlag);
			m_editFlagFile.SetWindowText(strPort);

			//if(nFlag == 0)
			//	m_cbxBackup.SetCurSel(0);
			//else if (nFlag & 0x1)
			//{
			//	m_cbxBackup.SetCurSel(1);
			//}else{

			//	m_cbxBackup.SetCurSel(2);
			//}
		}

	}else{

	}

	return isResult;
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDmsTestDlgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CDmsTestDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDmsTestDlgDlg::OnBnClickedOk()
{
	if(stopMonitorFile != NULL){
		stopMonitorFile();
		sCloseUniverDB();
	}
	OnOK();
}

void CDmsTestDlgDlg::OnBnClickedWiznext()
{
	CString strText;
	m_BtnConnect.GetWindowText(strText);
	if(!strText.CompareNoCase("�Ͽ�") && stopMonitorFile){
		if(stopMonitorFile()){
			m_editPort.EnableWindow(TRUE);
			m_editFilePort.EnableWindow(TRUE);
			m_AddressCtrl.EnableWindow(TRUE);
			m_editWebPort.EnableWindow(TRUE);
			m_BtnConnect.SetWindowText("����");
		}

	}else{
		if(sConnectServer && sConnectServer() == 0){
			m_editPort.EnableWindow(FALSE);
			m_editFilePort.EnableWindow(FALSE);
			m_AddressCtrl.EnableWindow(FALSE);
			m_editWebPort.EnableWindow(FALSE);
			m_BtnConnect.SetWindowText("�Ͽ�");
		}
	}

}

//ѡ���ļ��жԻ���ص�����  
int CALLBACK BrowseCallBackFun(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)  
{  
	switch(uMsg)  
	{  
	case BFFM_INITIALIZED:  //ѡ���ļ��жԻ����ʼ��  
		//����Ĭ��·��ΪlpData��'D:\'  
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);  
		//��STATUSTEXT������ʾ��ǰ·��  
		::SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, lpData);  
		//����ѡ���ļ��жԻ���ı���  
		::SetWindowText(hwnd, TEXT("�������ø�����Ŀ¼"));   
		break;  
	case BFFM_SELCHANGED:   //ѡ���ļ��б��ʱ  
		{  
			TCHAR pszPath[MAX_PATH];  
			//��ȡ��ǰѡ��·��  
			SHGetPathFromIDList((LPCITEMIDLIST)lParam, pszPath);  
			//��STATUSTEXT������ʾ��ǰ·��  
			::SendMessage(hwnd, BFFM_SETSTATUSTEXT, TRUE, (LPARAM)pszPath);  
		}  
		break;  
	}  
	return 0;  
} 


void CDmsTestDlgDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here  
	TCHAR pszPath[MAX_PATH];  
	BROWSEINFO bi;   
	bi.hwndOwner      = this->GetSafeHwnd();  
	bi.pidlRoot       = NULL;  
	bi.pszDisplayName = NULL;   
	bi.lpszTitle      = TEXT("��ѡ���ļ���");   
	bi.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;  
	bi.lpfn           = BrowseCallBackFun;     //�ص�����  
	bi.lParam         = (LPARAM)TEXT("D:\\");  //�����ص������Ĳ���,����Ĭ��·��  
	bi.iImage         = 0;   

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);  
	if (pidl == NULL)  
	{  
		return;  
	}  

	if (SHGetPathFromIDList(pidl, pszPath))  
	{  
		//AfxMessageBox(pszPath); 
		this->m_editDir.SetWindowText(pszPath);
	} 
}


 

void CDmsTestDlgDlg::OnBnClickedCancel()
{
	if(stopMonitorFile != NULL){
		stopMonitorFile();
		sCloseUniverDB();
	}
	OnCancel();
}

void CDmsTestDlgDlg::OnBnClickedButton3()
{
	BYTE nFild[4];
	UpdateData();
	m_AddressCtrl.GetAddress(nFild[0],nFild[1],nFild[2],nFild[3]);
	CString sIP;
	sIP.Format("%d.%d.%d.%d",nFild[0],nFild[1],nFild[2],nFild[3]);

	CString strPort,webPort,strFileFlag;
	m_editPort.GetWindowText(strPort);

	m_editWebPort.GetWindowText(webPort);

	CString strFilePort,bufDir;
	m_editFilePort.GetWindowText(strFilePort);
	m_editDir.GetWindowText(bufDir);

	CString strCurPath;
	GetModuleFileName(NULL,strCurPath.GetBuffer(MAX_PATH),MAX_PATH);
	strCurPath.ReleaseBuffer();//Must ReleaseBuffer, or GetLength=0
	int pos= strCurPath.ReverseFind(_T('\\'));
	strCurPath = strCurPath.Left(pos);

	CString strCfg = strCurPath + "\\config\\config.cfg";

	CString strOtherExt;
	this->m_editOtherExt.GetWindowText(strOtherExt);

	//int nFlag = m_cbxBackup.GetCurSel();

	//if(nFlag == 0){
	//	nFlag = 0x00;
	//}else if (nFlag == 1)
	//{
	//	nFlag = 0x01;
	//}else{
	//	nFlag = 0x04;
	//}

	CStdioFile file;
	BOOL isResult = file.Open(strCfg,CFile::modeWrite|CFile::modeCreate);
	if(isResult){

		CString strText;
		
		strText.Format("ServerIP =%s\n",sIP);
		file.WriteString(strText);

		strText.Format("logPort =%s\n",strPort);
		file.WriteString(strText);

		strText.Format("filePort =%s\n",strFilePort);
		file.WriteString(strText);

		strText.Format("webPort =%s\n",webPort);
		file.WriteString(strText);
	
		strText.Format("bufDir =%s\n",bufDir);
		file.WriteString(strText);

		strText.Format("otherExt =%s\n",strOtherExt);
		file.WriteString(strText);

		//strText.Format("BackupFlag =%d\n",nFlag);
		//file.WriteString(strText);


		this->m_editFlagFile.GetWindowText(strFileFlag);
		strText.Format("FlagFile =%s\n",strFileFlag);
		file.WriteString(strText);

		file.Close();

		AfxMessageBox("�����ļ��ɹ�"); 
	}



	if(sSetMonitoringParam){
		int del = 0;
		sSetMonitoringParam(sIP,strPort,strFilePort,bufDir,strOtherExt,strFileFlag);
		//SetFlagFile();
	}



}

//BOOL sGetLogUserXP(CString& strName)
//{
//	BOOL bRet = FALSE;
//	strName = _T("");
//	//for xp or above
//	TCHAR *szLogName = NULL;
//	DWORD dwSize = 0;
//	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
//		WTS_CURRENT_SESSION,
//		WTSUserName,
//		&szLogName,
//		&dwSize))
//	{
//		strName = szLogName;
//		WTSFreeMemory(szLogName);
//		bRet = TRUE;
//	}
//	return bRet;
//}
//
//CString sGetLoginName(){
//
//	CString loginName;
//	if(!sGetLogUserXP(loginName)){
//		const int MAX_BUFFER_LEN = 500;  
//		char  szBuffer[MAX_BUFFER_LEN];  
//		DWORD dwNameLen;  
//		dwNameLen = MAX_BUFFER_LEN;  
//		CString loginName = "";
//		if (GetUserName(szBuffer, &dwNameLen))  
//			loginName.Format("%s", szBuffer); 
//	}
//
//	return loginName;
//}

void CDmsTestDlgDlg::OnBnClickedClearList()
{
	m_listInfo.ResetContent();

	//CString loginName ;
	//sGetLogUserXP(loginName);
	//m_listInfo.AddString(loginName);

	//loginName = sGetLoginName();

	//m_listInfo.AddString(loginName);

}

void CAboutDlg::OnBnClickedButton1()
{
	sGetLicInfo();
}

void CAboutDlg::OnBnClickedButton2()
{
	CFileDialog dlg(TRUE, "lic", NULL,
		OFN_EXPLORER | OFN_ENABLESIZING | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST,
		"����ļ�(*.lic)|*.lic|�����ļ�(*.*)|*.*||");

	CString str = "������ļ�";
	dlg.m_pOFN->lpstrTitle = str;
	if (dlg.DoModal() != IDOK)
		return;

	str = dlg.GetPathName();

	CString str1 = s_strHomeDir + "\\Config\\Dms.lic";
	if (str1.CompareNoCase(str))
	{
		if (!CopyFile(str, str1, FALSE))
		{
			AfxMessageBox("��������ļ�����");
			return;
		}
	}

	AfxMessageBox("����ļ��ɹ�����һ�����������Ч��");

}

BOOL CAboutDlg::OnInitDialog()
{
	char chType;
	CString str;
	int nResult = 0;
	CDialog::OnInitDialog();

	// ȡ�����Ϣ
	nResult = sCheckLicence(chType);
	if (chType == 2) // ����
		str = "������:  ������ɡ�";
	else if (nResult >= 1) // ����
		if (chType == 3)
			str.Format("������:  ��������ڻ���%d �졣", nResult);
		else
			str.Format("������:  ����ר��,��������ڻ���%d �졣", nResult);
	else // ��Ч
		str = "������:  �Ƿ���ɡ�";


	//if (nResult == 1) // ��
	//	str = "������:  �������ɡ�";
	//else if (nResult == 4) // ����
	//	str = "������:  ������ɡ�";


	// ���������Ϣ
	m_staLicence.SetWindowText(str);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDmsTestDlgDlg::OnBnClickedButton4()
{
	CAboutDlg dlg;
	dlg.DoModal();
}





void CDmsTestDlgDlg::OnBnClickedCheck1()
{
	if(m_checkCopyDb.GetCheck()){
		setAutoCopyDB(TRUE);
	}else{
		setAutoCopyDB(FALSE);
	}
}

void CDmsTestDlgDlg::OnBnClickedButton5()
{

	CSelectTimeDlg dlg;

	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	if (ScanningLogFile)
	{
		ScanningLogFile(dlg.m_StartTime,dlg.m_EndTime);
	}
}

void CDmsTestDlgDlg::OnBnClickedButton6()
{
	//CFileDialog dlg(TRUE, "lic", NULL,
	//	OFN_EXPLORER | OFN_ENABLESIZING | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST,
	//	"���ݿ��ļ�(*.dat)|*.dat|�����ļ�(*.*)|*.*||");

	//CString str = "������ļ�";
	//dlg.m_pOFN->lpstrTitle = str;
	//if (dlg.DoModal() != IDOK){
	//	str = "";
	//	m_editFlagFile.SetWindowText(str);
	//	return;
	//}

	//str = dlg.GetPathName();
	//m_editFlagFile.SetWindowText(str);

}

void CAboutDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}
