#ifndef __CFINDRESULT_H__
#define __CFINDRESULT_H__

#include "IVirtualDiskProxy.h"

class CFindResult:public IFindResult
{
public:
	virtual void Release() =0;
	virtual unsigned int GetCount()=0;   
	virtual IDiskObj* GetItemByIdx(unsigned int)=0;



};

#endif