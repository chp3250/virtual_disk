#ifndef __FILE_DIR_BASE_H__
#define __FILE_DIR_BASE_H__

/*******************************************************************************************
* @file file_dir_bash.h
* @author: 陈潘
* @brief: 定义目录树节点结构， 以及目录，文件类

*******************************************************************************************/

#include "mylist.h"

#define MAX_PATH 260
typedef unsigned int DWORD;

enum ENUM_FILE_TYPE
{
	EFT_START = 0, 
	EFT_FILE,						 // 文件
	EFT_DIR,						 // 目录

	EFT_END
};

class ITreeNode
{
public:
	ITreeNode(){ Clear(); }
	virtual ~ITreeNode() = 0;
	virtual int ChangeOffset(int nPlace, int nSize) = 0;	//若有其他磁盘文件删除，则校正本文件的位置
						

protected:
	ENUM_FILE_TYPE m_eType;		    // 树节点类型
	DWORD m_dwDate;					// 创建日期		
	DWORD m_dwTime;					// 创建时间
	char m_szName[MAX_PATH];			// 名称

protected:
	virtual void Clear()
	{
		m_eType = EFT_START;
		m_dwDate = 0;
		m_dwTime = 0;

		memset(m_szName, 0, sizeof(m_szName));
	}

};

class CDirectoryNode: public ITreeNode
{
public:
	CDirectoryNode();
	virtual ~CDirectoryNode();

	CMyList<ITreeNode> m_Nodes;				// 节点链表
	
	bool EmptyDir()
	{
		return m_Nodes.empty();
	}
	
	virtual int ChangeOffset(int nPlace, int nSize) { return 0;} // 目录不需要校正

};

class CFileNode: public ITreeNode
{
public:
	CFileNode();
	virtual ~CFileNode();

private:
	int m_nSize;	// 文件大小
	int m_nPlace;	// 文件在磁盘中的位置(相对于申请空间首地址偏移量)

public:
	virtual int ChangeOffset(int nPlace, int nSize);

	virtual void Clear()
	{
		m_nSize = 0;
		m_nPlace = 0;
	}
};

#endif