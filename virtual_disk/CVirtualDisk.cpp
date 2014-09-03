#include "stdafx.h"
#include "CVirtualDisk.h"


CVirtualDisk::CVirtualDisk()
{
	Clear();
}

CVirtualDisk::~CVirtualDisk()
{

}

int CVirtualDisk::WriteTo(char* szBuff, int nSize)
{
	if(NULL == szBuff)
	{
		return -1;
	}

	char* p = m_szDiskBuf+m_nCurPos;
	if(NULL == p)
	{
		return -1;
	}

	if(m_nCurPos + nSize > MAX_DISK_SIZE)
	{
		return -2;			// ´ÅÅÌ¿Õ¼ä²»×ã
	}

	memcpy(p, szBuff, nSize);

	m_nCurPos += nSize;

	return 0;
}

int CVirtualDisk::GetData(int nType)
{
	if(nType == 0)
	{
		return m_nDirCount;
	}
	else if(nType == 1)
	{
		return m_nFileCount;
	}
	else if(nType == 2)
	{
		return m_nCurPos;
	}
	else
	{
		return -1;
	}
}

void CVirtualDisk::ChangeData(int nType, int nCount)
{
	if(nType == 0)
	{
		m_nDirCount += nCount;
	}
	else if(nType == 1)
	{
		m_nFileCount += nCount;
	}
	else if(nType == 2)
	{
		m_nCurPos += nCount;
	}
	else
	{
		
	}
}

int CVirtualDisk::DelFile(int nPos, int nSize)
{
	if(m_nCurPos <= nPos)
	{
		return -1;
	}

	if(nSize <= 0)
	{
		return -1;
	}

	if(nPos + nSize > m_nCurPos)
	{
		return -1;
	}

	memset(m_szDiskBuf+nPos, 0, nSize);
	memcpy(m_szDiskBuf+nPos, m_szDiskBuf+m_nCurPos, (MAX_DISK_SIZE - m_nCurPos));

	m_nCurPos -= nSize;

	return m_nCurPos;
}