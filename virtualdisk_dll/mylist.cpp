#include "stdafx.h"
#include "mylist.h"

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

	for(it, it!=NULL; it = it->Next)
	{
		if(it->Value != NULL)
		{
			delete it->Value;
			it->Value = NULL;
		}

		if(it->Pre != NULL)
		{
			delete it->Pre;
		}
	}
}

template<class T>
SNode<T>* CMyList<T>::create_node(T& Value)
{
	T* NewValue = new T;
	*NewValue = Value;

	SNode<T>* NewNode = new SNode<T>;
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

	for(it; it != NULL; it=it->Pre)
	{
		if(it->Pre != NULL)
		{
			continue;
		}
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

	for(it; it != NULL; it=it->Next)
	{
		if(it->Next != NULL)
		{
			continue;
		}
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
		if (Value == it->Value)
		{
			it->Pre->Next = it->Next;
			it->Next->Pre = it->Pre;

			delete it->Value;
			it->Value = NULL;

			delete it;
		}
	}

	return true;
}

template<class T>
SNode<T>* CMyList<T>::search(T& Value)
{

	SNode<T>* it = m_Head;
	if(it == NULL)
	{
		return false;
	}

	for(it; it != NULL; it=it->Next)
	{
		// == 自己重载
		if (Value == it->Value)
		{
			return it;		
		}
	}
}

#endif