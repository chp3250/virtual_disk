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

char *StrStr_Wild(const char *src, const char *target)
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
		while( (s>0) && ( (LOWER(src[i]) != LOWER(target[s]) && target[s] != '?' ) ) )
			s = f[s];
		if( (LOWER(src[i]) == LOWER(target[s])) || (target[s] == '?') )
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

char* TranslateInput(char* str)
{
	const int MAX_PARA_COUNT = 4;
	char* para[MAX_PARA_COUNT] = {"/ad/s", "/s/ad", "/s", "/ad"};
	char* para_t[MAX_PARA_COUNT] = {"\\ad\\s", "\\s\\ad", "\\s", "\\ad"};

	char* p1 = NULL;
	char* p2 = NULL;

	int nLength1 = 0;
	int nLength2 = 0;
	char c = ' ';

	//1, ʹ��ͷ�ǿո�
	nLength1 = strlen(str);
	for(int i=0; i<nLength1; i++)
	{
		if(str[i] == ' ')
		{

		}
		else
		{
			if(i!=0)
			{
				memmove(str, str+i, nLength1-i);
				str[nLength1-i] = '\0';
			}

			break;
		}
	}

	//2, ·������  \ /
	nLength1 = strlen(str);
	for(int i=0; i<nLength1; i++)
	{
		if(str[i] == '/')
		{
			str[i] = '\\';
		}
	}

	//3, ���������.. ��. ����
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
				// ����һ���ո�
				for(int j=nLength1; j>=i; j--)
				{
					str[j+1] = str[j];
				}

				str[i] = ' ';
			}

			break;
		}
	}

	//4, ��������� \����
	p1 = NULL;
	p2 = NULL;
	nLength1 = strlen(str);
	for(int i=0; i<nLength1; i++)
	{
		if(str[i] != ' ')
		{
			for(int j=i; j<nLength1; j++)
			{
				// ���пո� ����'\\'

				if(str[j] == ' ')
				{
					break;
				}

				if(str[j] == '\\')
				{
					// ����һ���ո�
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

	p1 = NULL;
	p2 = NULL;
	//5, �������� /s /ad
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
				// ������
			}
			else
			{
				nLength2 = strlen(p2);

				for(int k=nLength2+1; k>=0; k--) // ��1 Ϊ�˲���һ�ո�
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

	return str;
}

bool CheckFileName(const char* FileName)
{
	const unsigned int InValidCount = 8;
	char* InValid[InValidCount] = {"\\", "/", ":", "?", "*", "<", ">", "|"};


	for(int i=0; i<InValidCount; i++)
	{

		if(NULL != strstr(FileName, InValid[i]))
		{
			return false;
		}
	}

	return true;
}

bool CheckPathName(const char* PathName)
{
	const unsigned int InValidCount = 6;
	char* InValid[InValidCount] = {":", "?", "*", "<", ">", "|"};


	for(int i=0; i<InValidCount; i++)
	{

		if(NULL != strstr(PathName, InValid[i]))
		{
			return false;
		}
	}

	return true;
}

bool strCompare(const char *str1, const char *str2)  
{  
	int slen1 = strlen(str1);    
	int slen2 = strlen(str2);  


	int i,j,k;    
	//������ѭ���ķ�Χ  
	int lbound = 0;  
	int upbound = 0;  
	int bMatched = 0;//����Ƿ�match  
	//ʵ��ʹ��ʱ����str1�ĳ�������̬�������ڴ�  
	char matchmap[MAX_PATH][MAX_PATH];  
	memset(matchmap, 0, MAX_PATH*MAX_PATH);    

	if(*str1 == '*')
		return true;

	//����Ŀ���ַ�������  
	for(i = 0; i< slen1; ++i)    
	{  
		//����ͨ�����  

		int upthis = upbound;  
		for(j = lbound; j<=upthis ; ++j)  
		{  
			//ƥ�䵱ǰ�ַ�  
			if(str1[i] == str2[j] || str2[j] == '?')  
			{   
				matchmap[i][j] = 1;  
				if(0 == bMatched)  
				{  
					lbound = j+1;  
				}  
				upbound = j+1;  
				bMatched = 1;  
				if(i == slen1 - 1)  
				{  
					//����ĩβ��*���������  
					for(k = j+1 ; k < slen2 && '*' == str2[k] ; ++k)  
					{  
						matchmap[i][k] = 1;  
					}  
				}  
			}  
			else if(str2[j] == '*')  
			{  
				if(0 == bMatched)  
				{  
					lbound = j;  
				}  
				//�����Ǻţ�Ŀ���ַ�����ĩβ���ܵõ�ƥ��  
				for(k = i; k< slen1; ++k)  
				{  
					matchmap[k][j] = 1;    
				}  
				k = j;  
				while( '*' == str2[++k])  
				{  
					matchmap[i][k] = 1;  
				}  
				if(str1[i] == str2[k] || str2[k] == '?')  
				{  
					matchmap[i][k] = 1;  
					upbound = k+1;  
					if(i == slen1 - 1)  
					{  
						//����ĩβ��*���������  
						for(k = k+1 ; k < slen2 && '*' == str2[k] ; ++k)  
						{  
							matchmap[i][k] = 1;  
						}  
					}  
				}else{  
					upbound = k;  
				}  
				bMatched = 1;  
			}  
		}  
	}  

	if(!bMatched )  
	{  
		//û��ƥ�䵽  
		//return 0;  
		return false;
	}  
	else  
	{  
		//ƥ�䵽��  
		return true; 
	}  
}  