#ifndef __MYLIST_H__
#define __MYLIST_H__
#include "globalbase.h"

template<class T>
struct SNode
{
	SNode<T>* Pre;
	SNode<T>* Next;

	T* Value;
};

template<class T>
class __declspec(dllexport) CMyList
{
public:

	CMyList();
	~CMyList();

	bool head_insert(T& Value);
	bool tail_insert(T& Value);


	bool del(T& Value);
	SNode<T>* search(T& Value);
	SNode<T>* get_head() { return m_Head; }
	void set_head(SNode<T>* head) { m_Head = head;}
	SNode<T>* next(SNode<T>* Cur) { return Cur->Next; }
	bool empty();
	void clear();
	bool erase(SNode<T>* pNode);
	T* GetValueByIndex(int nIndex);

private:
	SNode<T>* create_node(T& Value);

private:
	SNode<T> *m_Head;
};


template<class T>
CMyList<T>::CMyList()
{
	m_Head = NULL;
}

template<class T>
CMyList<T>::~CMyList()
{
	SNode<T> *it = NULL;
	it = m_Head;

	for(it; it!=NULL; it = it->Next)
	{
		erase(it);
	}
}

template<class T>
SNode<T>* CMyList<T>::create_node(T& Value)
{
	T* NewValue = &Value;

	SNode<T>* NewNode = new SNode<T>();
	NewNode->Next = NULL;
	NewNode->Pre = NULL;
	NewNode->Value = NewValue;

	return NewNode;
}

template<class T>
bool CMyList<T>::head_insert(T& Value)
{
	SNode<T>* NewNode = create_node(Value);
	if(NewNode == NULL)
	{
		return false;
	}

	SNode<T>* it = m_Head;
	if(it == NULL)
	{
		m_Head = NewNode;
		return true;
	}

	for(it; it->Pre!= NULL; it=it->Pre)
	{

	}


	NewNode->Next = it;
	it->Pre = NewNode;

	m_Head = NewNode;

	return true;
}

template<class T>
bool CMyList<T>::tail_insert(T& Value)
{
	SNode<T>* NewNode = create_node(Value);
	if(NewNode == NULL)
	{
		return false;
	}

	SNode<T>* it = m_Head;
	if(it == NULL)
	{
		m_Head = NewNode;
		return true;
	}

	for(it; it->Next!= NULL; it=it->Next)
	{

	}

	NewNode->Pre = it;
	it->Next = NewNode;

	return true;
}

template<class T>
bool CMyList<T>::del(T& Value)
{
	SNode<T>* it = m_Head;
	if(it == NULL)
	{
		return false;
	}

	for(it; it != NULL; it=it->Next)
	{
		// == 自己重载
		if (Value == *it->Value)
		{
			erase(it);
			break;
		}
	}

	if(it == NULL)
	{
		return false;
	}
	else
	{
		return true;
	}
}

template<class T>
SNode<T>* CMyList<T>::search(T& Value)
{

	SNode<T>* it = m_Head;
	if(it == NULL)
	{
		return NULL;
	}

	for(it; it != NULL; it=it->Next)
	{
		// == 自己重载
		if (Value == it->Value)
		{
			return it;		
		}
	}

	return NULL;
}

template<class T>
bool CMyList<T>::empty()
{
	if (m_Head == NULL)
		return true;

	return false;
}

template<class T>
void CMyList<T>::clear()
{
	SNode<T> *it = NULL;
	it = m_Head;

	SNode<T>* tmp = NULL;

	for(it; it!=NULL; it = tmp)
	{
		tmp = it->Next;
		delete it;
	}

	m_Head = NULL;
}

template<class T>
bool CMyList<T>::erase(SNode<T>* pNode)
{
	SNode<T> *it = NULL;
	it = m_Head;

	for(it; it!=NULL; it = it->Next)
	{
		if(it != pNode)
		{
			continue;
		}

		if(it->Pre != NULL)
		{
			it->Pre->Next = it->Next;
			if(it->Next != NULL)
			{
				it->Next->Pre = it->Pre;
			}
		}
		else
		{
			m_Head = it->Next;
			if( NULL != it->Next)
			{
				it->Next->Pre = NULL;
			}
		}

		delete it;
		break;
	}

	return true;
}

template<class T>
T* CMyList<T>::GetValueByIndex(int nIndex)
{
	SNode<T>* it = m_Head;
	if(NULL == it)
	{
		return NULL;
	}

	for(int i=0; i<nIndex && it != NULL; i++)
	{
		it = it->Next;
	}

	if(NULL != it)
		return it->Value;
	else
		return NULL;
}

#endif