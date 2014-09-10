#ifndef __CVIRTUALDISK_H__
#define __CVIRTUALDISK_H__

/************************************************************************/
/*	@File CVitualDisk.h
/*					虚拟磁盘类
/*	@auchor chenpan
/*	@date 2014-9-1
/************************************************************************/

#include "globalbase.h"
#include "IVirtualDiskProxy.h"

const int MAX_DISK_SIZE = 1024*1024*10;				// 10M 空间

enum ENUM_DISK_DATA_TYPE
{
	EDDT_DIR = 0,
	EDDT_FILE,
	EDDT_POS,
};

class VIRTUALDISK_API CVirtualDisk
{
public:
		CVirtualDisk();
		~CVirtualDisk();

		int WriteTo(char* szBuff, int nSize);
		int ReadFrom(char* szBuff, int nPos, int nSize);

		/************************************************************************/
		/*	@Func GetData
		/*				获取成员变量值
		/*	@Para nType
		/*				=0,  DirCount
		/*				=1,  FileCount
		/*				=2,  nCurPos
		/************************************************************************/
		int GetData(int nType);

		void ChangeData(int nType, int nCount);

		int DelFile(int nPos, int nSize);

private:
	char m_szDiskBuf[MAX_DISK_SIZE];			// 申请的磁盘空间地址
	int m_nDirCount;			// 目录总数
	int m_nFileCount;			// 文件总数
	int m_nCurPos;				// 当前已用字节数 （删减文件及增加文件时记得修改）

private:
	void Clear()
	{
		memset(m_szDiskBuf, 0, sizeof(m_szDiskBuf));
		m_nDirCount = 0;
		m_nFileCount = 0;
		m_nCurPos = 0;
	}

};


#endif