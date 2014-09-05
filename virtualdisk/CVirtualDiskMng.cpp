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
	GetLocalTime(&time);
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
		printf("������·��\n");
		return -1;
	}
	
	szTmp = Path;
	ITreeNode* Node_Parent = GetNode(szTmp, true);
	if(Node_Parent == NULL)
	{
		//printf("ϵͳ�Ҳ���ָ��·��\n");
		return -1;
	}

	const char* p = szTmp.GetBuf();
	if(NULL == p)
	{
		return -1;
	}

	// �ݹ鴴��
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

	// �ݹ鴴��
	ITreeNode* Node_Create = NULL;
	Node_Create = CFileNode::create(EFT_FILE, szName, Node_Parent, nSize, m_Disk.GetData(EDDT_POS) - nSize);
	if(Node_Create == NULL)
	{
		return -1;
	}

	Node_Parent->InsertTree(Node_Create);

	return 0;
}


// FIXME: �������ܱ�����һ��
// FINISH
int CVirtualDiskMng::ListDir(char Path[], int nType)
{
	CMyString szTmp = Path;

	if(Path == NULL) // ΪNULL�Ļ��г���ǰ
	{
		szTmp = m_CurDir;
	}
	
	CMyString szTmp1 = Path;
	CoverToAbsolutePath(szTmp1);

	ITreeNode *Node = GetNode(szTmp);
	if(NULL == Node)
	{
		printf("ϵͳ�Ҳ���ָ��·��\n");
		return -1;
	}

	// for test

	if(nType == 0)
	{

		SNode<ITreeNode>* t = Node->GetNodes()->get_head(), *f = NULL, *k = NULL;
		
		printf("%s ��Ŀ¼\n", szTmp1.GetBuf());

		Node->Print(1);
		if(Node->m_Parent != NULL)
			Node->m_Parent->Print(2);

		for (t; t!= NULL; t = t->Next)
		{
			t->Value->Print();
		}
	}
	else if(nType == 1)
	{

		SNode<ITreeNode>* t = Node->GetNodes()->get_head(), *f = NULL, *k = NULL;
		printf("%s ��Ŀ¼\n", szTmp1.GetBuf());

		Node->Print(1);
		if(Node->m_Parent != NULL)
			Node->m_Parent->Print(2);

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
		printf("ϵͳ�Ҳ���ָ����·����\n");
		return -1;
	}

	if(Node->m_eType == EFT_FILE)
	{
		printf("Ŀ¼������Ч��\n");
		return -1;
	}



	m_CurDir = szTmp1;

	//PrintCurPath(false);

	return 0;
}

// FIXME: ��Դ�ͷ�δ���
// ���
int CVirtualDiskMng::RmDir(char Path[], int nType)
{
	CMyString szTmp = Path;

	if(Path == NULL)			// ɾ����ǰ·��
	{
		szTmp = m_CurDir;
		ChangeDir("..");
	}

	ITreeNode* Node = GetNode(szTmp);
	if(NULL == Node)
	{
		printf("ϵͳ�Ҳ���ָ����·����\n");
		return -1;
	}

	if(Node->m_eType == EFT_FILE)
	{
		printf("Ŀ�����ļ���\n");
		return -1;
	}

	if(!strcmp(Node->m_szName, "\\"))
	{
		return -1;
	}

	if(nType == 0)
	{
		if(NULL != Node->GetNodes()->get_head())
		{
			printf("Ŀ¼���ǿյġ�\n");
			return -1;
		}
	}
	else
	{
		char c = '0';
		printf("ȷ��Ҫɾ����\n");
		printf("Y/y OR N/n ��");
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
		if(NULL != p2)
		{
			strcpy_s(pData->Suffix1, p2);
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
		GetPathFromStr(Path2);
		nType2 = DIR_T;
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
				nType2 = FILE_T;			// ��������в����ڣ� ����
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
				printf("�ļ��Ѵ���, ȷ��Ҫ������\n");
				printf("Y/y OR N/n��");
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
	char* p1 = NULL, *p2 = NULL;					// ��ָͨ������
	char szBuff1[MAX_PATH] = {0};
	char szBuff2[MAX_PATH] = {0};

	if(NULL == Path1 || NULL == Path2)
	{
		printf("�����﷨����ȷ��\n");

		return -1;
	}

	memcpy(szBuff1, Path1, MAX_PATH);
	memcpy(szBuff2, Path2, MAX_PATH);

	SJudgePath_Data Judge_Data;
	Judge_Data.clear();
	ENUM_PATH_TYPE eType = JudgePath(szBuff1, szBuff2, &Judge_Data);
	if(eType == EPT_ERROR)
	{
		printf("ϵͳ�Ҳ���ָ����·����\n");
		return -1;
	}

	// ��ȡ�ļ� �� д��
	WIN32_FIND_DATA Find_Data;
	HANDLE h_File = FindFirstFile(szBuff1, &Find_Data);

	p1 = strtok_s(szBuff1, "?*", &p2);				// ��ȡ����ͨ����Ĳ���
	if(h_File == INVALID_HANDLE_VALUE)
	{
		printf("�ļ�����Ŀ¼������ȷ��\n");
		return -1;
	}
	do 
	{
		if( (Find_Data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
		{
			
		}
		else		// �ļ�
		{

			//sprintf_s(szPath_Tmp, "%s%s\n", p1, Find_Data.cFileName);
			CMyString szTmp1 = szBuff1;
			szTmp1 += "\\";
			szTmp1 += Find_Data.cFileName;

			FILE* fp = NULL;
			errno_t Ret_t = fopen_s(&fp, szTmp1.GetBuf(), "rb");
			if(Ret_t != 0)
			{
				printf("����Ԥ�ϵĴ���...\n");
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
						printf("��ͬ���ļ����Ƿ񸲸ǣ�\n");
						printf("Y/y OR N/n ��");
						c = getchar();
						if(c == 'Y' || c == 'y')
						{
							getchar();

							DelFiles(const_cast<char *>(szTmp3.GetBuf()));
						}
						else
						{
							getchar();
							printf("�ļ�����ʧ�ܡ�\n");
							FindClose(h_File);
							return -1;
						}
					}
				}

				// 1, ����д�����
				int readbytes = 0;
				int nStartPost = m_Disk.GetData(EDDT_POS);
				int nEndPos = 0;

				while( (readbytes = fread(szBuff, 1, sizeof(szBuff), fp)) )
				{
					if(m_Disk.WriteTo(szBuff, readbytes) < 0)
					{
						printf("���̿ռ䲻�㡣\n");
						FindClose(h_File);
						return -1;
					}
				}

				m_Disk.ChangeData(EDDT_FILE, 1);
				nEndPos = m_Disk.GetData(EDDT_POS);
			
				// 2, ��Ŀ¼���д洢�ļ���Ϣ
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
			printf("ָ��·�������ڡ�\n");
			return -1;
		}

		nType2 = DIR_T;

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
			printf("ָ��·�������ڡ�\n");
			return -1;
		}
		else
		{
			if (Node->m_eType == EFT_DIR)
			{
				nType2 = DIR_T;

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

void CVirtualDiskMng::NoticeAllFileToChangeData(int nPos, int nSize)
{
	m_RootDir.UpdatePos(nPos, nSize);
}

int CVirtualDiskMng::CompareFile(char Path1[], char Path2[])
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
		printf("ϵͳ�Ҳ���ָ��·����\n");
		return -1;
	}
	CFileNode* File = (CFileNode*)Node;

	Ret_t = fopen_s(&fp, Path1, "rb");
	if( Ret_t != 0)
	{
		printf("ϵͳ�Ҳ���ָ��·����\n");
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
		printf("���ݱȽ�һ��\n");
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

ITreeNode* CVirtualDiskMng::GetNode(CMyString &Path, bool bCreate)
{
	char szPath[MAX_PATH] = {0};

	//char szBuff[MAX_PATH] = {0};

	//// ��ȡ�ļ���
	//CMyString szTmp = Path;
	//bool bPoint = false;

	//GetFileNameFromStr(szBuff, const_cast<char *>(szTmp.GetBuf()));

	//if(strcmp(szBuff, ".")&&strcmp(szBuff, ".."))
	//{
	//	bPoint = true;
	//	GetPathFromStr(const_cast<char *>(szTmp.GetBuf()));
	//}


	CoverToAbsolutePath(Path);

	////���Ͻ�ȡ���ļ���

	//if(bPoint != true)
	//{
	//	szTmp += "\\";
	//	szTmp += szBuff;
	//}


	//Path = szTmp;

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

	while(p1 != NULL)
	{
		for(Node_T; Node_T != NULL; Node_T = Node_T->Next)
		{

			Node = Node_T->Value;

			if (strcmp(p1, Node->m_szName) == 0) // ��ͬ
			{
				break;
			}
			else
			{
				continue;
			}
		}

		if(Node_T != NULL) // �ҵ���
		{
			p1 = strtok_s(NULL, "\\", &p2);
			if(p1 != NULL && Node->m_eType == EFT_FILE)
			{
				printf("ϵͳ���������⣬ ���������ļ�\n");
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

				printf("��Ŀ¼���ļ� %s �Ѿ�����\n", p);
				return NULL;
			}

		}
		else//δ�ҵ�
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

// ת�ɾ���·��
CMyString& CVirtualDiskMng::CoverToAbsolutePath(CMyString& Path)
{
	CMyString szTmp;

	char* p = const_cast<char *>(Path.GetBuf());

	int nLength = strlen(p);

	char* p2 = NULL;
	char* p1 = NULL;
	const char* p3 = NULL;
	char* p6 = NULL;

	// �ж��Ƿ��к�׺
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

		szTmp += "\\";

		szTmp += p1;

		p1 = strtok_s(NULL, ":.\\", &p2);
	}

	// ����к�׺���Ϻ�׺
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

	// �ж��Ƿ����·��
	// 1, �����̷���ʶ��
	// 2, ���̷���ʶ��
	if(  (*p == '\\') 
		|| (NULL != p3 && (*(p3+1) == '\\')) )			// �������Ǿ���·��
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

	// ƴ�����·��ǰ�Ĵ�
	// 1, �� '..'��
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
					*(p4+i) = '\0';				// ���·��β����"\"
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