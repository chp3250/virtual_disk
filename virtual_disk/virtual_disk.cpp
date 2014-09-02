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

		szTmp[strlen(szTmp)-1] = '\0';		// �����س������������
		printf("your input is: %s\n", szTmp);

		fflush(stdin);

		if(!strcmp(szTmp, "exit"))
		{
			break;
		}

		// for test
		p1 = strtok_s(szTmp, " \0", &p2);

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

			if(!strcmp(p1, "/ad")) // ֻ�г���Ŀ¼
			{
				CTmp.ListDir(p2, 1);
			}
			else if(!strcmp(p1, "/s")) // ���Ŀ¼����Ŀ¼�µ������ļ�
			{
				CTmp.ListDir(p2, 2);
			}
			else
			{
				CTmp.ListDir(p1);
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
			CTmp.CopyFiles(p1, p2);
		}

		// end test
	}



	//CMyString szTmp2 = "123";

	//printf("�����ǣ�%s \n", szTmp2.GetBuf());

	//szTmp2 += "adsf";

	//CMyString szTmp3 = "rrrrrrrrrrrrrrrrrrrrrr";
	//szTmp2 += szTmp3;

	//szTmp2 += szTmp3;

	//szTmp2 += szTmp3;

	//szTmp2 += szTmp3;

	//szTmp2 = szTmp3;

	//printf("�����ǣ�%s \n", szTmp2.GetBuf());


	// test list

	//CMyList<int> list;
	//nTmp = 1;
	//list.tail_insert(nTmp);
	//nTmp = 2;
	//list.tail_insert(nTmp);
	//nTmp = 3;
	//list.tail_insert(nTmp);

	//SNode<int> *tmp;
	//tmp = list.get_head();

	//printf("list test : %d \n", *(tmp->Value));

	//printf("list test : %d \n", *(tmp->Next->Value));

	//printf("list test : %d \n", *(tmp->Next->Next->Value));


	//fgets(szTmp, MAX_INPUT- 1, stdin);

	//CMyString sztmp4 = szTmp;

	////CTmp.CoverToAbsolutePath(sztmp4);

	//CTmp.CreateDir(szTmp);
	//CTmp.ListDir(szTmp);

	system("pause");

	return 0;
}