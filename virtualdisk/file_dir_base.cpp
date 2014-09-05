#include "stdafx.h"
#include "file_dir_base.h"
#include "inline_func.h"
#include "mystring.h"
#include "CVirtualDiskMng.h"

VIRTUALDISK_API CVirtualDiskMng CTmp;

ITreeNode::~ITreeNode()
{
	Clear();
	//printf("release %s ", m_szName);
};

bool ITreeNode::operator== (ITreeNode& Node)
{
	if(strcmp(m_szName, Node.m_szName))
	{
		return false;
	}
	else
	{
		return true;
	}
}

CDirectoryNode::CDirectoryNode()
{

};

CDirectoryNode::~CDirectoryNode()
{
	m_Nodes.clear();
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

int CDirectoryNode::RecursionPrint(CMyString& szTmp)
{
	CMyString szTmp1 = szTmp;
	if(m_szName[0] != '\\')
	{
		szTmp1 += "\\";
		szTmp1 += m_szName;
	}

	printf("%s 的目录。\n", szTmp1);
	Print(1);
	if(m_Parent != NULL)
	{
		m_Parent->Print(2);
	}
	else
	{

	}

	if(m_Nodes.get_head() == NULL)
	{

	}
	else
	{

		SNode<ITreeNode> *Node_t = m_Nodes.get_head();
		for(Node_t; Node_t!=NULL; Node_t = Node_t->Next)
		{
			if( NULL == Node_t->Value)
				continue;

			Node_t->Value->Print(0);
		}

		Node_t = m_Nodes.get_head();
		for(Node_t; Node_t!=NULL; Node_t = Node_t->Next)
		{
			Node_t->Value->RecursionPrint(szTmp1);
		}
	}

	return 0;
}

void CDirectoryNode::Release(int nType)
{
	if(nType == 1)
		return;

	if(m_Nodes.get_head() == NULL)
	{
		
	}
	else
	{
		SNode<ITreeNode> *Node_t = m_Nodes.get_head();
		for(Node_t; Node_t!=NULL; Node_t = Node_t->Next)
		{
			if(Node_t->Value == NULL)
				continue;

			Node_t->Value->Release(nType);
		}
	}


	if(strcmp(m_szName, "\\"))
	{
		delete this;
	}
	else
	{
		m_Nodes.clear();
	}
}

void CDirectoryNode::ChangeData(DWORD dwDate, DWORD dwTime, ITreeNode* Parent, int nPos, int nSize)
{
	if(dwDate != 0)
	{
		m_dwDate = dwDate;
	}

	if(dwTime != 0)
	{
		m_dwTime = dwTime;
	}

	if(Parent != NULL)
	{
		m_Parent = Parent;
	}
}

void CDirectoryNode::UpdatePos(int nPos, int nSize)
{

	if(m_Nodes.get_head() == NULL)
	{

	}
	else
	{
		SNode<ITreeNode> *Node_t = m_Nodes.get_head();
		for(Node_t; Node_t!=NULL; Node_t = Node_t->Next)
		{
			if(Node_t->Value == NULL)
				continue;

			Node_t->Value->UpdatePos(nPos, nSize);
		}
	}

	return;
}

void CDirectoryNode::ReleaseChild(int nType, char* szBuff)
{
	if(m_Nodes.get_head() == NULL)
	{

	}
	else
	{
		SNode<ITreeNode> *Node_t = m_Nodes.get_head(), *Node_Tmp = NULL;
		for(Node_t; Node_t!=NULL; Node_t = Node_t == NULL? m_Nodes.get_head():Node_t->Next)
		{
			if(Node_t->Value == NULL)
				continue;

			if(nType == 0)
			{

			}
			else
			{
				Node_t->Value->ReleaseChild(nType, szBuff);
			}

			if(Node_t->Value->m_eType != EFT_FILE)
			{
				continue;
			}

			if(NULL == szBuff || strstr(Node_t->Value->m_szName, szBuff))
			{
				//FIXME:
				Node_Tmp = Node_t->Pre;
				Node_t->Value->Release();
				m_Nodes.erase(Node_t);
				Node_t = Node_Tmp;

			}
			else
			{
				
			}
		}
	}

	return;
}

IFindResult* CDirectoryNode::Find(const char* findstr,bool bRecursion)
{
	if(NULL == findstr)
	{
		return NULL;
	}

	CMyString szTmp = const_cast<char *>(findstr);
	if(bRecursion)
	{

	}
	else
	{

	}
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

	Node->m_nPlace = nCurPos;

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

void CFileNode::Release(int nType)
{
	if(nType != 2)
	{

		CTmp.GetDisk()->DelFile(m_nPlace, m_nSize);
		CTmp.NoticeAllFileToChangeData(m_nPlace, m_nSize);

		delete this;
	}
}

int CFileNode::RecursionPrint(CMyString& szTmp)
{
	//CMyString szTmp1 = szTmp;
	//szTmp1 += "\\";
	//szTmp1 += m_szName;

	//printf("%s 的目录。\n", szTmp1.GetBuf());

	return 0;
}

void CFileNode::ChangeData(DWORD dwDate, DWORD dwTime, ITreeNode* Parent, int nPos, int nSize)
{
	if(dwDate != 0)
	{
		m_dwDate = dwDate;
	}

	if(dwTime != 0)
	{
		m_dwTime = dwTime;
	}

	if(Parent != NULL)
	{
		m_Parent = Parent;
	}

	if(nPos != 0)
	{
		m_nPlace = nPos;
	}

	if(nSize != 0)
	{
		m_nSize = nSize;
	}
}

void CFileNode::UpdatePos(int nPos, int nSize)
{
	if(nPos >= m_nPlace)
	{

	}
	else
	{
		m_nPlace -= nSize;
	}
}