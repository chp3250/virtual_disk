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
	strcpy_s(m_RootDir.m_szName, "\\");
	SYSTEMTIME time;
	CoverTimeToUInt(time, m_RootDir.m_dwDate, m_RootDir.m_dwTime);
	m_RootDir.m_Parent = NULL;

}

CVirtualDiskMng::~CVirtualDiskMng()
{
	m_RootDir.Release();
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

	CMyString szTmp;

	if(NULL == Path)
	{
		return -1;
	}

	szTmp = Path;
	ITreeNode* Node_Parent = GetNode(szTmp);
	if(Node_Parent == NULL || Node_Parent->m_eType != EFT_DIR)
	{
		return -1;
	}

	const char* p = szTmp.GetBuf();
	if(NULL == p)
	{
		return -1;
	}

	// 递归创建
	ITreeNode* Node_Create = NULL;
	Node_Create = CFileNode::create(EFT_DIR, szName, Node_Parent, nSize, m_Disk.GetData(EDDT_POS));
	if(Node_Create == NULL)
	{
		return -1;
	}

	Node_Parent->InsertTree(Node_Create);

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


// FIXME: 遍历仅能遍历第一层
// FINISH
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

	if(nType == 0)
	{

		SNode<ITreeNode>* t = Node->GetNodes()->get_head(), *f = NULL, *k = NULL;
		CMyString szTmp1 = szTmp;
		printf("%s 的目录\n", szTmp1);

		Node->Print(1);
		Node->Print(2);

		for (t; t!= NULL; t = t->Next)
		{
			t->Value->Print();
		}
	}
	else if(nType == 1)
	{

		SNode<ITreeNode>* t = Node->GetNodes()->get_head(), *f = NULL, *k = NULL;
		CMyString szTmp1 = szTmp;
		printf("%s 的目录\n", szTmp1);

		Node->Print(1);
		Node->Print(2);

		for (t; t!= NULL; t = t->Next)
		{
			if(t->Value->m_eType == EFT_DIR)
				t->Value->Print();
		}
	}
	else if(nType == 2)
	{
		CMyString szTmp2 = "C:";
		Node->RecursionPrint(szTmp2);
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
// 完成
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
	}
	else
	{
		char c = '0';
		printf("确定要删除吗？\n");
		printf("Y/y OR N/n ？");
		c = getchar();
		if(c == 'N' || c == 'n')
		{
			return -1;
		}
		else if(c == 'Y' || c == 'y')
		{

		}
		
		fflush(stdin);
	}

	Node->m_Parent->GetNodes()->del(*Node);

	Node->Release();

	return 0;
}

ENUM_PATH_TYPE CVirtualDiskMng::JudgePath(char Path1[], char Path2[], SJudgePath_Data* pData)
{
	ENUM_PATH_TYPE eType = EPT_ERROR;
	char szTmp1[MAX_PATH] = {0};
	char szTmp2[MAX_PATH] = {0};

	if(NULL == Path1 || NULL == Path2 || NULL == pData)
	{
		return eType;
	}

	//memcpy(szTmp1, Path1, MAX_PATH);
	//memcpy(szTmp2, Path2, MAX_PATH);

	char* p1 = NULL;
	char* p2 = NULL;
	char* p3 = NULL;
	char* p4 = NULL;

	char* szWildCard[] = {"*", "?"};

	enum
	{
		DIR_T,
		FILE_T
	};

	int nType1 = -1;
	int nType2 = -1;

	p1 = strstr(Path1, szWildCard[0]);
	if(NULL == p1)
	{
		p1 = strstr(Path1, szWildCard[1]);
	}

	p3 = strstr(Path2, szWildCard[0]);
	if(NULL == p3)
	{
		p3 = strstr(Path2, szWildCard[1]);
	}

	if(NULL != p1)
	{
		p2 = strstr(p1, ".");
		strcpy_s(pData->Suffix1, p2);
	}
	else
	{

		WIN32_FIND_DATA Find_Data;
		HANDLE tmp_f;

		tmp_f = FindFirstFile(Path1, &Find_Data);
		if(INVALID_HANDLE_VALUE == tmp_f)
		{
			FindClose(tmp_f);
			return eType;
		}

		if(Find_Data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			nType1 = DIR_T;
			int nLength = strlen(Path1);
			*(Path1+nLength) = '\\';
			*(Path1+nLength+1) = '*';
			*(Path1+nLength+2) = '\0';
		}
		else
		{
			nType1 = FILE_T;
			GetPathFromStr(Path1);
			memcpy(pData->FileName1, Find_Data.cFileName, sizeof(pData->FileName1));
		}

		FindClose(tmp_f);
	}

	if(NULL != p3)
	{
		p4 = strstr(p3, ".");
		strcpy_s(pData->Suffix2, p4);
		GetPathFromStr(Path2);
	}
	else
	{
		CMyString szTmp = Path2;
		ITreeNode* Node = GetNode(szTmp);
		if(NULL == Node)
		{
			Node = GetNode(szTmp);
			if(NULL == Node)
			{
				return eType;
			}
			else
			{
				nType2 = FILE_T;			// 虚拟磁盘中不存在， 创建
				GetFileNameFromStr(pData->FileName2, Path2);
				GetPathFromStr(Path2);
			}
		}
		else
		{
			if (Node->m_eType == EFT_DIR)
			{
				nType2 = DIR_T;
			}
			else
			{
				printf("文件已存在, 确定要继续吗？\n");
				printf("Y/y OR N/n？");
				char c = getchar();
				if(c == 'Y' || c == 'y')
				{

				}
				else
				{
					return EPT_ERROR;
				}

				nType2 = FILE_T;
				GetFileNameFromStr(pData->FileName2, Path2);
				GetPathFromStr(Path2);
			}
		}
	}

	if( (NULL != p1 && NULL == p2) &&
		(NULL != p3 && NULL == p4) )		
	{
		eType = EPT_WILDCARD_NO_SUFFIX;

	}
	else if( (NULL != p1 && NULL != p2) &&
		(NULL != p3 && NULL == p4) )
	{
		eType = EPT_WILDCARD_DIFF_ONE;
	}
	else if( (NULL != p1 && NULL == p2) &&
		(NULL != p3 && NULL != p4) )
	{
		eType = EPT_WILDCARD_DIFF_TWO;
	}
	else if( (NULL == p1 && nType1 == FILE_T) &&
		(NULL == p3 && nType2 == DIR_T) )			
	{
		eType = EPT_FILE_DIR;
	}
	else if( (NULL == p1 && nType1 == DIR_T) &&
		(NULL == p3 && nType2 == DIR_T) )
	{
		eType = EPT_DIR;
	}
	else if( (NULL == p1 && nType1 == FILE_T) &&
		(NULL == p3 && nType2 == FILE_T) )
	{
		eType = EPT_SINGLE_FILE;
	}

	return eType;
}

int CVirtualDiskMng::CopyFiles(char Path1[], char Path2[])
{
	const unsigned int MAX_BUFF_SIZE = 1024;
	char szBuff[MAX_BUFF_SIZE] = {0};
	char* p1 = NULL, *p2 = NULL;					// 普通指针算子
	char szBuff1[MAX_PATH] = {0};
	char szBuff2[MAX_PATH] = {0};

	if(NULL == Path1 || NULL == Path2)
	{
		printf("命令语法不正确。\n");

		return -1;
	}

	memcpy(szBuff1, Path1, MAX_PATH);
	memcpy(szBuff2, Path2, MAX_PATH);

	SJudgePath_Data Judge_Data;
	Judge_Data.clear();
	ENUM_PATH_TYPE eType = JudgePath(szBuff1, szBuff2, &Judge_Data);
	if(eType == EPT_ERROR)
	{
		printf("系统找不到指定的路径。\n");
		return -1;
	}

	// 读取文件 并 写入
	WIN32_FIND_DATA Find_Data;
	HANDLE h_File = FindFirstFile(Path1, &Find_Data);

	p1 = strtok_s(szBuff1, "?*", &p2);				// 截取出除通配符的部分
	if(h_File == INVALID_HANDLE_VALUE)
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

			//sprintf_s(szPath_Tmp, "%s%s\n", p1, Find_Data.cFileName);
			CMyString szTmp1 = szBuff1;
			szTmp1 += "\\";
			szTmp1 += Find_Data.cFileName;

			FILE* fp = NULL;
			errno_t Ret_t = fopen_s(&fp, szTmp1.GetBuf(), "rb");
			if(Ret_t != 0)
			{
				printf("不可预料的错误...\n");
				return -1;
			}
			else
			{

				// 1, 内容写入磁盘
				int readbytes = 0;
				int nStartPost = m_Disk.GetData(EDDT_POS);
				int nEndPos = 0;

				while( (readbytes = fread(szBuff, 1, sizeof(szBuff), fp)) )
				{
					if(m_Disk.WriteTo(szBuff, readbytes) < 0)
					{
						printf("磁盘空间不足。\n");
						return -1;
					}
				}

				m_Disk.ChangeData(EDDT_FILE, 1);
				nEndPos = m_Disk.GetData(EDDT_POS);
			
				// 2, 在目录树中存储文件信息

				if(eType != EPT_SINGLE_FILE)
				{
					CreateFile(Find_Data.cFileName, szBuff2, nEndPos - nStartPost);
				}
				else
				{
					CreateFile(Judge_Data.FileName2, szBuff2, nEndPos - nStartPost);
				}
			}

			fclose(fp);
		}

	}while(FindNextFile(h_File, &Find_Data));
		
	FindClose(h_File);

	return 0;
}

int CVirtualDiskMng::DelFiles(char Path[], int nType)
{
	char* p1 = NULL;
	char* p2 = NULL;

	char szSuffix[MAX_SUFFIX_LENGTH] = {0};

	char* szWildCard[] = {"*", "?"};

	enum
	{
		DIR_T,
		FILE_T
	};
	int nType1 = 0, nType2 = 0;


	p1 = strstr(Path, szWildCard[0]);
	if(NULL == p1)
	{
		p1 = strstr(Path, szWildCard[1]);
	}

	if(NULL != p1)
	{
		p4 = strstr(p1 ".");
		strcpy_s(szSuffix, p4);
		GetPathFromStr(Path);
	}
	else
	{
		CMyString szTmp = Path;
		ITreeNode* Node = GetNode(szTmp);
		if(NULL == Node)
		{
			printf("指定路径不存在。\n");
			return -1;
		}
		else
		{
			if (Node->m_eType == EFT_DIR)
			{
				nType2 = DIR_T;
			}
			else
			{
				//printf("确定要删除吗？\n");
				//printf("Y/y OR N/n？");
				//char c = getchar();
				//if(c == 'Y' || c == 'y')
				//{

				//}
				//else
				//{
				//	return -1;
				//}

				nType2 = FILE_T;
				int nPos = ((CFileNode*)Node)->m_nPlace;
				int nSize = ((CFileNode*)Node)->m_nSize;

				m_Disk.DelFile(nPos, nSize);
				NoticeAllFileToChangeData(nPos, nSize);
			}
		}
	}

	return 1;
}

void CVirtualDiskMng::NoticeAllFileToChangeData(int nPos, int nSize)
{
	m_RootDir.UpdatePos(nPos, nSize);
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
