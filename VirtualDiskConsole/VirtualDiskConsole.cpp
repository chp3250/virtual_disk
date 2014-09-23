// VirtualDiskConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "vld.h"
#include <stdlib.h>
#include <iostream>
#include <string>

// for test
#include "mystring.h"
#include "CVirtualDiskProxy.h"
// end test

#define MAX_INPUT 1024

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

		if ( -2 == ITmp->ExecCommand(szTmp) )
		{
			break;
		}

		// end test
	}

	system("pause");

	return 0;
}