#ifndef __COMMAND_H__
#define __COMMAND_H__

/************************************************************************/
/*   @File command.h
/*					定义命令对象的文件
/*	 @author 陈潘
/*	 @date	2014-9-4
/************************************************************************/

#include "mystring.h"

class ICommand
{
public:
	ICommand(){};
	virtual ~ICommand(){};


	CMyString m_para;

	
	virtual int Exec() = 0;
};

class CDir: public ICommand
{
public:
	CDir();
	~CDir();

	virtual int Exec();
};

class CMkdir: public ICommand
{
public:
	CMkdir();
	~CMkdir();

	virtual int Exec();
};

class CCd: public ICommand
{
public:
	CCd();
	~CCd();

	virtual int Exec();
};

class CCopy: public ICommand
{
public:
	CCopy();
	~CCopy();

	virtual int Exec();
};

class CDel: public ICommand
{
public:
	CDel();
	~CDel();

	virtual int Exec();
};

class CRmdir: public ICommand
{
public:
	CRmdir();
	~CRmdir();

	virtual int Exec();
};

class CCompare: public ICommand
{
public:
	CCompare();
	~CCompare();

	virtual int Exec();
};

#endif