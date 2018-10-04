#pragma once
#include "NTFileStream.h"

class NTWriteStream : protected NTFileStream
{
public:
	void Write(void* _Buf, int _ByteSize);

	template<typename T>
	void Write2(T* _Buf, int _ByteSize)
	{
		NTFileStream::Write((void*)_Buf, _ByteSize);
	}
public:
	NTWriteStream(const wchar_t* _Path);
	~NTWriteStream();
};

