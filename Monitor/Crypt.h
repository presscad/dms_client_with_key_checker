#pragma once
class CCrypt
{
public:
	CCrypt(WORD Key);
	~CCrypt(void);
	CString Decrypt(CString S); // ���ܺ���
	CString Encrypt(CString S); // ���ܺ���

private:
	WORD m_Key;
};

