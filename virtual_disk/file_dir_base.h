#ifndef __FILE_DIR_BASE_H__
#define __FILE_DIR_BASE_H__

/*******************************************************************************************
* @file file_dir_bash.h
* @author: ����
* @brief: ����Ŀ¼���ڵ�ṹ�� �Լ�Ŀ¼���ļ���

*******************************************************************************************/

#include "mylist.h"

#define MAX_PATH 260
typedef unsigned int DWORD;

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
						

protected:
	ENUM_FILE_TYPE m_eType;		    // ���ڵ�����
	DWORD m_dwDate;					// ��������		
	DWORD m_dwTime;					// ����ʱ��
	char m_szName[MAX_PATH];			// ����

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

	CMyList<ITreeNode> m_Nodes;				// �ڵ�����
	
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