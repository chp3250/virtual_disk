#ifndef __GLOBALBASE_H__
#define __GLOBALBASE_H__

#include "stdio.h"

#ifdef VIRTUALDISK_EXPORTS
#define VIRTUALDISK_API __declspec(dllexport)
#else
#define VIRTUALDISK_API __declspec(dllimport)
#endif






#endif