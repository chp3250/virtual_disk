#include "stdafx.h"
#include "mystring.h"
#include "inline_func.h"

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

CMyString::CMyString(CMyString& Str, int nStart, int nEnd)
{
	if(nStart >= 0 && nStart < nEnd && nEnd <= Str.Length())
	{
		char* pTmp = new char[nEnd - nStart + 1];
		memcpy(pTmp, Str.GetBuf()+nStart, nEnd - nStart + 1);
		pTmp[nEnd - nStart + 1] = '\0';

		m_Buf = new char[BASE_SIZE];
		m_nSize = BASE_SIZE;
		memset(m_Buf, 0, m_nSize);

		AllockBuf(pTmp);
	}
	else
	{
		Clear();

		m_Buf = new char[BASE_SIZE];
		m_nSize = BASE_SIZE;
		memset(m_Buf, 0, m_nSize);
	}
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

bool CMyString::operator == (char* Str)
{
	if(strcmp(m_Buf, Str))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CMyString::operator == (CMyString& Str)
{
	const char* pTmp = Str.GetBuf();
	if(strcmp(m_Buf, pTmp))
	{
		return false;
	}
	else
	{
		return true;
	}
}

char& CMyString::operator[](int nPos)
{
	return m_Buf[nPos];
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

	return nLength;
}

//bool CMyString::WildCmp(CMyString &other)
//{
//	const char* pThis = m_Buf;
//	const char* pOther = other.GetBuf();
//	const char* p1 = NULL;
//	const char* p2 = NULL;
//
//	int nLength_This = strlen(pThis);
//	int nLength_Other = strlen(pOther);
//
//	//1, 若长度不相等， 且other中无通配符*，则不匹配
//	if( (nLength_Other != nLength_This) && (NULL == strstr(pOther, "*")) )
//		return false;
//
//	bool bMatch = false;
//	for(int j=0,i=0; j<nLength_Other && j<nLength_This; j++,i++)
//	{
//		if(pOther[j] == '*')
//		{
//			// 比较剩下的串的长短，若other长于this 则不匹配
//			if(strlen(pOther+j+1) > strlen(pThis+j))
//			{
//				return false;
//			}
//
//			// 贪婪匹配
//			p1 = pOther+nLength_Other-1;
//			p2 = pThis+nLength_This-1;
//
//			while(p1 != pOther+j)
//			{
//				// 如果是*的话，往前匹配
//				if(*p1 == '*' && *(p1-1) != *p2)
//				{
//					p2--;
//					continue;
//				}
//				else
//				{
//					p1--;
//					continue;
//				}
//
//				if(*p1 == '?')
//				{
//					p1--;
//					p2--;
//					continue;
//				}
//
//				if(*p1 != *p2)
//				{
//					return false;
//				}
//
//				p1--;
//				p2--;
//			}
//
//			// 是遍历完毕后推出的
//			if(p1 == pOther+j)
//				return true;
//		}
//		else if(pOther[j] == '?')
//		{
//			continue;
//		}	
//		else if(pOther[j] == pThis[i])
//		{
//
//		}
//		else
//		{
//			return false;
//		}
//	}
//
//
//	// 经过上述处理剩下这些情况
//	// 1, 长度相等，带？的
//	// 2, 字符串完全一样的
//	// 3, other前面相同， 最后是一个通配符*的
//
//	return true;
//}

bool CMyString::WildCmp(CMyString &other)
{
	char* pThis = NULL;
	char* pOther = NULL;
	const char* p1 = NULL;
	char* p2 = NULL;

	// *.txt == *.txt*
	if(other.Length() >= 5)
	{
		CMyString tmp(other, other.Length() - 5, other.Length());
		if(tmp == "*.txt")
		{
			other += "*";
		}
	}

	int nLength_This = this->Length();
	int nLength_Other = other.Length();

	pThis = new char[nLength_This+1];
	pOther = new char[nLength_Other+1];

	memcpy(pThis, m_Buf, nLength_This+1);
	memcpy(pOther, other.GetBuf(), nLength_Other+1);


	bool bMatch = true;
	bool bWildEnd = true;
	if(pOther[nLength_Other-1] != '*')
	{
		bWildEnd = false;
	}

	p1 = strtok_s(pOther, "*", &p2);
	if(NULL == p1 && !bWildEnd)
	{
		if(nLength_This != nLength_Other)
		{
			bMatch = false;
		}
		else
		{
			for(int i=0; i<nLength_This; i++)
			{
				if(pOther[i] == '?' || LOWER(pOther[i]) == LOWER(pThis[i]))
				{
					continue;
				}

				bMatch = false;
				break;
			}
		}
	}
	else
	{
		if(!bWildEnd && nLength_This < nLength_Other)
		{
			bMatch = false;
		}
		else
		{
			char* pTmp = pThis;
			char* pSave = NULL;
			while(p1 != NULL)
			{
				while(1)
				{
					pTmp = StrStr_Wild(pTmp, p1);
					if(NULL == pTmp)
					{
						break;
					}
					pTmp = pTmp + strlen(p1);
					pSave = pTmp;
				}

				pTmp = pSave;
				if(pSave == NULL)
				{
					bMatch = false;
					break;
				}
				pSave = NULL;
				p1 = strtok_s(NULL, "*", &p2);
			}

			if(p1 == NULL && bWildEnd )
			{
				bMatch = true;
			}
			else
			{
				if(NULL != pTmp && *pTmp != '\0')
				{
					bMatch = false;
				}
			}
		}
	}


	delete [] pThis;
	delete [] pOther;

	return bMatch;
}