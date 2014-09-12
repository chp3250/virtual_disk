#ifndef __CVIRTUALDISKPROXY_H__
#define __CVIRTUALDISKPROXY_H__

#include "CVolumn.h"

#define DISK_COUNT_INIT	2					// 磁盘数
#define DRIVE_LETTER_ZERO "C:"				// 索引为0的盘符名
#define DRIVE_LETTER_ONE "D:"				// 索引为1的盘符名

class CVirtualDiskProxy:public IVirtualDiskProxy
{
public:
	CVirtualDiskProxy();
	~CVirtualDiskProxy();


private:
	CVolumn m_Volumn[DISK_COUNT_INIT];
	int m_nCurVolumn;

	void Clear()
	{
		m_nCurVolumn = 0;
	}

public:

	///////////////////////////接口实现start////////////////////////////////////////////
	virtual int ExecCommand(const char* command); //返回1表示成功，0表示失败
	virtual int GetVolumnCount(){ return DISK_COUNT_INIT; }
	virtual IVolumnProxy* GetVolumnByIdx(int nIndex)
	{
		if(nIndex >= DISK_COUNT_INIT || nIndex < 0)
		{
			return NULL;
		}

		return &(m_Volumn[nIndex]);
	}
	///////////////////////////接口实现end/////////////////////////////////////////////


	void PrintCurPath(bool bBegin/*是否作为前导指示*/ = true);

	/************************************************************************/
	/*	@Func SetVolumn
	/*					设置当前盘符
	/*	@Para nIndex	当前盘符索引
	/*					=0 默认的C盘
	/*					=1 d盘
	/************************************************************************/ 
	int SetVolumn(int nIndex = 0)	
	{
		if(nIndex >= DISK_COUNT_INIT || nIndex < 0)
		{
			return -1;
		}

		m_nCurVolumn = nIndex;

		return 0;
	}

};

extern CVirtualDiskProxy CDisk;

extern "C" {
	inline VIRTUALDISK_API IVirtualDiskProxy*  GetVirtualDiskProxy()
	{
		return &CDisk;
	}
}

#endif