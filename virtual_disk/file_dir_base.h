#ifndef __FILE_DIR_BASE_H__
#define __FILE_DIR_BASE_H__

/*******************************************************************************************
* @file file_dir_bash.h
* @author: ����
* @brief: ����Ŀ¼���ڵ�ṹ�� �Լ�Ŀ¼���ļ���

*******************************************************************************************/

#include "mylist.h"
#include "windows.h"

#define MAX_PATH 260
//typedef unsigned int DWORD;

enum ENUM_FILE_TYPE
{
	EFT_START = 0, 
	EFT_FILE,						 // �ļ�
	EFT_DIR,						 // Ŀ¼

	EFT_END
};

class ITreeNode
{
public:
	ITreeNode(){ Clear(); }
	virtual ~ITreeNode() = 0;
	virtual int ChangeOffset(int nPlace, int nSize) = 0;	//�������������ļ�ɾ������У�����ļ���λ��
	virtual int InsertTree(ITreeNode* Node) = 0;			
	virtual CMyList<ITreeNode>* GetNodes() = 0;				// ��ýڵ��б�	


	/************************************************************************/
	/* @Func Print		
	/*					��ʽ����ӡ��Ϣ
	/* @Para nType		1, ���Լ���ӡ�Լ� 2, ����Ŀ¼��ӡ�Լ� 3����Ŀ¼��ӡ�Լ� 
	/************************************************************************/
	virtual void Print(int nType = 0) = 0;								// ��ӡ�ڵ���Ϣ

public:
	ENUM_FILE_TYPE m_eType;		    // ���ڵ�����
	DWORD m_dwDate;					// ��������		
	DWORD m_dwTime;					// ����ʱ��
	char m_szName[MAX_PATH];		// ����
	ITreeNode *m_Parent;			// ���ڵ��ַ

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

	CMyList<ITreeNode> m_Nodes;				// �ڵ�����

	/*
	* @Func create() ����ʵ������
	* @Para ��Ա������ʼֵ
	* @para nCurPos ��ǰ������ʹ�õ��ֽ���
	*/
	static ITreeNode* create(ENUM_FILE_TYPE m_eType, char szName[MAX_PATH], ITreeNode *Parent);

	virtual int InsertTree(ITreeNode *Node);
	virtual CMyList<ITreeNode>* GetNodes() { return &m_Nodes; }

	virtual void Print(int nType = 0);


	bool EmptyDir()
	{
		return m_Nodes.empty();
	}

	virtual int ChangeOffset(int nPlace, int nSize) { return 0;} // Ŀ¼����ҪУ��

};

class CFileNode: public ITreeNode
{
public:
	CFileNode();
	virtual ~CFileNode();

	/*
	* @Func create() ����ʵ������
	* @Para ��Ա������ʼֵ
	* @para nCurPos ��ǰ������ʹ�õ��ֽ���
	*/
	static ITreeNode* create(ENUM_FILE_TYPE eType, char szName[MAX_PATH], ITreeNode *Parent, int nSize, int nCurPos);

	virtual int InsertTree(ITreeNode* Node) { return 0; }
	virtual CMyList<ITreeNode>* GetNodes() { return NULL; }
	virtual void Print(int nType = 0);

private:
	int m_nSize;	// �ļ���С
	int m_nPlace;	// �ļ��ڴ����е�λ��(���������ռ��׵�ַƫ����)

public:
	virtual int ChangeOffset(int nPlace, int nSize);

	virtual void Clear()
	{
		m_nSize = 0;
		m_nPlace = 0;
	}
};

#endif