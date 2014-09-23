#include "stdafx.h"
#include "CVirtualDiskProxy.h"
#include "inline_func.h"

#define MAX_INPUT 1024

CVirtualDiskProxy CDisk;

CVirtualDiskProxy::CVirtualDiskProxy()
{
	Clear();

	SetVolumn();

	for(int i=EVI_C; i<EVI_END; i++)
	{
		m_Volumn[i].SetVolumn(i);
	}
}

CVirtualDiskProxy::~CVirtualDiskProxy()
{

}

int CVirtualDiskProxy::ExecCommand(const char* command)
{
	int nRet = 0;
	char szTmp[MAX_INPUT] = {0};
	memcpy(szTmp, command, strlen(command)+1);

	TranslateInput(szTmp);

	char* p1 = NULL;
	char* p2 = NULL; 
	char* p3 = NULL;
	bool bContinue = true;

	if( NULL != (p1 = StriStr(szTmp, "c:")))
	{
		SetVolumn(EVI_C);
	}
	else if( NULL != (p2 = StriStr(szTmp, "d:")))
	{
		SetVolumn(EVI_D);
	}

	if(NULL != p1)
	{
		p3 = p1;
	}
	else if(NULL != p2)
	{
		p3 = p2;
	}

	if(NULL != p3)
	{
		for(p3; p3 != szTmp; p3--)
		{
			if(*p3 != ' ')
			{
				bContinue = false;
				break;
			}
		}
	
		if(p3 != szTmp)
		{
			bContinue = true;
		}
	}

	if(bContinue)
	{
		 nRet = m_Volumn[m_nCurVolumn].ExecCommand(szTmp);
	}

	PrintCurPath();

	if(nRet < 0)
	{
		nRet = 0;
	}
	else
	{
		nRet = 1;
	}

	return nRet;
}

void CVirtualDiskProxy::PrintCurPath(bool bBegin/*是否作为前导指示*/)
{ 
	if(m_nCurVolumn == EVI_C)
	{
		printf("%s", DRIVE_LETTER_ZERO);
	}
	else if(m_nCurVolumn == EVI_D)
	{
		printf("%s", DRIVE_LETTER_ONE);
	}

	m_Volumn[m_nCurVolumn].PrintCurPath(bBegin); 
}