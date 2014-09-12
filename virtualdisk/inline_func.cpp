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

char *StriStr(const char *src, const char *target)
{
	int m, t = 0, s = 0;
	unsigned int i;
	int *f;

	m = strlen(target);
	if (m == 0)
	{
		return (char *)src;
	}
	f = new int[m+1];

	f[1] = 0;
	for(s=1; s < m; s++)
	{
		while((t > 0) && (LOWER(target[s]) != LOWER(target[t]))) 
			t = f[t];

		if(LOWER(target[t]) == LOWER(target[s]))
		{
			f[s+1] = ++t;
		}
		else
		{
			f[s+1] = 0;
		}
	}

	for(i=0, s=0;i<strlen(src);i++)
	{
		while((s>0) && (LOWER(src[i]) != LOWER(target[s])))
			s = f[s];
		if(LOWER(src[i]) == LOWER(target[s]))
			s++;
		if(s == m)
		{
			delete []f;
			return ((char *)src+i-m+1);
		}
	}

	delete []f;

	return NULL;
}
