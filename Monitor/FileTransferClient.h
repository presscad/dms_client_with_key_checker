#pragma once

#include <afxsock.h>
#include <vector>
#include <set>
#include "..\SuperDb\Encrypt.h"

using namespace std;


typedef struct _file_sets{
	std::set<CString>  mSetFilePath;
	std::set<CString>  mSetFileName;

	CString strLogFile;
}FILE_SETS;



class CFileTransferClient
{
public:
	CFileTransferClient(void);
public:
	~CFileTransferClient(void);
	static int ConnectFileServer(const CString& pathName,vector<CString>& vec,char type,BOOL isConnect);
	static int ConnectFileServer(CString strZipPath,char type );
	static BOOL SendFile(SOCKET &socket,CString strZipFilePath,char type);
	static CString ZipFile(const CString& pathName,vector<CString>& vec,char type,BOOL isConnect);
	//static void SendOnceFile(FILE_SETS& fileSets);
	static void SendOnceFile(const CString& RoomId,FILE_SETS& fileSets,BOOL isConnect,char initType = 2);
	//static DWORD WINAPI ThreadProcSendOnceFile(LPVOID lpParam) ;
	static void SendOldZipFile();
	static int s_version;

	//static CEncrypt m_Encrypts[4];

	//static void InitEncrypt();

};
