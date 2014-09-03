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
class CMyString;
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
	virtual int RecursionPrint(CMyString& szTmp) = 0;									// 递归打印


	/************************************************************************/
	/*  @Func Release
	/*				 释放资源
	/*	@Para nType		0 都释放
	/*					1 只释放文件
	/*					2 只释放目录
	/************************************************************************/
	virtual void Release(int nType = 0) = 0;											// 释放

	/************************************************************************/
	/*  @Func ChangeData()		
	/*					改变数据
	/*	@Para dwDate	日期
	/*	@Para dwTime	时间
	/*	@Para Parent  父节点地址
	/*	@Para nPos		文件会有磁盘位置
	/*	@Para nSize		文件大小
	/************************************************************************/
	virtual void ChangeData(DWORD dwDate = 0, DWORD dwTime = 0, ITreeNode* Parent = NULL, int nPos = 0, int nSize = 0) = 0;

	/************************************************************************/
	/*	@Func UpdatePos
	/*				当有文件删除时，更新本文件位置信息
	/*	@Para	nPos			与本文件pos做对比若小与则更新
	/*	@Para	nSize			更新的大小
	/************************************************************************/
	virtual void UpdatePos(int nPos, int nSize) = 0;

	virtual bool operator== (ITreeNode& Node);

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
	virtual int RecursionPrint(CMyString& szTmp);		

	virtual void Release(int nType = 0);											// 释放

	virtual void ChangeData(DWORD dwDate = 0, DWORD dwTime = 0, ITreeNode* Parent = NULL, int nPos = 0, int nSize = 0);

	virtual void UpdatePos(int nPos, int nSize);

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
	virtual int RecursionPrint(CMyString& szTmp);

	virtual void Release(int nType = 0);											// 释放

	virtual void ChangeData(DWORD dwDate = 0, DWORD dwTime = 0, ITreeNode* Parent = NULL, int nPos = 0, int nSize = 0);

	virtual void UpdatePos(int nPos, int nSize);

public:
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