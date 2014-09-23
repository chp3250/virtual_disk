#ifndef __CVIRTUALDISKPROXY_H__
#define __CVIRTUALDISKPROXY_H__

#include "CVolumn.h"

#define DISK_COUNT_INIT	2					// ������
#define DRIVE_LETTER_ZERO "C:"				// ����Ϊ0���̷���
#define DRIVE_LETTER_ONE "D:"				// ����Ϊ1���̷���

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

	///////////////////////////�ӿ�ʵ��start////////////////////////////////////////////
	virtual int ExecCommand(const char* command); //����1��ʾ�ɹ���0��ʾʧ��
	virtual int GetVolumnCount(){ return DISK_COUNT_INIT; }
	virtual IVolumnProxy* GetVolumnByIdx(int nIndex)
	{
		if(nIndex >= DISK_COUNT_INIT || nIndex < 0)
		{
			return NULL;
		}

		return &(m_Volumn[nIndex]);
	}
	///////////////////////////�ӿ�ʵ��end/////////////////////////////////////////////


	void PrintCurPath(bool bBegin/*�Ƿ���Ϊǰ��ָʾ*/ = true);

	/************************************************************************/
	/*	@Func SetVolumn
	/*					���õ�ǰ�̷�
	/*	@Para nIndex	��ǰ�̷�����
	/*					=0 Ĭ�ϵ�C��
	/*					=1 d��
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