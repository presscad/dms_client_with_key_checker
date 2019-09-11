#include "StdAfx.h"
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm> 
#include <openssl/opensslv.h>
#include <openssl/rsa.h>
#include <openssl/des.h>
#include <openssl/pem.h>
#include <openssl/err.h>
using namespace std;

#include "Encrypts.h"

#ifdef _DEBUG
#pragma comment(lib, "libcrypto.lib")
#else
#pragma comment(lib, "libcrypto.lib")
#endif // DEBUG


CStringArray CEncrypts::m_arrStrName;
CEncrypts::CEncrypts(void)
{
}

CEncrypts::~CEncrypts(void)
{
}

string getRSAKey(string ff)
{
	string ret;
	ifstream infile(ff, ios_base::in | ios_base::binary);
	ostringstream inbuf; inbuf << infile.rdbuf();
	ret = inbuf.str();
	infile.close();
	return ret;
}

// 公钥解密    
std::string rsa_pub_decrypt(const std::string &encText, std::string &sKey)
{
	std::string strRet;
	BIO *keybio = BIO_new_mem_buf((unsigned char *)sKey.c_str(), -1);

	EVP_PKEY* evpkey = NULL;
	PEM_read_bio_PUBKEY(keybio, &evpkey, NULL, NULL);
	RSA* rsa = EVP_PKEY_get1_RSA(evpkey);

	int len = RSA_size(rsa);
	char *clearText = (char *)malloc(len + 1);
	memset(clearText, 0, len + 1);

	//解密
	int ret = RSA_public_decrypt(encText.length(), (const unsigned char*)encText.c_str(), (unsigned char*)clearText, rsa, RSA_PKCS1_PADDING);
	if (ret >= 0){ strRet = std::string(clearText, ret); }
	else{ char err[128]; printf("%s\n", ERR_error_string(ERR_get_error(), err)); }


	// 释放内存  
	free(clearText);
	BIO_free_all(keybio);
	//RSA_free(rsa);
	EVP_cleanup();
	return strRet;
}

//公钥解密 + 分片
std::string rsa_pub_split128_decrypt(const std::string &encText, std::string &sKey)
{
	std::string result;
	std::string input;
	result.clear();
	for (int i = 0; i< encText.length() / 128; i++)
	{
		input.clear();
		input.assign(encText.begin() + i * 128, encText.begin() + i * 128 + 128);

		result = result + rsa_pub_decrypt(input, sKey);
	}
	if (encText.length() % 128 != 0)
	{
		int tem1 = encText.length() / 128 * 128;
		int tem2 = encText.length() - tem1;
		input.clear();
		input.assign(encText.begin() + tem1, encText.end());
		result = result + rsa_pub_decrypt(input, sKey);
	}
	return result;
}

int verify(const unsigned char* msg, size_t mlen, const unsigned char* sig, size_t slen, unsigned char* pkey)
{
	int result = -1;
	if (!msg || !mlen || !sig || !slen || !pkey) return result;

	EVP_PKEY *evpkey = EVP_PKEY_new();
	BIO *bp = BIO_new(BIO_s_mem());
	BIO_write(bp, pkey, strlen((char*)pkey));
	evpkey = PEM_read_bio_PUBKEY(bp, &evpkey, NULL, NULL);

	EVP_MD_CTX* ctx = EVP_MD_CTX_create();
	do
	{
		const EVP_MD* md = EVP_get_digestbyname("SHA1");
		int rc = EVP_DigestInit_ex(ctx, md, NULL);
		rc = EVP_DigestVerifyInit(ctx, NULL, md, NULL, evpkey);
		rc = EVP_DigestVerifyUpdate(ctx, msg, mlen);
		/* Clear any errors for the call below */
		ERR_clear_error();
		rc = EVP_DigestVerifyFinal(ctx, sig, slen);
		char err[128]; printf("%s\n", ERR_error_string(ERR_get_error(), err));
		result = 1;
	} while (0);
	if (ctx) {
		EVP_MD_CTX_destroy(ctx);
		ctx = NULL;
	}
	return !!result;
}

BOOL CEncrypts::OpenLicence(CString& strPath){
	BOOL ret = false;
	string str = strPath + "\\config\\pubkey.pem";
	string strsa = getRSAKey(str);
	str = strPath + "\\config\\Dms.lic";
	ifstream infile(str, ios_base::in | ios_base::binary);
	infile.seekg(0, ios_base::end);
	int len = infile.tellg();
	infile.seekg(ios_base::beg);
	char * buffer = new char[len];
	infile.read(buffer, len);

	string out;
	out = rsa_pub_split128_decrypt(string(buffer, len), strsa);
	size_t pos = out.find("\n\r\n\r");
	string firstEncoderData(out.begin(), out.begin() + pos);
	string secondSignData(out.begin() + pos + 4, out.end());
	ret = verify((unsigned char*)firstEncoderData.c_str(), firstEncoderData.size(), (unsigned char*)secondSignData.c_str(), secondSignData.size(), (unsigned char*)strsa.c_str());
	if (!ret) return ret;
	out = rsa_pub_split128_decrypt(firstEncoderData, strsa);
	istringstream ss; ss.str(out); string minfo = sGetLinInfo();
	BOOL bMacOK = FALSE; BOOL bBoardOK = FALSE; BOOL bCPUOK = FALSE; BOOL hasMAC = FALSE; BOOL hasBoard = FALSE; BOOL hasCPU = FALSE;
	for (string line; getline(ss, line);){
		for_each(line.begin(), line.begin() + 9, [](char & c){ c = ::toupper(c); });
		auto itr = line.begin(); string label = string(itr, itr + 9); char cc[32] = { 0 };
		if (label == "LICENSEID"){
			// DMS license 1, das license 2
			itr += 10; 	cc[0] = 0x20;	cc[1] = *(itr); m_arrStrName.SetAtGrow(0x20, cc); continue;
		}
		else if (label == "LICENSETY"){
			//证书类型;
			itr += 12;  cc[0] = 0x21;   cc[1] = *(itr); m_arrStrName.SetAtGrow(0x21, cc); continue;
		}
		else if (label == "PRINTCLIE")
		{
			//最大许可数和服务数
			itr += 17; cc[0] = 0x39;  strcpy(cc + 1, &*itr);	m_arrStrName.SetAtGrow(0x39, cc); continue; 
		}
		else if (label == "USERCOUNT"){
			itr += 10; cc[0] == 0x38;  strcpy(cc + 1, &*itr);   m_arrStrName.SetAtGrow(0x38, cc); continue;
		}
		else if (label == "EXPIREDAY")
		{
			//过期天数
			itr += 10; cc[0] = 0x40;
			time_t rawtime;
			struct tm timeinfo{0};
			char buffer[80];
			time(&rawtime);
			sscanf(&*itr, "%d-%d-%d", &timeinfo.tm_year, &timeinfo.tm_mon, &timeinfo.tm_mday);
			timeinfo.tm_hour = 0; timeinfo.tm_min = 0; timeinfo.tm_sec = 0;
			timeinfo.tm_year -= 1900; timeinfo.tm_mon -= 1;
			time_t expired = mktime(&timeinfo);
			expired = (expired - rawtime) / (60 * 60 * 24);
			itoa(expired, cc + 1, 10);
			m_arrStrName.SetAtGrow(0x40, cc);
		}
		else if (label == "LICENSENA")
		{
			itr += 12; cc[0] = 0x37; strcpy(cc + 1, &*itr); m_arrStrName.SetAtGrow(0x37, cc); continue;
		}
		else if (label == "MACADDRES")
		{
			itr += 13; 
			if (line.end() - itr == 0) continue;
			char* pos = strstr(&*itr, ";");
			if (!pos) {
				pos = strstr(&*itr, ",");
			}
			if (!pos) pos = &(*line.rbegin());
			string mac(&*itr, pos);
			ret = (string::npos != minfo.find(mac));
			if (ret) bMacOK = TRUE;
			hasMAC = TRUE;
		}
		else if (label == "CPUSERIAL")
		{
			itr += 13;
			if (line.end() - itr == 0) continue;
			char* pos = strstr(&*itr, ";");
			if (!pos) {
				pos = strstr(&*itr, ",");
			}
			if (!pos) pos = &(*line.rbegin());
			string sscpu(&*itr, pos);
			ret = (string::npos != minfo.find(sscpu));
			if (ret) bCPUOK = TRUE;
			hasCPU = TRUE;
		}
		else if (label == "MOTHERBOA")
		{
			itr += 17;
			if (line.end() - itr == 0) continue;
			char* pos = strstr(&*itr, ";");
			if (!pos) {
				pos = strstr(&*itr, ",");
			}
			if (!pos) pos = &(*line.rbegin());
			string ssbd(&*itr, pos);
			ret = (string::npos != minfo.find(ssbd));
			if (ret) bBoardOK = TRUE;
			hasBoard = TRUE;
		}
	}
	bBoardOK = hasBoard & bBoardOK | !hasBoard;
	bCPUOK = hasCPU & bCPUOK | !hasCPU;
	bMacOK = hasMAC & bMacOK | !hasMAC;
	m_arrStrName.SetAtGrow(0x22, "\"14875");
	m_arrStrName.SetAtGrow(0x23, "#2");  //服务端端口号

	ret = bMacOK & bCPUOK & bBoardOK;
	if (!ret) m_arrStrName.RemoveAll();

	return ret;
}

//--------------------------------------------------------------
//						MACADDR 获取不到时为 None
//--------------------------------------------------------------
BOOL GetMacAddrByCmd(char *lpszBaseBoard, int len)
{
	const long MAX_COMMAND_SIZE = 10000; // 命令行输出缓冲大小	
	CHAR szFetCmd[] = "getmac /NH"; // mac	

	BOOL   bret = FALSE;
	HANDLE hReadPipe = NULL; //读取管道
	HANDLE hWritePipe = NULL; //写入管道	
	PROCESS_INFORMATION pi;   //进程信息	
	STARTUPINFO			si;	  //控制命令行窗口信息
	SECURITY_ATTRIBUTES sa;   //安全属性

	char			szBuffer[MAX_COMMAND_SIZE + 1] = { 0 }; // 放置命令行结果的输出缓冲区
	char*			ptr = szBuffer;
	string			strBuffer;
	unsigned long	count = 0;
	long			ipos = 0;

	memset(&pi, 0, sizeof(pi));
	memset(&si, 0, sizeof(si));
	memset(&sa, 0, sizeof(sa));

	pi.hProcess = NULL;
	pi.hThread = NULL;
	si.cb = sizeof(STARTUPINFO);
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	//1.0 创建管道
	bret = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
	if (!bret)
	{
		goto END;
	}

	//2.0 设置命令行窗口的信息为指定的读写管道
	GetStartupInfo(&si);
	si.hStdError = hWritePipe;
	si.hStdOutput = hWritePipe;
	si.wShowWindow = SW_HIDE; //隐藏命令行窗口
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	//3.0 创建获取命令行的进程
	bret = CreateProcess(NULL, szFetCmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
	if (!bret)
	{
		goto END;
	}

	//4.0 读取返回的数据
	WaitForSingleObject(pi.hProcess, INFINITE);
	bret = ReadFile(hReadPipe, szBuffer, MAX_COMMAND_SIZE, &count, 0);
	if (!bret)
	{
		goto END;
	}

	//5.0 查找序列号
	bret = FALSE;
	strBuffer = szBuffer;
	ipos = strBuffer.find("\r\n");
	if (ipos < 0) // 没有找到
	{
		goto END;
	}
	else
	{
		strBuffer = strBuffer.substr(ipos + 2);
		ptr = szBuffer + ipos + 2;
	}
	//ipos = strBuffer.find("\r\n");
	//if (ipos < 0) // 没有找到
	//{		
	//	goto END;
	//}
	//else
	//{
	//	strBuffer = strBuffer.substr(ipos+2);
	//	ptr= szBuffer + ipos + 2;
	//}
	ipos = strBuffer.find(" ");
	strBuffer = strBuffer.substr(0, ipos);


	memset(szBuffer, 0x00, sizeof(szBuffer));
	strcpy_s(szBuffer, strBuffer.c_str());

	//去掉中间的空格 \r \n -
	int j = 0;
	for (int i = 0; i < strlen(szBuffer); i++)
	{
		if (szBuffer[i] != ' ' && szBuffer[i] != '\n' && szBuffer[i] != '\r' && szBuffer[i] != '-')
		{
			lpszBaseBoard[j] = szBuffer[i];
			j++;
		}
	}

	lpszBaseBoard[j] = '\0';

	bret = TRUE;

END:
	//关闭所有的句柄
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return(bret);
}

//--------------------------------------------------------------
//						主板序列号 -- 获取不到时为 None
//--------------------------------------------------------------
BOOL GetBaseBoardByCmd(char *lpszBaseBoard, int len/*=128*/)
{
	const long MAX_COMMAND_SIZE = 10000; // 命令行输出缓冲大小	
	CHAR szFetCmd[] = "wmic BaseBoard get SerialNumber"; // 获取主板序列号命令行	
	const string strEnSearch = "SerialNumber"; // 主板序列号的前导信息

	BOOL   bret = FALSE;
	HANDLE hReadPipe = NULL; //读取管道
	HANDLE hWritePipe = NULL; //写入管道	
	PROCESS_INFORMATION pi;   //进程信息	
	STARTUPINFO			si;	  //控制命令行窗口信息
	SECURITY_ATTRIBUTES sa;   //安全属性

	char			szBuffer[MAX_COMMAND_SIZE + 1] = { 0 }; // 放置命令行结果的输出缓冲区
	string			strBuffer;
	unsigned long	count = 0;
	long			ipos = 0;

	memset(&pi, 0, sizeof(pi));
	memset(&si, 0, sizeof(si));
	memset(&sa, 0, sizeof(sa));

	pi.hProcess = NULL;
	pi.hThread = NULL;
	si.cb = sizeof(STARTUPINFO);
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	//1.0 创建管道
	bret = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
	if (!bret)
	{
		goto END;
	}

	//2.0 设置命令行窗口的信息为指定的读写管道
	GetStartupInfo(&si);
	si.hStdError = hWritePipe;
	si.hStdOutput = hWritePipe;
	si.wShowWindow = SW_HIDE; //隐藏命令行窗口
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	//3.0 创建获取命令行的进程
	bret = CreateProcess(NULL, szFetCmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
	if (!bret)
	{
		goto END;
	}

	//4.0 读取返回的数据
	WaitForSingleObject(pi.hProcess, 500/*INFINITE*/);
	bret = ReadFile(hReadPipe, szBuffer, MAX_COMMAND_SIZE, &count, 0);
	if (!bret)
	{
		goto END;
	}

	//5.0 查找主板序列号
	bret = FALSE;
	strBuffer = szBuffer;
	ipos = strBuffer.find(strEnSearch);

	if (ipos < 0) // 没有找到
	{
		goto END;
	}
	else
	{
		strBuffer = strBuffer.substr(ipos + strEnSearch.length());
	}

	memset(szBuffer, 0x00, sizeof(szBuffer));
	strcpy_s(szBuffer, strBuffer.c_str());

	//去掉中间的空格 \r \n
	int j = 0;
	for (int i = 0; i < strlen(szBuffer); i++)
	{
		if (szBuffer[i] != ' ' && szBuffer[i] != '\n' && szBuffer[i] != '\r')
		{
			lpszBaseBoard[j] = szBuffer[i];
			j++;
		}
	}

	lpszBaseBoard[j] = '\0';

	bret = TRUE;

END:
	//关闭所有的句柄
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return(bret);
}

CString GetCPUID()
{
	CString CPUID;
	unsigned long s1, s2;
	unsigned char vendor_id[] = "------------";
	char sel;
	sel = '1';
	CString VernderID;
	CString MyCpuID, CPUID1, CPUID2;
	switch (sel)
	{
	case '1':
		__asm{
			xor eax, eax      //eax=0:取Vendor信息
				cpuid    //取cpu id指令，可在Ring3级使用
				mov dword ptr vendor_id, ebx
				mov dword ptr vendor_id[+4], edx
				mov dword ptr vendor_id[+8], ecx
		}
		VernderID.Format("%s-", vendor_id);
		__asm{
			mov eax, 01h   //eax=1:取CPU序列号
				xor edx, edx
				cpuid
				mov s1, edx
				mov s2, eax
		}
		CPUID1.Format("%08X%08X", s1, s2);
		__asm{
			mov eax, 03h
				xor ecx, ecx
				xor edx, edx
				cpuid
				mov s1, edx
				mov s2, ecx
		}
		CPUID2.Format("%08X%08X", s1, s2);
		break;
	case '2':
	{
		__asm{
			mov ecx, 119h
				rdmsr
				or eax, 00200000h
				wrmsr
		}
	}
	//AfxMessageBox("CPU id is disabled.");
	break;
	}
	MyCpuID = CPUID1;//+CPUID2;
	CPUID = MyCpuID;
	return CPUID;
}

CString CEncrypts::sGetLinInfo(){
	CStringArray arrMac;
	CEncrypts::GetMacAddress(&arrMac);
	CString strResult = "";
	int cnt = (int)arrMac.GetCount();
	for (int ii = 0; ii < cnt; ii++)
	{
		strResult += "MacAddress = ";
		strResult += arrMac[ii] + "\r\n";
	}

	char lpszBaseBoard[128];
	if (GetBaseBoardByCmd(lpszBaseBoard, 128)){
	}
	strResult += "MotherboardSN = ";  strResult += lpszBaseBoard;  strResult += "\r\n";
	strResult += "CPUSerial = "; strResult += GetCPUID() +"\r\n";
	return strResult;
}


BOOL CEncrypts::OpenLicence(){
	return m_arrStrName.GetCount()>0;
}


int CEncrypts::CheckLicence(CString strPath,char & type){
	type = 0;
	if(!OpenLicence()) return type;

	string strType = m_arrStrName.GetAt(0x21);
	type = atoi(&strType[1]);
	////////
	string strcli = m_arrStrName.GetAt(0x40);
	return atoi(&strcli[1]);
}

CStringArray& CEncrypts::sGetArrName(){
	return m_arrStrName;
}


void CEncrypts::GetMacAddress(CStringArray* arrAddress){
	char addrs[1000];
	int ret = GetMacAddrByCmd(addrs,1000);
	if(ret) arrAddress->Add(addrs);
}