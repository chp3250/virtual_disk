#ifndef __MYSTRING_H__
#define __MYSTRING_H__

#include "globalbase.h"

class VIRTUALDISK_API CMyString
{
public:
	///////////////////////////////////////////////////
	// ���캯��
	CMyString();
	CMyString(char* Str);
	CMyString(CMyString& Str);
	/////////////////////////////

	~CMyString();

	//////////////////////////////////////////////////
	// ���ط���
	CMyString operator +=(char* Str);		
	CMyString operator +=(CMyString& Str);	
	CMyString operator =(char* Str);
	CMyString operator =(CMyString& Str);
	
	const char* GetBuf() {return m_Buf;}
	int Length() { return strlen(m_Buf); };

private:
	int AllockBuf(char* Str, bool bAdd = false/*�Ǽӵ��ַ���β�������Ǹ���*/);				// ��������ַ��������������
	void Clear() {

		m_Buf = NULL;
		m_nSize = 0;
	}


private:
	char *m_Buf;							// ����ռ��ַ
	int m_nSize;							// ��ǰ����ռ��С
};



#endif