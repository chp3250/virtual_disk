#include "stdafx.h"
#include "CFindResult.h"

CFindResult::CFindResult()
{
	clear();
}

CFindResult::~CFindResult()
{
	clear();
}

void CFindResult::Release()
{
	m_Results.clear();

	delete this;
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
	CFindResult* CTmp = new CFindResult();

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