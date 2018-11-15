#pragma once

#ifdef _DEBUG
	#ifdef _M_IX86
		#pragma comment(lib, "libfbxsdk-md_D32")
	#else
		#pragma comment(lib, "libfbxsdk-md_D64")
	#endif
#else
	#ifdef _M_IX86
		#pragma comment(lib, "libfbxsdk-md_R32")
	#else
		#pragma comment(lib, "libfbxsdk-md_R64")
	#endif
#endif

#include <fbxsdk.h>

class NTFBX
{
public:
	static void Load(const wchar_t* _Path);
public:
	NTFBX();
	~NTFBX();
};