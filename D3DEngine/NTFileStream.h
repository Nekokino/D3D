#pragma once
#include <iostream>

class NTFileStream
{
public:
	FILE* File;

public:
	void Seek(long _Size, int _Mode = SEEK_SET);
	void Write(void* _Buf, int _ByteSize);
	void Read(void* _Buf, int _BufByteSize, int _ReadByteSize);
public:
	NTFileStream(const wchar_t* _FileName, const wchar_t* _Mode);
	~NTFileStream();
};

