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
	CString filePath;		// 文件路径
	CString handle;		// 操作
	CString ip;		// ip
	CString fileSize;		// 文件大小
	CString describe1;		// 描述D
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
	//打开指定的数据库  
	BOOL OpenDatabase(CString strDbName, CString strUserName, CString strUserPwd);  
public:  
	//执行sql语句，增加数据，删除数据  
	BOOL ExecuteSQL(CString sql);  
	//查询数据  
	BOOL Select_From_User(vector<OBJ_USER> &vecObjUser);  
	//插入数据, 可以插入图片，二进制数据（大数据）  
	BOOL Insert_Into_User(OBJ_USER &objUser);  
	//更新数据， 可更新大数据  
	BOOL Update_For_User(OBJ_USER &objUser);  
	BOOL SelectFromFileLog(vector<OBJ_FILE_LOG> &vecObjFileLog);
public:  
	//连接对象  
	_ConnectionPtr m_pConnection;  
}; 
