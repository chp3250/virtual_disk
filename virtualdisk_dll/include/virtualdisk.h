// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the VIRTUALDISK_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// VIRTUALDISK_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef VIRTUALDISK_EXPORTS
#define VIRTUALDISK_API __declspec(dllexport)
#else
#define VIRTUALDISK_API __declspec(dllimport)
#endif

// This class is exported from the virtualdisk.dll
class VIRTUALDISK_API Cvirtualdisk {
public:
	Cvirtualdisk(void);
	// TODO: add your methods here.
};

extern VIRTUALDISK_API int nvirtualdisk;

VIRTUALDISK_API int fnvirtualdisk(void);
