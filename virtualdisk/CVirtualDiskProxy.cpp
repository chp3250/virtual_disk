#include "stdafx.h"
#include "CVirtualDiskProxy.h"

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
	if(StriStr(command, "c:"))
	{
		SetVolumn(EVI_C);
	}
	else if(StriStr(command, "d:"))
	{
		SetVolumn(EVI_D);
	}
	else
	{
		m_Volumn[m_nCurVolumn].ExecCommand(command);
	}

	PrintCurPath();

	return 1;
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