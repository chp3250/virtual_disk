#include "stdafx.h"
#include "mystring.h"

#define  BASE_SIZE 16 // string 类申请的足以容纳字符串的字节数， 不够再次申请， 每次以此数为阶梯， 直至够用 

CMyString::CMyString()
{
	Clear();

	m_Buf = new char[BASE_SIZE];
	m_nSize = BASE_SIZE;
	memset(m_Buf, 0, m_nSize);
}

CMyString::CMyString(char* Str)
{
	Clear();

	m_Buf = new char[BASE_SIZE];
	m_nSize = BASE_SIZE;
	memset(m_Buf, 0, m_nSize);

	if( NULL == Str )
	{
		return;
	}

	AllockBuf(Str);
}

CMyString::CMyString(CMyString& Str)
{
	Clear();

	m_Buf = new char[BASE_SIZE];
	m_nSize = BASE_SIZE;
	memset(m_Buf, 0, m_nSize);

	AllockBuf(const_cast<char *>(Str.GetBuf()));
}


CMyString::~CMyString()
{
	if(NULL != m_Buf)
	{
		delete [] m_Buf;
	}

	Clear();
}

CMyString CMyString::operator +=(char* Str)
{
	if( NULL == Str )
	{
		return *this;
	}

	AllockBuf(Str, true);

	return *this;
}

CMyString CMyString::operator +=(CMyString& Str)
{

	AllockBuf(const_cast<char *>(Str.GetBuf()), true);

	return *this;
}

CMyString CMyString::operator =(char* Str)
{
	if( NULL == Str )
	{
		return *this;
	}

	AllockBuf(Str);

	return *this;
}


CMyString CMyString::operator =(CMyString& Str)
{

	AllockBuf(const_cast<char *>(Str.GetBuf()));

	return *this;
}

int CMyString::AllockBuf(char* Str, bool bAdd)
{
	int nLength = 0;
	int nLength_Buf = strlen(m_Buf);
	int nLength_Str = strlen(Str);

	if(!bAdd)
	{
		nLength = nLength_Str + 1;
	}
	else
	{
		nLength = nLength_Str + nLength_Buf + 1;
	}

	int nTimes = nLength / BASE_SIZE;
	int nRemainder = nLength % BASE_SIZE;
	int nNeedSize = (nTimes * BASE_SIZE) + (nRemainder ==0?0:BASE_SIZE);

	if(m_nSize >= nNeedSize)
	{

	}
	else
	{
		char* szTmp = new char[nNeedSize];
		memset(szTmp, 0, nLength);
		memcpy(szTmp, m_Buf, nLength_Buf+1);

		delete [] m_Buf;
		m_Buf = szTmp;
		m_nSize = nNeedSize;
	}


	if(!bAdd)
	{
		memset(m_Buf, 0, m_nSize);
		memcpy(m_Buf, Str, nLength_Str+1);
	}
	else
	{
		memcpy(m_Buf+nLength_Buf, Str, nLength_Str+1);
	}

	m_Buf[nLength - 1] = '\0';

	//printf("nLength_buf %d\n", nLength_Buf);
	//printf("nLength_Str %d\n", nLength_Str);
	//printf("nLength %d\n", nLength);
	//printf("nRealLength %d\n", strlen(m_Buf));

	return nLength;
}