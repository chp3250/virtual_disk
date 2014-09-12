// VirtualDiskConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
#include <string>

// for test
#include "mystring.h"
#include "CVirtualDiskProxy.h"
// end test

#define MAX_INPUT 1024

char* TranslateInput(char* str)
{
	const int MAX_PARA_COUNT = 2;
	char* para[MAX_PARA_COUNT] = {"/s", "/ad"};
	char* para_t[MAX_PARA_COUNT] = {"\\s", "\\ad"};

	char* p1 = NULL;
	char* p2 = NULL;

	int nLength1 = 0;
	int nLength2 = 0;
	char c = ' ';

	//1, 路径兼容  \ /
	nLength1 = strlen(str);
	for(int i=0; i<nLength1; i++)
	{
		if(str[i] == '/')
		{
			str[i] = '\\';
		}
	}

	p1 = NULL;
	p2 = NULL;
	//2, 参数处理 /s /ad
	for(int i=0; i<MAX_PARA_COUNT; i++)
	{
		p1 = strstr(str, para_t[i]);
		while(NULL != p1)
		{
			nLength1 = strlen(para_t[i]);
			if(p1[nLength1] != ' ' && p1[nLength1] != '\0')
			{
				p1 = strstr(p1+nLength1, para_t[i]);
				continue;
			}

			for(int j=0; j<nLength1; j++)
			{
				p1[j] = ' ';
			}

			p2 = strstr(str, " ");
			p2 += 1;
			if(NULL == p2)
			{
				// 不可能
			}
			else
			{
				nLength2 = strlen(p2);
				
				for(int k=nLength2+1; k>=0; k--) // 加1 为了插入一空格
				{
					p2[k+nLength1+1] = p2[k];
				}



				for(int m=0; m<nLength1; m++)
				{
					p2[m] = para[i][m];
				}

				p2[nLength1] = ' '; 
			}

			p1 = NULL;
			p2 = NULL;
		}


		nLength1 = 0;
		nLength2 = 0;
	}

	//3, 挨着命令的.. 和. 处理
	p1 = NULL;
	p2 = NULL;
	nLength1 = strlen(str);
	for(int i=0; i<nLength1; i++)
	{
		if(str[i] == '.')
		{
			if(i != nLength1-1 && (str[i+1] != '\\' && str[i+1] != '.'))
			{
				break;
			}

			if(i !=0 && str[i-1] != ' ')
			{
				// 插入一个空格
				for(int j=nLength1; j>=i; j--)
				{
					str[j+1] = str[j];
				}

				str[i] = ' ';
			}

			break;
		}
	}

	//4, 挨着命令的 \处理
	p1 = NULL;
	p2 = NULL;
	nLength1 = strlen(str);
	for(int i=0; i<nLength1; i++)
	{
		if(str[i] != ' ')
		{
			for(int j=i; j<nLength1; j++)
			{
				// 先判空格 后判'\\'

				if(str[j] == ' ')
				{
					break;
				}

				if(str[j] == '\\')
				{
					// 插入一个空格
					for(int m=nLength1; m>=j; m--)
					{
						str[m+1] = str[m];
					}

					str[j] = ' ';

					break;
				}
			}

			break;
		}
	}

	return str;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int nTmp = 0;

	char szTmp[MAX_INPUT];

	int nFinish = 0;
	char* p1 = NULL;
	char* p2 = NULL;
	char* p3 = NULL;

	//for test;
	IVirtualDiskProxy* ITmp = GetVirtualDiskProxy();  
	printf("%s\\>", DRIVE_LETTER_ZERO);

	while(nFinish != 1)
	{
		memset(szTmp, 0, sizeof(szTmp));

		fgets(szTmp, MAX_INPUT-1, stdin);

		nFinish = nTmp;

		szTmp[strlen(szTmp)-1] = '\0';		// 会读入回车符，将其清除
		printf("your input is: %s\n", szTmp);

		fflush(stdin);

		TranslateInput(szTmp);
		//printf("translated: == %s\n", szTmp);

		if ( -2 == ITmp->ExecCommand(szTmp) )
		{
			break;
		}

		// end test
	}

	system("pause");

	return 0;
}