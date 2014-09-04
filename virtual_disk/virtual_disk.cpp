// virtual_disk.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
#include <string>

// for test
#include "mystring.h"
#include "mylist.h"
#include "CVirtualDiskMng.h"
// end test

#define MAX_INPUT 1024

CVirtualDiskMng CTmp;

int _tmain(int argc, _TCHAR* argv[])
{
	int nTmp = 0;

	char szTmp[MAX_INPUT];

	int nFinish = 0;
	char* p1 = NULL;
	char* p2 = NULL;
	char* p3 = NULL;

	while(nFinish != 1)
	{

		CTmp.PrintCurPath();

		memset(szTmp, 0, sizeof(szTmp));

		//scanf_s("%s", szTmp);
		fgets(szTmp, MAX_INPUT-1, stdin);

		nFinish = nTmp;

		szTmp[strlen(szTmp)-1] = '\0';		// 会读入回车符，将其清除
		printf("your input is: %s\n", szTmp);

		fflush(stdin);

		if(!strcmp(szTmp, "exit"))
		{
			break;
		}

		// for test
		p1 = strtok_s(szTmp, " \0", &p2);
		if(p1 == NULL)
		{
			continue;
		}

		if(!strcmp(p1, "mkdir"))
		{
			CTmp.CreateDir(p2);
		}
		else if(!strcmp(p1, "dir"))
		{
			p1 = strtok_s(NULL, " \0", &p2);
			if(NULL == p1)
			{
				CTmp.ListDir(NULL);
				continue;
			}

			if(!strcmp(p1, "/ad")) // 只列出子目录
			{

				p1 = strtok_s(NULL, " \0", &p2);
				CTmp.ListDir(p1, 1);

				while(p1 != NULL)
				{
					p1 = strtok_s(NULL, " \0", &p2);
					if( NULL != p1) 
						CTmp.ListDir(p1, 1);
				}
			}
			else if(!strcmp(p1, "/s")) // 输出目录及子目录下的所有文件
			{
				p1 = strtok_s(NULL, " \0", &p2);
				CTmp.ListDir(p1, 2);

				while(p1 != NULL)
				{
					p1 = strtok_s(NULL, " \0", &p2);
					if( NULL != p1) 
						CTmp.ListDir(p1, 2);
				}
			}
			else
			{
				CTmp.ListDir(p1);

				while(p1 != NULL)
				{
					p1 = strtok_s(NULL, " \0", &p2);
					if( NULL != p1) 
						CTmp.ListDir(p1);
				}
			}

		}
		else if(!strcmp(p1, "cd"))
		{
			p1 = strtok_s(NULL, " \0", &p2);
			if(NULL == p1)
			{
				CTmp.PrintCurPath(false);
			}
			else
			{
				CTmp.ChangeDir(p1);
			}
		}
		else if(!strcmp(p1, "copy"))
		{
			p1 = strtok_s(NULL, " \0", &p2);
			if(NULL ==p1 || NULL == p2)
			{
				continue;;
			}
			CTmp.CopyFiles(p1, p2);
		}
		else if(!strcmp(p1, "rmdir"))
		{
			p1 = strtok_s(NULL, " \0", &p2);
			if(NULL ==p1)
			{
				continue;;
			}

			if(!strcmp(p1, "/s"))
			{
				p1 = strtok_s(NULL, " \0", &p2);
				CTmp.RmDir(p1, 1);

				while(p1 != NULL)
				{
					p1 = strtok_s(NULL, " \0", &p2);
					if(NULL != p1)
						CTmp.RmDir(p1, 1);
				}
			}
			else
			{
				CTmp.RmDir(p1);

				while(p1 != NULL)
				{
					p1 = strtok_s(NULL, " \0", &p2);
					if(NULL != p1)
						CTmp.RmDir(p1);
				}
			}
		}
		else if(!strcmp(p1, "compare"))
		{
			p1 = strtok_s(NULL, " \0", &p2);
			CTmp.CompareFile(p1, p2);
		}
		else if(!strcmp(p1, "del"))
		{
			p1 = strtok_s(NULL, " \0", &p2);
			if(NULL ==p1)
			{
				continue;;
			}
			if(!strcmp(p1, "/s"))
			{
				p1 = strtok_s(NULL, " \0", &p2);
				CTmp.DelFiles(p1, 1);

				while(p1 != NULL)
				{
					p1 = strtok_s(NULL, " \0", &p2);
					if(NULL != p1)
						CTmp.DelFiles(p1, 1);
				}
			}
			else
			{
				CTmp.DelFiles(p1);

				while(p1 != NULL)
				{
					p1 = strtok_s(NULL, " \0", &p2);
					if(NULL != p1)
						CTmp.DelFiles(p1);
				}
			}
		}

		// end test
	}

	system("pause");

	return 0;
}