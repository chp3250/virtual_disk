#ifndef __CVIRTUALDISKMNG_H__
#define __CVIRTUALDISKMNG_H__

#include "CVirtualDisk.h"
#include "file_dir_base.h"
#include "mystring.h"

//class ITreeNode;
//class CDirectoryNode;
//class CFileNode;

//class CMyString;

#define INIT_CUR_PATH "C:\\"
#define MAX_SUFFIX_LENGTH	8				// �ļ�����׺�����

// copyʱ ��ͨ���������ö�ٱ�
enum ENUM_PATH_TYPE
{
	EPT_SINGLE_FILE = 0,				//  ���������path��Ϊȫ·��������һ���ļ�
	EPT_DIR,							//  ���������path��Ϊ��ѰĿ¼������Ŀ¼1�������ļ�
	EPT_WILDCARD_NO_SUFFIX,				//	���������pathĩβΪͨ���, �Ҳ�����׺, ����path1 ��ȡͨ����� ����Ŀ¼�������ļ�
	EPT_WILDCARD_HAVE_SUFFIX,			//	���������pathĩβ��ͨ���, �Ҵ���׺, ����path1 ��ȡͨ����� ����Ŀ¼���Ϻ�׺�������ļ�
	EPT_WILDCARD_DIFF_ONE,				//  ���� e:\abc\?.txt  c:\def\* ������path1������txt�ļ���path2Ŀ¼��
	EPT_WILDCARD_DIFF_TWO,				//	���� e:\abc\*	   c:\def\*.txt (��path1 �������ļ����Ƶ�path2Ŀ¼�� ������׺��Ϊtxt);
	EPT_FILE_DIR,						//  ���� e:\abc\file.txt c:\def\ 

	EPT_ERROR,							//  ����
};

struct SJudgePath_Data
{
	char Suffix1[MAX_SUFFIX_LENGTH];			// ���к�׺���� ��˴������׺������Ϊ��
	char Suffix2[MAX_SUFFIX_LENGTH];			// ͬ��
	char FileName1[MAX_PATH];					// ����������ļ������ļ�����������
	char FileName2[MAX_PATH];					// ͬ��

	void clear()
	{
		memset(Suffix1, 0, sizeof(Suffix1));
		memset(Suffix2, 0, sizeof(Suffix2));
		memset(FileName1, 0, sizeof(FileName1));
		memset(FileName2, 0, sizeof(FileName2));
	}
};

class CVirtualDiskMng
{
public:
	CVirtualDiskMng();
	~CVirtualDiskMng();


	int CreateDir(char Path[]);						// ����Ŀ¼
	int CreateFile(char szName[], char Path[], int nSize);			// �����ļ�

	/************************************************************************/
	/*  @Func ListDir
	/*			�г�Ŀ��·���µ��ļ���Ŀ¼
	/*	@Para Path							·��
	/*	@Para int nType						0, ��ͨ�޲��� 1, ����/ad 2, ����/s
	/************************************************************************/
	int ListDir(char Path[], int nType/*�Ƿ�ݹ��г�����d*/= 0);

	int ChangeDir(char Path[]);										// �ı䵱ǰĿ¼

	/************************************************************************/
	/*  @Func RmDir
	/*				ɾ��Ŀ¼��Ŀ¼�µ���������
	/*	@Para nType					0, �޲��� ɾ����Ŀ¼  1�� ���� /s ɾ��Ŀ¼��Ŀ¼�µ���������
	/************************************************************************/
	int RmDir(char Path[], int nType = 0);

	ENUM_PATH_TYPE JudgePath(char Path1[], char Path2[], SJudgePath_Data* pData);

	/************************************************************************/
	/*	@Func CopyFile(char Path1[], char Path2[])   
	/*	@Para Path1 �������·��
	/*	@Para Path2 �������·��
	/************************************************************************/
	int CopyFiles(char Path1[], char Path2[]);

	/************************************************************************/
	/*	@Func DelFiles
	/*					ɾ��ָ���ļ�
	/*	@Para Path						·��
	/*	@Para nType						0, ��ͨ�޲���
	/*									1, ���� /s
	/************************************************************************/
	int DelFiles(char Path[], int nType = 0);

	/************************************************************************/
	/*	@Func NoticeAllFileToChangeData
	/*					֪ͨ�����ļ���������
	/************************************************************************/
	void NoticeAllFileToChangeData(int nPos, int nSize);

	ITreeNode* GetNode(CMyString& Path, bool bCreate /*�Ƿ��Ǵ����ļ���Ŀ¼*/ = false);

	int ResolvePath(char Path[], char* szNames[]);					// ����·��

	CMyString& CoverToAbsolutePath(CMyString& Path);					// ת�ɾ���·��

	/************************************************************************/
	/*  @Func PrintCurPath
	/*	@Para bBegin			�Ƿ���Ϊǰ��ָʾ
	/************************************************************************/
	void PrintCurPath(bool bBegin = true) { 
		if (bBegin)
		{
			printf("%s>", m_CurDir);
		}
		else
		{
			printf("%s\n", m_CurDir);
		}
	}									// ��ʾ��ǰ·��

private:
	CVirtualDisk   m_Disk;
	CDirectoryNode m_RootDir;   // ��Ŀ¼
	//CDirectoryNode* m_CurDir;	// ��ǰĿ¼
	CMyString m_CurDir;


	void Clear()
	{
		//m_CurDir = &m_RootDir;			// ��ʼ��ǰĿ¼Ϊ��Ŀ¼
		m_CurDir = INIT_CUR_PATH;
	}

};


#endif