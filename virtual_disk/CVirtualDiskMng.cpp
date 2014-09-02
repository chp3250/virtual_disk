#include "stdafx.h"
#include "CVirtualDiskMng.h"
#include "mystring.h"
#include "mylist.h"
#include "inline_func.h"
#include "io.h"
#include "windows.h"
#include "stdio.h"

CVirtualDiskMng::CVirtualDiskMng()
{
	Clear();

	m_RootDir.m_eType = EFT_DIR;
	strcpy_s(m_RootDir.m_szName, "根目录");
	SYSTEMTIME time;
	CoverTimeToUInt(time, m_RootDir.m_dwDate, m_RootDir.m_dwTime);
	m_RootDir.m_Parent = NULL;

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


// FIXME: 遍历仅能遍历第一层
int CVirtualDiskMng::ListDir(char Path[], int nType)
{
	CMyString szTmp = Path;

	if(Path == NULL) // 为NULL的话列出当前
	{
		szTmp = m_CurDir;
	}
	
	ITreeNode *Node = GetNode(szTmp);
	if(NULL == Node)
	{
		printf("系统找不到指定路径\n");
		return -1;
	}

	// for test
	SNode<ITreeNode>* t = Node->GetNodes()->get_head(), *f = NULL, *k = NULL;
	CMyString szTmp1 = szTmp;
	szTmp1 += "\\";
	szTmp1 += Node->m_szName;
	printf("%s 的目录\n", szTmp1);

	Node->Print(1);
	Node->Print(2);

	if(nType == 0)
	{
		for (t; t!= NULL; t = t->Next)
		{
			t->Value->Print();
		}
	}
	else if(nType == 1)
	{
		for (t; t!= NULL; t = t->Next)
		{
			if(t->Value->m_eType == EFT_DIR)
				t->Value->Print();
		}
	}
	else if(nType == 2)
	{

		for(t; t!=NULL; t = (t->Value)->GetNodes()->get_head())
		{
			f = t;

			// 横向打印

				for (t; t!= NULL; t = t->Next)
				{
					t->Value->Print();
				}

			t = f;
		}
	}
	// end test

	return 0;
}

int CVirtualDiskMng::ChangeDir(char Path[])
{
	if(NULL == Path)
	{
		return -1;
	}

	CMyString szTmp1 = Path;

	CoverToAbsolutePath(szTmp1);
	if(!strcmp(szTmp1.GetBuf(), INIT_CUR_PATH))
	{
		m_CurDir = INIT_CUR_PATH;
		return 0;
	}

	CMyString szTmp2 = szTmp1;

	ITreeNode* Node = GetNode(szTmp2);
	if(NULL == Node)
	{
		printf("系统找不到指定的路径。\n");
		return -1;
	}

	if(Node->m_eType == EFT_FILE)
	{
		printf("目录名称无效。\n");
		return -1;
	}



	m_CurDir = szTmp1;

	//PrintCurPath(false);

	return 0;
}

// FIXME: 资源释放未完成
int CVirtualDiskMng::RmDir(char Path[], int nType)
{
	CMyString szTmp = Path;

	if(Path == NULL)			// 删除当前路径
	{
		szTmp = m_CurDir;
		ChangeDir("..");
	}

	ITreeNode* Node = GetNode(szTmp);
	if(NULL == Node)
	{
		printf("系统找不到指定的路径。\n");
		return -1;
	}

	if(Node->m_eType == EFT_FILE)
	{
		printf("目标是文件。\n");
		return -1;
	}

	if(nType == 0)
	{
		if(NULL != Node->GetNodes())
		{
			printf("目录不是空的。\n");
			return -1;
		}

		Node->GetNodes()->clear();
	}
	else
	{
		char c = '0';
		c = getchar();
		if(c == 'N' || c == 'n')
		{
			return -1;
		}
		else if(c == 'Y' || c == 'y')
		{

		}

	}

	return 0;
}


static int JudgePath(char Path[])
{
	if(NULL == Path)
	{
		return -1;
	}

	char* p1 = NULL;
	char* p2 = NULL;
	char* p3 = NULL;

	p1 = strtok_s(Path, "*?", &p2);

	if (p1 == NULL)
	{
		return 0;
	}
	
	p1 = strtok_s(NULL, "*?", &p2);
	if(p1 != NULL)
	{
		return -1;
	}

	return 0;
}
int CVirtualDiskMng::CopyFiles(char Path1[], char Path2[])
{
	char szPath_Tmp[MAX_PATH] = {0};

	if(NULL == Path1 || NULL == Path2)
	{
		printf("命令语法不正确。\n");

		return -1;
	}

	CMyString szTmp = Path2;
	ITreeNode* Node = GetNode(szTmp);
	if(NULL == Node)
	{
		printf("系统找不到指定的路径。\n");
		return -1;
	}

	WIN32_FIND_DATA Find_Data;
	HANDLE h_File = FindFirstFile(Path1, &Find_Data);
	char* p1 = NULL, *p2 = NULL;
	p1 = strtok_s(Path1, "?*", &p2);
	if(h_File == (HANDLE)0xffffffff)
	{
		printf("文件名或目录名不正确。\n");
		return -1;
	}
	do 
	{
		if( (Find_Data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
		{
			
		}
		else		// 文件
		{

			sprintf_s(szPath_Tmp, "%s%s\n", p1, Find_Data.cFileName);
			CMyString szTmp1 = p1;
			szTmp1 += Find_Data.cFileName;
			FILE* fp = NULL;
			errno_t Ret_t = fopen_s(*fp, szTmp1.GetBuf(), "rb");
			if(Ret_t != 0)
			{
				printf("不可预料的错误...\n");
				return -1;
			}
			else
			{
				while(read(,))
			}
		}

	}while(FindNextFile(h_File, &Find_Data));
		

	return 0;
}

ITreeNode* CVirtualDiskMng::GetNode(CMyString &Path, bool bCreate)
{
	char szPath[MAX_PATH] = {0};

	CoverToAbsolutePath(Path);

	//printf("%s\n", Path.GetBuf());

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
		//printf("%d:%s\n", i, p1);

		szTmp += "\\";

		szTmp += p1;

		p1 = strtok_s(NULL, ":.\\", &p2);
	}

	//printf("%s\n", szTmp.GetBuf());

	// 判断是否绝对路径
	// 1, 不带盘符标识的
	// 2, 带盘符标识的
	if(  (*p == '\\') 
		|| (NULL != p3 && (*(p3+1) == '\\')) )			// 本身就是绝对路径
	{
		Path = INIT_CUR_PATH;

		if(szTmp.Length() == 0)
		{
			
		}
		else
		{
			char* p4 = const_cast<char *>(Path.GetBuf());
			*(p4+strlen(p4)-1) = '\0';

			Path += szTmp;
		}			
		return Path;
	}

	// 拼出相对路径前的串
	// 1, 是 '..'的
	{
		nLength = m_CurDir.Length();
		CMyString szTmp1 = m_CurDir;
		char* p4 = const_cast<char *>(szTmp1.GetBuf());
		char* p5 = const_cast<char *>(p);

		if(*(p4+nLength-1) == '\\')
		{
			*(p4+nLength-1) = '\0';
		}

		while( NULL != (p5 = strstr(p5, "..")))
		{
			for(int i = nLength-1; i>=3; i--)
			{
				if(*(p4+i) == '\\')
				{
					*(p4+i) = '\0';				// 清除路径尾部的"\"
					nLength = i;
					break;
				}
				else
				{
					*(p4+i) = '\0';
				}
			}

			p5 += 2;
		}

		Path = szTmp1;
		Path += szTmp;
	}

	//printf("%s\n", Path.GetBuf());

	return Path;
}
