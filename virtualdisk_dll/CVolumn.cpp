#include "stdafx.h"
#include "CVolumn.h"
#include "mystring.h"
#include "mylist.h"
#include "inline_func.h"
#include "io.h"
#include "windows.h"
#include "stdio.h"

CVolumn::CVolumn()
{
	Clear();

	m_RootDir.m_eType = EFT_DIR;
	strcpy_s(m_RootDir.m_szName, "\\");
	SYSTEMTIME time;
	GetLocalTime(&time);
	CoverTimeToUInt(time, m_RootDir.m_dwDate, m_RootDir.m_dwTime);
	m_RootDir.m_Parent = NULL;
}

CVolumn::~CVolumn()
{
	
}

int CVolumn::ExecCommand(const char* command) 
{
	char* szTmp = const_cast<char *>(command);
	char* p1 = NULL;
	char* p2 = NULL;
	char* p3 = NULL;
	int nRet = 0;

	if(!strcmp(szTmp, "exit"))
	{
		return -2;
	}

	char* Para = {",mkdir,dir,cd,copy,rmdir,compare,del,"};		// 命令集合

	// for test
	p1 = strtok_s(szTmp, " \0", &p2);

	if(p1 == NULL)
	{
		return 0;
	}

	CMyString szTmp1 = ",";
	szTmp1 += p1;
	szTmp1 += ",";
	p1 = StriStr(Para, szTmp1.GetBuf());
	if(p1 == NULL)
	{
		printf("命令不存在\n");
		return -1;
	}

	int nIndex = (int)(p1 - Para);
	switch(nIndex) {
	case 0:
		{
			nRet = CreateDir(p2);
		}
		break;
	case 6:
		{
			p1 = strtok_s(NULL, " \0", &p2);
			if(NULL == p1)
			{
				nRet = ListDir(NULL);
				return 0;
			}

			if(!strcmp(p1, "/ad")) // 只列出子目录
			{

				p1 = strtok_s(NULL, " \0", &p2);
				nRet = ListDir(p1, 1);

				while(p1 != NULL)
				{
					p1 = strtok_s(NULL, " \0", &p2);
					if( NULL != p1) 
						nRet = ListDir(p1, 1);
				}
			}
			else if(!strcmp(p1, "/s")) // 输出目录及子目录下的所有文件
			{
				p1 = strtok_s(NULL, " \0", &p2);
				nRet = ListDir(p1, 2);

				while(p1 != NULL)
				{

					p1 = strtok_s(NULL, " \0", &p2);
					if( NULL != p1) 
						nRet = ListDir(p1, 2);
				}
			}
			else if(!strcmp(p1, "/s/ad") || !strcmp(p1, "/ad/s"))
			{
				p1 = strtok_s(NULL, " \0", &p2);
				nRet = ListDir(p1, 3);

				while(p1 != NULL)
				{

					p1 = strtok_s(NULL, " \0", &p2);
					if( NULL != p1) 
						nRet = ListDir(p1, 3);
				}
			}
			else
			{
				nRet = ListDir(p1);

				while(p1 != NULL)
				{
					p1 = strtok_s(NULL, " \0", &p2);
					if( NULL != p1) 
						nRet = ListDir(p1);
				}
			}

		}
		break;
	case 10:
		{
			p1 = strtok_s(NULL, " \0", &p2);
			if(NULL == p1)
			{
				PrintCurPath(false);
			}
			else
			{
				nRet = ChangeDir(p1);
			}
		}
		break;
	case 13:
		{
			p1 = strtok_s(NULL, " \0", &p2);
			if(NULL ==p1 || NULL == p2)
			{
				return 0;;
			}
			nRet = CopyFiles(p1, p2);
		}
		break;
	case 18:
		{
			p1 = strtok_s(NULL, " \0", &p2);
			if(NULL ==p1)
			{
				nRet = RmDir(NULL);
				return 0;;
			}

			if(!strcmp(p1, "/s"))
			{
				p1 = strtok_s(NULL, " \0", &p2);
				nRet = RmDir(p1, 1);

				while(p1 != NULL)
				{
					p1 = strtok_s(NULL, " \0", &p2);
					if(NULL != p1)
						nRet = RmDir(p1, 1);
				}
			}
			else
			{
				nRet = RmDir(p1);

				while(p1 != NULL)
				{
					p1 = strtok_s(NULL, " \0", &p2);
					if(NULL != p1)
						nRet = RmDir(p1);
				}
			}
		}
		break;
	case 24:
		{
			p1 = strtok_s(NULL, " \0", &p2);
			nRet = CompareFile(p1, p2);
		}
		break;
	case 32:
		{
			p1 = strtok_s(NULL, " \0", &p2);
			if(NULL ==p1)
			{
				return 0;;
			}
			if(!strcmp(p1, "/s"))
			{
				p1 = strtok_s(NULL, " \0", &p2);
				nRet = DelFiles(p1, 1);

				while(p1 != NULL)
				{
					p1 = strtok_s(NULL, " \0", &p2);
					if(NULL != p1)
						nRet = DelFiles(p1, 1);
				}
			}
			else
			{
				nRet = DelFiles(p1);

				while(p1 != NULL)
				{
					p1 = strtok_s(NULL, " \0", &p2);
					if(NULL != p1)
						nRet = DelFiles(p1);
				}
			}
		}
		break;
	}

	return nRet;
}

int CVolumn::CreateDir(char Path[])
{
	CMyString szTmp;

	if(NULL == Path)
	{
		printf("请输入路径\n");
		return -1;
	}

	if(!CheckPathName(Path))
	{
		printf("目录名不合法。\n");
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

		Node_Create->SetVolumn(m_nVolumnIndex);

		Node_Parent->InsertTree(Node_Create);

		Node_Parent = Node_Create;

		p1 = strtok_s(NULL, "\\", &p2);
	}

	return 0;
}

int CVolumn::CreateFile(char szName[], char Path[], int nSize)
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
	Node_Create = CFileNode::create(EFT_FILE, szName, Node_Parent, nSize, m_Disk.GetData(EDDT_POS) - nSize);
	if(Node_Create == NULL)
	{
		return -1;
	}

	Node_Create->SetVolumn(m_nVolumnIndex);


	Node_Parent->InsertTree(Node_Create);

	return 0;
}


// FIXME: 遍历仅能遍历第一层
// FINISH
int CVolumn::ListDir(char Path[], int nType)
{
	CMyString szTmp = Path;

	if(Path == NULL) // 为NULL的话列出当前
	{
		szTmp = m_CurDir;
	}
	
	CMyString szTmp1 = Path;
	CoverToAbsolutePath(szTmp1);

	ITreeNode *Node = GetNode(szTmp);
	if(NULL == Node)
	{
		printf("系统找不到指定路径\n");
		return -1;
	}

	// for test

	if(nType == 0 || nType == 1)
	{
		printf("%s 的目录\n", szTmp1.GetBuf());
		Node->Print(1);

		if(NULL != Node->GetNodes())
		{

			SNode<ITreeNode>* t = Node->GetNodes()->get_head(), *f = NULL, *k = NULL;
			if(NULL == t)
			{
				return -1;
			}

			if(Node->m_Parent != NULL)
				Node->m_Parent->Print(2);

			if(nType == 1)
			{
				for (t; t!= NULL; t = t->Next)
				{
					if(t->Value->m_eType == EFT_DIR)
						t->Value->Print();
				}
			}
			else
			{
				for (t; t!= NULL; t = t->Next)
				{
					t->Value->Print();
				}
			}
		}


	}
	else if(nType == 2)
	{
		char szBuf[MAX_PATH] = {0};
		memcpy(szBuf, szTmp1.GetBuf(), MAX_PATH);
		GetPathFromStr(szBuf);
		CMyString szTmp2 = szBuf;	

		Node->RecursionPrint(szTmp2);
	}
	else if(nType == 3)
	{
		char szBuf[MAX_PATH] = {0};
		memcpy(szBuf, szTmp1.GetBuf(), MAX_PATH);
		GetPathFromStr(szBuf);
		CMyString szTmp2 = szBuf;	

		Node->RecursionPrint(szTmp2, true);
	}
	// end test

	return 0;
}

int CVolumn::ChangeDir(char Path[])
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

static inline int AskFromUser()
{
	char c = '0';
	printf("确定要删除吗？\n");
	printf("Y/y OR N/n ？");
	c = getchar();
	if(c == 'N' || c == 'n')
	{
		getchar();
		return -1;
	}
	else if(c == 'Y' || c == 'y')
	{

	}

	getchar();
	return 0;
}

// FIXME: 资源释放未完成
// 完成
int CVolumn::RmDir(char Path[], int nType)
{
	CMyString szTmp = Path;
	bool bChange = false;

	if(Path == NULL)			// 删除当前路径
	{
		szTmp = m_CurDir;
		bChange = true;
	}

	if(szTmp.Back() == '.')
	{
		bChange = true;
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

	//if(!strcmp(Node->m_szName, "\\"))
	//{
	//	return -1;
	//}

	if(nType == 0)
	{
		if(NULL != Node->GetNodes()->get_head())
		{
			printf("目录不是空的。\n");
			return -1;
		}
	}
	else
	{
		if(AskFromUser()<0)
		{
			return -1;
		}
	}

	if(strcmp(Node->m_szName, "\\"))
	{
		Node->m_Parent->GetNodes()->del(*Node);
	}

	Node->Release();


	if(bChange)
	{
		ChangeDir("..");
	}

	return 0;
}

ENUM_PATH_TYPE CVolumn::JudgePath(char Path1[], char Path2[], SJudgePath_Data* pData)
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
		if(NULL != p2)
		{
			strcpy_s(pData->Suffix1, p2);

			if(*p1 == '*')
			{
				int nLen_p2 = strlen(p2);
				p2[nLen_p2] = '*';
				p2[nLen_p2+1] = '\0';
			}
		}

		nType1 = DIR_T;
		//GetPathFromStr(Path1);
	}
	else
	{

		WIN32_FIND_DATA Find_Data;
		HANDLE tmp_f;
		int nLength = 0;

		nLength = strlen(Path1);
		if(Path1[nLength-1] == '\\')
		{
			Path1[nLength] = '*';
			Path1[nLength+1] = '\0';
		}

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
			if(Path1[nLength-1] != '*')
			{
				*(Path1+nLength) = '\\';
				*(Path1+nLength+1) = '*';
				*(Path1+nLength+2) = '\0';
			}
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
		if(NULL != p4)
		{
			strcpy_s(pData->Suffix2, p4);
		}
		GetFileNameFromStr(pData->FileName2, Path2);
		GetPathFromStr(Path2);
		nType2 = DIR_T;
	}
	else
	{
		CMyString szTmp = Path2;
		ITreeNode* Node = GetNode(szTmp);
		if(NULL == Node)
		{

			GetFileNameFromStr(pData->FileName2, Path2);
			GetPathFromStr(Path2);
			szTmp = Path2;
			Node = GetNode(szTmp);
			if(NULL == Node)
			{
				return eType;
			}
			else
			{
				nType2 = FILE_T;			// 虚拟磁盘中不存在， 创建
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
				getchar();
				if(c == 'Y' || c == 'y')
				{

				}
				else
				{
					return EPT_ERROR;
				}

				DelFiles(Path2);
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
		nType2 == DIR_T )			
	{
		eType = EPT_FILE_DIR;
	}
	else if( nType1 == DIR_T &&
		nType2 == DIR_T)
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

int CVolumn::GetNewName(char FileName1[], char FileName2[], CMyString& NewName)
{
	CMyString RealName = FileName1;
	CMyString VirtualName = FileName2;

	if(VirtualName == "*.*")
		VirtualName = "*";

	if( (NULL == strstr(FileName2, "*")) && (NULL == strstr(FileName2, "?")) )
	{
		NewName = FileName2;
	}
	else if(VirtualName.WildCmp(RealName))
	{
		NewName = RealName;
	}
	else
	{
		int i = 0;
		int pos = 0;
	
		while(i < VirtualName.Length())
		{
			if(VirtualName[i] == '*')
			{
				CMyString tmpName(VirtualName, pos, i);
				NewName += tmpName;
				NewName += RealName;
				pos = i + 1;
			}
			else if(VirtualName[i] == '?')
			{
				CMyString tmpName(VirtualName, pos, i);
				NewName += tmpName;
				pos = i + 1;
			}
			else if(i == VirtualName.Length() - 1)
			{
				CMyString tmpName(VirtualName, pos, VirtualName.Length());
				NewName += tmpName;
				pos = i + 1;
			}
			i++;
		}
	}

	return 1;
}

int CVolumn::CopyFiles(char Path1[], char Path2[])
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

	if(eType == EPT_SINGLE_FILE && 
		!CheckFileName(Judge_Data.FileName2)	)
	{
		printf("文件名不合法。\n");
		return -1;
	}

	// 读取文件 并 写入
	WIN32_FIND_DATA Find_Data;
	HANDLE h_File = INVALID_HANDLE_VALUE;
	if(eType != EPT_FILE_DIR && eType != EPT_SINGLE_FILE)
	{
		h_File = FindFirstFile(szBuff1, &Find_Data);
	}
	else
	{
		h_File = FindFirstFile(Path1, &Find_Data);
	}

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

				if(eType != EPT_SINGLE_FILE )
				{
					char c = '0';
					CMyString szTmp2 = szBuff2;
					szTmp2 += "\\";
					szTmp2 += Find_Data.cFileName;
					CMyString szTmp3 = szTmp2;
					ITreeNode* Node = GetNode(szTmp2);
					if(Node != NULL)
					{
						printf("有同名文件，是否覆盖？\n");
						printf("Y/y OR N/n ？");
						c = getchar();
						if(c == 'Y' || c == 'y')
						{
							getchar();

							DelFiles(const_cast<char *>(szTmp3.GetBuf()));
						}
						else
						{
							getchar();
							printf("文件复制失败。\n");
							FindClose(h_File);
							return -1;
						}
					}
				}

				// 1, 内容写入磁盘
				int readbytes = 0;
				int nStartPost = m_Disk.GetData(EDDT_POS);
				int nEndPos = 0;

				while( (readbytes = fread(szBuff, 1, sizeof(szBuff), fp)) )
				{
					if(m_Disk.WriteTo(szBuff, readbytes) < 0)
					{
						printf("磁盘空间不足。\n");
						FindClose(h_File);
						return -1;
					}
				}

				m_Disk.ChangeData(EDDT_FILE, 1);
				nEndPos = m_Disk.GetData(EDDT_POS);
			
				if (eType == EPT_SINGLE_FILE)
				{
					CreateFile(Judge_Data.FileName2, szBuff2, nEndPos - nStartPost);
				}
				else 
				{

					// 2, 在目录树中存储文件信息
					if(strlen(Judge_Data.FileName2) != 0)
					{
						CMyString UseName; 
						GetNewName(Find_Data.cFileName, Judge_Data.FileName2, UseName);
						CreateFile(const_cast<char *>(UseName.GetBuf()), szBuff2, nEndPos - nStartPost);
					}
					else
					{
						CreateFile(Find_Data.cFileName, szBuff2, nEndPos - nStartPost);
					}
				}
			}

			fclose(fp);
		}

	}while(FindNextFile(h_File, &Find_Data));
		
	FindClose(h_File);

	return 0;
}

int CVolumn::DelFiles(char Path[], int nType)
{
	char* p1 = NULL;
	char* p2 = NULL;

	char szSuffix[MAX_SUFFIX_LENGTH] = {0};
	char szBuff[MAX_PATH] = {0};

	char* szWildCard[] = {"*", "?"};

	enum
	{
		DIR_T,
		FILE_T
	};
	int nType1 = 0, nType2 = 0;

	if(NULL == Path)
	{
		return -1;
	}

	p1 = strstr(Path, szWildCard[0]);
	if(NULL == p1)
	{
		p1 = strstr(Path, szWildCard[1]);
	}

	if(NULL != p1)
	{
		p2 = strstr(p1, ".");
		if(NULL != p2)
		{
			strcpy_s(szSuffix, p2);
		}

		GetPathFromStr(Path);

		memcpy(szBuff, Path, MAX_PATH);
		CMyString szTmp = Path;

		ITreeNode* Node = GetNode(szTmp);
		if(NULL == Node)
		{
			printf("指定路径不存在。\n");
			return -1;
		}

		nType2 = DIR_T;

		if(AskFromUser()<0)
		{
			return -1;
		}

		if(NULL != p2)
		{
			Node->ReleaseChild(nType, szSuffix);
		}
		else
		{
			Node->ReleaseChild(nType);
		}
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

				if(AskFromUser()<0)
				{
					return -1;
				}

				Node->ReleaseChild(nType);
			}
			else
			{
				nType2 = FILE_T;
				int nPos = ((CFileNode*)Node)->m_nPlace;
				int nSize = ((CFileNode*)Node)->m_nSize;

				m_Disk.DelFile(nPos, nSize);

				if(NULL != Node->m_Parent)
				{
					Node->m_Parent->GetNodes()->del(*Node);
				}

				Node->Release();

				NoticeAllFileToChangeData(nPos, nSize);

				return 0;
			}
		}
	}

	return 0;
}

void CVolumn::NoticeAllFileToChangeData(int nPos, int nSize)
{
	m_RootDir.UpdatePos(nPos, nSize);
}

int CVolumn::CompareFile(char Path1[], char Path2[])
{	
	FILE* fp = NULL;
	errno_t Ret_t = 0;
	

	enum {

		etmp_b,

		etmp_f
	};

	int nType1 = etmp_f;
	int nType2 = etmp_f;

	bool bSame = true;

	CMyString szTmp = Path2;
	ITreeNode* Node = GetNode(szTmp);
	if(NULL == Node || Node->m_eType != EFT_FILE)
	{
		printf("系统找不到指定路径。\n");
		return -1;
	}
	CFileNode* File = (CFileNode*)Node;

	Ret_t = fopen_s(&fp, Path1, "rb");
	if( Ret_t != 0)
	{
		printf("系统找不到指定路径。\n");
		return -1;
	}

	char c1 = '0';
	char c2 = '0';
	int i = 0;
	int nPos = File->m_nPlace;
	bool bFinish1 = 0, bFinish2 = 0;
	while(1)
	{

		if(!bFinish1)
		{
			if(!fread(&c1, 1, 1, fp))
			{
				bFinish1 = true;
			}
			if(c1 == '\0')
			{
				nType1 = etmp_b;
			}
		}

		if(!bFinish2)
		{
			if(nPos - File->m_nPlace < File->m_nSize)
			{
				m_Disk.ReadFrom(&c2, nPos++, 1);
			}
			else
			{
				bFinish2 = true;
			}

			if(c2 == '\0')
			{
				nType2 = etmp_b;
			}
		}

		if(c1 != c2)
		{
			bSame = false;
		}

		if(bSame)
		{
			i++;
		}

		if(bFinish1&bFinish2)
		{
			break;
		}
	}

	if(bSame)
	{
		printf("内容比较一致\n");
		return 0;
	}

	fseek(fp, i, SEEK_SET);
	int nOutSize = 16;
	int j = 0;
	printf("%s ==>\n", Path1);
	if(nType1 == etmp_b)
	{
		do
		{
			printf("0x%08x	", c1);
			j++;
			if(j >= 16)
			{
				break;
			}
		}while(fread(&c1, 1, 1, fp));
	}
	else
	{
		do
		{
			j++;
			if(j >= 16)
			{
				break;
			}
			printf("%c", c1);
		}while(fread(&c1, 1, 1, fp));
	}

	printf("\n");

	j = 0;
	printf("%s ==>\n", Path2);
	if(nType2 == etmp_b)
	{
		while(!m_Disk.ReadFrom(&c2, File->m_nPlace+i, 1))	
		{
			i++;
			j++;
			if(j >= 16)
			{
				break;
			}
			printf("0x%08x	", c2);
		}
	}
	else
	{
		while(!m_Disk.ReadFrom(&c2, File->m_nPlace+i, 1))
		{
			i++;
			j++;
			if(j >= 16)
			{
				break;
			}
			printf("%c", c2);
		}
	}

	printf("\n");


	fclose(fp);

	return 0;
}

ITreeNode* CVolumn::GetNode(CMyString &Path, bool bCreate)
{
	char szPath[MAX_PATH] = {0};

	//char szBuff[MAX_PATH] = {0};


	CoverToAbsolutePath(Path);

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
	if(p1 == NULL)
	{
		return &m_RootDir;
	}	

	SNode<ITreeNode> *Node_T = m_RootDir.m_Nodes.get_head();

	ITreeNode *Node = NULL;
	if(Node_T == NULL)
	{
		Node = &m_RootDir;
	}


	bool bIsRetParent = true;
	while(p1 != NULL)
	{
		for(Node_T; Node_T != NULL; Node_T = Node_T->Next)
		{

			Node = Node_T->Value;

			//if (strcmp(p1, Node->m_szName) == 0) // 相同
			if( (strlen(p1) == strlen(Node->m_szName)) 
				&& (NULL != StriStr(p1, Node->m_szName)) )
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
				if(Node_T == NULL)
				{
					bIsRetParent = false;
				}
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


				if(bIsRetParent)
				{
					return Node->m_Parent == NULL?Node:Node->m_Parent;
				}
				else
				{
					return Node;
				}
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
CMyString& CVolumn::CoverToAbsolutePath(CMyString& Path)
{
	CMyString szTmp;

	char* p = const_cast<char *>(Path.GetBuf());

	int nLength = strlen(p);

	char* p2 = NULL;
	char* p1 = NULL;
	const char* p3 = NULL;
	char* p6 = NULL;

	// 判断是否有后缀
	if( NULL != (p6 = HaveSuffix(p)) )
	{
		*p6 = '*';
	}

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

		szTmp += p1;

		p1 = strtok_s(NULL, ":.\\", &p2);

		if(p1 != NULL)
		{
			szTmp += "\\";
		}
	}

	// 如果有后缀加上后缀
	if(NULL != p6)
	{
		p6 = const_cast<char *>(szTmp.GetBuf());
		for(int i=strlen(p6)-1; i>=0; i--)
		{
			if(p6[i] == '*')
			{
				p6[i] = '.';
			}
		}
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
			//*(p4+strlen(p4)-1) = '\0';

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

		if( nLength > 3 && *(p4+nLength-1) == '\\')
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

		if(szTmp1.Back() != '\\' && szTmp.Length() != 0)
		{
			szTmp1 += "\\";
		}

		Path = szTmp1;
		Path += szTmp;
	}

	//printf("%s\n", Path.GetBuf());

	return Path;
}
