#ifndef __INLINE_FUNC_H__
#define __INLINE_FUNC_H__

/************************************************************************/
/* @Func CoverTimeToUInt()                                                                     
/*       ��ϵͳʱ��ת��Ϊ����unsigned long ���ͱ���                                                               
/* @Para time ʱ�� ����Ϊ SYSTEMTME                                                                    
/* @Para dwDate �洢ת����ɵ�����
/* @Para dwTime �洢ת����ɵ�ʱ��                                                                     
/************************************************************************/

#include "windows.h"
#include "winbase.h"

void CoverTimeToUInt(SYSTEMTIME& time, DWORD& dwDate, DWORD& dwTime)
{
	dwDate = time.wYear *10000 + time.wMonth * 100 + time.wDay;

	dwTime = time.wHour * 10000 + time.wMinute * 100 + time.wSecond;
}

/************************************************************************/
/* @Func CoverUIntToTime()                                                                     
/*       ������unsigned long ���ͱ���ת��Ϊϵͳʱ��                                                              
/* @Para time ʱ�� ����Ϊ SYSTEMTME                                                                    
/* @Para dwDate �洢��Ҫת��������
/* @Para dwTime �洢��Ҫת����ʱ��                                                                     
/************************************************************************/

void CoverUIntToTime(SYSTEMTIME& time, DWORD& dwDate, DWORD& dwTime)
{
	time.wYear = (WORD)(dwDate / 10000);
	time.wMonth = (WORD)((dwDate % 10000) / 100);
	time.wDay = (WORD)(dwDate % 100);

	time.wHour = (WORD)(dwTime / 10000);
	time.wMinute = (WORD)((dwTime % 10000) / 100);
	time.wSecond = (WORD)(dwTime % 100);
}

#endif