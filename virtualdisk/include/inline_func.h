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

#define LOWER(x) (x >= 'A' && x <='Z' ? x + 32 : x)

inline void CoverTimeToUInt(SYSTEMTIME& time, DWORD& dwDate, DWORD& dwTime)
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

inline void CoverUIntToTime(SYSTEMTIME& time, DWORD& dwDate, DWORD& dwTime)
{
	time.wYear = (WORD)(dwDate / 10000);
	time.wMonth = (WORD)((dwDate % 10000) / 100);
	time.wDay = (WORD)(dwDate % 100);

	time.wHour = (WORD)(dwTime / 10000);
	time.wMinute = (WORD)((dwTime % 10000) / 100);
	time.wSecond = (WORD)(dwTime % 100);
}

/************************************************************************/
/*	@Func GetPathFromStr
/*					从字符串末尾非'\'字符起， 至'\'字符终止全部赋值为'\0'
/*	@Para 待处理字符串
/************************************************************************/
inline void GetPathFromStr(char* Str)
{
	int nLength = strlen(Str);
	bool bOk = false;
	for(int i = nLength-1; i>=0; i--)
	{
		if(Str[i] == '\\')
		{
			if(bOk == false)
			{
				continue;
			}
			else
			{
				Str[i] = '\0';
				break;
			}
		}

		Str[i] = '\0';
		bOk = true;
	}
}

/************************************************************************/
/*	@Func GetFileNameFromStr
/*					取得从字符串末尾非'\'字符起， 至'\'字符终止的字符串
/*	@Para 待处理字符串
/************************************************************************/
extern char* GetFileNameFromStr(char* D_Str, char* S_Str);

/************************************************************************/
/*	@Func HaveSuffix()
/*					判断是否有后缀
/*	@Para Str		待处理字符串
/*	@Return		char *  返回发下后缀的.的位置 =NULL则无后缀
/************************************************************************/
extern char* HaveSuffix(char* Str);

/************************************************************************/
/*	@Func StriStr()
/*				   查询在源字符串中是否有目标子串---> 不区分大小写
/*	@Para src	   查询字符串
/*	@Para target   子串
/************************************************************************/

extern char *StriStr(const char *src, const char *target);

#endif