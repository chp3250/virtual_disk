#ifndef __MYSTRING_H__
#define __MYSTRING_H__

#include "globalBase.h"

class VIRTUALDISK_API CMyString
{
public:
	///////////////////////////////////////////////////
	// 构造函数
	CMyString();
	CMyString(char* Str);
	CMyString(CMyString& Str);
	CMyString(CMyString& Str, int nStart, int nEnd);
	/////////////////////////////

	~CMyString();

	//////////////////////////////////////////////////
	// 重载符号
	CMyString& operator +=(char* Str);		
	CMyString& operator +=(CMyString& Str);	
	CMyString& operator =(char* Str);
	CMyString& operator =(CMyString& Str);
	
	bool operator == (char* Str);
	bool operator == (CMyString& Str);

	char& operator[](int nPos);

	const char* GetBuf() {return m_Buf;}
	int Length() { return strlen(m_Buf); };
	char Back() { return m_Buf[strlen(m_Buf)-1]; }

	bool WildCmp(CMyString &other);
private:
	int AllockBuf(char* Str, bool bAdd = false/*是加到字符串尾部，还是覆盖*/);				// 抽出来的字符串构造主体语句
	void Clear() {

		m_Buf = NULL;
		m_nSize = 0;
	}


private:
	char *m_Buf;							// 申请空间地址
	int m_nSize;							// 当前申请空间大小
};



#endif