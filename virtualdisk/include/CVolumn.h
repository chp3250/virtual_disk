#ifndef __CVolumn_H__
#define __CVolumn_H__

#include "globalBase.h"
#include "CVirtualDisk.h"
#include "file_dir_base.h"
#include "mystring.h"


#define INIT_CUR_PATH "\\"
#define MAX_SUFFIX_LENGTH	8				// 文件名后缀最长多少

// copy时 有通配符，类型枚举表
enum ENUM_PATH_TYPE
{
	EPT_SINGLE_FILE = 0,				//  传入的两个path皆为全路径，则复制一个文件
	EPT_DIR,							//  传入的两个path皆为可寻目录，拷贝目录1下所有文件
	EPT_WILDCARD_NO_SUFFIX,				//	传入的两个path末尾为通配符, 且不带后缀, 拷贝path1 截取通配符后， 所得目录的所有文件
	EPT_WILDCARD_HAVE_SUFFIX,			//	传入的两个path末尾含通配符, 且带后缀, 拷贝path1 截取通配符后， 所得目录符合后缀的所有文件
	EPT_WILDCARD_DIFF_ONE,				//  形如 e:\abc\?.txt  c:\def\* （则复制path1下所有txt文件至path2目录）
	EPT_WILDCARD_DIFF_TWO,				//	形如 e:\abc\*	   c:\def\*.txt (将path1 下所有文件复制到path2目录， 并将后缀改为txt);
	EPT_FILE_DIR,						//  形如 e:\abc\file.txt c:\def\ 

	EPT_ERROR,							//  错误
};

struct SJudgePath_Data
{
	char Suffix1[MAX_SUFFIX_LENGTH];			// 若有后缀需求， 则此处填入后缀，否则为空
	char Suffix2[MAX_SUFFIX_LENGTH];			// 同上
	char FileName1[MAX_PATH];					// 若处理后是文件，则将文件名存入这里
	char FileName2[MAX_PATH];					// 同上

	void clear()
	{
		memset(Suffix1, 0, sizeof(Suffix1));
		memset(Suffix2, 0, sizeof(Suffix2));
		memset(FileName1, 0, sizeof(FileName1));
		memset(FileName2, 0, sizeof(FileName2));
	}
};

class VIRTUALDISK_API CVolumn:public IVolumnProxy
{
public:
	CVolumn();
	~CVolumn();


	/************************************************************************/
	// 继承的接口实现
	//////////////////start //////////////////////////////////////////////////

	virtual IDirProxy* GetRootDir() { return &m_RootDir; }


	int ExecCommand(const char* command); //返回1表示成功，0表示失败

	//////////////////end ///////////////////////////////////////////////////


	int CreateDir(char Path[]);						// 创建目录
	int CreateFile(char szName[], char Path[], int nSize);			// 创建文件

	/************************************************************************/
	/*  @Func ListDir
	/*			列出目标路径下的文件与目录
	/*	@Para Path							路径
	/*	@Para int nType						0, 普通无参数 1, 参数/ad 2, 参数/s
	/************************************************************************/
	int ListDir(char Path[], int nType/*是否递归列出所有d*/= 0);

	int ChangeDir(char Path[]);										// 改变当前目录

	/************************************************************************/
	/*  @Func RmDir
	/*				删除目录或目录下的所有内容
	/*	@Para nType					0, 无参数 删除空目录  1， 参数 /s 删除目录及目录下的所有内容
	/************************************************************************/
	int RmDir(char Path[], int nType = 0);

	ENUM_PATH_TYPE JudgePath(char Path1[], char Path2[], SJudgePath_Data* pData);

	/************************************************************************/
	/*	@Func CopyFile(char Path1[], char Path2[])   
	/*	@Para Path1 物理磁盘路径
	/*	@Para Path2 虚拟磁盘路径
	/************************************************************************/
	int CopyFiles(char Path1[], char Path2[]);

	/************************************************************************/
	/*	@Func DelFiles
	/*					删除指定文件
	/*	@Para Path						路径
	/*	@Para nType						0, 普通无参数
	/*									1, 参数 /s
	/************************************************************************/
	int DelFiles(char Path[], int nType = 0);

	/************************************************************************/
	/*	@Func NoticeAllFileToChangeData
	/*					通知所有文件更新数据
	/************************************************************************/
	void NoticeAllFileToChangeData(int nPos, int nSize);

	/************************************************************************/
	/*	@Func CompareFile()
	/*						比较物理磁盘文件及虚拟磁盘文件
	/*	@Para Path1			物理磁盘上的文件
	/*	@Para Path2			虚拟磁盘上的文件
	/************************************************************************/
	int CompareFile(char Path1[], char Path2[]);

	ITreeNode* GetNode(CMyString& Path, bool bCreate /*是否是创建文件或目录*/ = false);

	int ResolvePath(char Path[], char* szNames[]);					// 解析路径

	CMyString& CoverToAbsolutePath(CMyString& Path);					// 转成绝对路径

	CVirtualDisk* GetDisk(){return &m_Disk;}

	CMyString& GetCurDir() { return m_CurDir; }

	/************************************************************************/
	/*  @Func PrintCurPath
	/*	@Para bBegin			是否作为前导指示
	/************************************************************************/
	void PrintCurPath(bool bBegin = true) { 
		if (bBegin)
		{
			printf("%s>", m_CurDir);
		}
		else
		{
			printf("%s\n", m_CurDir);
		}
	}									// 显示当前路径

	/************************************************************************/
	/*	@Func SetVolumn
	/*					设置所在盘符
	/*	@Para nIndex	当前盘符索引
	/*					=0 默认的C盘
	/*					=1 d盘
	/************************************************************************/ 
	int SetVolumn(int nIndex = 0)	
	{
		if(nIndex > EVI_END || nIndex <= EVI_INVALID)
		{
			return -1;
		}

		m_nVolumnIndex = nIndex;

		return 0;
	}

private:
	CVirtualDisk   m_Disk;
	CDirectoryNode m_RootDir;   // 根目录

	CMyString m_CurDir;

	int m_nVolumnIndex;			// 所在盘符索引

	void Clear()
	{
		//m_CurDir = &m_RootDir;			// 初始当前目录为根目录
		m_CurDir = INIT_CUR_PATH;
		m_nVolumnIndex = EVI_INVALID;
	}
};

#endif