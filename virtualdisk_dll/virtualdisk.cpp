// virtualdisk.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "virtualdisk.h"


// This is an example of an exported variable
VIRTUALDISK_API int nvirtualdisk=0;

// This is an example of an exported function.
VIRTUALDISK_API int fnvirtualdisk(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see virtualdisk.h for the class definition
Cvirtualdisk::Cvirtualdisk()
{
	return;
}
