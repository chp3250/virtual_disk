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

	/************************************************************************/
	/*	@Func CopyFile(char Path1[], char Path2[])   
	/*	@Para Path1 �������·��
	/*	@Para Path2 �������·��
	/************************************************************************/
	int CopyFiles(char Path1[], char Path2[]);

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