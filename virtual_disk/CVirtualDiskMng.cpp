#include "stdafx.h"
#include "CVirtualDiskMng.h"
#include "mystring.h"

CVirtualDiskMng::CVirtualDiskMng()
{
	Clear();
}

CVirtualDiskMng::~CVirtualDiskMng()
{

}

ITreeNode* CVirtualDiskMng::GetNode(CMyString &Path)
{
	char szPath[MAX_PATH] = {0};

	CoverToAbsolutePath(Path);



	return NULL;
}

// ת�ɾ���·��
CMyString& CVirtualDiskMng::CoverToAbsolutePath(CMyString& Path)
{
	CMyString szTmp;

	const char *p = Path.GetBuf();

	int nLength = strlen(p);
	for(int i=0; i<nLength; i++)
	{
		if(*(p+i) == ':')
		{
			if(*(p+i+1) == '\\')			// ������Ǿ���·��
			{
				return Path;
			}
		}
	}

	char *p2 = NULL;
	char *p1 = strtok_s(const_cast<char *>(p), ":", &p2);
	for(int i=0; p1 != NULL; i++)
	{
		printf("%d:%s\n", i, p1);

		szTmp += p1;
		szTmp += "\\";

		p1 = strtok_s(NULL, ":.\\", &p2);
	}

	printf("%s\n", szTmp.GetBuf());

	// ƴ�����·��ǰ�Ĵ�

	Path = m_CurDir;
	Path += szTmp;

	printf("%s\n", Path.GetBuf());

	return Path;
}
