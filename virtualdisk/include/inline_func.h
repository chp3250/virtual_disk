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

#define LOWER(x) (x >= 'A' && x <='Z' ? x + 32 : x)

inline void CoverTimeToUInt(SYSTEMTIME& time, DWORD& dwDate, DWORD& dwTime)
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
/*					���ַ���ĩβ��'\'�ַ��� ��'\'�ַ���ֹȫ����ֵΪ'\0'
/*	@Para �������ַ���
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
/*					ȡ�ô��ַ���ĩβ��'\'�ַ��� ��'\'�ַ���ֹ���ַ���
/*	@Para �������ַ���
/************************************************************************/
extern char* GetFileNameFromStr(char* D_Str, char* S_Str);

/************************************************************************/
/*	@Func HaveSuffix()
/*					�ж��Ƿ��к�׺
/*	@Para Str		�������ַ���
/*	@Return		char *  ���ط��º�׺��.��λ�� =NULL���޺�׺
/************************************************************************/
extern char* HaveSuffix(char* Str);

/************************************************************************/
/*	@Func StriStr()
/*				   ��ѯ��Դ�ַ������Ƿ���Ŀ���Ӵ�---> �����ִ�Сд
/*	@Para src	   ��ѯ�ַ���
/*	@Para target   �Ӵ�
/************************************************************************/

extern char *StriStr(const char *src, const char *target);

#endif