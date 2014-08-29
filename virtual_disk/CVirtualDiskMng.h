#ifndef __CVIRTUALDISKMNG_H__
#define __CVIRTUALDISKMNG_H__

#include "CVirtualDisk.h"
#include "file_dir_base.h"
#include "mystring.h"

//class ITreeNode;
//class CDirectoryNode;
//class CFileNode;

//class CMyString;

class CVirtualDiskMng
{
public:
	CVirtualDiskMng();
	~CVirtualDiskMng();


	int CreateDir(char szName[], char Path[]);						// ����Ŀ¼
	int CreateFile(char szName[], char Path[], int nSize);			// �����ļ�

	int ListDir(char Path[], bool bRecursion /*�Ƿ�ݹ��г�����*/= false);

	int ChangeDir(char Path[]);										// �ı䵱ǰĿ¼

	ITreeNode* GetNode(CMyString& Path);

	int ResolvePath(char Path[], char* szNames[]);					// ����·��

	CMyString& CoverToAbsolutePath(CMyString& Path);					// ת�ɾ���·��

private:
	CVirtualDisk   m_Disk;
	CDirectoryNode m_RootDir;   // ��Ŀ¼
	//CDirectoryNode* m_CurDir;	// ��ǰĿ¼
	CMyString m_CurDir;


	void Clear()
	{
		//m_CurDir = &m_RootDir;			// ��ʼ��ǰĿ¼Ϊ��Ŀ¼
		m_CurDir = "d:\\Work\\";
	}

};


#endif