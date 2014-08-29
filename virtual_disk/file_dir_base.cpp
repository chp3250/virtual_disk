#include "stdafx.h"
#include "file_dir_base.h"

ITreeNode::~ITreeNode()
{
	Clear();
};

CDirectoryNode::CDirectoryNode()
{

};

CDirectoryNode::~CDirectoryNode()
{

}

CFileNode::CFileNode()
{
	Clear();
}

CFileNode::~CFileNode()
{
	Clear();
}

int CFileNode::ChangeOffset(int nPlace, int nSize)
{

	if(m_nPlace > nPlace)
	{
		if(m_nPlace > nSize)
		{
			m_nPlace -= nSize;
			return m_nPlace;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return 0;
	}
}