#include "stdafx.h"
#include "CCmd.h"
#include "inline_func.h"

const unsigned int CMD_COUNT=7; 
// 初始化命令串 begin
char* CDir::m_CStr = "dir";
char* CCd::m_CStr = "cd";
char* CMkdir::m_CStr = "mkdir";
char* CCopy::m_CStr = "copy";
char* CRmdir::m_CStr = "rmdir";
char* CCompare::m_CStr = "compare";
char* CDel::m_CStr = "del";
// end

CCmdProxy::CCmdProxy(char szCommand[], CVolumn* TargetVolumn):
CCmd()
{

	m_Volumn = TargetVolumn; // 待操作卷赋值
	m_RealCmd = NULL;

	char* p1 = NULL;
	char* p2 = NULL;

	p1 = strtok_s(szCommand, " \0", &p2);
	if(p1 == NULL)
	{
		return;
	}

	if(strlen(p1) == strlen(CCd::m_CStr) && StriStr(p1, CCd::m_CStr))
	{
		m_RealCmd = new CCd();
	}
	else if(strlen(p1) == strlen(CMkdir::m_CStr) && StriStr(p1, CMkdir::m_CStr))
	{
		m_RealCmd = new CMkdir();
	}
	else if(strlen(p1) == strlen(CCopy::m_CStr) && StriStr(p1, CCopy::m_CStr))
	{
		m_RealCmd = new CCopy();
	}
	else if(strlen(p1) == strlen(CRmdir::m_CStr) && StriStr(p1, CRmdir::m_CStr))
	{
		m_RealCmd = new CRmdir();
	}
	else if(strlen(p1) == strlen(CCompare::m_CStr) && StriStr(p1, CCompare::m_CStr))
	{
		m_RealCmd = new CCompare();
	}
	else if(strlen(p1) == strlen(CDel::m_CStr) && StriStr(p1, CDel::m_CStr))
	{
		m_RealCmd = new CDel();
	}
	else if(strlen(p1) == strlen(CDir::m_CStr) && StriStr(p1, CDir::m_CStr))
	{
		m_RealCmd = new CDir();
	}
	
	if(m_RealCmd != NULL)
	{
		m_RealCmd->m_Para = p2;	
		m_RealCmd->m_Volumn = TargetVolumn;
	}
}

CCmdProxy::~CCmdProxy()
{
	if(m_RealCmd != NULL)
	{
		delete m_RealCmd;
	}
	m_RealCmd = NULL;
	m_Volumn = NULL;
}

int CCmdProxy::Exec()
{
	return m_RealCmd->Exec();
}

int CDir::Exec()
{
	char* p1 = NULL;
	char* p2 = NULL;
	int nRet = 0;

	char* p = const_cast<char*>(m_Para.GetBuf());

	if(NULL == p || NULL == m_Volumn)
	{
		return -1;
	}

	p1 = strtok_s(p, " \0", &p2);
	if(NULL == p1)
	{
		nRet = m_Volumn->ListDir(NULL);
		return 0;
	}

	if(!strcmp(p1, "/ad")) // 只列出子目录
	{

		p1 = strtok_s(NULL, " \0", &p2);
		nRet = m_Volumn->ListDir(p1, 1);

		while(p1 != NULL)
		{
			p1 = strtok_s(NULL, " \0", &p2);
			if( NULL != p1) 
				nRet = m_Volumn->ListDir(p1, 1);
		}
	}
	else if(!strcmp(p1, "/s")) // 输出目录及子目录下的所有文件
	{
		p1 = strtok_s(NULL, " \0", &p2);
		nRet = m_Volumn->ListDir(p1, 2);

		while(p1 != NULL)
		{

			p1 = strtok_s(NULL, " \0", &p2);
			if( NULL != p1) 
				nRet = m_Volumn->ListDir(p1, 2);
		}
	}
	else if(!strcmp(p1, "/s/ad") || !strcmp(p1, "/ad/s"))
	{
		p1 = strtok_s(NULL, " \0", &p2);
		nRet = m_Volumn->ListDir(p1, 3);

		while(p1 != NULL)
		{

			p1 = strtok_s(NULL, " \0", &p2);
			if( NULL != p1) 
				nRet = m_Volumn->ListDir(p1, 3);
		}
	}
	else
	{
		nRet = m_Volumn->ListDir(p1);

		while(p1 != NULL)
		{
			p1 = strtok_s(NULL, " \0", &p2);
			if( NULL != p1) 
				nRet = m_Volumn->ListDir(p1);
		}
	}

	return nRet;
}

int CMkdir::Exec()
{
	char* p = const_cast<char*>(m_Para.GetBuf());
	int nRet = 0;

	if(NULL == p || NULL == m_Volumn)
	{
		return -1;
	}

	nRet = m_Volumn->CreateDir(p);

	return nRet;
}

int CCd::Exec()
{
	char* p1 = NULL;
	char* p2 = NULL;
	int nRet = 0;

	char* p = const_cast<char*>(m_Para.GetBuf());

	if(NULL == p || NULL == m_Volumn)
	{
		return -1;
	}

	p1 = strtok_s(p, " \0", &p2);
	if(NULL == p1)
	{
		m_Volumn->PrintCurPath(false);
	}
	else
	{
		nRet = m_Volumn->ChangeDir(p1);
	}

	return nRet;
}


int CCopy::Exec()
{
	char* p1 = NULL;
	char* p2 = NULL;
	int nRet = 0;

	char* p = const_cast<char*>(m_Para.GetBuf());

	if(NULL == p || NULL == m_Volumn)
	{
		return -1;
	}

	p1 = strtok_s(p, " \0", &p2);
	if(NULL ==p1 || NULL == p2)
	{
		return -1;;
	}
	nRet = m_Volumn->CopyFiles(p1, p2);

	return nRet;
}

int CRmdir::Exec()
{
	char* p1 = NULL;
	char* p2 = NULL;
	int nRet = 0;

	char* p = const_cast<char*>(m_Para.GetBuf());

	if(NULL == p || NULL == m_Volumn)
	{
		return -1;
	}

	p1 = strtok_s(p, " \0", &p2);
	if(NULL ==p1)
	{
		nRet = m_Volumn->RmDir(NULL);
		return 0;;
	}

	if(!strcmp(p1, "/s"))
	{
		p1 = strtok_s(NULL, " \0", &p2);
		nRet = m_Volumn->RmDir(p1, 1);

		while(p1 != NULL)
		{
			p1 = strtok_s(NULL, " \0", &p2);
			if(NULL != p1)
				nRet = m_Volumn->RmDir(p1, 1);
		}
	}
	else
	{
		nRet = m_Volumn->RmDir(p1);

		while(p1 != NULL)
		{
			p1 = strtok_s(NULL, " \0", &p2);
			if(NULL != p1)
				nRet = m_Volumn->RmDir(p1);
		}
	}

	return nRet;
}

int CCompare::Exec()
{
	char* p1 = NULL;
	char* p2 = NULL;
	int nRet = 0;

	char* p = const_cast<char*>(m_Para.GetBuf());

	if(NULL == p || NULL == m_Volumn)
	{
		return -1;
	}

	p1 = strtok_s(p, " \0", &p2);
	nRet = m_Volumn->CompareFile(p1, p2);

	return nRet;
}

int CDel::Exec()
{
	char* p1 = NULL;
	char* p2 = NULL;
	int nRet = 0;

	char* p = const_cast<char*>(m_Para.GetBuf());

	if(NULL == p || NULL == m_Volumn)
	{
		return -1;
	}

	p1 = strtok_s(p, " \0", &p2);
	if(NULL ==p1)
	{
		return 0;;
	}
	if(!strcmp(p1, "/s"))
	{
		p1 = strtok_s(NULL, " \0", &p2);
		nRet = m_Volumn->DelFiles(p1, 1);

		while(p1 != NULL)
		{
			p1 = strtok_s(NULL, " \0", &p2);
			if(NULL != p1)
				nRet = m_Volumn->DelFiles(p1, 1);
		}
	}
	else
	{
		nRet = m_Volumn->DelFiles(p1);

		while(p1 != NULL)
		{
			p1 = strtok_s(NULL, " \0", &p2);
			if(NULL != p1)
				nRet = m_Volumn->DelFiles(p1);
		}
	}

	return nRet;
}