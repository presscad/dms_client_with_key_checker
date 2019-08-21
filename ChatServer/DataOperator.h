#pragma once
#include <vector>
using namespace std;

#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF", "adoEOF")  

typedef struct _OBJ_USER  
{  
	int User_ID;  
	TCHAR User_Name[32];  
}OBJ_USER; 

typedef struct _OBJ_FILE_LOG{
	CString filePath;		// �ļ�·��
	CString handle;		// ����
	CString ip;		// ip
	CString fileSize;		// �ļ���С
	CString describe1;		// ����D
	CString handler;		// handler
	CString fileName;
	COleDateTime createDate;
	COleDateTime updateDate;
}OBJ_FILE_LOG;

class CDataOperator  
{  
public:  
	CDataOperator();  
	~CDataOperator();  
public:  
	//��ָ�������ݿ�  
	BOOL OpenDatabase(CString strDbName, CString strUserName, CString strUserPwd);  
public:  
	//ִ��sql��䣬�������ݣ�ɾ������  
	BOOL ExecuteSQL(CString sql);  
	//��ѯ����  
	BOOL Select_From_User(vector<OBJ_USER> &vecObjUser);  
	//��������, ���Բ���ͼƬ�����������ݣ������ݣ�  
	BOOL Insert_Into_User(OBJ_USER &objUser);  
	//�������ݣ� �ɸ��´�����  
	BOOL Update_For_User(OBJ_USER &objUser);  
	BOOL SelectFromFileLog(vector<OBJ_FILE_LOG> &vecObjFileLog);
public:  
	//���Ӷ���  
	_ConnectionPtr m_pConnection;  
}; 
