#include "StdAfx.h"
#include "FileTransferClient.h"
#include ".\zip\ZipImplement.h"
#include "support.h"
#include "fun.h"

#include <shlwapi.h>


int CFileTransferClient::s_version(0);
//CEncrypt CFileTransferClient::m_Encrypts[4];
CFileTransferClient::CFileTransferClient(void)
{
}

CFileTransferClient::~CFileTransferClient(void)
{
}

void sCopyFileToBuf(CString strZipPath,char type){

	if (type == 3)
	{
		return;
	}
	CString fileName = sGetFileFromPath(strZipPath);
	CString  strDir;
	strDir.Format("%s\\%d",CSupport::GetBufDir(),type);
	if(!PathFileExists(strDir))//�ļ��в������򴴽�-1
		CreateDirectory(strDir, NULL);

	strDir += "\\" + fileName;

	rename(strZipPath,strDir);

}

CString CFileTransferClient::ZipFile(const CString& pathName,vector<CString>& vec,char type,BOOL isConnect){

	//s_RoomId = pathName;
	CString path("");
	if(type == 0){
		path = "backups\\" + pathName;
	}else{
		path = pathName;
	}

	CZipImplement compress(path,type);

	CString strDirPath(CSupport::GetBufDir() + "\\");
	CString  strDir(strDirPath);
	if(!isConnect){

		strDir.Format("%s\\%d\\",CSupport::GetBufDir(),type);
		if(!PathFileExists(strDir))//�ļ��в������򴴽�-1
			CreateDirectory(strDir, NULL);
	}

	CTime time = CTime::GetCurrentTime();
	CString str =time.Format("%Y_%m_%d_%H_%M_%S");//��ʽ������ʱ��  
	//CSupport::GetFileNameForDateTime(strZipFilePath,time);
	CString strZipFilePath;
	strZipFilePath.Format("%supload%s.zip",strDir ,str);
	vector<CString>::iterator it;

	vector<CString> map;
	CString key;
	//for (it = vec.begin(); it != vec.end(); it++){
	int cnt = (int)vec.size() - 1;
	for (int ii = 0 ; ii <  cnt; ii ++){
		key	=  vec[ii];
		map.push_back(key);
	}

	if(map.size() > 0)
		compress.SetZipFile(&map);
	else
		return "";
	//strDirPath += pathName;
	if(!compress.Zip_PackFiles(strDirPath,strZipFilePath,""))//123456789ֻ�Ǹ�Demo�Ͳ����߳�ȥ����
		return "";
	else
		return strZipFilePath;
}

int CFileTransferClient::ConnectFileServer(const CString& pathName,vector<CString>& vec,char type ,BOOL isConnect)      
{ 
	//= s_isConnect;
	CString strZipPath = ZipFile(pathName,vec,type,isConnect);
	if(strZipPath.IsEmpty() || !isConnect)
		return 0;

	CFileTransferClient::ConnectFileServer(strZipPath,type);

	return 0;
}

BOOL CFileTransferClient::SendFile(SOCKET &socket,CString strZipFilePath,char type){


	CFile file;

	if(file.Open(strZipFilePath,CFile::modeRead|CFile::typeBinary)){
		CString fileName = file.GetFileTitle();
		//send(socket, (LPSTR)&fileName,fileName.GetLength(),0);

		int result = send(socket, &type,sizeof(char),0);
		if (result == SOCKET_ERROR)
		{
			return FALSE;
		}

		if (s_version >= 2){
			ULONGLONG len = file.GetLength();
			char ch[8];
			memcpy(ch,&len,8);
			result = send(socket, ch,8,0);
			TRACE("file len%I64d\t%d\n",len,sizeof(len));//
		}

		//CEncrypt en;
		//en.KeyStringToBit("32145678");
		char datas[1024];
		UINT pos = file.Read(datas,1024);
		//if (pos >= 96)
		//{
		//	char chEn[96];
		//	memcpy(chEn,datas,96);

		//	for (int ii(0); ii < 24; ii +=8)
		//	{
		//		m_Encrypts[0].Encrypt(&chEn[ii],&datas[ii]);
		//		ii += 8;
		//		m_Encrypts[1].Encrypt(&chEn[ii],&datas[ii]);
		//		ii += 8;
		//		m_Encrypts[2].Encrypt(&chEn[ii],&datas[ii]);
		//		ii += 8;
		//		m_Encrypts[3].Encrypt(&chEn[ii],&datas[ii]);
		//	}
		//}

		result = send(socket, (LPSTR)datas,pos,0);

		if (result == SOCKET_ERROR)
		{
			file.Close();
			return FALSE;
		}

		while(pos > 0){
			pos = file.Read(datas,pos);

			if(pos > 0){
				result = send(socket, (LPSTR)datas,pos,0);
				if (result == SOCKET_ERROR)
				{
					file.Close();
					return FALSE;
				}
			}
		}

		file.Close();

	}
	DeleteFile(strZipFilePath);

	return TRUE;
}

//void CFileTransferClient::InitEncrypt(){
//
//	m_Encrypts[0].KeyStringToBit("LiFeng89");
//	m_Encrypts[1].KeyStringToBit("75Wdlc98");
//	m_Encrypts[2].KeyStringToBit("09Live27");
//	m_Encrypts[3].KeyStringToBit("JinXia29");
//}

int CFileTransferClient::ConnectFileServer(CString strZipPath,char type ) 
{

	//WSADATA wsaData;      
	//���������׽���      

	//����Զ�̷������ĵ�ַ��Ϣ      
	SOCKADDR_IN server;      


	// Initialize Windows socket library      
	//WSAStartup(0x0202, &wsaData);      

	// �����ͻ����׽���      
	SOCKET FileClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //AF_INETָ��ʹ��TCP/IPЭ���壻      
	//SOCK_STREAM, IPPROTO_TCP����ָ��ʹ��TCPЭ��      
	// ָ��Զ�̷������ĵ�ַ��Ϣ(�˿ںš�IP��ַ��)      
	memset(&server, 0, sizeof(SOCKADDR_IN)); //�Ƚ������ַ��server��Ϊȫ0      
	server.sin_family = PF_INET; //������ַ��ʽ��TCP/IP��ַ��ʽ      
	server.sin_port = htons(CSupport::GetFilePort()) ; //htons(PORT); //ָ�����ӷ������Ķ˿ںţ�htons()���� converts values between the host and network byte order      
	server.sin_addr.s_addr = inet_addr(CSupport::GetServerIP()); //SERVER_ADDRESSָ�����ӷ�������IP��ַ      
	//�ṹSOCKADDR_IN��sin_addr�ֶ����ڱ���IP��ַ��sin_addr�ֶ�Ҳ��һ���ṹ�壬sin_addr.s_addr�������ձ���IP��ַ      
	//inet_addr()���ڽ� �����"127.0.0.1"�ַ���ת��ΪIP��ַ��ʽ      
	//�����ղ�ָ���ķ�������      
	int result = connect(FileClient, (struct sockaddr *) &server, sizeof(SOCKADDR_IN)); //���Ӻ������sClient��ʹ���������      

	if(result == 0){

		//HANDLE hThread = CreateThread(NULL, 0, ThreadProcFileRead, (LPVOID)FileClient, 0, NULL);
		BOOL IsResult = SendFile(FileClient,strZipPath,type);

		if(!IsResult)
			sCopyFileToBuf(strZipPath,type);
	}else{

		sCopyFileToBuf(strZipPath,type);

	}

	// �ͷ����Ӻͽ��н�������  
	//if(type < 2)
	closesocket(FileClient);      
	//WSACleanup();
	return result;      
} 

void CFileTransferClient::SendOldZipFile(){
	TRACE("----------------------------------------------\n");
	CFileFind ff; 
	CString szDir = CSupport::GetBufDir() + "\\0\\upload*.zip"; 

	BOOL res = ff.FindFile(szDir); 
	while(res) 
	{ 
		res = ff.FindNextFile(); 
		if(!ff.IsDirectory() && !ff.IsDots()) 
		{ 
			//��ʾ��ǰ���ʵ��ļ�(����·��) 
			CString strPath = ff.GetFilePath(); 
			TRACE("%s\n",strPath);
			ConnectFileServer(strPath,0);
			Sleep(500);

		} 
	} 


	szDir = CSupport::GetBufDir() + "\\1\\upload*.zip";
	res = ff.FindFile(szDir); 
	while(res) 
	{ 
		res = ff.FindNextFile(); 
		if(!ff.IsDirectory() && !ff.IsDots()) 
		{ 
			//��ʾ��ǰ���ʵ��ļ�(����·��) 
			CString strPath = ff.GetFilePath(); 
			TRACE("%s\n",strPath);
			ConnectFileServer(strPath,1);
			Sleep(500);

		} 
	} 


	szDir = CSupport::GetBufDir() + "\\2\\upload*.zip";
	res = ff.FindFile(szDir); 
	while(res) 
	{ 
		res = ff.FindNextFile(); 
		if(!ff.IsDirectory() && !ff.IsDots()) 
		{ 
			//��ʾ��ǰ���ʵ��ļ�(����·��) 
			CString strPath = ff.GetFilePath(); 
			TRACE("%s\n",strPath);
			ConnectFileServer(strPath,2);
			Sleep(500);

		} 
	}

	szDir = CSupport::GetBufDir() + "\\3\\upload*.zip";
	res = ff.FindFile(szDir); 
	while(res) 
	{ 
		res = ff.FindNextFile(); 
		if(!ff.IsDirectory() && !ff.IsDots()) 
		{ 
			//��ʾ��ǰ���ʵ��ļ�(����·��) 
			CString strPath = ff.GetFilePath(); 
			TRACE("%s\n",strPath);
			ConnectFileServer(strPath,3);
			Sleep(500);

		} 
	} 

	TRACE("----------------------------------------------\n");

	//�ر� 
	ff.Close(); 
}

void CFileTransferClient::SendOnceFile(const CString& RoomId,FILE_SETS& fileSets,BOOL isConnect,char initType ){

	std::set<CString>&  setFilePath = fileSets.mSetFilePath;
	std::set<CString>&  setFileName = fileSets.mSetFileName;

	ULONGLONG size = 0;

	vector<CString> vec;
	set<CString>::iterator it; //����ǰ�������
	//������������е�����Ԫ��
	for(it = setFilePath.begin(); it != setFilePath.end(); it++)
	{
		CFileStatus rStatus;
		rStatus.m_size = 0;
		CString strNewFileName = *it;
		if (strNewFileName.IsEmpty())
		{
			continue;
		}
		if(CFile::GetStatus(strNewFileName,rStatus))
			size += rStatus.m_size;
		//cout << *it << " ";


		if(size >= 104857600){
			TRACE("1upload--------------------------------------\n");
			vec.push_back("0");
			ConnectFileServer(RoomId,vec,0,isConnect);
			vec.clear();
			size = 0;//rStatus.m_size;
			Sleep(1000);
		}
		vec.push_back(strNewFileName);

	}

	if(vec.size() > 0){
		TRACE("2upload--------------------------------------\n");
		vec.push_back("0");
		ConnectFileServer(RoomId,vec,0,isConnect);
		Sleep(1000);
	}

	vec.clear();

	size = 0;
	char type = initType;
	CFileStatus rStatus;
	if(CFile::GetStatus(fileSets.strLogFile,rStatus))
		size += rStatus.m_size;

	vec.push_back(fileSets.strLogFile);
	//������������е�����Ԫ��
	for(it = setFileName.begin(); it != setFileName.end(); it++)
	{

		rStatus.m_size = 0;
		CString strNewFileName = *it;
		if (strNewFileName.IsEmpty())
		{
			continue;
		}
		CString strFilePath = CSupport::GetBufDir() + "\\" + strNewFileName;
		if(CFile::GetStatus(strFilePath,rStatus))
			size += rStatus.m_size;
		//cout << *it << " ";

		if(size >= 104857600){
			TRACE("3upload--------------------------------------\n");
			vec.push_back("1");
			ConnectFileServer(RoomId,vec,type,isConnect);
			type = 1;
			//DeleteFile(CSupport::GetBufDir() + "\\LogFile.txt");
			vec.clear();
			size = 0;//rStatus.m_size;
			Sleep(1000);
		}

		vec.push_back(strNewFileName);
	}

	if(vec.size() > 0){
		TRACE("4upload--------------------------------------\n");
		vec.push_back("1");
		ConnectFileServer(RoomId,vec,type,isConnect);
		//DeleteFile(CSupport::GetBufDir() + "\\LogFile.txt");
	}

}


