#ifndef __INLINE_FUNC_H__
#define __INLINE_FUNC_H__

/************************************************************************/
/* @Func CoverTimeToUInt()                                                                     
/*       将系统时间转换为两个unsigned long 类型变量                                                               
/* @Para time 时间 类型为 SYSTEMTME                                                                    
/* @Para dwDate 存储转换完成的日期
/* @Para dwTime 存储转换完成的时间                                                                     
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
/*       将两个unsigned long 类型变量转换为系统时间                                                              
/* @Para time 时间 类型为 SYSTEMTME                                                                    
/* @Para dwDate 存储需要转换的日期
/* @Para dwTime 存储需要转换的时间                                                                     
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