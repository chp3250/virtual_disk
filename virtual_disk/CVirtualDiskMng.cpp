#include "stdafx.h"
#include "CVirtualDiskMng.h"
#include "mystring.h"
#include "mylist.h"


CVirtualDiskMng::CVirtualDiskMng()
{
	Clear();

	m_RootDir.m_eType = EFT_DIR;
	strcpy_s(m_RootDir.m_szName, "根目录");
}

CVirtualDiskMng::~CVirtualDiskMng()
{

}

int CVirtualDiskMng::CreateDir(char Path[])
{
	CMyString szTmp;

	if(NULL == Path)
	{
		printf("请输入路径\n");
		return -1;
	}
	
	szTmp = Path;
	ITreeNode* Node_Parent = GetNode(szTmp, true);
	if(Node_Parent == NULL)
	{
		//printf("系统找不到指定路径\n");
		return -1;
	}

	const char* p = szTmp.GetBuf();
	if(NULL == p)
	{
		return -1;
	}

	// 递归创建
	char* p1 = NULL;
	char* p2 = NULL;
	ITreeNode* Node_Create = NULL;
	p1 = strtok_s(const_cast<char *>(p), "\\", &p2);
	while(p1 != NULL)
	{
		Node_Create = CDirectoryNode::create(EFT_DIR, p1, Node_Parent);
	
		if(Node_Create == NULL)
		{
			return -1;
		}

		Node_Parent->InsertTree(Node_Create);

		Node_Parent = Node_Create;

		p1 = strtok_s(NULL, "\\", &p2);
	}

	// for test
	SNode<ITreeNode>* t = m_RootDir.m_Nodes.get_head(), *f = NULL;
	for(t; t!=NULL; t =((CDirectoryNode*)(t->Value))->m_Nodes.get_head())
	{
		f = t;
		for (t; t!= NULL; t = t->Next)
		{
			printf("test dir == %s\n", t->Value->m_szName);
		}
		t = f;
	}
	// end test

	return 0;
}

int CVirtualDiskMng::CreateFile(char szName[], char Path[], int nSize)
{
	return 0;
}

int CVirtualDiskMng::ListDir(char Path[], bool bRecursion /*是否递归列出所有d*/)
{
	if(Path == NULL)
	{
		return -1;
	}

	CMyString szTmp = Path;
	
	ITreeNode *Node = GetNode(szTmp);
	if(NULL == Node)
	{
		printf("系统找不到指定路径\n");
		return -1;
	}

	// for test
	SNode<ITreeNode>* t = Node->GetNodes()->get_head(), *f = NULL;
	Node->Print(1);
	Node->Print(2);
	if (bRecursion)
	{
		for(t; t!=NULL; t = (t->Value)->GetNodes()->get_head())
		{
			f = t;
			for (t; t!= NULL; t = t->Next)
			{
				/*printf("test dir == %s\n", t->Value->m_szName);*/
				t->Value->Print();
			}
			t = f;
		}
	}
	else
	{
		for (t; t!= NULL; t = t->Next)
		{
			//printf("test dir == %s\n", t->Value->m_szName);
			t->Value->Print();
		}
	}
	// end test

	return 0;
}

int CVirtualDiskMng::ChangeDir(char Path[])
{
	return 0;
}

ITreeNode* CVirtualDiskMng::GetNode(CMyString &Path, bool bCreate)
{
	char szPath[MAX_PATH] = {0};

	CoverToAbsolutePath(Path);

	printf("%s\n", Path.GetBuf());

	const char* p = Path.GetBuf();
	int nLength = Path.Length();
	char* p1 = NULL;
	char* p2 = NULL;

	p = strstr(p, "\\");
	if( p == NULL)
	{
		return NULL;
	}

	p1 = strtok_s(const_cast<char *>(p), "\\", &p2);
	SNode<ITreeNode> *Node_T = m_RootDir.m_Nodes.get_head();

	ITreeNode *Node = NULL;
	if(Node_T == NULL)
	{
		Node = &m_RootDir;
	}

	while(p1 != NULL)
	{
		for(Node_T; Node_T != NULL; Node_T = Node_T->Next)
		{

			Node = Node_T->Value;

			if (strcmp(p1, Node->m_szName) == 0) // 相同
			{
				break;
			}
			else
			{
				continue;
			}
		}

		if(Node_T != NULL) // 找到了
		{
			p1 = strtok_s(NULL, "\\", &p2);
			if(p1 != NULL && Node->m_eType == EFT_FILE)
			{
				printf("系统编码有问题， 不可能是文件\n");
				return NULL;
			}
			else if(p1 != NULL && Node->m_eType == EFT_DIR)
			{
				Node_T = ((CDirectoryNode *)Node)->m_Nodes.get_head();
			}
			else if(p1 == NULL && bCreate)
			{
				CMyString szTmp;
				p1 = const_cast<char *>(p);
				while(p1 != p2 - 1)
				{
					if(*p1 == '\0')
					{
						*p1 = '\\';
					}

					p1++;
				}

				printf("子目录或文件 %s 已经存在\n", p);
				return NULL;
			}

		}
		else//未找到
		{
			if(bCreate)
			{
				CMyString szTmp = "\\";
				//while( p1 != NULL)
				//{
				//	szTmp += p1; 
				//	szTmp += "\\";
				//	p1 = strtok_s(NULL, "\\", &p2);			
				//}
				szTmp += p1;
				szTmp += "\\";
				szTmp += p2;
				Path = szTmp;

				return Node->m_Parent == NULL?Node:Node->m_Parent;
			}
			else
			{
				return NULL;
			}
		}
	}

	return Node;
}

// 转成绝对路径
CMyString& CVirtualDiskMng::CoverToAbsolutePath(CMyString& Path)
{
	CMyString szTmp;

	const char* p = Path.GetBuf();

	int nLength = strlen(p);

	char* p2 = NULL;
	char* p1 = NULL;
	const char* p3 = NULL;
	
	p3 = strstr(p, ":");

	if(NULL != p3)
	{
		p1 = strtok_s(const_cast<char *>(p), ":", &p2);
		p1 = strtok_s(NULL, ".\\", &p2);
	}
	else
	{
		p1 = strtok_s(const_cast<char *>(p), ".\\", &p2);
	}

	for(int i=0; p1 != NULL; i++)
	{
		printf("%d:%s\n", i, p1);

		szTmp += "\\";

		szTmp += p1;

		p1 = strtok_s(NULL, ":.\\", &p2);
	}

	printf("%s\n", szTmp.GetBuf());

	// 判断是否绝对路径
	// 1, 不带盘符标识的
	if(*p == '\\')
	{
		Path = szTmp;

		return Path;
	}

	// 2, 带盘符标识的
	if( NULL != p3 && (*(p3+1) == '\\') )			// 本身就是绝对路径
	{
		Path = szTmp;
					
		return Path;
	}

	// 拼出相对路径前的串
	// 1, 是 '..'的
	if(NULL != strstr(p, ".."))
	{
		nLength = m_CurDir.Length();
		CMyString szTmp1 = m_CurDir;
		const char* p4 = szTmp1.GetBuf();

		if(*(p4+nLength-1) == '\\')
		{
			*(const_cast<char *>(p4+nLength-1)) = '\0';
		}
		for(int i = nLength-1; i>=0; i--)
		{
			if(*(p4+i) == '\\')
			{
				break;
			}
			else
			{
				*(const_cast<char *>(p4+i)) = '\0';
			}
		}

		Path = szTmp1;
		Path += szTmp;
	}
	else
	{
		Path = m_CurDir;
		Path += szTmp;
	}

	printf("%s\n", Path.GetBuf());

	return Path;
}
