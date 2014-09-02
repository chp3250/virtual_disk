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


	int CreateDir(char Path[]);						// 创建目录
	int CreateFile(char szName[], char Path[], int nSize);			// 创建文件

	/************************************************************************/
	/*  @Func ListDir
	/*			列出目标路径下的文件与目录
	/*	@Para Path							路径
	/*	@Para int nType						0, 普通无参数 1, 参数/ad 2, 参数/s
	/************************************************************************/
	int ListDir(char Path[], int nType/*是否递归列出所有d*/= 0);

	int ChangeDir(char Path[]);										// 改变当前目录

	/************************************************************************/
	/*  @Func RmDir
	/*				删除目录或目录下的所有内容
	/*	@Para nType					0, 无参数 删除空目录  1， 参数 /s 删除目录及目录下的所有内容
	/************************************************************************/
	int RmDir(char Path[], int nType = 0);

	/************************************************************************/
	/*	@Func CopyFile(char Path1[], char Path2[])   
	/*	@Para Path1 物理磁盘路径
	/*	@Para Path2 虚拟磁盘路径
	/************************************************************************/
	int CopyFiles(char Path1[], char Path2[]);

	ITreeNode* GetNode(CMyString& Path, bool bCreate /*是否是创建文件或目录*/ = false);

	int ResolvePath(char Path[], char* szNames[]);					// 解析路径

	CMyString& CoverToAbsolutePath(CMyString& Path);					// 转成绝对路径

	/************************************************************************/
	/*  @Func PrintCurPath
	/*	@Para bBegin			是否作为前导指示
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
	}									// 显示当前路径

private:
	CVirtualDisk   m_Disk;
	CDirectoryNode m_RootDir;   // 根目录
	//CDirectoryNode* m_CurDir;	// 当前目录
	CMyString m_CurDir;


	void Clear()
	{
		//m_CurDir = &m_RootDir;			// 初始当前目录为根目录
		m_CurDir = INIT_CUR_PATH;
	}

};


#endif