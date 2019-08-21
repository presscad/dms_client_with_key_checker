// FileSecurityDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FileSecurity.h"
#include "FileSecurityDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CFileSecurityDlg 对话框




CFileSecurityDlg::CFileSecurityDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileSecurityDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileSecurityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_chkDecryption);
}

BEGIN_MESSAGE_MAP(CFileSecurityDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CFileSecurityDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CFileSecurityDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK1, &CFileSecurityDlg::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CFileSecurityDlg 消息处理程序

BOOL CFileSecurityDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	CString strCurPath;
	GetModuleFileName(NULL,strCurPath.GetBuffer(MAX_PATH),MAX_PATH);
	strCurPath.ReleaseBuffer();//Must ReleaseBuffer, or GetLength=0
	int pos= strCurPath.ReverseFind(_T('\\'));
	strCurPath = strCurPath.Left(pos);


	if (!m_Encrypt.OpenLicence(strCurPath))
	{
		return FALSE;
	}

	m_chkDecryption.SetCheck(1);
	m_strFilter1 = "压缩文件(*.zip)|*.zip|所有文件(*.*)|*.*||";
	m_strFilter2 = "压缩文件(*.zipx)|*.zipx|所有文件(*.*)|*.*||";


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFileSecurityDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFileSecurityDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CFileSecurityDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFileSecurityDlg::OnBnClickedButton1()
{
	CString strFilter;
	if (m_chkDecryption.GetCheck())
	{
		strFilter = m_strFilter1;
	}else{
		strFilter = m_strFilter2;
	}
	CFileDialog dlg(TRUE, "zip", NULL,
		OFN_EXPLORER | OFN_ENABLESIZING | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST,
		strFilter);

	CString str = "打开许可文件";
	dlg.m_pOFN->lpstrTitle = str;
	if (dlg.DoModal() != IDOK)
		return;

	str = dlg.GetPathName();

	m_strOrgFileName = str;
	CWnd* cwnd = GetDlgItem(IDC_EDIT1);
	if(cwnd){
		cwnd->SetWindowText(str);
	}

}

void CFileSecurityDlg::OnBnClickedButton2()
{
	CString strFilter;
	CString ext("zip");
	if (m_chkDecryption.GetCheck())
	{
		strFilter = m_strFilter2;
		ext = "zipx";
	}else{
		strFilter = m_strFilter1;
	}
	CFileDialog dlg(FALSE, ext, NULL,
		OFN_EXPLORER | OFN_ENABLESIZING | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST,
		strFilter);

	CString str = "打开许可文件";
	dlg.m_pOFN->lpstrTitle = str;
	if (dlg.DoModal() != IDOK)
		return;

	str = dlg.GetPathName();

	m_strNewFileName = str;
	CWnd* cwnd = GetDlgItem(IDC_EDIT2);
	if(cwnd){
		cwnd->SetWindowText(str);
	}

	char licType;
	if (CEncrypt::CheckLicence("DmsClien",licType) == 0)
	{
		AfxMessageBox("1加密解密文件失败");
		return ;
	}

	CFile fileSrc ;
	if(!fileSrc.Open(m_strOrgFileName,CFile::typeBinary|CFile::modeRead)){
		AfxMessageBox("2加密解密文件失败");
		return;
	}

	CStdioFile file;

	if(m_chkDecryption.GetCheck()){//|CFile::modeNoTruncate
		if(!file.Open(str,CFile::modeCreate|CFile::typeBinary|CFile::modeWrite|CFile::shareDenyNone)){
			AfxMessageBox("3加密解密文件失败");
			fileSrc.Close();
			return;	
		}
		int head(321456);
		file.Write(&head,sizeof(int));
		char chVal[8];
		char chOutVal[8];
		UINT len(0); 
		while(len = fileSrc.Read(chVal,8) > 0){
			m_Encrypt.Encrypt(chVal,chOutVal);
			file.Write(chOutVal,8);
		}

		file.Close();

	}else{

		int head;
		fileSrc.Read(&head,sizeof(int));
		if(head == 321456 && file.Open(str,CFile::modeCreate|CFile::typeBinary|CFile::modeWrite)){

			char chVal[8];
			char chOutVal[8];
			UINT len(0); 

			while(len = fileSrc.Read(chVal,8) > 0){
				m_Encrypt.Decrypt(chVal,chOutVal);
				file.Write(chOutVal,8);
			}

			file.Close();
			
		}else{
			AfxMessageBox("3加密解密文件失败");
		}

	}

	
	fileSrc.Close();




}

void CFileSecurityDlg::OnBnClickedCheck1()
{
	CString str("解密文件");
	if(m_chkDecryption.GetCheck()){
		str= "加密文件";

	}

	CWnd* cwnd = GetDlgItem(IDC_BUTTON2);
	if(cwnd){
		cwnd->SetWindowText(str);
	}

	cwnd = GetDlgItem(IDC_EDIT1);
	if(cwnd){
		cwnd->SetWindowText("");
	}

	cwnd = GetDlgItem(IDC_EDIT2);
	if(cwnd){
		cwnd->SetWindowText("");
	}

	m_strOrgFileName = "";
	m_strNewFileName = "";
}
