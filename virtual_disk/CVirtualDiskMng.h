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


	int CreateDir(char szName[], char Path[]);						// 创建目录
	int CreateFile(char szName[], char Path[], int nSize);			// 创建文件

	int ListDir(char Path[], bool bRecursion /*是否递归列出所有*/= false);

	int ChangeDir(char Path[]);										// 改变当前目录

	ITreeNode* GetNode(CMyString& Path);

	int ResolvePath(char Path[], char* szNames[]);					// 解析路径

	CMyString& CoverToAbsolutePath(CMyString& Path);					// 转成绝对路径

private:
	CVirtualDisk   m_Disk;
	CDirectoryNode m_RootDir;   // 根目录
	//CDirectoryNode* m_CurDir;	// 当前目录
	CMyString m_CurDir;


	void Clear()
	{
		//m_CurDir = &m_RootDir;			// 初始当前目录为根目录
		m_CurDir = "d:\\Work\\";
	}

};


#endif