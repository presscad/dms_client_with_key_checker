#pragma once
class CCrypt
{
public:
	CCrypt(WORD Key);
	~CCrypt(void);
	CString Decrypt(CString S); // 解密函数
	CString Encrypt(CString S); // 加密函数

private:
	WORD m_Key;
};

