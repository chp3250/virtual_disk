#ifndef __CFINDRESULT_H__
#define __CFINDRESULT_H__

#include "IVirtualDiskProxy.h"
#include "inline_func.h"
#include "mylist.h"

struct SFindResultData
{
	char szReCord[MAX_PATH];

	void clear()
	{
		memset(szReCord, 0, sizeof(szReCord));
	}
};

class CFindResult:public IFindResult
{
public:
	CFindResult();
	~CFindResult();

	virtual void Release();
	virtual unsigned int GetCount(){ return m_nCount; } 
	virtual IDiskObj* GetItemByIdx(unsigned int dwIndex);

	static CFindResult* Create();
	bool AddToTail(IDiskObj* pData);

	CMyList<IDiskObj> m_Results;

	int m_nCount;									// ÊýÁ¿

	static IFindResult* m_Find_Data;

	void clear()
	{
		m_nCount = 0;
	}
};

#endif