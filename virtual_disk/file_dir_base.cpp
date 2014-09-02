#include "stdafx.h"
#include "file_dir_base.h"
#include "inline_func.h"

ITreeNode::~ITreeNode()
{
	Clear();
};

CDirectoryNode::CDirectoryNode()
{

};

CDirectoryNode::~CDirectoryNode()
{

}

/*
* @Func create() 构造实例函数
* @Para 成员变量初始值
* @para nCurPos 当前磁盘已使用的字节数
*/
ITreeNode* CDirectoryNode::create(ENUM_FILE_TYPE eType, char szName[MAX_PATH], ITreeNode *Parent)
{

	CDirectoryNode* Node = new CDirectoryNode();

	Node->m_eType = eType;

	int nLength = strlen(szName);
	memcpy(Node->m_szName, szName, nLength);

	Node->m_szName[nLength] = '\0';

	Node->m_Parent = Parent;

	SYSTEMTIME time;
	GetLocalTime(&time);

	CoverTimeToUInt(time, Node->m_dwDate, Node->m_dwTime);

	return Node;
}

int CDirectoryNode::InsertTree(ITreeNode *Node)
{
	if(NULL == Node)
	{
		return -1;
	}

	if(m_Nodes.tail_insert(*Node))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

void CDirectoryNode::Print(int nType)
{
	SYSTEMTIME time;
	CoverUIntToTime(time, m_dwDate, m_dwTime);

	printf("%04u/%02u/%02u  ", time.wYear, time.wMonth, time.wDay);
	printf("%02u:%02u    ", time.wHour, time.wMinute);
	printf("<DIR>          ");

	if(nType == 0)
	{
		printf("%s\n", m_szName);

	}
	else if(nType == 1)
	{
		printf(".\n");
	}
	else
	{
		printf("..\n");
	}
}

int CDirectoryNode::RecursionPrint()
{
	//if(RecursionPrint() == 1)
	//ITreeNode *Node = m_Nodes.get_head()

	if(m_Nodes.get_head() == NULL)
	{
		return 1;
	}
	else
	{
		SNode<ITreeNode> *Node_t = m_Nodes.get_head();
		for(Node_t; Node_t!=NULL; Node_t = Node_t->Next)
		{
			Print(1);
			if(m_Parent != NULL)
			{
				m_Parent->Print(2);
			}
			else
			{
				
			}

			Node_t->Value->Print(0);

			Node_t->Value->RecursionPrint();
		}
	}

	return 0;
}

CFileNode::CFileNode()
{
	Clear();
}

CFileNode::~CFileNode()
{
	Clear();
}

int CFileNode::ChangeOffset(int nPlace, int nSize)
{

	if(m_nPlace > nPlace)
	{
		if(m_nPlace > nSize)
		{
			m_nPlace -= nSize;
			return m_nPlace;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return 0;
	}
}

/*
* @Func create() 构造实例函数
* @Para 成员变量初始值
* @para nCurPos 当前磁盘已使用的字节数
*/
ITreeNode* CFileNode::create(ENUM_FILE_TYPE eType, char szName[MAX_PATH], ITreeNode *Parent, int nSize, int nCurPos)
{

	CFileNode* Node = new CFileNode();

	Node->m_eType = eType;
	
	int nLength = strlen(szName);
	memcpy(Node->m_szName, szName, nLength);
	
	Node->m_szName[nLength] = '\0';

	Node->m_Parent = Parent;

	Node->m_nSize = nSize;

	Node->m_nPlace = nCurPos + nSize;

	SYSTEMTIME time;
	GetLocalTime(&time);

	CoverTimeToUInt(time, Node->m_dwDate, Node->m_dwTime);

	return Node;
}

void CFileNode::Print(int nType)
{
	SYSTEMTIME time;
	CoverUIntToTime(time, m_dwDate, m_dwTime);

	printf("%04u/%02u/%02u  ", time.wYear, time.wMonth, time.wDay);
	printf("%02u:%02u    ", time.wHour, time.wMinute);
	printf("     "); // 5 个空格

	printf("%9d ", m_nSize);

	printf("%s\n", m_szName);
}