#ifndef __FILE_DIR_BASE_H__
#define __FILE_DIR_BASE_H__

/*******************************************************************************************
* @file file_dir_bash.h
* @author: 陈潘
* @brief: 定义目录树节点结构， 以及目录，文件类

*******************************************************************************************/

#include "mylist.h"
#include "windows.h"

#define MAX_PATH 260
//typedef unsigned int DWORD;

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
	virtual int InsertTree(ITreeNode* Node) = 0;			
	virtual CMyList<ITreeNode>* GetNodes() = 0;				// 获得节点列表	


	/************************************************************************/
	/* @Func Print		
	/*					格式化打印信息
	/* @Para nType		1, 是自己打印自己 2, 是子目录打印自己 3，父目录打印自己 
	/************************************************************************/
	virtual void Print(int nType = 0) = 0;								// 打印节点信息

public:
	ENUM_FILE_TYPE m_eType;		    // 树节点类型
	DWORD m_dwDate;					// 创建日期		
	DWORD m_dwTime;					// 创建时间
	char m_szName[MAX_PATH];		// 名称
	ITreeNode *m_Parent;			// 父节点地址

protected:
	virtual void Clear()
	{
		m_eType = EFT_START;
		m_dwDate = 0;
		m_dwTime = 0;

		memset(m_szName, 0, sizeof(m_szName));

		m_Parent = NULL;
	}

};

class CDirectoryNode: public ITreeNode
{
public:
	CDirectoryNode();
	virtual ~CDirectoryNode();

	CMyList<ITreeNode> m_Nodes;				// 节点链表

	/*
	* @Func create() 构造实例函数
	* @Para 成员变量初始值
	* @para nCurPos 当前磁盘已使用的字节数
	*/
	static ITreeNode* create(ENUM_FILE_TYPE m_eType, char szName[MAX_PATH], ITreeNode *Parent);

	virtual int InsertTree(ITreeNode *Node);
	virtual CMyList<ITreeNode>* GetNodes() { return &m_Nodes; }

	virtual void Print(int nType = 0);


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

	/*
	* @Func create() 构造实例函数
	* @Para 成员变量初始值
	* @para nCurPos 当前磁盘已使用的字节数
	*/
	static ITreeNode* create(ENUM_FILE_TYPE eType, char szName[MAX_PATH], ITreeNode *Parent, int nSize, int nCurPos);

	virtual int InsertTree(ITreeNode* Node) { return 0; }
	virtual CMyList<ITreeNode>* GetNodes() { return NULL; }
	virtual void Print(int nType = 0);

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