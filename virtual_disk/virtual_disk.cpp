// virtual_disk.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
#include <string>
#include "mystring.h"
#include "mylist.h"

#define MAX_INPUT 1024

int _tmain(int argc, _TCHAR* argv[])
{
	int nTmp = 0;

	char szTmp[MAX_INPUT];

	int nFinish = 0;


	printf("C:\\>");

	while(nFinish != 1)
	{

		memset(szTmp, 0, sizeof(szTmp));

		//scanf_s("%s", szTmp);
		fgets(szTmp, MAX_INPUT, stdin);

		nFinish = nTmp;

		szTmp[strlen(szTmp)-1] = '\0';		// 会读入回车符，将其清除
		printf("your input is: %s\n", szTmp);

		fflush(stdin);

		if(!strcmp(szTmp, "exit"))
		{
			break;
		}

		
	}



	CMyString szTmp2 = "123";

	printf("输入是：%s \n", szTmp2.GetBuf());

	szTmp2 += "adsf";

	CMyString szTmp3 = "rrrrrrrrrrrrrrrrrrrrrr";
	szTmp2 += szTmp3;

	szTmp2 += szTmp3;

	szTmp2 += szTmp3;

	szTmp2 += szTmp3;

	szTmp2 = szTmp3;

	printf("输入是：%s \n", szTmp2.GetBuf());


	// test list

	CMyList<int> list;
	nTmp = 1;
	list.tail_insert(nTmp);
	nTmp = 2;
	list.tail_insert(nTmp);
	nTmp = 3;
	list.tail_insert(nTmp);

	SNode<int> *tmp;
	tmp = list.get_head();

	printf("list test : %d \n", *(tmp->Value));

	printf("list test : %d \n", *(tmp->Next->Value));

	printf("list test : %d \n", *(tmp->Next->Next->Value));


	system("pause");

	return 0;
}