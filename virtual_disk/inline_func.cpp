#include "stdafx.h"
#include "inline_func.h"

char* GetFileNameFromStr(char* D_Str, char* S_Str)
{
	int nLength = strlen(S_Str);
	int i,j = 0;
	bool bOk = false;
	for(i = nLength-1; i>=0; i--)
	{
		if(S_Str[i] == '\\')
		{
			if(bOk == false)
			{
				continue;
			}
			else
			{
				break;
			}

		}

		if(bOk == false)
		{
			j = i;
		}

		bOk = true;
	}

	D_Str[j-i] = '\0';
	while(j != i)
	{
		D_Str[j-i-1] = S_Str[j];
		j--;
	}

	return D_Str;
}

char* HaveSuffix(char* Str)
{
	if(NULL == Str)
		return NULL;

	int nLength = strlen(Str);
	for(int i = nLength-1; i>=0; i--)
	{
		if(Str[i] == '\\')
		{
			break;
		}

		if(Str[i] == '.')
		{
			if( i != nLength-1 )
			{
				return Str+i;
			}
			else
			{
				break;
			}
		}
	}

	return NULL;
}