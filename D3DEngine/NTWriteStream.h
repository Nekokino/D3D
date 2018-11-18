#pragma once
#include "NTFileStream.h"

class NTWriteStream : protected NTFileStream
{
public:
	void Write(void* _Buf, int _ByteSize);

	template<typename T>
	void Write(T& _Buf)
	{
		Write((void*)&_Buf, sizeof(T));
	}
public:
	NTWriteStream(const wchar_t* _Path);
	~NTWriteStream();
};

