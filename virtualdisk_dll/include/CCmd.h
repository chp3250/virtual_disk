#ifndef __CCMD_H__
#define __CCMD_H__

#include "mystring.h"
#include "CVolumn.h"
//class CVolumn;
class CCmd
{
public:
	CCmd(){m_Volumn = NULL;}
	virtual ~CCmd(){}

	virtual int Exec() = 0;

	CVolumn* m_Volumn;

	CMyString m_Para;
};

// 代理
class CCmdProxy:public CCmd
{
public:
	CCmdProxy(char szCommand[], CVolumn* TargetVolumn);
	~CCmdProxy();
	virtual int Exec();
	CCmd* m_RealCmd;
};

// 命令集合
class CDir:public CCmd
{
public:
	~CDir(){}

	virtual int Exec();

	static char* m_CStr;
};

class CMkdir:public CCmd
{
public:
	~CMkdir(){}

	virtual int Exec();

	static char* m_CStr;
};

class CCd:public CCmd
{
public:
	~CCd(){}

	virtual int Exec();

	static char* m_CStr;
};

class CCopy:public CCmd
{
public:
	~CCopy(){}

	virtual int Exec();

	static char* m_CStr;
};

class CRmdir:public CCmd
{
public:
	~CRmdir(){}

	virtual int Exec();

	static char* m_CStr;
};

class CCompare:public CCmd
{
public:
	~CCompare(){}

	virtual int Exec();

	static char* m_CStr;
};

class CDel:public CCmd
{
public:
	~CDel(){}

	virtual int Exec();

	static char* m_CStr;
};

#endif