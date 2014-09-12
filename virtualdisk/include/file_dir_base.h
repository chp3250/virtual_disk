#ifndef __FILE_DIR_BASE_H__
#define __FILE_DIR_BASE_H__

/*******************************************************************************************
* @file file_dir_bash.h
* @author: ����
* @brief: ����Ŀ¼���ڵ�ṹ�� �Լ�Ŀ¼���ļ���

*******************************************************************************************/

#include "IVirtualDiskProxy.h"

#include "mylist.h"
#include "windows.h"
#include "CFindResult.h"

#define MAX_PATH 260
//typedef unsigned int DWORD;
class CMyString;
enum ENUM_FILE_TYPE
{
	EFT_START = 0, 
	EFT_FILE,						 // �ļ�
	EFT_DIR,						 // Ŀ¼

	EFT_END
};

enum ENUM_VOLUMN_INDEX
{
	EVI_INVALID = -1,
	EVI_C		= 0,				// c��
	EVI_D,							// d��

	EVI_END
};

class VIRTUALDISK_API ITreeNode
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
	virtual int RecursionPrint(CMyString& szTmp) = 0;									// �ݹ��ӡ


	/************************************************************************/
	/*  @Func Release
	/*				 �ͷ���Դ
	/*	@Para nType		0 ���ͷ�
	/*					1 ֻ�ͷ��ļ�
	/*					2 ֻ�ͷ�Ŀ¼
	/************************************************************************/
	virtual void Release(int nType = 0) = 0;											// �ͷ�

	/************************************************************************/
	/*  @Func ChangeData()		
	/*					�ı�����
	/*	@Para dwDate	����
	/*	@Para dwTime	ʱ��
	/*	@Para Parent  ���ڵ��ַ
	/*	@Para nPos		�ļ����д���λ��
	/*	@Para nSize		�ļ���С
	/************************************************************************/
	virtual void ChangeData(DWORD dwDate = 0, DWORD dwTime = 0, ITreeNode* Parent = NULL, int nPos = 0, int nSize = 0) = 0;

	/************************************************************************/
	/*	@Func UpdatePos
	/*				�����ļ�ɾ��ʱ�����±��ļ�λ����Ϣ
	/*	@Para	nPos			�뱾�ļ�pos���Ա���С�������
	/*	@Para	nSize			���µĴ�С
	/************************************************************************/
	virtual void UpdatePos(int nPos, int nSize) = 0;

	/************************************************************************/
	/*	@Func ReleaseChild
	/*	@Para nType
	/*				 0 ɾ����Ŀ¼�����ļ�
	/*				 1 �ݹ�ɾ����Ŀ¼����Ŀ¼�����ļ�
	/*	@Para szBuff  
	/*				 != NULL ʱ ɾ��ָ����׺�ļ�
	/************************************************************************/
	virtual void ReleaseChild(int nType, char* szBuff = NULL) = 0;

	virtual int GetPos() = 0;
	virtual int GetSize() = 0;

	virtual bool operator== (ITreeNode& Node);

	/************************************************************************/
	/*	@Func SetVolumn
	/*						���ýڵ������̷�
	/************************************************************************/
	void SetVolumn(int nIndex)
	{
		if(nIndex>EVI_INVALID && nIndex < EVI_END)
		{
			m_nVolumnIndex = nIndex;
		}
	}

	bool ValidIndex()
	{
		if(m_nVolumnIndex>EVI_INVALID && m_nVolumnIndex < EVI_END)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

public:
	ENUM_FILE_TYPE m_eType;		    // ���ڵ�����
	DWORD m_dwDate;					// ��������		
	DWORD m_dwTime;					// ����ʱ��
	char m_szName[MAX_PATH];		// ����
	ITreeNode *m_Parent;			// ���ڵ��ַ
	int m_nVolumnIndex;				// �����̷�����

protected:
	virtual void Clear()
	{
		m_eType = EFT_START;
		m_dwDate = 0;
		m_dwTime = 0;

		memset(m_szName, 0, sizeof(m_szName));

		m_Parent = NULL;
		
		m_nVolumnIndex = EVI_INVALID;
	}

};

class VIRTUALDISK_API CDirectoryNode: public ITreeNode, public IDirProxy
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
	virtual int RecursionPrint(CMyString& szTmp);		

	virtual void Release(int nType = 0);											// �ͷ�

	virtual void ChangeData(DWORD dwDate = 0, DWORD dwTime = 0, ITreeNode* Parent = NULL, int nPos = 0, int nSize = 0);

	virtual void UpdatePos(int nPos, int nSize);

	virtual void ReleaseChild(int nType, char* szBuff = NULL);

	virtual int GetPos(){return 0;}
	virtual int GetSize(){return 0;}

	
	////////////////////////////////////  ʵ�ּ̳��Խӿ�IDirProxy�ĺ��� /////////////////////////////////////
	virtual const char* GetName() {return m_szName;};
	virtual ETYPE  GetType() {return ETYPE(m_eType-1);};

	//findstr:Ҫ���ҵ��ļ���(������Ŀ¼��)��Ҫ֧��ͨ���* ��
	//bRecursion���Ƿ�ݹ����
	virtual IFindResult* Find(const char* findstr,bool bRecursion);
	void RecursionFind(IFindResult *pFind);
	///////////////////////////////////////////////////////////////////////////////////////////////////////


	bool EmptyDir()
	{
		return m_Nodes.empty();
	}

	virtual int ChangeOffset(int nPlace, int nSize) { return 0;} // Ŀ¼����ҪУ��

};

class VIRTUALDISK_API CFileNode: public ITreeNode, public IFileProxy
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
	virtual int RecursionPrint(CMyString& szTmp);

	virtual void Release(int nType = 0);											// �ͷ�

	virtual void ChangeData(DWORD dwDate = 0, DWORD dwTime = 0, ITreeNode* Parent = NULL, int nPos = 0, int nSize = 0);

	virtual void UpdatePos(int nPos, int nSize);

	virtual void ReleaseChild(int nType, char* szBuff = NULL) {};

	virtual int GetPos(){ return m_nPlace; }
	virtual int GetSize(){ return m_nSize; }

	////////////////////////////////////  ʵ�ּ̳��Խӿ�IFileProxy�ĺ��� /////////////////////////////////////
	virtual const char* GetName() {return m_szName;};
	virtual ETYPE  GetType() {return ETYPE(m_eType-1);};
	///////////////////////////////////////////////////////////////////////////////////////////////////////

public:
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