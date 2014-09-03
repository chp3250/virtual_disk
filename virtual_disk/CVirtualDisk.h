#ifndef __CVIRTUALDISK_H__
#define __CVIRTUALDISK_H__

const int MAX_DISK_SIZE = 1024*1024*10;				// 10M �ռ�

enum ENUM_DISK_DATA_TYPE
{
	EDDT_DIR = 0,
	EDDT_FILE,
	EDDT_POS,
};

class CVirtualDisk
{
public:
		CVirtualDisk();
		~CVirtualDisk();

		int WriteTo(char* szBuff, int nSize);

		/************************************************************************/
		/*	@Func GetData
		/*				��ȡ��Ա����ֵ
		/*	@Para nType
		/*				=0,  DirCount
		/*				=1,  FileCount
		/*				=2,  nCurPos
		/************************************************************************/
		int GetData(int nType);

		void ChangeData(int nType, int nCount);

		int DelFile(int nPos, int nSize);

private:
	char m_szDiskBuf[MAX_DISK_SIZE];			// ����Ĵ��̿ռ��ַ
	int m_nDirCount;			// Ŀ¼����
	int m_nFileCount;			// �ļ�����
	int m_nCurPos;				// ��ǰ�����ֽ��� ��ɾ���ļ��������ļ�ʱ�ǵ��޸ģ�

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