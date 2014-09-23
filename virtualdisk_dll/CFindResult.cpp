#include "stdafx.h"
#include "CFindResult.h"


IFindResult* CFindResult::m_Find_Data = NULL;
CFindResult::CFindResult()
{
	clear();
}

CFindResult::~CFindResult()
{
	clear();

	if(NULL != m_Find_Data)
	{
		delete m_Find_Data;
		m_Find_Data = NULL;
	}
}

void CFindResult::Release()
{
	m_Results.clear();

	m_nCount = 0;
	//delete this;
}


IDiskObj* CFindResult::GetItemByIdx(unsigned int dwIndex)
{
	unsigned int nIndex_l = 0;
	SNode<IDiskObj>* Node = NULL;
	Node = m_Results.get_head();
	for(Node; Node != NULL; Node = Node->Next)
	{
		if(dwIndex == nIndex_l)
		{
			return Node->Value;
		}
		nIndex_l++;
	}

	return NULL;
}

CFindResult* CFindResult::Create()
{
	CFindResult* CTmp = NULL;
		
	if(NULL != m_Find_Data)
	{
		
	}
	else
	{
		m_Find_Data = new CFindResult();
	}

	CTmp =  dynamic_cast<CFindResult* >(m_Find_Data);

	return CTmp;
}

bool CFindResult::AddToTail(IDiskObj* pData)
{
	if(pData == NULL)
		return false;


	m_Results.tail_insert(*pData);

	m_nCount++;

	return true;
}