#pragma once
#include "NTFileStream.h"

class NTReadStream : protected NTFileStream
{
public:
	template<typename T>
	void Read(T& _Buf)
	{
		Read(&_Buf, sizeof(T), sizeof(T));
	}

	void Read(void* _Buf, int _BufByteSize, int _ReadBufSize);
	void Read(void* _Buf, int _BufSize);
	void Seek(long _Size, int _Mode = SEEK_SET);

public:
	NTReadStream(const wchar_t* _Path);
	~NTReadStream();
};

