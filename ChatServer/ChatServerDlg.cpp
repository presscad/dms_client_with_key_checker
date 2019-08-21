// ChatServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChatServer.h"
#include "ChatServerDlg.h"
#include "..\BaseDb\fun.h"
#include "..\BaseDb\Encrypt.h"
//#include "DataOperator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
CString s_strHomeDir = "";
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
	virtual BOOL OnInitDialog();
	CStatic m_staLicence;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedButton2();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_STATIC1, m_staLicence);
	//}}AFX_DATA_MAP
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ȡ�����Ϣ
	CString str;
	int nResult = CEncrypt::CheckLicence("DmsClien");
	if (nResult == 1) // ��
		str = "������:  �������ɡ�";
	else if (nResult == 2) // ����
		str = "������:  ������ɡ�";
	else if (nResult == 4) // ����
		str = "������:  ������ɡ�";
	else if (nResult >= 10) // ����
		str.Format("������:  ��������ڻ���%d �졣", nResult - 10 + 1);
	else // ��Ч
		str = "������:  �Ƿ���ɡ�";

	// ���������Ϣ
	m_staLicence.SetWindowText(str);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CAboutDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CAboutDlg::OnBnClickedButton2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatServerDlg dialog



CChatServerDlg::CChatServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatServerDlg::IDD, pParent),m_isConnect(FALSE)
{
	//{{AFX_DATA_INIT(CChatServerDlg)
	m_sWords = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChatServerDlg)
	DDX_Control(pDX, IDC_LIST_WORDS, m_ListWords);
	DDX_Control(pDX, IDC_BUTTON_SEND, m_ButtonSend);
	DDX_Text(pDX, IDC_EDIT_WORDS, m_sWords);
	//}}AFX_DATA_MAP
//	DDX_Control(pDX, IDC_EDIT2, m_editText);
	DDX_Control(pDX, IDC_EDIT1, m_editDir);
	DDX_Control(pDX, IDC_EDIT2, m_editPort);
}

BEGIN_MESSAGE_MAP(CChatServerDlg, CDialog)
	//{{AFX_MSG_MAP(CChatServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, OnButtonAbout)
	ON_BN_CLICKED(IDC_BUTTON1, &CChatServerDlg::OnBnClickedButtonDir)
	ON_BN_CLICKED(IDC_BUTTON2, &CChatServerDlg::OnBnClickedButtonSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatServerDlg message handlers

BOOL CChatServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	ReadConfig();
	CString strPort;
	m_editPort.GetWindowText(strPort);
	int port = toInt(strPort);
	if(port > 10){
		m_ListenSocket.Create(port);
		m_ListenSocket.Listen(15);
	}else{
		AfxMessageBox("��������ʧ��"); 
	}

	
	/////////////////////////////

	CHAR szFileName[256];
	memset(szFileName,'0',sizeof(szFileName));

	::GetModuleFileName(NULL,szFileName, sizeof(szFileName)); 

	CString strCurPath;
	GetModuleFileName(NULL,strCurPath.GetBuffer(MAX_PATH),MAX_PATH);
	strCurPath.ReleaseBuffer();//Must ReleaseBuffer, or GetLength=0
	int pos= strCurPath.ReverseFind(_T('\\'));
	strCurPath = strCurPath.Left(pos);
	s_strHomeDir = strCurPath;

	OpenServerLicense();

	//CDataOperator dataOperator;
	//if(dataOperator.OpenDatabase("dms","root","123456")){
	//vector<OBJ_FILE_LOG> vecObjFileLog;
	//dataOperator.SelectFromFileLog(vecObjFileLog);
	//}


	//m_pMySQL = mysql_init(NULL);//ʧ�ܷ���NULL,һ�㲻��ʧ��   
	//if (nullptr == mysql_real_connect(m_pMySQL, "localhost", "root", "", "hundun", 3306, NULL, 0)){
	//	MessageBox(_T("���ݿ�����ʧ��!"));
	//}
	///////////////////
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChatServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChatServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChatServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CChatServerDlg::OnButtonSend() 
{
		UpdateData();

		SEND_INFO sendInfo(1,m_sWords);
		m_ServerSocket.Send(&sendInfo,m_sWords.GetLength() + 2);
        m_ListWords.AddString("����: "+m_sWords);
        m_ListWords.SetTopIndex(m_ListWords.GetCount()-1);	
}

void CChatServerDlg::OnButtonClear() 
{
	// TODO: Add your control notification handler code here
       m_ListWords.ResetContent();

	   //CString path("d:\\test");

	   //path += ",";
	   //path += "AF\\AVBCFSBD\\GTENCHBDJDARFKHIJKBPGTIABHEOAGHQEFDYEFDUJRCMHQDPHJCCGIEBEXHSEV";
	   //path += ",";
	   //path += "AF\\AVBCFSBD\\GTENCHBDJDARFKHIJKBPGTIABHEOAHCECVJAIRAPHDIUHVEYJSEEGGGWFUFRFX";

	   //path += ",";
	   //path += "AF\\AVBCFSBD\\GTENCHBDJDARFKHIELHYAOHCGBBS\\GTENCHBDJDARFKHIJKBPGTIABHEOAGHQEFDYEFDUJRCMHQDPHJCCGIEBEXHSEV";
	   //SEND_INFO sendInfo(10,path);
	   //m_ServerSocket.Send(&sendInfo,path.GetLength() + 2);

}

void CChatServerDlg::OnButtonAbout() 
{
	// TODO: Add your control notification handler code here
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();	
}

void CChatServerDlg::addServerSockets()
{
	
}

CChatServerDlg::~CChatServerDlg()
{
	int cnt = (int)this->m_arrayServerSockets.GetCount();
	for (int ii = 0 ; ii < cnt ; ii ++)
	{
		delete m_arrayServerSockets.GetAt(ii);
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

void CChatServerDlg::OnBnClickedButtonDir()
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

void CChatServerDlg::ReadConfig(){
	CString strCurPath;
	GetModuleFileName(NULL,strCurPath.GetBuffer(MAX_PATH),MAX_PATH);
	strCurPath.ReleaseBuffer();//Must ReleaseBuffer, or GetLength=0
	int pos= strCurPath.ReverseFind(_T('\\'));
	strCurPath = strCurPath.Left(pos);

	CString strCfg = strCurPath + "\\config\\configServer.cfg";

	CString strPort,bufDir;
	m_editPort.GetWindowText(strPort);
	m_editDir.GetWindowText(bufDir);

	CStdioFile file;
	BOOL isResult = file.Open(strCfg,CFile::modeRead);
	if(isResult){

		CString strText;

		while(file.ReadString( strText )){
			 if(strText.Find(_T("Port")) >= 0){
				pos = strText.Find(_T("=")) + 1;
				strText = strText.Right(strText.GetLength() - pos);
				strText.Trim();
				m_editPort.SetWindowText(strText);
				TRACE(_T("filePort:%s\n"),strText);
			}else if(strText.Find(_T("bufDir")) >= 0){
				pos = strText.Find(_T("=")) + 1;
				strText = strText.Right(strText.GetLength() - pos);
				strText.Trim();
				m_editDir.SetWindowText(strText);
				TRACE(_T("bufDir:%s\n"),strText);
			}
			// 
		}

		file.Close();

	}
}

int CChatServerDlg::toInt(LPCSTR strVal)
{
	if (strlen(strVal) <= 0)
		return 0;

	long nVal = 0;
	sscanf_s(strVal, "%d", &nVal);
	return nVal;
}

CString CChatServerDlg::GetBufDir(){
	CString bufDir;
	this->m_editDir.GetWindowText(bufDir);
	return bufDir;
}


int CChatServerDlg::CheckLicence(){
	return CEncrypt::CheckLicence("DmsClien");
}


// �򿪷��������
BOOL CChatServerDlg::OpenServerLicense()
{
	CEncrypt enc;
	if (!enc.OpenLicence("FlyF0xcn",s_strHomeDir))
	{
		//CAboutDlg dlg(NULL);
		//dlg.DoModal();
		return FALSE;
	}
	if (CEncrypt::CheckLicence("DmsClien") == 0)
		 
	{
		//CAboutDlg dlg(NULL);
		//dlg.DoModal();
		return FALSE;
	}


	int m_maxLic;
	int m_maxSvc;

	//long time;
	//int maxClient;
	//int maxLogin;

	char ch;
	CString str;
	int ii, pos, val;
	CStringArray& arrName = CEncrypt::sGetArrName();
	int cnt = (int)arrName.GetCount();
	for (ii = 0; ii < cnt; ii ++)
	{
		str = arrName[ii];

		ch = str[0];
		if (ch == '#') // ����˶˿ں�
			m_usPort = htons((u_short)sToInteger(str.Mid(1)));
		else if (ch == '@') // ���������ͷ�����
		{
			pos = 0;
			val = sToInteger(sGetSubStr(str, ';', pos + 1, &pos));
			if (val > 0)
				m_maxLogin = val;

			val = sToInteger(sGetSubStr(str, ';', pos + 1));
			if (val > 0)
				m_maxClient = val;
		}
	}

	if (m_usPort == 0)
	{
		//CAboutDlg dlg(NULL);
		//dlg.DoModal();
		return FALSE;
	}

	return TRUE;
}

void GetLicInfo()
{
	CFileDialog dlg(FALSE, "lin", NULL,
		OFN_EXPLORER | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
		"������������ļ�(*.lin)|*.lin||");

	CString str = "����������������ļ�";
	dlg.m_pOFN->lpstrTitle = str;
	if (dlg.DoModal() != IDOK)
		return;

	str = dlg.GetPathName();

	CStringArray arrAddress;
	CEncrypt::GetMacAddress(&arrAddress);

	int cnt = (int)arrAddress.GetCount();
	if (cnt == 0)
	{
		AfxMessageBox("���ܻ������������ϣ����������̻򿪷�����ϵ��");
		return;
	}

	FILE* fp(NULL);
	fopen_s(&fp, str, "wb");
	if (fp == NULL)
	{
		AfxMessageBox("���ܴ���������������ļ�");
		return;
	}

	CString strText;
	for (int ii = 0; ii < cnt; ii ++)
	{
		strText.Format("MacAddress = %s\r\n", arrAddress[ii]);
		fwrite(strText, 1, strText.GetLength(), fp);
	}

	fclose(fp);
}



void CChatServerDlg::OnBnClickedButtonSave(){
	CString strCurPath;
	GetModuleFileName(NULL,strCurPath.GetBuffer(MAX_PATH),MAX_PATH);
	strCurPath.ReleaseBuffer();//Must ReleaseBuffer, or GetLength=0
	int pos= strCurPath.ReverseFind(_T('\\'));
	strCurPath = strCurPath.Left(pos);
	strCurPath.Replace("\.","");

	CString strCfg = strCurPath + "\\config\\config.cfg";

	CString strPort,bufDir;
	m_editPort.GetWindowText(strPort);
	m_editDir.GetWindowText(bufDir);

	CStdioFile file;
	BOOL isResult = file.Open(strCfg,CFile::modeWrite|CFile::modeCreate);
	if(isResult){

		CString strText;

		strText.Format("Port =%s\n",strPort);
		file.WriteString(strText);

		int port = toInt(strPort);
		if(port > 10){
			m_ListenSocket.Detach();
			m_ListenSocket.Create(port);
			m_ListenSocket.Listen(15);
		}else{
			AfxMessageBox("��������ʧ��"); 
		}

		strText.Format("bufDir =%s\n",bufDir);
		file.WriteString(strText);

		file.Close();

		AfxMessageBox("�����ļ��ɹ�"); 
	}
}


void CAboutDlg::OnBnClickedButton1()
{
	GetLicInfo();
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
