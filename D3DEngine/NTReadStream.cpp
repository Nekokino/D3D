#include "PreCom.h"
#include "NTReadStream.h"


NTReadStream::NTReadStream(const wchar_t* _Path) : NTFileStream(_Path, L"rb")
{
}

NTReadStream::~NTReadStream()
{
}

void NTReadStream::Read(void * _Buf, int _BufByteSize, int _ReadBufSize)
{
	NTFileStream::Read(_Buf, _BufByteSize, _ReadBufSize);
}

void NTReadStream::Read(void * _Buf, int _BufSize)
{
	Read(_Buf, _BufSize, _BufSize);
}

void NTReadStream::Seek(long _Size, int _Mode)
{
	NTFileStream::Seek(_Size, _Mode);
}


