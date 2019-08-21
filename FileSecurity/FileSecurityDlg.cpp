// FileSecurityDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FileSecurity.h"
#include "FileSecurityDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CFileSecurityDlg �Ի���




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


// CFileSecurityDlg ��Ϣ�������

BOOL CFileSecurityDlg::OnInitDialog()
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

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
	m_strFilter1 = "ѹ���ļ�(*.zip)|*.zip|�����ļ�(*.*)|*.*||";
	m_strFilter2 = "ѹ���ļ�(*.zipx)|*.zipx|�����ļ�(*.*)|*.*||";


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFileSecurityDlg::OnPaint()
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

	CString str = "������ļ�";
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

	CString str = "������ļ�";
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
		AfxMessageBox("1���ܽ����ļ�ʧ��");
		return ;
	}

	CFile fileSrc ;
	if(!fileSrc.Open(m_strOrgFileName,CFile::typeBinary|CFile::modeRead)){
		AfxMessageBox("2���ܽ����ļ�ʧ��");
		return;
	}

	CStdioFile file;

	if(m_chkDecryption.GetCheck()){//|CFile::modeNoTruncate
		if(!file.Open(str,CFile::modeCreate|CFile::typeBinary|CFile::modeWrite|CFile::shareDenyNone)){
			AfxMessageBox("3���ܽ����ļ�ʧ��");
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
			AfxMessageBox("3���ܽ����ļ�ʧ��");
		}

	}

	
	fileSrc.Close();




}

void CFileSecurityDlg::OnBnClickedCheck1()
{
	CString str("�����ļ�");
	if(m_chkDecryption.GetCheck()){
		str= "�����ļ�";

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
